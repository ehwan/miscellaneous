#include "Debug.h"
#include <vector>

namespace EH
{
    namespace GL
    {
        namespace Debug
        {
#ifndef EH_NO_DEBUG_DRAW
            GL::VertexArray vao;
            GL::VertexBuffer vbo;
            GL::Program program;
            GL::VertexShader vertexS;
            GL::FragmentShader fragmentS;

            std::vector< GLfloat > vertices;
            GLenum cur_mode;
            bool is_begin;
            int dimension;

            void Begin( GLenum mode )
            {
                if( is_begin )
                {
                    ERROR( "Drawing Already Begun.. stop the original first" );
                    std::exit( 0 );
                    return;
                }
                vertices.clear();
                is_begin = true;
                cur_mode = mode;
                dimension = 0;
            }
            void Vertex2f( const vecf< 2 >& v )
            {
                if( dimension == 0 )
                {
                    dimension = 2;
                }else if( dimension != 2 )
                {
                    ERROR( "you already used VertexNf() once; end previous rendering" );
                    return;
                }
                vertices.push_back( v.x );
                vertices.push_back( v.y );
            }
            void Vertex3f( const vecf< 3 >& v )
            {
                if( dimension == 0 )
                {
                    dimension = 3;
                }else if( dimension != 3 )
                {
                    ERROR( "you already used VertexNf() once; end previous rendering" );
                    return;
                }
                vertices.push_back( v.x );
                vertices.push_back( v.y );
                vertices.push_back( v.z );
            }
            void Vertex2f( std::vector< vecf< 2 > > vs , std::vector< std::size_t > is )
            {
                for( std::size_t i : is )
                {
                    Vertex2f( vs[ i ] );
                }
            }
            void Vertex3f( std::vector< vecf< 3 > > vs , std::vector< std::size_t > is )
            {
                for( std::size_t i : is )
                {
                    Vertex3f( vs[ i ] );
                }
            }
            static void LoadProgram()
            {
                const GLchar *vs =
                    "#version 330 core\n"
                    "uniform mat4 u_projection;"
                    "\n"
                    "in vec4 vert;"
                    "\n"
                    "void main(){"
                    "gl_Position = u_projection * vert;"
                    "}";
                const GLchar *fs =
                    "#version 330 core\n"
                    "uniform vec4 u_color;"
                    "out vec4 outcol;"
                    "\n"
                    "void main(){"
                    "outcol = u_color;"
                    "}";

                vertexS = GL::VertexShader( vs ).Move();
                fragmentS = GL::FragmentShader( fs ).Move();

                program = Program( vertexS , fragmentS ).Move();

                program.BindAttribLocation( 0 , "vert" );
                program.BindFragDataLocation( 0 , "outcol" );

                program.Link();

                SetProjection( 1 );
                SetColor( 1 );
            }
            void SetProjection( const Matrix::Matrix< GLfloat , 4 >& mat )
            {
                if( program.handler == 0 )
                {
                    LoadProgram();
                }
                program.Use();
                program.SetUniformMatrix4( "u_projection" , mat.data() );
                program.UnUse();
            }
            void SetColor( const vecf< 4 >& c )
            {
                if( program.handler == 0 )
                {
                    LoadProgram();
                }
                program.Use();
                program.SetUniform( "u_color" , c.x , c.y , c.z , c.w );
                program.UnUse();
            }
            void End()
            {
                if( is_begin == false )
                {
                    ERROR( "Must call Begin() before calling End()" );
                    return;
                }
                is_begin = false;
                if( vertices.size() == 0 )
                {
                    return;
                }

                if( vbo.handler == 0 )
                {
                    vbo = VertexBuffer( create_flag ).Move();
                }
                vbo.Begin();
                if( vbo.size < sizeof(GLfloat) * vertices.size() )
                {
                    vbo.BufferData( sizeof( GLfloat )*vertices.size()*2 , GL_DYNAMIC_DRAW , vertices.data() );
                }else
                {
                    vbo.SubData( 0 , sizeof( GLfloat )*vertices.size() , vertices.data() );
                }
                vbo.End();

                if( program.handler == 0 )
                {
                    LoadProgram();
                }
                if( vao.handler == 0 )
                {
                    vao = VertexArray( create_flag );
                    vao.Begin();
                    vbo.Begin();
                    GL::Vertex( 0 ).EnableVertexAttribArray().VertexAttribPointer( dimension , GL_FLOAT , 0 , 0 );
                    vbo.End();
                    vao.End();
                }
                program.Use();
                vao.Begin();
                glDrawArrays( cur_mode , 0 , vertices.size()/dimension );
                GL::CheckError( "Debug fixed rendering pipeline" );
                vao.End();
                program.UnUse();
            }
#endif
        };
    };  // namespace GL
}; // namespace EH
