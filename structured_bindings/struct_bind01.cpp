#include <iostream>

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

  if (id > 30) {
      std::cout << val;
  }
} 
