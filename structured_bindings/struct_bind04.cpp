#include <iostream>
#include <map>
#include <string>
#include <assert.h>
#include <utility>
#include <array>
#include <tuple>


std::array<int,4> getArray() 
{
    return std::array<int, 4> {1, 2, 3, 4};
}

std::array<int, 4>& getArrayByRef()
{
    auto* r = new std::array<int, 4> ({1, 2, 3, 4});
    return *r;
}

std::array<int, 4>&& getArrayByRValueRef()
{
    auto* r = new std::array<int, 4> ({1, 2, 3, 4});
    return std::move(*r);
}

std::tuple<char,float,std::string> getTuple() 
{
    return std::tuple<char, float, std::string>('b', 1.1f, "Hello");
}

int main(int argc, char * argv[])
{
    {
         auto [i,j,k,l] = getArray();
         std::cout << "i = " << i << '\n';
         std::cout << "j = " << j << '\n';
         std::cout << "k = " << k << '\n';
         std::cout << "l = " << l << '\n';
    }

    {
         // write access is also supported, provided the value for initialization
         // is not a temporary return value.
         auto& [i,j,k,l] = getArrayByRef();
         std::cout << "i = " << i << '\n';
         std::cout << "j = " << j << '\n';
         std::cout << "k = " << k << '\n';
         std::cout << "l = " << l << '\n';
    }

    {
         // write access is also supported, provided the value for initialization
         // is not a temporary return value.
         auto&& [i,j,k,l] = getArrayByRValueRef();
         std::cout << "i = " << i << '\n';
         std::cout << "j = " << j << '\n';
         std::cout << "k = " << k << '\n';
         std::cout << "l = " << l << '\n';
    }
}
