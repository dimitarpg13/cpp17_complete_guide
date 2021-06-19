#include <iostream>
#include <map>
#include <string>
#include <assert.h>
#include <utility>

struct MyStruct {
    int i = 0;
    std::string s;
};

MyStruct getStruct() {
    return MyStruct{42, "hello"};
};

int main(int argc, char * argv[])
{
    {
       MyStruct ms{42, "hello"};
       auto [u, v] = ms;
       ms.i = 77;
       std::cout << u; // prints 42
       u = 99;
       std::cout << ms.i; // prints 77
       // the exact behavior of initialization 
       // auto [u, v] = ms;
       // behaves as if we'd initialize a new entity e with ms 
       // and let the structured bindings u and v become 
       // alias names for the members of this new object, similar
       // to defining:
       // auto e = ms;
       // aliasname u = e.i;
       // aliasname v = e.s;
       //
       // Note that u and v are not references to e.i and e.s, respectively. 
       // They are just other names for the members. Thus, decltype(u) is
       // the type of the member i and decltype(v) is the type of the member
       // s. We also do not have name for e so that we can't access the
       // initialized entity directly by name. As a result,
       // std::cout << u << ' ' << v << '\n';
       // prints the values of e.i and e.s which are copies of ms.i and ms.s
       // e exists as long as the structured bindings to it exist. Thus, it 
       // is destroyed when the structured bindings go out of scope.
       // As a consequence, unless references are used, modifying the value
       // used for initialization has no effect on the names initialized by
       // a structured binding and vice versa.
    }

    {
       // when using structured bindings for return values, the same principle
       // applies. An initialization such as:
       auto [u,v] = getStruct();
       // behaves as we'd initialize a new entity e with the return value of 
       // getStruct() so that the structured bindings u and v become alias
       // names for the two members of e similar to defining:
       // auto e = getStruct();
       // aliasname u = e.i;
       // aliasname v = e.s;
       // That is, structured bidnings bind to a new entity, which is initialized
       //  from a return value, instead of binding to the return value directly.
    }

    {
       // to the anonymous entity e the usual address and alignment guarantees 
       // apply, so that the structured bindings are aligned as the corresponding
       // members they bind to. For example:
       MyStruct ms{42, "hello"};
       auto [u,v] = ms;
       assert(&((MyStruct*)&u)->s == &v); // OK
    }

    {
        MyStruct ms{42, "hello"};
        // we can use qualifiers such as const and references. Again, these 
        // qualifiers apply to the anonymous entity e as a whole. Ususally, the 
        // effect is similar to applying the qualifiers to the structured bindings
        // directly but beware this is not always the case.
        // For example, we can declare structured bindings to a const reference:
        const auto& [u, v] = ms; // a reference, so that u/v refer to ms.i/ms.s
        // Here, the anonymous entity is declared as a const reference, which means
        //  that u and v are the names of the members i and s of the initialized
        //  const reference to ms. As a consquence, any change to the members of
        //  ms affect the value of u and/or v.
        ms.i = 77;  // affects the value of u
        std::cout << u; // priints 77
    }

    {
        // declared as a non-const reference, you can even modify the members of
        //  the object/value used for the initialization:
        MyStruct ms{42, "hello"};
        auto& [u,v] = ms;   // the initialized entity is a reference to ms
        ms.i = 77;          // affects the value of u
        std::cout << u;     // prints 77
        u = 99;             // modifies ms.i
        std::cout << ms.i;  // prints 99

    }

    {
        // if the value used to initialize a structured bindings reference a 
        // temporary object, as usual the lifetime of the temporary is extended
        // to the lifetime of the bound structure
        const auto& [a, b] = getStruct();
        std::cout << "a: " << a << '\n';  // OK

    }

    {
        // Qualifiers do not necessarily apply to the structured bindings
        // As written, the qualifiers apply to the new anonymous entity. They don't
        // necessarily apply to the new names introduced as structured bindings. 
        // In fact, after
        MyStruct ms{42, "hello"};
        const auto& [u, v] = ms; // a reference, so that u/v referes to ms.i/ms.s
        // both u and v are not declared as being references. This only specifies
        // that the anonymous entity e is a reference. u and v have the type of the
        // members of ms. With our initial declaration this means that decltype(u) is
        // int and decltype(v) is std::string.
    }

    {
        MyStruct ms{42, "hello"};
        // u and v have the type of the
        // members of ms. With our initial declaration this means that decltype(u) is
        // int and decltype(v) is std::string.
        // This also makes a difference when specifying an alignment:
        alignas(16) auto [u,v] = ms; // align the object, not v
        // Here we align the initialized anonymous entity and not the structured 
        // bindings u and v. This means that u as the first member is forced to be
        // aligned to 16 while v is not.
        // For the same reason, structured bindings do not decay although auto is used.
        // For example, if we have a structure of raw arrays:
        struct S {
            const char x[6];
            const char y[3];
        };

        S s1{};
        auto [a, b] = s1; // a and b get the exact member types - the type of a is const char[6]
        // again, auto applies to the anonymous entity, which as a whole doesn't decay.
        // This is different from initializing a new object with auto where types decay:
        auto a2 = a; // a2 gets decayed type of a i.e. char* 
    }

    {
        // Move semantics
        // Move semantics is supported following the rules just introduced. In the following 
        // declarations:
        MyStruct ms = { 42, "Jim" };
        auto&& [v,n] = std::move(ms);  // entity is rvalue reference to ms
        // the structured bindings v and n refer to an anonymous entity being an rvalue
        // reference to ms. ms still holds its value:
        std::cout << "ms.s: " << ms.s << '\n'; // prints "Jim"
        // but you can move assign n, which refers to ms.s:
        std::string s = std::move(n);   // moves ms.s to s
        std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
        std::cout << "n:    " << n << '\n';    // prints unspecified value
        std::cout << "s:    " << s << '\n';    // prints "Jim"
        // as usual moved-from objects are in valid state with unspecified value  

    }

    {
        // the previous code was slightly different from initializing the new entity with
        // the moved values of ms
        MyStruct ms = {42, "Jim"};
        auto [v,n] = std::move(ms);  // new entity with moved-from values from ms
        // Here, the initialized anonymous entity is a new object initialized with the 
        // moved values of ms. So ms already lost its value:
        std::cout << "ms.s: " << ms.s << '\n'; // prints unspecified value
        std::cout << "n:    " << n << '\n';    // prints "Jim"
        // you can still move assign the value of n or assing a new value there, but 
        // this does not affect ms.s:
        std::string s = std::move(n);   // moves n to s
        n = "Lara";
        std::cout << "ms.s: " << ms.s << '\n';  // prints unspecified value
        std::cout << "n:    " << n << '\n';     // prints "Lara"
        std::cout << "s:    " << s << '\n';     // prints "Jim"

    }


} 
