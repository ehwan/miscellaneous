#pragma once

#include "EHLog.h"
#include <memory>
#include <utility>
#include <fstream>

namespace EH
{
    namespace File
    {
        template < typename SZ_TYPE = int >
        static std::unique_ptr< char[] > LoadText( const char *name , SZ_TYPE *size = 0 )
        {
            std::ifstream fbuf( name , std::ios_base::in );

            fbuf.seekg( 0 , std::ios_base::end );
            const long sz = fbuf.tellg();
            LOG( "File Load Text : \n" , name , "\n" , sz , " bytes\n" );
            fbuf.seekg( 0 , std::ios_base::beg );

            if( size ){ *size = ( SZ_TYPE )sz; }

            char *ptr = new char[ sz + 1 ];
            ptr[ sz ] = 0;
            fbuf.read( ptr , sz );
            fbuf.close();

            return std::unique_ptr< char[] >( ptr );
            //return std::move( ptr );
        }


        template < typename SZ_TYPE = int >
        static std::unique_ptr< char[] > LoadBinary( const char *name , SZ_TYPE *size = 0 )
        {
            std::ifstream fbuf( name , std::ios_base::in | std::ios_base::binary );

            fbuf.seekg( 0 , std::ios_base::end );
            const long sz = fbuf.tellg();
            LOG( "File Load Binary : \n" , name , "\n" , sz , " bytes\n" );
            fbuf.seekg( 0 , std::ios_base::beg );

            if( size ){ *size = ( SZ_TYPE )sz; }

            char *ptr = new char[ sz + 1 ];
            ptr[ sz ] = 0;
            fbuf.read( ptr , sz );
            fbuf.close();

            return std::unique_ptr< char[] >( ptr );
        }
    };  // namespace File
};  // namespace EH
