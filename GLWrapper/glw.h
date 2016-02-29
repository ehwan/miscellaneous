#pragma once

//#include <GL/gl.h>
//#include <GLES2/gl2.h>
#include "../EHLog.h"
#include "../EHMatrix/EHMatrix.h"
#include "../EHUtil/Memory.h"

#include "gl_core_3_3.h"

#include <unordered_map>
#include <vector>
#include <cassert>

#ifndef EH_NO_LODEPNG
#define LODEPNG_NO_COMPILE_ENCODER
#define LODEPNG_NO_COMPILE_DISK
#define LODEPNG_NO_COMPILE_ERROR_TEXT
#define LODEPNG_NO_COMPILE_CPP
#include "../lodepng/lodepng.h"
#endif

#ifdef NDEBUG
    #define EH_GL_NO_LOG
    #define EH_GL_NO_DEBUG
#endif

namespace EH
{
    namespace GL
    {
        template < typename VecType , std::size_t Align >
        struct alignas( Align ) eh_vec_type : VecType
        {
            using VecType::VecType;
            using VecType::operator=;
        };

        using vec2f = Matrix::vec2< GLfloat >;
        using vec3f = Matrix::vec3< GLfloat >;
        using vec4f = Matrix::vec4< GLfloat >;

        using vec2i = Matrix::vec2< GLint >;
        using vec3i = Matrix::vec3< GLint >;
        using vec4i = Matrix::vec4< GLint >;

        using vec2ui = Matrix::vec2< GLuint >;
        using vec3ui = Matrix::vec3< GLuint >;
        using vec4ui = Matrix::vec4< GLuint >;

        using eh_float  = GLfloat;
        using eh_float2 = eh_vec_type< vec2f , sizeof( eh_float ) * 2 >;
        using eh_float3 = eh_vec_type< vec3f , sizeof( eh_float ) * 4 >;
        using eh_float4 = eh_vec_type< vec4f , sizeof( eh_float ) * 4 >;

        using eh_int  = GLint;
        using eh_int2 = eh_vec_type< vec2i , sizeof( eh_int ) * 2 >;
        using eh_int3 = eh_vec_type< vec3i , sizeof( eh_int ) * 4 >;
        using eh_int4 = eh_vec_type< vec4i , sizeof( eh_int ) * 4 >;

        using eh_uint  = GLuint;
        using eh_uint2 = eh_vec_type< vec2ui , sizeof( eh_uint ) * 2 >;
        using eh_uint3 = eh_vec_type< vec3ui , sizeof( eh_uint ) * 4 >;
        using eh_uint4 = eh_vec_type< vec4ui , sizeof( eh_uint ) * 4 >;

        static inline void LoadGLFunctions()
        {
            if( ogl_LoadFunctions() == ogl_LOAD_FAILED )
            {
                ERROR( "glLoadGen Error" );
                std::exit( 1 );
            }
            LOG( "OpenGL Version : " , glGetString( GL_VERSION ) );
            LOG( "GLSL Version : " , glGetString( GL_SHADING_LANGUAGE_VERSION ) );
        }
        struct create_flag_t{};
        constexpr static create_flag_t create_flag;

        template < typename Handler , typename CRTP >
        struct GLObject
        {
            using value_type = Handler;
            using this_type = GLObject< Handler , CRTP >;
            using reference_type = std::uint_fast8_t;
            using handler_type = Handler;

            handler_type handler;
            reference_type *ref;

            GLObject() :
                handler( 0 ) , ref( 0 )
            {
            }
            GLObject( const this_type& rhs ) :
                handler( rhs.handler ) , ref( rhs.ref )
            {
                _private_retain();
            }
            GLObject( this_type&& rhs ) :
                handler( rhs.handler ) , ref( rhs.ref )
            {
                rhs.handler = 0;
                rhs.ref     = 0;
            }

            ~GLObject()
            {
                _private_release();
            }

            CRTP& operator = ( const this_type& rhs )
            {
                _private_release();
                handler = rhs.handler;
                ref     = rhs.ref;
                _private_retain();

                return static_cast< CRTP& >( *this );
            }
            CRTP& operator = ( this_type&& rhs )
            {
                _private_release();
                handler = rhs.handler;
                ref     = rhs.ref;
                rhs.handler = 0;
                rhs.ref     = 0;

                return static_cast< CRTP& >( *this );
            }

            inline void load()
            {
                assert( ref == 0 );
                LOG( "GLObject" , " Construct" );
                ref = new reference_type( 1 );
            }
            inline void _private_retain()
            {
                static_cast< CRTP& >( *this ).retain();
            }
            void _private_release()
            {
                if( ref )
                {
                    if( (--( *ref )) == 0 )
                    {
                        LOG( "GLObject" , " Destruct" );
                        delete ref;
                        static_cast< CRTP& >( *this ).release();
                    }
                    handler = 0;
                    ref     = 0;
                }
            }
            void retain()
            {
                if( ref )
                {
                    ++( *ref );
                }
            }
            void release()
            {
            }
            inline operator CRTP& ()
            {
                return static_cast< CRTP& >( *this );
            }
            inline operator const CRTP& () const
            {
                return static_cast< const CRTP& >( *this );
            }
            inline handler_type operator () () const
            {
                return handler;
            }

            this_type&& Move()
            {
                return std::move( *this );
            }
        };

        static const char* GetErrorName( GLenum err )
        {
#ifndef EH_GL_NO_LOG
            switch( err )
            {
                case GL_NO_ERROR:
                    return "GL_NO_ERROR";
                case GL_INVALID_ENUM:
                    return "GL_INVALID_ENUM";
                case GL_INVALID_VALUE:
                    return "GL_INVALID_VALUE";
                case GL_INVALID_OPERATION:
                    return "GL_INVALID_OPERATION";
                case GL_OUT_OF_MEMORY:
                    return "GL_OUT_OF_MEMORY";
                default:
                    return "NAN";
            }
#else
            return "";
#endif
        }
        template < typename ... Ts >
        void CheckError( Ts&& ... args )
        {
            GLenum err;
            while( ( err=glGetError() ) != GL_NO_ERROR )
            {
                ERROR( std::forward< Ts >( args )... , " " , GetErrorName( err ) , " : " , err );
            }
        }

        static inline void DrawArrays( GLenum mode , GLint first , GLsizei count )
        {
            glDrawArrays( mode , first , count );
            CheckError( "glDrawArrays" );
        }
        static inline void DrawArraysInstanced( GLenum mode , GLint first , GLsizei count , GLsizei primcount )
        {
            glDrawArraysInstanced( mode , first , count , primcount );
            CheckError( "glDrawArraysInstanced" );
        }
        static inline void DrawElements( GLenum mode , GLsizei count , GLenum type , std::size_t offset )
        {
            glDrawElements( mode , count , type , ( const GLvoid* )offset );
            CheckError( "glDrawElements" );
        }
        static inline void DrawElementsInstanced( GLenum mode , GLsizei count , GLenum type , std::size_t offset , GLsizei primcount )
        {
            glDrawElementsInstanced( mode , count , type , ( const GLvoid* )offset , primcount );
            CheckError( "glDrawElementsInstanced" );
        }

        template < GLenum ShaderType >
        struct Shader : GLObject< GLuint , Shader< ShaderType > >
        {
            constexpr static const char* ShaderNameFromType()
            {
                return ShaderType == GL_VERTEX_SHADER ?
                            "VertexShader" :
                            ShaderType == GL_FRAGMENT_SHADER ?
                                "FragmentShader" :
                                ShaderType == GL_GEOMETRY_SHADER ?
                                    "GeometryShader" :
                                    "UnknownShader" ;
            }
            using parent = GLObject< GLuint , Shader< ShaderType > >;
            using parent::parent;
            using parent::operator=;
            using parent::handler;

            void release()
            {
                glDeleteShader( handler );
                CheckError( "glDeleteShader" );
            }
            Shader() :
                parent()
            {
            }
            Shader( const Shader< ShaderType >& rhs ) :
                parent( rhs )
            {
            }
            Shader( Shader< ShaderType >&& rhs ) :
                parent( rhs )
            {
            }

            Shader< ShaderType >& operator = ( const Shader< ShaderType >& rhs )
            {
                return parent::operator = ( rhs );
            }
            Shader< ShaderType >& operator = ( Shader< ShaderType >&& rhs )
            {
                return parent::operator = ( rhs );
            }

            explicit Shader( std::vector< const GLchar* > strings ) :
                parent()
            {
                handler = glCreateShader( ShaderType );
                this->load();
                CheckError( "glCreateShader 1" );
                glShaderSource( handler , strings.size() , strings.data() , 0 );
                CheckError( "glShaderSource 1" );
                glCompileShader( handler );
                CheckError( "glCompileShader 1" );
                CheckCompile();
            }
            explicit Shader( const GLchar *source ) :
                parent()
            {
                handler = glCreateShader( ShaderType );
                this->load();
                CheckError( "glCreateShader 2" );
                glShaderSource( handler , 1 , &source , 0 );
                CheckError( "glShaderSource 2" );
                glCompileShader( handler );
                CheckError( "glCompileShader 2" );
                CheckCompile();
            }
            void CheckCompile()
            {
                std::string shadername = ShaderNameFromType();

                GLint status;
                glGetShaderiv( handler , GL_COMPILE_STATUS , &status );
                if( status == GL_TRUE )
                {
                    LOG( shadername , " compile : " , "SUCCESS" );
                }else
                {
                    LOG( shadername , " compile : " , "Fail" );
                    GLint buflen;
                    glGetShaderiv( handler , GL_INFO_LOG_LENGTH , &buflen );
                    std::vector< GLchar > info_string( buflen+1 );
                    glGetShaderInfoLog( handler , buflen , 0 , info_string.data() );
                    info_string.back() = 0;

                    ERROR( shadername , " Log : \n" , info_string.data() );
                }
            }
        };
        using VertexShader = Shader< GL_VERTEX_SHADER >;
        using FragmentShader = Shader< GL_FRAGMENT_SHADER >;
        using GeometryShader = Shader< GL_GEOMETRY_SHADER >;


        struct Program : GLObject< GLuint , Program >
        {
#ifndef EH_GL_NO_DEBUG
            std::unordered_map< GLuint , std::string > _attributes_map;
            std::unordered_map< GLuint , std::string > _fragout_map;
#endif

            using parent = GLObject< GLuint , Program >;
            using parent::parent;
            using parent::operator=;
            using parent::handler;
            void release()
            {
                glDeleteProgram( handler );
                CheckError( "glDeleteProgram" );
            }
            Program() :
                parent()
            {
            }
            Program( const Program& rhs ) :
                parent( rhs )
#ifndef EH_GL_NO_DEBUG
                , _attributes_map( rhs._attributes_map )
                , _fragout_map   ( rhs._fragout_map )
#endif
            {
            }
            Program( Program&& rhs ) :
                parent( rhs )
#ifndef EH_GL_NO_DEBUG
                , _attributes_map( std::move( rhs._attributes_map ) )
                , _fragout_map   ( std::move( rhs._fragout_map ) )
#endif
            {
            }


            template < GLenum Type0 , GLenum ... Types >
            explicit Program( const Shader< Type0 >& shader0 , const Shader< Types >& ... shaders ) :
                parent()
            {
                handler = glCreateProgram();
                this->load();
                GLuint m_shaders[] = { shader0() , shaders() ... };
                for( auto shader : m_shaders )
                {
                    glAttachShader( handler , shader );
                    CheckError( "glAttachShader" );
                }
            }

            Program& operator = ( const Program& rhs )
            {
#ifndef EH_GL_NO_DEBUG
                _attributes_map = rhs._attributes_map;
                _fragout_map    = rhs._fragout_map;
#endif
                return parent::operator = ( rhs );
            }
            Program& operator = ( Program&& rhs )
            {
#ifndef EH_GL_NO_DEBUG
                _attributes_map = std::move( rhs._attributes_map );
                _fragout_map    = std::move( rhs._fragout_map );
#endif
                return parent::operator = ( rhs );
            }

            void Link()
            {
                glLinkProgram( handler );
                CheckError( "glLinkProgram" );
            }

            auto GetUniformLocation( const GLchar* name ) const
            {
                auto ret = glGetUniformLocation( handler , name );
                CheckError( "glGetUniformLocation : " , name );
                return ret;
            }
            auto GetAttribLocation( const GLchar* name ) const
            {
                auto ret = glGetAttribLocation( handler , name );
                CheckError( "glGetAttribLocation : " , name );
                return ret;
            }
            auto GetUniformBlockIndex( const GLchar *name ) const
            {
                auto ret = glGetUniformBlockIndex( handler , name );
                CheckError( "glGetUniformBlockIndex : " , name );
                return ret;
            }
            void BindUniformBlock( GLuint index , const GLchar *name )
            {
                glUniformBlockBinding( handler ,
                                       GetUniformBlockIndex( name ) ,
                                       index );
                CheckError( "glUniformBlockBinding" );
            }
            void BindAttribLocation( GLuint index , const GLchar *name )
            {
            #ifndef EH_GL_NO_DEBUG
                auto state = _attributes_map.emplace( index , name );
                if( state.second == false )
                {
                    ERROR( "Invalid attribute binding : " , name , " at " , index );
                    ERROR( "Already binded with : " , _attributes_map.find( index )->second );
                }
            #endif
                glBindAttribLocation( handler , index,  name );
                CheckError( "glBindAttribLocation : " , name );
            }
            void BindAttribLocation( std::initializer_list< const char* > names )
            {
                GLuint id = 0;
                for( auto i=names.begin(); i!=names.end(); ++i )
                {
                    BindAttribLocation( id++ , *i );
                }
            }
            void BindFragDataLocation( GLuint index , const GLchar *name )
            {
            #ifndef EH_GL_NO_DEBUG
                auto state = _fragout_map.emplace( index , name );
                if( state.second == false )
                {
                    ERROR( "Invalid FragData binding : " , name , " at " , index );
                    ERROR( "Already binded with : " , _fragout_map.find( index )->second );
                }
            #endif
                glBindFragDataLocation( handler , index , name );
                CheckError( "glBindFragDataLocation : " , name );
            }
            void BindFragDataLocation( std::initializer_list< const char* > names )
            {
                GLuint id = 0;
                for( auto i=names.begin(); i!=names.end(); ++i )
                {
                    BindFragDataLocation( id++ , *i );
                }
            }
            void TransformFeedbackVaryings( std::initializer_list< const char* > names , GLenum bufmode = GL_INTERLEAVED_ATTRIBS ) const
            {
                glTransformFeedbackVaryings( handler , (GLsizei)names.size() , (const char **)names.begin() , bufmode );
                CheckError( "glTransformFeedbackVaryings" );
            }

            void SetTextureSamplers( std::initializer_list< const char* > names ) const
            {
                auto iter = names.begin();
                for( decltype( names )::size_type i=0; i<names.size(); ++i )
                {
                    SetUniform( *(iter++) , (int)i );
                }
            }
            inline void ActiveTexture( GLuint id ) const
            {
                glActiveTexture( GL_TEXTURE0 + id );
                CheckError( "glActiveTexture : " , id );
            }
            inline void BindTexture( GLuint texture , GLenum target = GL_TEXTURE_2D ) const
            {
                glBindTexture( target , texture );
                CheckError( "glBindTexture : TEXTURE" , texture );
            }
            inline void BindTextureTo( GLuint texture , GLuint id , GLenum target = GL_TEXTURE_2D ) const
            {
                ActiveTexture( id );
                BindTexture( texture , target );
            }

            inline void Use() const
            {
                glUseProgram( handler );
                CheckError( "glUseProgram : Set" );
            }
            inline void UnUse() const
            {
                glUseProgram( 0 );
                CheckError( "glUseProgram : Reset" );
            }


            inline void SetUniform( const char *name , float v1 ) const
            {
                glUniform1f( GetUniformLocation( name ) , v1 );
                CheckError( "glUniform1f : " , name );
            }
            inline void SetUniform( const char *name , float v1 , float v2 ) const
            {
                glUniform2f( GetUniformLocation( name ) , v1 , v2 );
                CheckError( "glUniform2f : " , name );
            }
            inline void SetUniform( const char *name , float v1 , float v2 , float v3 ) const
            {
                glUniform3f( GetUniformLocation( name ) , v1 , v2 , v3 );
                CheckError( "glUniform3f : " , name );
            }
            inline void SetUniform( const char *name , float v1 , float v2 , float v3 , float v4 ) const
            {
                glUniform4f( GetUniformLocation( name ) , v1 , v2 , v3 , v4 );
                CheckError( "glUniform4f : " , name );
            }
            inline void SetUniform1v( const char *name , const GLfloat *ptr , GLsizei count = 1 )
            {
                glUniform1fv( GetUniformLocation( name ) , count , ptr );
                CheckError( "glUniform1fv" );
            }
            inline void SetUniform2v( const char *name , const GLfloat *ptr , GLsizei count = 1 )
            {
                glUniform2fv( GetUniformLocation( name ) , count , ptr );
                CheckError( "glUniform2fv" );
            }
            inline void SetUniform3v( const char *name , const GLfloat *ptr , GLsizei count = 1 )
            {
                glUniform3fv( GetUniformLocation( name ) , count , ptr );
                CheckError( "glUniform3fv" );
            }
            inline void SetUniform4v( const char *name , const GLfloat *ptr , GLsizei count = 1 )
            {
                glUniform4fv( GetUniformLocation( name ) , count , ptr );
                CheckError( "glUniform4fv" );
            }
            inline void SetUniformMatrix2( const char *name , const GLfloat *mat ) const
            {
                glUniformMatrix2fv( GetUniformLocation( name ) , 1 , GL_FALSE , mat );
                CheckError( "glUniformMatrix2fv : " , name );
            }
            inline void SetUniformMatrix3( const char *name , const GLfloat *mat ) const
            {
                glUniformMatrix3fv( GetUniformLocation( name ) , 1 , GL_FALSE , mat );
                CheckError( "glUniformMatrix3fv : " , name );
            }
            inline void SetUniformMatrix4( const char *name , const GLfloat *mat ) const
            {
                glUniformMatrix4fv( GetUniformLocation( name ) , 1 , GL_FALSE , mat );
                CheckError( "glUniformMatrix4fv : " , name );
            }
            inline void SetUniformi( const char *name , int v1 ) const
            {
                glUniform1i( GetUniformLocation( name ) , v1 );
                CheckError( "glUniform1f : " , name );
            }
            inline void SetUniformi( const char *name , int v1 , int v2 ) const
            {
                glUniform2i( GetUniformLocation( name ) , v1 , v2 );
                CheckError( "glUniform2f : " , name );
            }
            inline void SetUniformi( const char *name , int v1 , int v2 , int v3 ) const
            {
                glUniform3i( GetUniformLocation( name ) , v1 , v2 , v3 );
                CheckError( "glUniform3f : " , name );
            }
            inline void SetUniformi( const char *name , int v1 , int v2 , int v3 , int v4 ) const
            {
                glUniform4i( GetUniformLocation( name ) , v1 , v2 , v3 , v4 );
                CheckError( "glUniform4f : " , name );
            }
        };

        struct VertexArray : GLObject< GLuint , VertexArray >
        {
            using parent = GLObject< GLuint , VertexArray >;
            using parent::parent;
            using parent::operator=;
            using parent::handler;
            void release()
            {
                glDeleteVertexArrays( 1 , &handler );
                CheckError( "glDeleteVertexArray" );
            }
            VertexArray() :
                parent()
            {
            }
            VertexArray( const VertexArray& rhs ) :
                parent( rhs )
            {
            }
            VertexArray( VertexArray&& rhs ) :
                parent( rhs )
            {
            }

            VertexArray( create_flag_t create ) :
                parent()
            {
                glGenVertexArrays( 1 , &handler );
                this->load();
                CheckError( "glGenVertexArrays" );
            }

            VertexArray& operator = ( const VertexArray& rhs )
            {
                return parent::operator = ( rhs );
            }
            VertexArray& operator = ( VertexArray&& rhs )
            {
                return parent::operator = ( rhs );
            }
            void Begin() const
            {
                glBindVertexArray( handler );
                CheckError( "glBindVertexArray : " , handler );
            }
            void End() const
            {
                glBindVertexArray( 0 );
                CheckError( "glBindVertexArray : 0" );
            }
        };
        struct Vertex
        {
            GLuint handler;

            Vertex( GLuint v ) :
                handler( v )
            {
            }

            inline const Vertex& EnableVertexAttribArray() const
            {
                glEnableVertexAttribArray( handler );
                CheckError( "glEnableVertexAttribArray : " , handler );

                return *this;
            }
            inline const Vertex& DisableVertexAttribArray() const
            {
                glDisableVertexAttribArray( handler );
                CheckError( "glDisableVertexAttribArray : " , handler );

                return *this;
            }
            inline const Vertex& VertexAttribDivisor( GLuint divisor = 0 ) const
            {
                glVertexAttribDivisor( handler , divisor );
                CheckError( "glVertexAttribDivisor : " , handler );

                return *this;
            }
            inline const Vertex& VertexAttribPointer( GLint size , GLenum type , GLsizei stride , std::size_t offset ) const
            {
                glVertexAttribPointer( handler , size , type , GL_FALSE , stride , ( const GLvoid* )offset );
                CheckError( "glVertexAttribPointer : " , handler );

                return *this;
            }
        };


        template < GLenum Target >
        struct Buffer : GLObject< GLuint , Buffer< Target > >
        {
            using parent = GLObject< GLuint , Buffer< Target > >;
            using parent::parent;
            using parent::operator=;
            using parent::handler;

            size_t size;

            void release()
            {
                glDeleteBuffers( 1 , &handler );
                CheckError( "glDeleteBuffers" );
            }
            Buffer() :
                parent() ,
                size( 0 )
            {
            }
            Buffer( const Buffer< Target >& rhs ) :
                parent( rhs ) ,
                size( rhs.size )
            {
            }
            Buffer( Buffer< Target >&& rhs ) :
                parent( rhs ) ,
                size( rhs.size )
            {
                rhs.size = 0;
            }

            explicit Buffer( create_flag_t create ) :
                parent() ,
                size( 0 )
            {
                glGenBuffers( 1 , &handler );
                this->load();
            }

            Buffer< Target >& operator = ( const Buffer< Target >& rhs )
            {
                size = rhs.size;
                return parent::operator = ( rhs );
            }
            Buffer< Target >& operator = ( Buffer< Target >&& rhs )
            {
                size = rhs.size;
                rhs.size = 0;
                return parent::operator = ( rhs );
            }


            inline void Begin() const
            {
                glBindBuffer( Target , handler );
                CheckError( "glBindBuffer_Begin" );
            }
            inline void End() const
            {
                glBindBuffer( Target , 0 );
                CheckError( "glBindBuffer_End" );
            }
            /*
                STREAM
                The data store contents will be modified once and used at most a few times.
                STATIC
                The data store contents will be modified once and used many times.
                DYNAMIC
                The data store contents will be modified repeatedly and used many times.


                DRAW
                The data store contents are modified by the application, and used as the source for GL drawing and image specification commands
                READ
                The data store contents are modified by reading data from the GL, and used to return that data when queried by the application
                COPY
                The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands
            */
            void BufferData( GLsizei _size , GLenum usage , GLvoid *data = 0 )
            {
                size = _size;
                glBufferData( Target , size , data , usage );
                CheckError( "glBufferData" );
            }
            void SubData( GLsizei offset , GLsizei _size , GLvoid *data )
            {
                glBufferSubData( Target , offset , _size , data );
                CheckError( "glBufferSubData" );
            }
            struct map_deleter_s
            {
                inline void operator () ( void *ptr )
                {
                    glUnmapBuffer( Target );
                }
            };
            /*
            GL_READ_ONLY 0x88B8
            GL_WRITE_ONLY 0x88B9
            GL_READ_WRITE 0x88BA
            */
            template < typename T >
            inline Ptr< T , map_deleter_s > MapBuffer( GLenum access ) const
            {
                void *ret = glMapBuffer( Target , access );
                CheckError( "glMapBuffer" );
                return Ptr< T , map_deleter_s >( reinterpret_cast< T* >( ret ) );
            }

            void BindBuffeRange( GLenum _target , GLuint index , GLintptr offset , GLsizei _size ) const
            {
                glBindBufferRange( _target , index , handler , offset , _size );
                CheckError( "glBindBufferRange" );
            }
            void BindBufferBase( GLenum _target , GLuint index ) const
            {
                glBindBufferBase ( _target , index , handler );
                CheckError( "glBindBufferBase" );
            }
        };
        using VertexBuffer = Buffer< GL_ARRAY_BUFFER >;
        using IndexBuffer  = Buffer< GL_ELEMENT_ARRAY_BUFFER >;
        using UniformBuffer = Buffer< GL_UNIFORM_BUFFER >;



        struct RenderBuffer : GLObject< GLuint , RenderBuffer >
        {
            using parent = GLObject< GLuint , RenderBuffer >;
            using parent::parent;
            using parent::operator=;
            using parent::handler;
            void release()
            {
                glDeleteRenderbuffers( 1 , &handler );
                CheckError( "glDeleteRenderbuffers" );
            }
            RenderBuffer() :
                parent()
            {
            }
            RenderBuffer( const RenderBuffer& rhs ) :
                parent( rhs )
            {
            }
            RenderBuffer( RenderBuffer&& rhs ) :
                parent( rhs )
            {
            }

            /*
            where # is the bit size of stencil buffer
            GL_STENCIL_INDEX#
            GL_DEPTH_COMPONENT#
            GL_DEPTH#_STENCIL#
            */
            explicit RenderBuffer( GLsizei width , GLsizei height , GLenum internalformat ) :
                parent()
            {
                glGenRenderbuffers( 1 , &handler );
                this->load();
                CheckError( "glGenRednerbuffers" );
                glBindRenderbuffer( GL_RENDERBUFFER , handler );
                CheckError( "glBindRenderbuffer" );
                glRenderbufferStorage( GL_RENDERBUFFER , internalformat , width , height );
                CheckError( "glRenderbufferStorage" );
                glBindRenderbuffer( GL_RENDERBUFFER , 0 );
                CheckError( "glBindRenderbuffer_0" );
            }

            RenderBuffer& operator = ( const RenderBuffer& rhs )
            {
                return parent::operator = ( rhs );
            }
            RenderBuffer& operator = ( RenderBuffer&& rhs )
            {
                return parent::operator = ( rhs );
            }

            inline void Begin() const
            {
                glBindRenderbuffer( GL_RENDERBUFFER , handler );
                CheckError( "glBindRenderbuffer" );
            }
            inline void End() const
            {
                glBindRenderbuffer( GL_RENDERBUFFER , 0 );
                CheckError( "glBindRenderbuffer_0" );
            }
        };  // struct RenderBuffer

        struct FrameBuffer : GLObject< GLuint , FrameBuffer >
        {
            using parent = GLObject< GLuint , FrameBuffer >;
            using parent::parent;
            using parent::operator=;
            using parent::handler;
            void release()
            {
                glDeleteFramebuffers( 1 , &handler );
                CheckError( "glDeleteFramebuffers" );
            }
            FrameBuffer() :
                parent()
            {
            }
            FrameBuffer( const FrameBuffer& rhs ) :
                parent( rhs )
            {
            }
            FrameBuffer( FrameBuffer&& rhs ) :
                parent( rhs )
            {
            }

            FrameBuffer( create_flag_t create ) :
                parent()
            {
                glGenFramebuffers( 1 , &handler );
                this->load();
                CheckError( "glGenFramebuffer" );
            }

            FrameBuffer& operator = ( const FrameBuffer& rhs )
            {
                return parent::operator = ( rhs );
            }
            FrameBuffer& operator = ( FrameBuffer&& rhs )
            {
                return parent::operator = ( rhs );
            }

            void CheckAttachment()
            {
                /*
                GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
            Not all framebuffer attachment points are framebuffer attachment complete. This means that at least one attachment point with a renderbuffer or texture attached has its attached object no longer in existence or has an attached image with a width or height of zero, or the color attachment point has a non-color-renderable image attached, or the depth attachment point has a non-depth-renderable image attached, or the stencil attachment point has a non-stencil-renderable image attached.

            Color-renderable formats include GL_RGBA4, GL_RGB5_A1, and GL_RGB565. GL_DEPTH_COMPONENT16 is the only depth-renderable format. GL_STENCIL_INDEX8 is the only stencil-renderable format.

            GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
            Not all attached images have the same width and height.

            GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
            No images are attached to the framebuffer.

            GL_FRAMEBUFFER_UNSUPPORTED
            The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.
                */
                GLenum st = glCheckFramebufferStatus( GL_FRAMEBUFFER );
                switch( st )
                {
                case GL_FRAMEBUFFER_COMPLETE:
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    EH::ERROR( "FrameBuffer : INCOMPLETE_ATTACHMENT" );
                    break;
                //case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                //  EH::ERROR( "FrameBuffer : INCOMPLETE_DIMENSIONS" );
                //	break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    EH::ERROR( "FrameBuffer : MISSING_ATTACHMENT" );
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    EH::ERROR( "FrameBuffer : UNSUPPORTED" );
                    break;
                }
                CheckError( "Framebuffer ErrorCheck" );
            }
            void AttachTexture( GLuint texture , GLenum attachment = GL_COLOR_ATTACHMENT0 )
            {
                glFramebufferTexture2D( GL_FRAMEBUFFER , attachment , GL_TEXTURE_2D , texture , 0 );
                CheckError( "glFramebufferTexture2D" );
                CheckAttachment();
            }
            void AttachRenderBuffer( GLuint renderbuf , GLenum attachment )
            {
                glFramebufferRenderbuffer( GL_FRAMEBUFFER , attachment , GL_RENDERBUFFER , renderbuf );
                CheckError( "glFramebufferRenderbuffer" );
                CheckAttachment();
            }
            inline void Begin() const
            {
                glBindFramebuffer( GL_FRAMEBUFFER , handler );
                CheckError( "glBindFramebuffer" );
            }
            inline void End() const
            {
                glBindFramebuffer( GL_FRAMEBUFFER , 0 );
                CheckError( "glBindFramebuffer" );
            }

            void DrawBuffers( std::initializer_list< GLenum > args ) const
            {
                glDrawBuffers( args.size() , (const GLenum*)args.begin() );
                CheckError( "glDrawBuffers" );
            }
        };  // struct Framebuffer

        struct Texture : GLObject< GLuint , Texture >
        {
            Matrix::vec2< GLsizei > size;

            using parent = GLObject< GLuint , Texture >;
            using parent::parent;
            using parent::operator=;
            using parent::handler;
            void release()
            {
                glDeleteTextures( 1 , &handler );
                CheckError( "glDeleteFramebuffers" );
            }
            Texture() :
                parent() ,
                size( 0 )
            {
            }
            Texture( const Texture& rhs ) :
                parent( rhs ) ,
                size( rhs.size )
            {
            }
            Texture( Texture&& rhs ) :
                parent( rhs ) ,
                size( rhs.size )
            {
                rhs.size = 0;
            }

            Texture( create_flag_t create ) :
                parent()
            {
                glGenTextures( 1 , &handler );
                this->load();
                CheckError( "GenTextures" );
            }

            Texture& operator = ( const Texture& rhs )
            {
                size = rhs.size;
                return parent::operator = ( rhs );
            }
            Texture& operator = ( Texture&& rhs )
            {
                size = rhs.size;
                rhs.size = 0;
                return parent::operator = ( rhs );
            }

            void Load( GLvoid *data ,
                       GLenum internalformat ,
                       GLenum format ,
                       GLenum type ,
                       GLfloat filter = GL_NEAREST ,
                       GLfloat edgeprm = GL_CLAMP_TO_EDGE )
            {
                assert( size[ 0 ] != 0 && size[ 1 ] != 0 );

                glBindTexture( GL_TEXTURE_2D , handler );
                CheckError( "BindTexture" );
                glTexParameterf( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , filter );
                glTexParameterf( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , filter );
                glTexParameterf( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , edgeprm );
                glTexParameterf( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , edgeprm );
                CheckError( "TexParameterf" );

                glTexImage2D( GL_TEXTURE_2D , 0 , internalformat , size[0] , size[1] , 0 , format , type , data );
                CheckError( "TexImage2D" );
            }
            //void LoadFile(  const char *name ,
                            //GLenum format ,
                            //GLenum type ,
                            //GLfloat filter = GL_NEAREST ,
                            //GLfloat edgeprm = GL_CLAMP_TO_EDGE );

            void SetSize( GLsizei w , GLsizei h )
            {
                size = { w , h };
            }
            inline void Viewport() const
            {
                glViewport( 0 , 0 , size[0] , size[1] );
                CheckError( "glViewport" );
            }
        };

            /*
#ifndef EH_NO_LODEPNG
        static LodePNGColorType GLType2LonePNGType( GLenum format )
        {
            switch( format )
            {
                case GL_LUMINANCE:
                    return LodePNGColorType::LCT_GREY;
                case GL_LUMINANCE_ALPHA:
                    return LodePNGColorType::LCT_GREY_ALPHA;
                case GL_RGB:
                    return LodePNGColorType::LCT_RGB;
                default:
                case GL_RGBA:
                    return LodePNGColorType::LCT_RGBA;
            }
        }

        static GLvoid *LodePNG( void *data , size_t size , unsigned int *w , unsigned int *h , GLenum format )
        {
            LodePNGState state;
            state.info_raw.bitdepth = 8;
            state.info_raw.colortype = GLType2LonePNGType( format );
            state.decoder.read_text_chunks = 0;

            unsigned char *out;
            unsigned int _w , _h;
            lodepng_decode( &out , &_w , &_h , &state , ( unsigned char* )data , size );
            lodepng_state_cleanup( &state );

            if( w ){ *w = _w; }
            if( h ){ *h = _h; }
            return out;
        }
#endif
        */

    };  // namespace GL
};  // namespace EH
