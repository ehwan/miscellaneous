#include "EHApplication.h"
#include <cassert>

namespace EH
{
    FrameBase *current_frame;
    FrameBase* GetCurrentFrame()
    {
        return current_frame;
    }
    FrameBase* SetCurrentFrame( FrameBase *frame )
    {
        auto ret = current_frame;
        current_frame = frame;
        return ret;
    }
    static void key_callback( GLFWwindow *win , int key , int scancode , int action , int mods )
    {
    }
    static void cursor_pos_callback( GLFWwindow *win , double xpos , double ypos )
    {
        current_frame->TouchMove();
    }
    static void mouse_button_callback( GLFWwindow *win , int button , int action , int mods )
    {
        switch( action )
        {
            case GLFW_RELEASE:
                current_frame->TouchUp( button );
                break;
            case GLFW_PRESS:
                current_frame->TouchDown( button );
                break;
        }
    }
#ifndef NDEBUG
    bool application_init = false;
#endif
    void InitApplication()
    {
        current_frame = 0;
    #ifndef NDEBUG
        application_init = true;
    #endif
        if( glfwInit() == 0 )
        {
            ERROR( "GLFW Init Failed;" );
            return;
        }

        glfwWindowHint( GLFW_CLIENT_API , GLFW_OPENGL_API );
        //glfwWindowHint( GLFW_CLIENT_API , GLFW_OPENGL_ES_API );

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR , 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR , 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE );
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );


        //glfwSwapInterval( 1 );
    }
    void ExitApplication()
    {
        glfwTerminate();
    }
    Window::Window( const char *title , int w , int h )
    {
    #ifndef NDEBUG
        if( application_init == false )
        {
            ERROR( "Please Initiate the application first!" );
            std::exit( 1 );
        }
    #endif

        window = glfwCreateWindow( w , h , title , 0 , 0 );
        if( window == 0 )
        {
            ERROR( "Failed to create glfw window" );
            ExitApplication();
            return;
        }

        glfwSetKeyCallback( window , key_callback );
        glfwSetCursorPosCallback( window , cursor_pos_callback );
        glfwSetMouseButtonCallback( window , mouse_button_callback );
    }
    Window::~Window()
    {
        if( window )
        {
            glfwDestroyWindow( window );
        }
    }
    void Window::Iconfy()
    {
        glfwIconifyWindow( window );
    }
    bool Window::IsIconfied()
    {
        return glfwGetWindowAttrib( window , GLFW_ICONIFIED );
    }
    bool Window::IsResizable()
    {
        return glfwGetWindowAttrib( window , GLFW_RESIZABLE );
    }
    void Window::Restore()
    {
        glfwRestoreWindow( window );
    }
    void Window::SetTitle( const char *str )
    {
        glfwSetWindowTitle( window , str );
    }
    void Window::SetPosition( int x , int y )
    {
        glfwSetWindowPos( window , x , y );
    }
    Matrix::vec2< int > Window::GetPosition()
    {
        Matrix::vec2< int > ret;
        glfwGetWindowPos( window , ret.data() , ret.data() + 1 );
        return ret;
    }
    void Window::SetSize( int x , int y )
    {
        glfwSetWindowSize( window , x , y );
    }
    Matrix::vec2< int > Window::GetSize()
    {
        Matrix::vec2< int > ret;
        glfwGetWindowSize( window , ret.data() , ret.data() + 1 );
        return ret;
    }

    void Window::MakeCurrent()
    {
        glfwMakeContextCurrent( window );
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose( window , true );
    }
    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose( window );
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers( window );
    }

    Matrix::vec2f Window::GetCursorPos()
    {
        Matrix::vec2< double > ret;
        glfwGetCursorPos( window , ret.data() , ret.data() + 1 );
        return ret.Convert< float >();
    }

    bool Window::IsButtonDown( int button )
    {
        return glfwGetMouseButton( window , button ) == GLFW_PRESS;
    }
    bool Window::IsKeyDown( int key )
    {
        return glfwGetKey( window , key ) == GLFW_PRESS;
    }

};  // namespace EH
