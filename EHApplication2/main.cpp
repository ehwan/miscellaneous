#include "iostream"
#include "EHApplication.h"
#include "../GLWrapper/Debug.h"

struct MyFrame : public EH::Frame< MyFrame >
{
    void EnterFrame()
    {
        EH::GL::Debug::Begin( GL_LINES );
        EH::GL::Debug::Vertex2f( 0 );
        EH::GL::Debug::Vertex2f( 3 );
        EH::GL::Debug::End();
        window->SwapBuffers();
    }
};

int main()
{
    EH::Application appli;
    EH::Window window( "Test Window" , 500 , 500 );

    window.MakeCurrent();
    EH::GL::LoadGLFunctions();

    const EH::Matrix::mat4f projection =
        EH::Matrix::Util::OrthologMatrix< 4 >(
                EH::Matrix::vec2f( 0 ) , EH::Matrix::vec2f( 3 ) ,
                EH::Matrix::vec2f( -1 ) , EH::Matrix::vec2f( 1 )
                );
    EH::GL::Debug::SetProjection( projection );

    MyFrame frame;
    frame.SetBound( { 0 , 0 } , { 3 , 3 } );
    frame.Run( &window );
}
