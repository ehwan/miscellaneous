#pragma once

#include "glw.h"
#include <vector>

#ifndef NDEBUG
#else
    #define EH_NO_DEBUG_DRAW
#endif

namespace EH
{
    namespace GL
    {
        namespace Debug
        {
            template < unsigned int N >
            using vecf = Matrix::Vector< GLfloat , N >;

#ifndef EH_NO_DEBUG_DRAW
            void Begin( GLenum mode );
            void Vertex2f( const vecf< 2 >& v );
            void Vertex3f( const vecf< 3 >& v );
            void Vertex2f( std::vector< vecf< 2 > > vs , std::vector< std::size_t > is );
            void Vertex3f( std::vector< vecf< 3 > > vs , std::vector< std::size_t > is );
            void End();
            void SetProjection( const Matrix::Matrix< GLfloat , 4 >& mat );
            void SetColor( const vecf< 4 >& col );
#else
            template < typename CRTP = void >
            void Begin( GLenum mode ){}
            template < typename CRTP = void >
            void Vertex2f( const vecf< 2 >& v ){}
            template < typename CRTP = void >
            void Vertex3f( const vecf< 3 >& v ){}
            template < typename CRTP = void >
            void Vertex2f( std::vector< vecf< 2 > > vs , std::vector< std::size_t > is ){}
            template < typename CRTP = void >
            void Vertex3f( std::vector< vecf< 3 > > vs , std::vector< std::size_t > is ){}
            template < typename CRTP = void >
            void End(){}
            template < typename CRTP = void >
            void SetProjection( const Matrix::Matrix< GLfloat , 4 >& mat ){}
            template < typename CRTP = void >
            void SetColor( const vecf< 4 >& c ){}
#endif
            template < typename IterType >
            void Vertex2f( IterType&& begin , IterType&& end , vecf< 2 > scale = 1.0f , vecf< 2 > offset = 0 )
            {
                for( ; begin != end; ++begin )
                {
                    Vertex2f( vecf< 2 >{ (*begin)[0] , (*begin)[1] } * scale + offset );
                }
            }
            template < typename IterType >
            void Vertex3f( IterType&& begin , IterType&& end , vecf< 3 > scale = 1.0f , vecf< 3 > offset = 0 )
            {
                for( ; begin != end; ++begin )
                {
                    Vertex3f( vecf< 3 >{ (*begin)[0] , (*begin)[1] , (*begin)[2] } * scale + offset );
                }
            }

        };
    };  // namespace GL
}; // namespace EH
