#pragma once

#include <memory>
#include <iostream>

namespace EH
{
    template < typename T >
    class Ptr
    {
    public:
        using value_type = T;
        using const_type = const value_type;

        using reference_type = T&;
        using const_reference_type = const_type&;

        using pointer_type = T*;
        using const_pointer_type = const_type*;

    protected:
        template < typename T0 >
        using other_type = Ptr< T0 >;

        using this_type  = other_type< T >;

        pointer_type ptr;

    public:
        Ptr() :
            ptr( 0 )
        {
        }
        explicit Ptr( std::size_t _count )
        {
            ptr = new value_type[ _count ];
        }
        Ptr( pointer_type rhs )
        {
            ptr = rhs;
        }
        template < typename T2 >
        Ptr( other_type< T2 >&& rhs )
        {
            move_from( rhs );
        }
        ~Ptr()
        {
            release();
        }

    protected:
        void retain()
        {
        }
        void release()
        {
            if( ptr )
            {
                delete[] ptr;
                ptr = 0;
            }
        }

        template < typename T2 >
        void move_from( other_type< T2 >& rhs )
        {
            ptr = reinterpret_cast< pointer_type >( rhs() );
            rhs() = 0;
        }
        template < typename T2 >
        void copy_from( const other_type< T2 >& rhs )
        {
            ptr = reinterpret_cast< pointer_type >( rhs() );
        }

    public:
        void reset()
        {
            release();
        }
        template < typename T2 >
        this_type& operator = ( other_type< T2 >&& rhs )
        {
            release();
            move_from( rhs );

            return *this;
        }
        this_type& operator = ( pointer_type rhs )
        {
            release();
            ptr = rhs;

            return *this;
        }

        pointer_type operator -> ()
        {
            return ptr;
        }
        pointer_type operator -> () const
        {
            return ptr;
        }

        pointer_type data()
        {
            return ptr;
        }
        pointer_type data() const
        {
            return ptr;
        }

        pointer_type& operator () ()
        {
            return ptr;
        }
        const pointer_type& operator () () const
        {
            return ptr;
        }
        pointer_type& operator () ( std::size_t i )
        {
            return ptr + i;
        }
        const pointer_type& operator () ( std::size_t i ) const
        {
            return ptr + i;
        }

        reference_type operator [] ( std::size_t i )
        {
            return ptr[ i ];
        }
        const_reference_type operator [] ( std::size_t i ) const
        {
            return ptr[ i ];
        }
    };
};
