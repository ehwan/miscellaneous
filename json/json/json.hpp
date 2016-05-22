#pragma once

#include "global.hpp"

#include <ostream>

namespace eh{ namespace json {


class JsonData
{
#define DEFINE_AS_JSON_TYPE_METHOD( typename ) \
  inline json_##typename##_t* as_##typename() const \
  { return reinterpret_cast< json_##typename##_t* >( data_ ); }

  DEFINE_AS_JSON_TYPE_METHOD( object );
  DEFINE_AS_JSON_TYPE_METHOD( array );
  DEFINE_AS_JSON_TYPE_METHOD( string );
  DEFINE_AS_JSON_TYPE_METHOD( int );
  DEFINE_AS_JSON_TYPE_METHOD( number );
  DEFINE_AS_JSON_TYPE_METHOD( bool );
  DEFINE_AS_JSON_TYPE_METHOD( null );

#undef DEFINE_AS_JSON_TYPE_METHOD

  void free()
  {
    if( data_ == nullptr ){ return; }
    switch( type_ )
    {
    default:
      break;
    case object_type:
      delete as_object();
      break;
    case array_type:
      delete as_array();
      break;
    case string_type:
      delete as_string();
      break;
    case int_type:
      delete as_int();
      break;
    case number_type:
      delete as_number();
      break;
    case bool_type:
      delete as_bool();
      break;
    }
  }

  // no free;
  // only copying
  void copy_from( JsonData const& rhs )
  {
    type_ = rhs.type_;

    switch( type_ )
    {
    default:
      data_ = nullptr;
      break;
    case object_type:
      data_ = new json_object_t( rhs.getObject() );
      break;
    case array_type:
      data_ = new json_array_t( rhs.getArray() );
      break;
    case string_type:
      data_ = new json_string_t( rhs.getString() );
      break;
    case int_type:
      data_ = new json_int_t( rhs.getInt() );
      break;
    case number_type:
      data_ = new json_number_t( rhs.getNumber() );
      break;
    case bool_type:
      data_ = new json_bool_t( rhs.getBool() );
      break;
    case null_type:
      data_ = nullptr;
      break;
    }
  }

  friend std::ostream& operator << ( std::ostream& , JsonData const& );
  void object_stream_out( std::ostream& stream ) const;
  void array_stream_out( std::ostream& stream ) const;

public:
  JsonData() :
    type_( null_type ) ,
    data_( nullptr )
  {
  }
  JsonData( JsonData const& rhs )
  {
    copy_from( rhs );
  }
  JsonData( JsonData&& rhs ) :
    type_( rhs.type_ ) ,
    data_( rhs.data_ )
  {
    rhs.type_ = null_type;
    rhs.data_ = nullptr;
  }
  template < typename DataPtrType >
  explicit JsonData( type_type type , DataPtrType rhs ) :
    type_( type ) ,
    data_( rhs )
  {
  }
  JsonData& operator = ( JsonData const& rhs )
  {
    free();
    copy_from( rhs );

    return *this;
  }
  JsonData& operator = ( JsonData&& rhs )
  {
    free();
    type_ = rhs.type_;
    data_ = rhs.data_;
    rhs.type_ = null_type;
    rhs.data_ = nullptr;

    return *this;
  }
  ~JsonData()
  {
    free();
  }

  JsonData( json_int_t i ) :
    JsonData( int_type , new json_int_t( i ) )
  {
  }
  JsonData( json_number_t d ) :
    JsonData( number_type , new json_number_t( d ) )
  {
  }
  JsonData( json_bool_t b ) :
    JsonData( bool_type , new json_bool_t( b ) )
  {
  }
  JsonData( json_null_t ) :
    JsonData( null_type , nullptr )
  {
  }
  JsonData( const char* str ) :
    JsonData( string_type , new json_string_t( str ) )
  {
  }
  JsonData( std::string const& str ) :
    JsonData( string_type , new json_string_t( str ) )
  {
  }
  JsonData( std::string&& str ) :
    JsonData( string_type , new json_string_t( str ) )
  {
  }
  JsonData( std::initializer_list< JsonData > list ) :
    JsonData( array_type , new json_array_t( list ) )
  {
  }

  // some convension operators...
  JsonData& operator = ( json_int_t i )
  {
    resetInt( i );
    return *this;
  }
  JsonData& operator = ( json_number_t d )
  {
    resetNumber( d );
    return *this;
  }
  JsonData& operator = ( json_bool_t b )
  {
    resetBool( b );
    return *this;
  }
  JsonData& operator = ( json_null_t )
  {
    reset();
    return *this;
  }
  JsonData& operator = ( const char* str )
  {
    resetString( str );
    return *this;
  }
  JsonData& operator = ( std::string const& str )
  {
    resetString( str );
    return *this;
  }
  JsonData& operator = ( std::string&& str )
  {
    resetString( str );
    return *this;
  }

  JsonData& operator = ( std::initializer_list< JsonData > rhs )
  {
    resetArray( rhs );
    return *this;
  }


  void reset( type_type type = null_type , void* rhs_ptr = nullptr )
  {
    free();
    type_ = type;
    data_ = rhs_ptr;
  }

#define DEFINE_JSON_RESET_FUNC( funcname , tpname ) \
  template < typename ... Ts > \
  inline JsonData& reset##funcname( Ts&& ... args ) \
  { \
    reset( tpname##_type , new json_##tpname##_t( args... ) ); \
    return *this;\
  }

  DEFINE_JSON_RESET_FUNC( Object , object );
  DEFINE_JSON_RESET_FUNC( Array , array );
  DEFINE_JSON_RESET_FUNC( String , string );
  DEFINE_JSON_RESET_FUNC( Int , int );
  DEFINE_JSON_RESET_FUNC( Number , number );
  DEFINE_JSON_RESET_FUNC( Bool , bool );

#undef DEFINE_JSON_RESET_FUNC

  inline type_type getType() const
  {
    return type_;
  }
  inline bool isNull() const
  {
    return getType() == null_type;
  }

#define DEFINE_JSON_GET_FUNC( funcname , typename ) \
  inline json_##typename##_t& get##funcname() const \
  { return *as_##typename(); }

  DEFINE_JSON_GET_FUNC( Object , object );
  DEFINE_JSON_GET_FUNC( Array ,  array );
  DEFINE_JSON_GET_FUNC( String , string );
  DEFINE_JSON_GET_FUNC( Int , int );
  DEFINE_JSON_GET_FUNC( Number , number );
  DEFINE_JSON_GET_FUNC( Bool , bool );

#undef DEFINE_JSON_GET_FUNC

  bool exists( const char* name ) const
  {
    if( type_ != object_type )
    {
      throw InvalidOperationException( "exist() only valid on object type" );
    }
    return getObject().find( name ) != getObject().end();
  }
  bool exists( std::string const& str ) const
  {
    if( type_ != object_type )
    {
      // throw error 
      // only objecet_type can handle name_access
      throw std::exception();
    }
    return getObject().find( str ) != getObject().end();
  }
  JsonData& operator [] ( const char* name ) const
  {
    if( type_ != object_type )
    {
      throw InvalidOperationException( 
          "operator[ char* ] only valid on object type" 
          );
    }

    return getObject()[ name ];
  }
  JsonData& operator [] ( const std::string& name ) const
  {
    if( type_ != object_type )
    {
      throw InvalidOperationException(
          "operator[ string ] only valid on object type"
          );
    }

    return getObject()[ name ];
  }
  JsonData& operator [] ( size_t i ) const
  {
    if( type_ != array_type )
    {
      throw InvalidOperationException(
          "operator[ size_t ] only valid on array type"
          );
    }

    return getArray()[ i ];
  }
  size_t size() const
  {
    if( type_ != array_type )
    {
      throw InvalidOperationException(
          "size() only valid on array type"
          );
    }
    return getArray().size();
  }

protected:
  type_type type_;
  void *data_;
};



}}
