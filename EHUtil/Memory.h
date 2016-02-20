#pragma once

#include "../EHLog.h"
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

    template < typename DataType , typename Deleter = std::default_delete< DataType > >
    class shared_obj
    {
    public:
        using this_type = shared_obj< DataType , Deleter >;
        using reference_counter_type = std::uint_fast16_t;
        using data_type = DataType;
        using deleter_type = Deleter;

        using value_type = DataType;
        using reference_type = value_type&;
        using const_type = const value_type;
        using const_reference_type = const_type&;

        shared_obj() :
            ref( 0 ) ,
            data() ,
            deleter()
        {
            _private_load();
        }
        shared_obj( const data_type& d_rhs , const Deleter& del_rhs ) :
            ref( 0 ) ,
            data( d_rhs ) ,
            deleter( del_rhs )
        {
            _private_load();
        }
        shared_obj( const data_type& d_rhs ) :
            ref( 0 ) ,
            data( d_rhs ) ,
            deleter()
        {
            _private_load();
        }

        shared_obj( const this_type& rhs ) :
            ref( rhs.ref ) ,
            data( rhs.data ) ,
            deleter( rhs.deleter )
        {
            _private_retain();
        }
        shared_obj( this_type&& rhs ) :
            ref( rhs.ref ) ,
            data( rhs.data ) ,
            deleter( rhs.deleter )
        {
            rhs.ref = 0;
        }
        ~shared_obj()
        {
            _private_release();
        }

        this_type& operator = ( const this_type& rhs )
        {
            _private_release();
            ref = rhs.ref;
            data = rhs.data;

            _private_retain();

            return *this;
        }
        this_type& operator = ( this_type&& rhs )
        {
            _private_release();
            ref = rhs.ref;
            data = rhs.data;

            rhs.ref = 0;

            return *this;
        }

        reference_type operator () ()
        {
            return data;
        }
        const_reference_type operator () () const
        {
            return data;
        }

        template < typename SFINE = data_type >
        decltype( std::declval< SFINE >().operator[]( 0 ) )
        operator [] ( std::size_t i )
        {
            return data[ i ];
        }
        template < typename SFINE = data_type >
        decltype( std::declval< const SFINE >().operator[]( 0 ) )
        operator [] ( std::size_t i ) const
        {
            return data[ i ];
        }

        inline reference_counter_type reference_count() const
        {
#ifndef NDEBUG
            if( ref == 0 )
            {
                ERROR( "try to get reference count on unloaded shared_obj" );
            }
#endif
            return *ref;
        }


    protected:
        reference_counter_type *ref;
        data_type data;
        deleter_type deleter;

        inline void _private_load()
        {
#ifndef NDEBUG
            if( ref )
            {
                ERROR( "shared_ptr already loaded" );
            }
#endif
            ref = new reference_counter_type( 1 );
        }
        inline void _private_retain()
        {
            if( ref )
            {
                ++( *ref );
            }
        }
        inline void _private_release()
        {
            if( ref )
            {
                if( (--( *ref )) == 0 )
                {
                    delete ref;
                    deleter( data );
                    // delete segments
                }
                ref = 0;
            }
        }
    };
};
