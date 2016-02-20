#pragma once

#include <memory>
#include <iostream>

namespace EH
{
    template < typename T , typename Deleter = std::default_delete< T[] > >
    class Ptr
    {
    public:
        using value_type = T;
        using const_type = const T;

        using reference_type = value_type&;
        using const_reference_type = const_type&;

        using pointer_type = value_type*;
        using const_pointer_type = const_type*;

    protected:
        using this_type  = Ptr< T , Deleter >;

        pointer_type ptr;
        Deleter deleter;

    public:
        Ptr() :
            ptr( 0 )
        {
        }
        explicit Ptr( std::size_t _count ) :
            ptr( new value_type[ _count ] ) ,
            deleter()
        {
        }
        Ptr( pointer_type rhs ) :
            ptr( rhs ) ,
            deleter()
        {
        }
        Ptr( pointer_type rhs , const Deleter& del ) :
            ptr( rhs ) ,
            deleter( del )
        {
        }
        Ptr( this_type&& rhs )
        {
            move_from( rhs );
        }
        ~Ptr()
        {
            release();
        }
        this_type& operator = ( this_type&& rhs )
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

    protected:
        void retain()
        {
        }
        void release()
        {
            if( ptr )
            {
                deleter( ptr );
                set_zero();
            }
        }

        void move_from( this_type& rhs )
        {
            ptr = rhs.set_zero();
            deleter = std::move( rhs.deleter );
        }
        void copy_from( const this_type& rhs )
        {
            ptr = rhs();
            deleter = rhs.deleter;
        }

    public:
        pointer_type set_zero()
        {
            pointer_type ret = ptr;
            ptr = 0;
            return ret;
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
        pointer_type operator () ( std::size_t i )
        {
            return ptr + i;
        }
        pointer_type operator () ( std::size_t i ) const
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
