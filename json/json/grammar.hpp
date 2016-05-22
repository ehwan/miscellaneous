#pragma once

#include "global.hpp"

#include <boost/spirit/home/x3.hpp>

namespace eh { namespace json {

namespace grammar
{
  namespace x3 = boost::spirit::x3;

  static x3::rule< class quote_r , std::string > quoted_rule = "quoted_rule";

  static x3::rule< class anydata_r , JsonData > anydata_rule = "any data rule";

  static x3::rule< class object_r , json_object_t > object_rule = "object rule";
  static x3::rule< class array_r , json_array_t > array_rule = "array rule";


  // string_r = quote_r
  // int_r = int
  // number_r = double
  // bool_r = bool
  
  static auto quoted_rule_def = 
    '"' >> *( x3::char_ - '"' ) >> '"';
  BOOST_SPIRIT_DEFINE( quoted_rule );

  // assign parsed data to JsonData
  // _val = JsonData
  // _attr = json_....specific;
  constexpr auto object_init_lambda = []( auto& context )
  {
    // unordered_map< string , Data >
    x3::_val( context ) = JsonData( 
        object_type ,
        new json_object_t( std::move( x3::_attr( context ) ) )
    );
  };
  constexpr auto array_init_lambda = []( auto& context )
  {
    // vector< Data >
    x3::_val( context ) = JsonData(
        array_type , 
        new json_array_t( std::move( x3::_attr( context ) ) )
    );
  };
  constexpr auto string_init_lambda = []( auto& context )
  {
    // string
    x3::_val( context ) = JsonData(
        string_type , 
        new json_string_t( std::move( x3::_attr( context ) ) )
    );
  };
  constexpr auto int_init_lambda = []( auto& context )
  {
    // int
    x3::_val( context ) = JsonData(
        int_type , 
        new json_int_t( x3::_attr( context ) )
    );
  };
  constexpr auto number_init_lambda = []( auto& context )
  {
    // number 
    x3::_val( context ) = JsonData(
        number_type , 
        new json_number_t( x3::_attr( context ) )
    );
  };
  constexpr auto bool_init_lambda = []( auto& context )
  {
    // bool 
    x3::_val( context ) = JsonData(
        bool_type , 
        new json_bool_t( x3::_attr( context ) )
    );
  };
  /*
  constexpr auto null_init_lambda = []( auto& context )
  {
    // do nothing, leave default constructed
  };
  */

  static auto anydata_rule_def = 
    ( object_rule[ object_init_lambda ] |
      array_rule [ array_init_lambda  ] |
      quoted_rule[ string_init_lambda ] |
      x3::double_[ number_init_lambda ] |
      x3::int_   [ int_init_lambda    ] |
      x3::bool_  [ bool_init_lambda   ] |
      x3::lit( "null" ) |
      x3::eps );
  BOOST_SPIRIT_DEFINE( anydata_rule );

  constexpr auto object_build_lambda = []( auto& context )
  {
    // attrib = tuple< string , JsonData >
    // val = map< string , JsonData >
    auto& attrib = x3::_attr( context );
    // string will be freed
    std::string var_name = std::move( boost::fusion::at_c< 0 >( attrib ) );

    x3::_val( context )[ var_name ] =
      std::move( boost::fusion::at_c< 1 >( attrib ) );
  };
  static auto object_rule_def = 
    '{' >> 
      ( ( quoted_rule >> ':' >> anydata_rule )[ object_build_lambda ] % ',' ) >>
    '}';
  BOOST_SPIRIT_DEFINE( object_rule );

  static auto array_rule_def = 
    '[' >>
      ( anydata_rule % ',' ) >>
    ']';
  BOOST_SPIRIT_DEFINE( array_rule );
}

template < typename IterType >
static 
JsonData parse( IterType begin , IterType end , bool *boolptr = nullptr )
{
  using namespace boost::spirit;
  JsonData ret;

  bool bret = 
    x3::phrase_parse( begin , end , grammar::anydata_rule , x3::space , ret );
  if( boolptr )
  {
    *boolptr = bret;
  }
  return ret;
}
template < typename IterType >
static 
JsonData parse( IterType& begin , IterType end , bool *boolptr = nullptr )
{
  using namespace boost::spirit;
  JsonData ret;

  bool bret = 
    x3::phrase_parse( begin , end , grammar::anydata_rule , x3::space , ret );
  if( boolptr )
  {
    *boolptr = bret;
  }

  return ret;
}

}}
