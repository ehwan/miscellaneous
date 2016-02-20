#pragma once

#include "../EHLog.h"
#include "Memory.h"
#include <memory>
#include <utility>
#include <fstream>

namespace EH
{
    namespace File
    {
        template < typename RET = char , typename SizeType >
        Ptr< RET , std::default_delete< RET[] > > LoadFile( const char *name , std::ios_base::openmode mode , SizeType size )
        {
            std::ifstream fbuf( name , mode );

            fbuf.seekg( 0 , std::ios_base::end );
            const long sz = fbuf.tellg();
            LOG( "File Load Text : \n" , name , "\n" , sz , " bytes\n" );
            fbuf.seekg( 0 , std::ios_base::beg );

            size = static_cast< SizeType >( sz );

            char *ptr = new char[ sz + 1 ];
            ptr[ sz ] = 0;
            fbuf.read( ptr , sz );
            fbuf.close();

            return Ptr< RET >( reinterpret_cast< RET* >( ptr ) );
        }


        template < typename RET = char , typename SizeType >
        Ptr< RET , std::default_delete< RET[] > > LoadText( const char *name , SizeType size )
        {
            return LoadFile< RET >( name , std::ios_base::in , size );
        }
        template < typename RET = char , typename SizeType >
        Ptr< RET , std::default_delete< RET[] > > LoadBinary( const char *name , SizeType size )
        {
            return LoadFile< RET >( name , std::ios_base::in | std::ios_base::binary , size );
        }
    };  // namespace File
};  // namespace EH
