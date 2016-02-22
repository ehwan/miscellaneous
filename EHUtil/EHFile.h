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
        auto LoadFile( const char *name , std::ios_base::openmode mode , SizeType&& size )
        {
            using remove_ref = typename std::remove_reference< SizeType >::type;
            using remove_con = typename std::remove_const< remove_ref >::type;
            std::ifstream fbuf( name , mode );

            fbuf.seekg( 0 , std::ios_base::end );
            const long sz = fbuf.tellg();
            LOG( "File Load Text : \n" , name , "\n" , sz , " bytes\n" );
            fbuf.seekg( 0 , std::ios_base::beg );

            size = static_cast< remove_con >( sz );

            char *ptr = new char[ sz + 1 ];
            ptr[ sz ] = 0;
            fbuf.read( ptr , sz );
            fbuf.close();

            return Ptr< RET , std::default_delete< RET[] > >( reinterpret_cast< RET* >( ptr ) );
        }


        template < typename RET = char , typename SizeType >
        auto LoadText( const char *name , SizeType&& size )
        {
            return LoadFile< RET >( name , std::ios_base::in , std::forward< SizeType >( size ) );
        }
        template < typename RET = char , typename SizeType >
        auto LoadBinary( const char *name , SizeType&& size )
        {
            return LoadFile< RET >( name , std::ios_base::in | std::ios_base::binary , std::forward< SizeType >( size ) );
        }
    };  // namespace File
};  // namespace EH
