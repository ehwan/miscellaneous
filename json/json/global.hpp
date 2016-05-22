#pragma once

#include <unordered_map>
#include <vector>

namespace eh { namespace json {

// contains type and data pointer
class JsonData;

using type_type = int;

//
//! { members.... }
using json_object_t = std::unordered_map< std::string , JsonData >;
constexpr type_type object_type = 1;
//
//! [ members... ]
using json_array_t  = std::vector< JsonData >;
constexpr type_type array_type = 2;
//
//! "str_value"
using json_string_t = std::string;
constexpr type_type string_type = 3;
//
//! 1231412
using json_int_t = int;
constexpr type_type int_type = 4;
//
//! 2313.23
using json_number_t = double;
constexpr type_type number_type = 5;
//
//! true false
using json_bool_t = bool;
constexpr type_type bool_type = 6;

using json_null_t = std::nullptr_t;
constexpr type_type null_type = 7;

struct Exception : public std::exception
{
  std::string str;
  Exception( const char* str_ ) :
    str( str_ )
  {
  }

  const char* what() const noexcept
  {
    return str.c_str();
  }
};
struct InvalidOperationException : Exception
{
  using Exception::Exception;
};

}}
