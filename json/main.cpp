#include <iostream>
#include "json.hpp"

using namespace boost::spirit;


int main()
{
  std::string str = 
    "{ \"name1\":1234 ,"
    " \"name2\" : , \"name4\" : 123.123 }";
  std::cout << str << std::endl;

  auto data = eh::json::parse( str.begin() , str.end() );

  data[ "name3" ] = "hello_str";
  data[ "name1" ] = 123.4231;
  std::cout << data;
}
