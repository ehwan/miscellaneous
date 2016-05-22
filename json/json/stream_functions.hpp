#pragma once

#include "json.hpp"

namespace eh { namespace json {

std::ostream& operator << ( std::ostream& stream , JsonData const& data )
{
  switch( data.getType() )
  {
  case object_type:
    data.object_stream_out( stream );
    break;
  case array_type:
    data.array_stream_out( stream );
    break;
  case string_type:
    stream << '"' << data.getString() << '"';
    break;
  case int_type:
    stream << data.getInt();
    break;
  case number_type:
    stream << data.getNumber();
    break;
  case bool_type:
    stream << ( data.getBool() ? "true" : "false" );
    break;
  case null_type:
    stream << "null";
    break;
  }

  return stream;
}

void JsonData::object_stream_out( std::ostream& stream ) const
{
  stream << "{ ";

  auto i = getObject().begin();
  auto end   = getObject().end();
  while( i != end )
  {
    auto& data = *i;
    stream << '"' << data.first << '"';
    stream << ':' << data.second;

    if( ++i != end )
    {
      stream << " , ";
    }
  }
  stream << " }";
}
void JsonData::array_stream_out( std::ostream& stream ) const
{
  stream << "[ ";

  auto i = getArray().begin();
  auto end = getArray().end();
  while( i != end )
  {
    stream << *i;

    if( ++i != end )
    {
      stream << " , ";
    }
  }
  stream << " ]";
}

}}
