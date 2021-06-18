#include <iostream>
#include <map>
#include <string>

struct MyStruct {
    int i = 0;
    std::string s;
};

MyStruct getStruct() {
    return MyStruct{42, "hello"};
};

int main(int argc, char * argv[])
{
  MyStruct ms;
  auto [u,v] = ms;
  auto [id,val] = getStruct(); // id and val name i and s of returned struct
  // the benefit here is direct access and the ability to make the code more readable
  // by binding the value directly to names that convey semantic meaning about their
  // purpose.
  if (id > 30) {
      std::cout << val;
  }
  // the following code demonstrates how code can significantly improve with 
  // structured bindings. To iterate over the elements of a std::map<> without 
  // structured bindings you would have to program
  std::map<int,std::string> mymap = { {1, "Hello"}, {2, "Dimitar"} }; 
  for (const auto& elem : mymap) {
      std::cout << elem.first << ": " << elem.second << "\n";
  }

  // by using structured bindings the code above gets more readable
  for (const auto& [key, val] : mymap) {
      std::cout << key << ": " << val << '\n';
  }
} 
