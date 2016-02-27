#include "../EHLog.h"
#include "../CLWrapper/clw.h"

using namespace EH;
cl::Platform clplatform;
cl::Device cldevice;
cl::Context clcontext;
cl::Queue clqueue;

void SetDevice()
{
    auto platforms = cl::Platform::get();
    for( auto p = platforms.begin(); p != platforms.end(); ++p )
    {
        auto devices = p->getDevices( CL_DEVICE_TYPE_CPU );
        for( auto d = devices.begin(); d != devices.end(); ++d )
        {
            cldevice = std::move( *d );
            clplatform = std::move( *p );
            return;
        }
    }
}

int main()
{
    using namespace EH;
    SetDevice();
    LOG( "Chosen : " );
    LOG( "Platform \t: " , clplatform.getName() );
    LOG( "Device \t: " , cldevice.getName() );

    LOG( "Extensions : \n" , cldevice.getExtensions() );
    LOG( "has GLInterpolate extension : " , cldevice.has_gl_sharing_extension() );

    clcontext = cl::Context( cldevice );
    clqueue = cl::Queue( clcontext , cldevice );

    cl::Buffer b1( clcontext , CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , 512 * sizeof( cl_float ) );
    cl::Buffer b2( clcontext , CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , 512 * sizeof( cl_float ) );
    cl::Buffer b3( clcontext , CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR , 512 * sizeof( cl_float ) );
}
