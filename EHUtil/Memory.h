#pragma once

#include <memory>
#include <iostream>

namespace EH
{
    template < typename T >
    struct Ptr
    {
        using value_type = T;

        template < typename T0 >
        using other_type = Ptr< T0 >;

        using this_type  = other_type< T >;

        value_type *ptr;

        Ptr() :
            ptr( 0 )
        {
        }
        explicit Ptr( std::size_t _count )
        {
            ptr = new value_type[ _count ];
            std::cout << "alloced " << sizeof( value_type ) * _count << " bytes\n";
        }
        Ptr( value_type *rhs )
        {
            ptr = rhs;
        }
        template < typename T2 >
        Ptr( other_type< T2 >&& rhs )
        {
            std::cout << "move\n";
            move_from( rhs );
        }
        ~Ptr()
        {
            release();
        }

        void retain()
        {
        }
        void release()
        {
            if( ptr )
            {
                std::cout << "freed\n";
                delete[] ptr;
                ptr = 0;
            }
        }

        template < typename T2 >
        void move_from( other_type< T2 >& rhs )
        {
            ptr = reinterpret_cast< value_type* >( rhs.ptr );
            rhs.ptr = 0;
        }
        template < typename T2 >
        void copy_from( const other_type< T2 >& rhs )
        {
            ptr = reinterpret_cast< value_type* >( rhs.ptr );
        }

        template < typename T2 >
        this_type& operator = ( other_type< T2 >&& rhs )
        {
            release();
            move_from( rhs );

            return *this;
        }
        this_type& operator = ( value_type *rhs )
        {
            release();
            ptr = rhs;

            return *this;
        }

        value_type* operator -> ()
        {
            return ptr;
        }
        const value_type* operator -> () const
        {
            return ptr;
        }

        value_type* data()
        {
            return ptr;
        }
        const value_type* data() const
        {
            return ptr;
        }

        value_type* operator () ()
        {
            return ptr;
        }
        const value_type* operator () () const
        {
            return ptr;
        }
        value_type* operator () ( std::size_t i )
        {
            return ptr + i;
        }
        const value_type* operator () ( std::size_t i ) const
        {
            return ptr + i;
        }

        value_type& operator [] ( std::size_t i )
        {
            return ptr[ i ];
        }
        const value_type& operator [] ( std::size_t i ) const
        {
            return ptr[ i ];
        }
    };
};
