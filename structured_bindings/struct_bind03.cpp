#include <iostream>
#include <map>
#include <string>
#include <assert.h>
#include <utility>

struct B {
    int a = 1;
    int b = 2;
};

struct D1 : B {
};

struct D2 : B {
    int c = 3;
};


int (*getArr())[2] {
   int * p_arr = new int[2];
   (p_arr)[0] = 1;
   (p_arr)[1] = 4;
   return reinterpret_cast<int(*)[2]>(p_arr);
};

int* getArr2() {
   int * arr = new int[2];
   arr[0] = 1;
   arr[1] = 4;
   return arr;
};

int (&getArr3())[2] {
   int * arr = new int[2];
   arr[0] = 1;
   arr[1] = 4;
   return reinterpret_cast<int(&)[2]>(*arr);
};

int (&&getArr4())[2] {
   int * arr = new int[2];
   arr[0] = 1;
   arr[1] = 4;
   return std::move(reinterpret_cast<int(&&)[2]>(*arr));
};

int main(int argc, char * argv[])
{
    {
        // Note that there is only limited usage of inheritance possible. All
        // non-static data members must be members of the same class definition
        // - thus they have to be direct members of the type of the same 
        // unambiguous public base class:
        auto [x, y] = D1{}; // OK
        // auto [i, j, k] = D2{}; // Compile time error
    }

    {
        // raw arrays
        // The following code initializes x and y by the two elements of the 
        // raw C-style array:
        int arr[] = { 47, 11 };
        auto [x, y] = arr;  // x and y are ints initialized by elems of arr
        // auto [z] = arr; // ERROR: the number of elements does not fit
        // this of course is possible as long as the array has known size. For
        // an array passed as argument, this is not possible because it decays
        // to the corresponding pointer type.
        //
    }

    {
        // Note that C++ allows us to return arrays with size by reference, so 
        // that this feature also applies to functions returning an array provided
        // its size is part of the return type:
        //auto getArr() -> int(&)[2];  // getArr() returns reference to raw int array
        auto p = getArr();
        auto& [x, y] = *p; // x and y are initialized by elements of returned
                           //  array
        std::cout << "x = " << x << '\n';
        std::cout << "y = " << y << '\n';

        auto p_arr2 = getArr2();
        std::cout << "p_arr2[0] = " << p_arr2[0] << '\n';
        std::cout << "p_arr2[1] = " << p_arr2[1] << '\n';

        //auto [x2, y2] = getArr2(); // compile time error

        auto& p_arr3 = getArr3();
        std::cout << "p_arr3[0] = " << (p_arr3)[0] << '\n';
        std::cout << "p_arr3[1] = " << (p_arr3)[1] << '\n';

        auto& [x3, y3] = getArr3();
        std::cout << "x3 = " << x3 << '\n';
        std::cout << "y3 = " << y3 << '\n';

        auto&& [x4, y4] = getArr4();
        std::cout << "x4 = " << x4 << '\n';
        std::cout << "y4 = " << y4 << '\n';
    }
}
