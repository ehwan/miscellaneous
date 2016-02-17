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

    {
        float *b1m = ( float* )b1.map( clqueue , true , CL_MAP_WRITE , 0 , 512 * sizeof( cl_float ) );
        float *b2m = ( float* )b2.map( clqueue , true , CL_MAP_WRITE , 0 , 512 * sizeof( cl_float ) );

        int cur = 0;
        auto a = b1m , b = b2m;
        while( a != b1m + 512 )
        {
            *( a++ ) = cur;
            *( b++ ) = cur;
            ++cur;
        }

        b1.unmap( clqueue , b1m );
        b2.unmap( clqueue , b2m );
    }



    const char *source =
        "__kernel void vector_add( __global float *a , __global float *b , __global float *c )"
        "{"
        "size_t id = get_global_id( 0 );"
        "c[ id ] = a[ id ] + b[ id ];"
        "}";

    cl::Program program( clcontext , source );
    program.build( cldevice );
    cl::Kernel kernel = program( "vector_add" );
    kernel[ 0 ] = b1;
    kernel[ 1 ] = b2;
    kernel[ 2 ] = b3;
    kernel( clqueue , { 512 } );

    return 0;
    float *b1m = ( float* )b3.map( clqueue , true , CL_MAP_READ , 0 , 512 * sizeof( cl_float ) );
    for( auto i = b1m; i != b1m + 512; ++i )
    {
        LOG( *i );
    }
    b3.unmap( clqueue , b1m );
}
