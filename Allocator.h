#pragma once

#include <utility>
#include <type_traits>
#include <typeinfo>
#include "EHLog.h"
#include "static_sequence/static_sequence.h"

namespace EH
{
    struct Allocator
    {
        char *memory;

        std::size_t pos;
        std::size_t size;

        Allocator( std::size_t mem_size )
        {
            alloc( mem_size );
        }
        Allocator()
        {
            memory = 0;
            pos    = 0;
            size   = 0;
        }
        Allocator( Allocator&& from )
        {
            move( from );
        }
        ~Allocator()
        {
            if( memory )
            {
                free();
            }
        }

        void alloc( std::size_t m )
        {
            LOG( "alloc memory : " , m , " bytes" );
            memory = new char[ m ];
            pos    = 0;
            size   = m;
        }
        void free()
        {
            LOG( "free memory" );
            delete[] memory;
            memory = 0;
            pos    = 0;
            size   = 0;
        }
        void copy( const Allocator &from )
        {
            memory = from.memory;
            pos    = from.pos;
            size   = from.size;
        }
        void move( Allocator &from )
        {
            LOG( "move memory" );
            copy( from );
            from.memory = 0;
            from.pos    = 0;
            from.size   = 0;
        }

        void operator = ( Allocator&& from )
        {
            if( memory )
            {
               free();
            }
            move( from );
        }

        template < typename T = char >
        T *malloc( std::size_t n )
        {
           LOG( "malloc " , sizeof( T ) * n , " to " , typeid( T ).name() );
           T *ret = reinterpret_cast< T* >( memory + pos );
           pos += sizeof( T ) * n;
#ifndef NDEBUG
           if( pos > size ){ ERROR( "memory alloc overflow" ); }
#endif
           return ret;
        }

        template < typename T0 >
        typename std::enable_if<
            std::is_pointer< T0 >::value
        >::type
        malloc_pointers( std::size_t n , T0& arg0 )
        {
            using c_type = typename std::pointer_traits< T0 >::element_type;
            arg0 = malloc< c_type >( n );
        }
        template < typename T0 , typename ... Ts >
        typename std::enable_if<
            static_sequence< bool , std::is_pointer< Ts >::value... >::all() &&
            std::is_pointer< T0 >::value
        >::type
        malloc_pointers( std::size_t n , T0& arg0 , Ts& ... args )
        {
            using c_type = typename std::pointer_traits< T0 >::element_type;
            arg0 = malloc< c_type >( n );
            malloc_pointers( n , args... );
        }
    };
};
