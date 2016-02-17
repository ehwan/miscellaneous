#pragma once

#include <type_traits>

#include "../GLWrapper/glw.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "EHApplication_Global.h"

namespace EH
{
    class FrameBase;

    void InitApplication();
    void ExitApplication();

    FrameBase* GetCurrentFrame();
    FrameBase* SetCurrentFrame( FrameBase *frame );

    class Application
    {
    public:
        Application()
        {
            InitApplication();
        }
        ~Application()
        {
            ExitApplication();
        }
    };
    class Window
    {
        using window_type = GLFWwindow*;
    public:
        Window() :
            window( 0 )
        {
        }
        Window( const char *title , int w , int h );
        Window( Window&& rhs )
        {
            window = rhs.window;
            rhs.window = 0;
        }
        ~Window();

        void Iconfy();
        void Restore();
        bool IsIconfied();
        bool IsResizable();

        void SetTitle( const char *str );

        void SetPosition( int x , int y );
        Matrix::vec2< int > GetPosition();

        void SetSize( int x , int y );
        Matrix::vec2< int > GetSize();

        void MakeCurrent();

        void Close();
        bool ShouldClose();

        void SwapBuffers();

        Matrix::vec2f GetCursorPos();
        bool IsButtonDown( int button );
        bool IsKeyDown( int key );
    protected:
        window_type window;
    };

    class FrameBase
    {
    public:
        //using clock = std::chrono::high_resolution_clock;
        using clock = std::chrono::steady_clock;
        using dt_type = float;
        using dt_duration_type = std::chrono::duration< dt_type , std::ratio< 1 > >;

        FrameBase *parent;
        Window *window;


        dt_type dt;

    protected:
        Matrix::vec2f bound_min;
        Matrix::vec2f bound_max;

        // touch_point = touch_a * raw + touch_b
        Matrix::vec2f touch_a , touch_b;

        clock::duration max_gap;
        clock::duration min_gap;

    public:
        FrameBase() :
            parent( 0 ) , window( 0 ) , bound_min( 0 ) , bound_max( 0 )
        {
            max_gap = std::chrono::duration_cast< clock::duration >( std::chrono::duration< int , std::ratio< 1 , 30 > >( 1 ) );
            min_gap = std::chrono::duration_cast< clock::duration >( std::chrono::duration< int , std::ratio< 1 , 60 > >( 1 ) );

            dt = 0.0f;
        }

        template < int MIN , int MAX >
        void SetFrameRate()
        {
            max_gap = std::chrono::duration_cast< clock::duration >( std::chrono::duration< int , std::ratio< 1 , MIN > >( 1 ) );
            min_gap = std::chrono::duration_cast< clock::duration >( std::chrono::duration< int , std::ratio< 1 , MAX > >( 1 ) );
        }

        void SetBound( const Matrix::vec2f min , const Matrix::vec2f max )
        {
            bound_min = min;
            bound_max = max;

            CacheTouch();
        }
        Matrix::vec2f GetCursorPos()
        {
            assert( window );
            return window->GetCursorPos() * touch_a + touch_b;
        }

        virtual void TouchDown( int button ){}
        virtual void TouchUp( int button ){}
        virtual void TouchMove(){}

    protected:
        void CopyFrom( const FrameBase& rhs )
        {
            bound_min = rhs.bound_min;
            bound_max = rhs.bound_max;

            max_gap = rhs.max_gap;
            min_gap = rhs.min_gap;
        }
        void CacheTouch()
        {
            if( window )
            {
                auto gsize = bound_max - bound_min;
                auto factor = gsize / window->GetSize().Convert< float >();

                touch_a = { factor[ 0 ] , -factor[ 1 ] };
                touch_b = { bound_min[ 0 ] , bound_max[ 1 ] };
            }
        }
    };

    template < typename CRTP >
    class Frame : protected FrameBase
    {
    protected:
        bool close_flag;

        void EnterFrame()
        {
            static_cast< CRTP& >( *this ).EnterFrame();
        }
    public:
        using FrameBase::SetBound;

        Frame() :
            FrameBase()
        {
            close_flag = false;

            parent = GetCurrentFrame();
            if( parent )
            {
                CopyFrom( *parent );
            }
        }

        inline void Close()
        {
            close_flag = false;
        }
        void Run( Window *_window )
        {
            window = _window;
            CacheTouch();
            auto last_frame = SetCurrentFrame( this );
            clock::time_point last = clock::now();
            clock::time_point now  = clock::now();
            clock::duration gap;
            while( close_flag == false && _window->ShouldClose() == false )
            {
                glfwPollEvents();
                now = clock::now();
                gap = now - last;
                if( gap > min_gap )
                {
                    last = now;
                    dt = std::chrono::duration_cast< dt_duration_type >( std::min( gap , max_gap ) ).count();

                    EnterFrame();

                    //_window->SwapBuffers();
                }
            }
            SetCurrentFrame( last_frame );
        }
    };
};  // namespace EH
