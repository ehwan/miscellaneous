#pragma once

#include "../EHLog.h"
#include <CL/cl.h>
#include <vector>
#include <string>
#include <memory>
#include <sstream>

#ifdef NDEBUG
    #define EH_NO_CL_LOG
#endif

namespace EH
{
    namespace cl
    {
#ifndef EH_NO_CL_LOG
        constexpr const char* err2str[] =
        {
            "CL_SUCCESS"                                  , // 0
            "CL_DEVICE_NOT_FOUND"                         , // -1
            "CL_DEVICE_NOT_AVAILABLE"                     , // -2
            "CL_COMPILER_NOT_AVAILABLE"                   , // -3
            "CL_MEM_OBJECT_ALLOCATION_FAILURE"            , // -4
            "CL_OUT_OF_RESOURCES"                         , // -5
            "CL_OUT_OF_HOST_MEMORY"                       , // -6
            "CL_PROFILING_INFO_NOT_AVAILABLE"             , // -7
            "CL_MEM_COPY_OVERLAP"                         , // -8
            "CL_IMAGE_FORMAT_MISMATCH"                    , // -9
            "CL_IMAGE_FORMAT_NOT_SUPPORTED"               , // -10
            "CL_BUILD_PROGRAM_FAILURE"                    , // -11
            "CL_MAP_FAILURE"                              , // -12
            "CL_MISALIGNED_SUB_BUFFER_OFFSET"             , // -13
            "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST" , // -14
            "CL_COMPILE_PROGRAM_FAILURE"                  , // -15
            "CL_LINKER_NOT_AVAILABLE"                     , // -16
            "CL_LINK_PROGRAM_FAILURE"                     , // -17
            "CL_DEVICE_PARTITION_FAILED"                  , // -18
            "CL_KERNEL_ARG_INFO_NOT_AVAILABLE"            , // -19
            "CL_INVALID_VALUE"                            , // -30
            "CL_INVALID_DEVICE_TYPE"                      , // -31
            "CL_INVALID_PLATFORM"                         , // -32
            "CL_INVALID_DEVICE"                           , // -33
            "CL_INVALID_CONTEXT"                          , // -34
            "CL_INVALID_QUEUE_PROPERTIES"                 , // -35
            "CL_INVALID_COMMAND_QUEUE"                    , // -36
            "CL_INVALID_HOST_PTR"                         , // -37
            "CL_INVALID_MEM_OBJECT"                       , // -38
            "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"          , // -39
            "CL_INVALID_IMAGE_SIZE"                       , // -40
            "CL_INVALID_SAMPLER"                          , // -41
            "CL_INVALID_BINARY"                           , // -42
            "CL_INVALID_BUILD_OPTIONS"                    , // -43
            "CL_INVALID_PROGRAM"                          , // -44
            "CL_INVALID_PROGRAM_EXECUTABLE"               , // -45
            "CL_INVALID_KERNEL_NAME"                      , // -46
            "CL_INVALID_KERNEL_DEFINITION"                , // -47
            "CL_INVALID_KERNEL"                           , // -48
            "CL_INVALID_ARG_INDEX"                        , // -49
            "CL_INVALID_ARG_VALUE"                        , // -50
            "CL_INVALID_ARG_SIZE"                         , // -51
            "CL_INVALID_KERNEL_ARGS"                      , // -52
            "CL_INVALID_WORK_DIMENSION"                   , // -53
            "CL_INVALID_WORK_GROUP_SIZE"                  , // -54
            "CL_INVALID_WORK_ITEM_SIZE"                   , // -55
            "CL_INVALID_GLOBAL_OFFSET"                    , // -56
            "CL_INVALID_EVENT_WAIT_LIST"                  , // -57
            "CL_INVALID_EVENT"                            , // -58
            "CL_INVALID_OPERATION"                        , // -59
            "CL_INVALID_GL_OBJECT"                        , // -60
            "CL_INVALID_BUFFER_SIZE"                      , // -61
            "CL_INVALID_MIP_LEVEL"                        , // -62
            "CL_INVALID_GLOBAL_WORK_SIZE"                 , // -63
            "CL_INVALID_PROPERTY"                         , // -64
            "CL_INVALID_IMAGE_DESCRIPTOR"                 , // -65
            "CL_INVALID_COMPILER_OPTIONS"                 , // -66
            "CL_INVALID_LINKER_OPTIONS"                   , // -67
            "CL_INVALID_DEVICE_PARTITION_COUNT"            // -68
        };
#endif
        static const char* Err2String( cl_int err )
        {
#ifndef EH_NO_CL_LOG
            err = -err;
            if( err <= 19 )
            {
                return err2str[ err ];
            }else
            {
                return err2str[ err - 10 ];
            }
#else
            return "";
#endif
        }
        template < typename ... Ts >
        void CheckError( cl_int err , Ts&& ... args )
        {
#ifndef EH_NO_CL_LOG
            if( err )
            {
                ERROR( std::forward< Ts >( args )... , " / Name : " , Err2String( err ) , " / Code : " , err );
            }
#endif
        }
        template < typename T , typename CRTP >
        struct CLObject
        {
            using this_type = CLObject< T , CRTP >;

            T handler;

            CLObject() :
                handler( 0 )
            {
            }
            CLObject( const this_type& rhs )
            {
                handler = rhs.handler;
                retain();
            }
            CLObject( const T& id )
            {
                handler = id;
                retain();
            }
            CLObject( this_type&& rhs )
            {
                handler = rhs.handler;
                rhs.handler = 0;
            }
            CLObject( T&& id )
            {
                handler = id;
            }
            ~CLObject()
            {
                release();
            }
            CRTP& operator = ( const this_type& rhs )
            {
                release();
                handler = rhs.handler;
                retain();
                return static_cast< CRTP& >( *this );
            }
            CRTP& operator = ( const T& id )
            {
                release();
                handler = id;
                retain();
                return static_cast< CRTP& >( *this );
            }
            CRTP& operator = ( this_type&& rhs )
            {
                release();
                handler = rhs.handler;
                rhs.handler = 0;
                return static_cast< CRTP& >( *this );
            }
            CRTP& operator = ( T&& id )
            {
                release();
                handler = id;
                return static_cast< CRTP& >( *this );
            }

            operator CRTP& ()
            {
                return static_cast< CRTP& >( *this );
            }
            operator const CRTP& () const
            {
                return static_cast< const CRTP& >( *this );
            }

            inline T operator ()() const
            {
                return handler;
            }
            inline operator T() const
            {
                return handler;
            }

            inline void retain()
            {
                static_cast< CRTP& >( *this ).retain();
            }
            inline void release()
            {
                static_cast< CRTP& >( *this ).release();
                handler = 0;
            }

            operator bool () const
            {
                return handler;
            }

            bool operator == ( const this_type& rhs ) const
            {
                return handler == rhs.handher;
            }
            bool operator != ( const this_type& rhs ) const
            {
                return handler != rhs.handler;
            }
        };
        struct Platform;
        struct Device;
        struct Context;
        struct Kernel;
        struct Program;
        struct Buffer;
        struct Queue;

        struct Platform : CLObject< cl_platform_id , Platform >
        {
            using parent = CLObject< cl_platform_id , Platform >;
            using parent::parent;
            using parent::operator=;
            void retain(){ }
            void release(){ }

            static std::vector< Platform > get()
            {
                LOG( "Loading Platforms..." );
                cl_uint num;
                cl_int err;
                err = clGetPlatformIDs( 0 , 0 , &num );
                CheckError( err , "GetPlatformIDs 1" );
                std::vector< cl_platform_id > platforms( num );
                err = clGetPlatformIDs( num , platforms.data() , 0 );
                CheckError( err , "clGetPlatformIDs 2" );
                LOG( num , " platforms fetched" );

                return std::vector< Platform >( platforms.begin() , platforms.end() );
            }

            template < typename T = char >
            std::vector< T > getInfo( cl_platform_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetPlatformInfo( handler , info , 0 , 0 , &num );
                CheckError( err , "clGetPlatformInfo 1 : " , info );
                std::vector< T > ret( num / sizeof( T ) );
                err = clGetPlatformInfo( handler , info , num , ret.data() , 0 );
                CheckError( err , "clGetPlatformInfo 2 : " , info );
                return ret;
            }
            inline std::string getInfoString( cl_platform_info info ) const
            {
                auto vec = getInfo( info );
                return std::string( vec.data() , vec.size() );
            }
            inline std::string getName() const
            {
                return getInfoString( CL_PLATFORM_NAME );
            }
            inline std::string getVersion() const
            {
                return getInfoString( CL_PLATFORM_VERSION );
            }
            inline std::string getVendor() const
            {
                return getInfoString( CL_PLATFORM_VENDOR );
            }
            inline std::string getExtensions() const
            {
                return getInfoString( CL_PLATFORM_EXTENSIONS );
            }
            bool has_gl_sharing_extension() const
            {
                return getExtensions().find( "gl_sharing" , 0 ) != std::string::npos;
            }

            std::vector< Device > getDevices( cl_device_type type )
            {
                LOG( "get devices..." );
                cl_uint num;
                cl_int err;
                err = clGetDeviceIDs( handler , type , 0 , 0 , &num );
                if( err == CL_SUCCESS )
                {
                    LOG( num , " devices fetched" );
                    std::vector< cl_device_id > devices( num );
                    if( num ){
                        err = clGetDeviceIDs( handler , type , num , devices.data() , 0 );
                        CheckError( err , "clGetDeviceIDs 2" );
                    }
                    return std::vector< Device >( devices.begin() , devices.end() );
                }else if( err != CL_DEVICE_NOT_FOUND )
                {
                    CheckError( err , "GetDeviceIDs 1" );
                }
                return std::vector< Device >();
            }
        };

        struct Device : CLObject< cl_device_id , Device >
        {
            using parent = CLObject< cl_device_id , Device >;
            using parent::parent;
            using parent::operator=;
            void retain()
            {
                if( handler )
                {
                    cl_int err = clRetainDevice( handler );
                    CheckError( err , "RetainDevice" );
                }
            }
            void release()
            {
                if( handler )
                {
                    cl_int err = clReleaseDevice( handler );
                    CheckError( err , "ReleaseDevice" );
                }
            }

            template < typename T = char >
            std::vector< T > getInfo( cl_device_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetDeviceInfo( handler , info , 0 , 0 , &num );
                CheckError( err , "GetDeviceInfo 1 : " , info );
                std::vector< T > data( num/sizeof( T ) );
                err = clGetDeviceInfo( handler, info , num , data.data() , 0 );
                CheckError( err , "GetDeviceInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getUnary( cl_device_info info ) const
            {
                RET ret;
                cl_int err = clGetDeviceInfo( handler , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , "clGetDeviceInfo 3 : " , info );
                return ret;
            }
            inline cl_device_type getType() const
            {
                return getUnary< cl_device_type >( CL_DEVICE_TYPE );
            }
            inline std::string getInfoString( cl_device_info info ) const
            {
                auto vec = getInfo( info );
                return std::string( vec.begin() , vec.end() );
            }
            inline std::string getName() const
            {
                return getInfoString( CL_DEVICE_NAME );
            }
            inline std::string getVendor() const
            {
                return getInfoString( CL_DEVICE_VENDOR );
            }
            inline std::string getOpenCLCVersion() const
            {
                return getInfoString( CL_DEVICE_OPENCL_C_VERSION );
            }
            inline std::string getVersion() const
            {
                return getInfoString( CL_DEVICE_VERSION );
            }
            std::string getExtensions() const
            {
                return getInfoString( CL_DEVICE_EXTENSIONS );
            }
            bool has_gl_sharing_extension() const
            {
                return getExtensions().find( "gl_sharing" , 0 ) != std::string::npos;
            }
            Device getParent() const
            {
                return getUnary< cl_device_id >( CL_DEVICE_PARENT_DEVICE );
            }
        };

        struct Context : CLObject< cl_context , Context >
        {
            using parent = CLObject< cl_context, Context >;
            using parent::parent;
            using parent::operator=;
            void retain()
            {
                if( handler )
                {
                    cl_int err = clRetainContext( handler );
                    CheckError( err , "RetainContext" );
                }
            }
            void release()
            {
                if( handler )
                {
                    cl_int err = clReleaseContext( handler );
                    CheckError( err , "ReleaseContext" );
                }
            }

            Context() :
                parent()
            {
            }
            explicit Context( const Device& device , const cl_context_properties *properties = 0 )
            {
                cl_int err;
                handler = clCreateContext( properties ,
                                           1 , &device.handler ,
                                           0 , 0 ,
                                           &err );
                CheckError( err , "CreateContext 1" );
            }
            template < typename Container , typename = decltype( std::begin( Container() ) ) >
            explicit Context( Container&& devices , const cl_context_properties *properties = 0 )
            {
                Context( std::begin( devices ) , std::end( devices ) , properties );
            }
            template < typename IterType >
            explicit Context( IterType&& begin , IterType&& end , const cl_context_properties *properties = 0 )
            {
                cl_int err;
                std::vector< cl_device_id > devs( std::forward< IterType >( begin ) , std::forward< IterType >( end ) );
                handler = clCreateContext( properties ,
                                           devs.size() , devs.data() ,
                                           0 , 0 ,
                                           &err );
                CheckError( err , "CreateContext 2" );
            }



            template < typename T = char >
            std::vector< T > getInfo( cl_context_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetContextInfo( handler , info , 0 , 0 , &num );
                CheckError( err , "GetContextInfo 1 : " , info );
                std::vector< T > data( num/sizeof( T ) );
                err = clGetContextInfo( handler, info , num , data.data() , 0 );
                CheckError( err , "GetContextInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getUnary( cl_context_info info ) const
            {
                RET ret;
                cl_int err = clGetContextInfo( handler , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , "GetContextInfo 3 : " , info );
                return ret;
            }

            cl_uint getNumDevices() const
            {
                return getUnary< cl_uint >( CL_CONTEXT_NUM_DEVICES );
            }
            std::vector< Device > getDevices() const
            {
                auto devs = getInfo< cl_device_id >( CL_CONTEXT_DEVICES );
                return std::vector< Device >(
                        devs.begin() , devs.end()
                        );
            }
            Device getDevice() const
            {
                return getUnary< cl_device_id >( CL_CONTEXT_DEVICES );
            }
        };

        struct Queue : CLObject< cl_command_queue , Queue >
        {
            using parent = CLObject< cl_command_queue , Queue >;
            using parent::parent;
            using parent::operator=;
            void retain()
            {
                if( handler )
                {
                    cl_int err = clRetainCommandQueue( handler );
                    CheckError( err , "RetainCommandQueue" );
                }
            }
            void release()
            {
                if( handler )
                {
                    cl_int err = clReleaseCommandQueue( handler );
                    CheckError( err , "ReleaseCommandQueue" );
                }
            }

            Queue() :
                parent()
            {
            }
            explicit Queue( const Context& context , const Device& device )
            {
                cl_int err;
                handler = clCreateCommandQueue( context() , device() , 0 , &err );
                CheckError( err , "CreateCommandQueue" );
            }

            template < typename T = char >
            std::vector< T > getInfo( cl_command_queue_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetCommandQueueInfo( handler , info , 0 , 0 , &num );
                CheckError( err , "clGetCommandQueueInfo 1 : " , info );
                std::vector< T > data( num/sizeof( T ) );
                err = clGetCommandQueueInfo( handler, info , num , data.data() , 0 );
                CheckError( err , "clGetCommandQueueInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getUnary( cl_context_info info ) const
            {
                RET ret;
                cl_int err = clGetCommandQueueInfo( handler , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , "clGetCommandQueueInfo 3 : " , info );
                return ret;
            }

            Context getContext() const
            {
                return getUnary< cl_context >( CL_QUEUE_CONTEXT );
            }
            Device getDevice() const
            {
                return getUnary< cl_device_id >( CL_QUEUE_DEVICE );
            }
            void finish() const
            {
                clFinish( handler );
            }
            void flush() const
            {
                clFlush( handler );
            }
        };
        struct Buffer : CLObject< cl_mem , Buffer >
        {
            using parent = CLObject< cl_mem , Buffer >;
            using parent::parent;
            using parent::operator=;
            void retain()
            {
                if( handler )
                {
                    cl_int err = clRetainMemObject( handler );
                    CheckError( err , "clRetainMemObject" );
                }
            }
            void release()
            {
                if( handler )
                {
                    cl_int err = clReleaseMemObject( handler );
                    CheckError( err , "clReleaseMemObject" );
                }
            }

            Buffer() :
                parent()
            {
            }
            template < typename Ptr = void >
            explicit Buffer( const Context& context ,
                             cl_mem_flags flags ,
                             size_t size ,
                             Ptr *host_ptr = reinterpret_cast< void* >( 0 ) )
            {
                cl_int err;
                handler = clCreateBuffer( context() , flags , size , host_ptr , &err );
                CheckError( err , "clCreateBuffer" );
            }

            void read( const Queue& queue ,
                       cl_bool block ,
                       size_t offset ,
                       size_t size ,
                       void *ptr ) const
            {
                cl_int err = clEnqueueReadBuffer( queue() ,
                                                  handler ,
                                                  block ,
                                                  offset ,
                                                  size ,
                                                  ptr ,
                                                  0 , 0 , 0 );
                CheckError( err , "clEnqueueReadBuffer" );
            }
            void write( const Queue& queue ,
                        cl_bool block ,
                        size_t offset ,
                        size_t size ,
                        const void* ptr ) const
            {
                cl_int err = clEnqueueWriteBuffer( queue() ,
                                                   handler ,
                                                   block ,
                                                   offset ,
                                                   size ,
                                                   ptr ,
                                                   0 , 0 , 0 );
                CheckError( err , "clEnqueueWriteBuffer" );
            }

            void readRect( const Queue& queue ,
                            cl_bool block ,
                            const size_t buffer_origin[ 3 ] ,
                            const size_t host_origin[ 3 ] ,
                            const size_t size[ 3 ] ,
                            const size_t buffer_width_surface[ 2 ] ,
                            const size_t host_width_surface[ 2 ] ,
                            void *ptr ) const
            {
                cl_int err = clEnqueueReadBufferRect( queue() ,
                                                      handler ,
                                                      block ,
                                                      buffer_origin ,
                                                      host_origin ,
                                                      size ,
                                                      buffer_width_surface[ 0 ] ,
                                                      buffer_width_surface[ 1 ] ,
                                                      host_width_surface[ 0 ] ,
                                                      host_width_surface[ 1 ] ,
                                                      ptr ,
                                                      0 , 0 , 0 );
                CheckError( err , "clEnqueueReadBufferRect" );
            }
            void writeRect( const Queue& queue ,
                            cl_bool block ,
                            const size_t buffer_origin[ 3 ] ,
                            const size_t host_origin[ 3 ] ,
                            const size_t size[ 3 ] ,
                            const size_t buffer_width_surface[ 2 ] ,
                            const size_t host_width_surface[ 2 ] ,
                            const void *ptr ) const
            {
                cl_int err = clEnqueueWriteBufferRect( queue() ,
                                                       handler ,
                                                       block ,
                                                       buffer_origin ,
                                                       host_origin ,
                                                       size ,
                                                       buffer_width_surface[ 0 ] ,
                                                       buffer_width_surface[ 1 ] ,
                                                       host_width_surface[ 0 ] ,
                                                       host_width_surface[ 1 ] ,
                                                       ptr ,
                                                       0 , 0 , 0 );
                CheckError( err , "clEnqueueWriteBufferRect" );
            }
            void copyFrom( const Queue& queue ,
                           const Buffer& from ,
                           size_t offset ,
                           size_t from_offset ,
                           size_t size ) const
            {
                cl_int err = clEnqueueCopyBuffer( queue() , from() , handler , from_offset , offset , size , 0 , 0 , 0 );
                CheckError( err , "clEnqueueCopyBuffer" );
            }
            void copyFromRect( const Queue& queue ,
                               const Buffer& from ,
                               const size_t origin[ 3 ] ,
                               const size_t from_origin[ 3 ] ,
                               const size_t size[ 3 ] ,
                               const size_t width_surface[ 2 ] ,
                               const size_t from_width_surface[ 2 ] )
            {
                cl_int err = clEnqueueCopyBufferRect( queue() ,
                                                      from() ,
                                                      handler ,
                                                      from_origin ,
                                                      origin ,
                                                      size ,
                                                      from_width_surface[ 0 ] ,
                                                      from_width_surface[ 1 ] ,
                                                      width_surface[ 0 ] ,
                                                      width_surface[ 1 ] ,
                                                      0 , 0 , 0 );
                CheckError( err , "clEnqueueCopyBufferRect" );
            }


            template < typename T = void >
            void fill( const Queue& queue ,
                       const T &ptrn ,
                       size_t offset ,
                       size_t count ,
                       size_t size_of = sizeof( T ) )
            {
                cl_int err = clEnqueueFillBuffer( queue() ,
                                                  handler ,
                                                  &ptrn ,
                                                  sizeof( T ) ,
                                                  offset * size_of ,
                                                  count * size_of ,
                                                  0 , 0 , 0 );
                CheckError( err , "clEnqueueFillBuffer" );
            }

            void* map( const Queue& queue ,
                    cl_bool block ,
                    cl_map_flags flags ,
                    size_t offset ,
                    size_t size )
            {
                cl_int err;
                void *ret = clEnqueueMapBuffer( queue() ,
                                                handler ,
                                                block ,
                                                flags ,
                                                offset , size ,
                                                0 , 0 , 0 ,
                                                &err );
                CheckError( err , "clEnqueueMapBuffer" );
                return ret;
            }
            void unmap( const Queue& queue ,
                        void *ptr )
            {
                cl_int err = clEnqueueUnmapMemObject( queue() , handler , ptr , 0 , 0 , 0 );
                CheckError( err , "clEnqueueUnmapMemObject" );
            }

            Buffer createSubBuffer( cl_mem_flags flags ,
                                   cl_buffer_create_type buf_create_type ,
                                   const void *buf_create_info )
            {
                cl_int err;
                cl_mem buf = clCreateSubBuffer( handler , flags , buf_create_type , buf_create_info , &err );
                CheckError( err , "clCreateSubBuffer" );
                return Buffer( std::move( buf ) );
            }
            Buffer createSubBuffer( cl_mem_flags flags ,
                                    size_t offset ,
                                    size_t size )
            {
                cl_buffer_region info;
                info.size = size;
                info.origin = offset;
                return createSubBuffer( flags , CL_BUFFER_CREATE_TYPE_REGION , &info );
            }
        };

        struct Program : CLObject< cl_program , Program >
        {
            using parent = CLObject< cl_program , Program >;
            using parent::parent;
            using parent::operator=;

            void retain()
            {
                if( handler )
                {
                    cl_int err = clRetainProgram( handler );
                    CheckError( err , "clRetainProgram" );
                }
            }
            void release()
            {
                if( handler )
                {
                    cl_int err = clReleaseProgram( handler );
                    CheckError( err , "clReleaseProgram" );
                }
            }

            Program() :
                parent()
            {
            }
            explicit Program( const Context& context , std::vector< const char* > strings , std::vector< size_t > lengths )
            {
                cl_int err;
                handler = clCreateProgramWithSource( context() , strings.size() , strings.data() , lengths.data() , &err );
                CheckError( err , "clCreateProgramWithSource 1" );
            }
            explicit Program( const Context& context , std::vector< const char* > strings )
            {
                cl_int err;
                handler = clCreateProgramWithSource( context() , strings.size() , strings.data() , 0 , &err );
                CheckError( err , "clCreateProgramWithSource 2" );
            }
            explicit Program( const Context& context , const char *source , size_t length = 0 )
            {
                cl_int err;
                handler = clCreateProgramWithSource( context() , 1 , &source , &length , &err );
                CheckError( err , "clCreateProgramWithSource 3" );
            }

            void build( const Device& device , const char *options = 0 ) const
            {
                cl_int err = clBuildProgram( handler , 1 , &device.handler , options , 0 , 0 );
                CheckError( err , "clBuildProgram 1" );
                build_check( device );
            }
            void build_check( const Device& device ) const
            {
                auto status = getBuildInfoUnary< cl_build_status >( device , CL_PROGRAM_BUILD_STATUS );
                switch( status )
                {
                    //case CL_BUILD_NONE:
                        //break;
                    case CL_BUILD_ERROR:
                    {
                        ERROR( "Build error;------------" );
                        auto ret = getBuildInfo( device , CL_PROGRAM_BUILD_LOG );
                        LOG( std::string( ret.begin() , ret.end() ) );
                        ERROR( "Log End; ---------------" );
                        break;
                    }
                    case CL_BUILD_SUCCESS:
                    {
                        LOG( "Build success" );
                        break;
                    }
                    //case CL_BUILD_IN_PROGRESS:
                        //break;
                }
            }
            template < typename IterType >
            void build( IterType&& begin , IterType&& end , const char *options = 0 ) const
            {
                std::vector< cl_device_id > devs( std::forward< IterType >( begin ) , std::forward< IterType >( end ) );
                cl_int err = clBuildProgram( handler , devs.size() , devs.data() , options , 0 , 0 );
                CheckError( err , "clBuildProgram 2" );
                for( ; begin != end; ++begin )
                {
                    build_check( *begin );
                }
            }
            template < typename Container , typename = decltype( std::begin( Container() ) ) >
            void build( Container&& devices , const char *options = 0 ) const
            {
                build( std::begin( devices ) , std::end( devices ) , options );
            }


            template < typename T = char >
            std::vector< T > getInfo( cl_program_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetProgramInfo( handler , info , 0 , 0 , &num );
                CheckError( err , "clGetProgramInfo 1 : " , info );
                std::vector< T > data( num/sizeof( T ) );
                err = clGetProgramInfo( handler, info , num , data.data() , 0 );
                CheckError( err , "clGetProgramInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getUnary( cl_program_info info ) const
            {
                RET ret;
                cl_int err = clGetProgramInfo( handler , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , "clGetProgramInfo 3 : " , info );
                return ret;
            }

            template < typename T = char >
            std::vector< T > getBuildInfo( const Device& device , cl_program_build_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetProgramBuildInfo( handler , device() , info , 0 , 0 , &num );
                CheckError( err , "clGetProgramBuildInfo 1 : " , info );
                std::vector< T > data( num / sizeof( T ) );
                err = clGetProgramBuildInfo( handler , device() , info , num , data.data() , 0 );
                CheckError( err , "clGetProgramBuildInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getBuildInfoUnary( const Device& device , cl_program_build_info info ) const
            {
                RET ret;
                cl_int err = clGetProgramBuildInfo( handler , device() , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , "clGetProgramBuildInfo 3 : " , info );
                return ret;
            }

            Context getContext() const
            {
                return getUnary< cl_context >( CL_PROGRAM_CONTEXT );
            }
            cl_uint getNumDevices() const
            {
                return getUnary< cl_uint >( CL_PROGRAM_NUM_DEVICES );
            }
            std::vector< Device > getDevices() const
            {
                auto devs = getInfo< cl_device_id >( CL_PROGRAM_DEVICES );
                return std::vector< Device >( devs.begin() , devs.end() );
            }
            Device getDevice() const
            {
                return Device( getUnary< cl_device_id >( CL_PROGRAM_DEVICES ) );
            }
            std::string getSource() const
            {
                auto ret = getInfo< char >( CL_PROGRAM_SOURCE );
                return std::string( ret.begin() , ret.end() );
            }

            using parent::operator();
            CLObject< cl_kernel , Kernel > operator () ( const char *name ) const
            {
                cl_int err;
                cl_kernel kernel = clCreateKernel( handler , name , &err );
                CheckError( err , "clCreateKernel 2 : " , name );
                return CLObject< cl_kernel , Kernel >( kernel );
            }
        };

        struct Kernel : CLObject< cl_kernel , Kernel >
        {
            using parent = CLObject< cl_kernel , Kernel >;
            using parent::parent;
            using parent::operator=;
            void retain()
            {
                if( handler )
                {
                    cl_int err = clRetainKernel( handler );
                    CheckError( err , "clRetainKernel" );
                }
            }
            void release()
            {
                if( handler )
                {
                    cl_int err = clReleaseKernel( handler );
                    CheckError( err , "clReleaseKernel" );
                }
            }

            Kernel() :
                parent()
            {
            }
            explicit Kernel( const Program& program , const char *name )
            {
                cl_int err;
                handler = clCreateKernel( program() , name , &err );
                CheckError( err , "clCreateKernel : " , name );
            }

            template < typename T >
            void SetArg( cl_uint index , const T *arg )
            {
                cl_int err = clSetKernelArg( handler , index , sizeof( T ) , arg );
                CheckError( err , "clSetKernelArg : " , index );
            }
            inline void SetArg( cl_uint index , const Buffer& buf )
            {
                SetArg( index , &buf.handler );
            }

            struct ArgumentWrapper
            {
                cl_uint index;
                cl_kernel kernel;

                template < typename T >
                const T& operator = ( const T& arg ) const
                {
                    cl_int err = clSetKernelArg( kernel , index , sizeof( T ) , &arg );
                    CheckError( err , "clSetKernelArg : " , index );
                    return arg;
                }
                const Buffer& operator = ( const Buffer& buf ) const
                {
                    cl_int err = clSetKernelArg( kernel , index , sizeof( cl_mem ) , &buf.handler );
                    CheckError( err , "clSetKernelArg : " , index );
                    return buf;
                }

                auto operator [] ( cl_uint id ) const
                {
                    return ArgumentWrapper{ id , kernel };
                }
                ArgumentWrapper operator ++ ( int )
                {
                    ArgumentWrapper ret = *this;
                    ++index;
                    return ret;
                }
                ArgumentWrapper& operator ++ ()
                {
                    ++index;
                    return *this;
                }
                ArgumentWrapper operator -- ( int )
                {
                    ArgumentWrapper ret = *this;
                    --index;
                    return ret;
                }
                ArgumentWrapper& operator -- ()
                {
                    --index;
                    return *this;
                }
                bool operator != ( const ArgumentWrapper& rhs ) const
                {
                    return index != rhs.index || kernel != rhs.kernel;
                }
                bool operator == ( const ArgumentWrapper& rhs ) const
                {
                    return index == rhs.index && kernel == rhs.kernel;
                }
                bool operator != ( cl_uint id ) const
                {
                    return index != id;
                }
                bool operator == ( cl_uint id ) const
                {
                    return index == id;
                }

                template < typename T = char >
                std::vector< T > getInfo( cl_kernel_arg_info info ) const
                {
                    size_t num;
                    cl_int err;
                    err = clGetKernelArgInfo( kernel , index , info , 0 , 0 , &num );
                    CheckError( err , "clGetKernelArgInfo 1 : " , info );
                    std::vector< T > data( num/sizeof( T ) );
                    err = clGetKernelArgInfo( kernel , index , info , num , data.data() , 0 );
                    CheckError( err , "clGetKernelArgInfo 2 : " , info );
                    return data;
                }
                template < typename RET >
                RET getUnary( cl_kernel_arg_info info ) const
                {
                    RET ret;
                    cl_int err = clGetKernelArgInfo( kernel , index , info , sizeof( RET ) , &ret , 0 );
                    CheckError( err , "clGetKernelArgInfo 3 : " , info );
                    return ret;
                }

                static const char* AddressQualifier2Str( cl_kernel_arg_address_qualifier aq )
                {
                    switch( aq )
                    {
                        default:
                            return "";
                        case CL_KERNEL_ARG_ADDRESS_GLOBAL:
                            return "global";
                        case CL_KERNEL_ARG_ADDRESS_LOCAL:
                            return "local";
                        case CL_KERNEL_ARG_ADDRESS_CONSTANT:
                            return "constant";
                        case CL_KERNEL_ARG_ADDRESS_PRIVATE:
                            return "private";
                    }
                }
                static const char* AccessQualifier2Str( cl_kernel_arg_access_qualifier aq )
                {
                    switch( aq )
                    {
                        default:
                            return "";
                        case CL_KERNEL_ARG_ACCESS_READ_ONLY:
                            return "RO";
                        case CL_KERNEL_ARG_ACCESS_WRITE_ONLY:
                            return "WO";
                        case CL_KERNEL_ARG_ACCESS_READ_WRITE:
                            return "RW";
                        case CL_KERNEL_ARG_ACCESS_NONE:
                            return "NA";
                    }
                    return "";
                }
                cl_kernel_arg_address_qualifier getAddressQualifier() const
                {
                    return getUnary< cl_kernel_arg_address_qualifier >( CL_KERNEL_ARG_ADDRESS_QUALIFIER );
                }
                cl_kernel_arg_access_qualifier getAccessQualifier() const
                {
                    return getUnary< cl_kernel_arg_access_qualifier >( CL_KERNEL_ARG_ACCESS_QUALIFIER );
                }
                std::string getTypeName() const
                {
                    auto vec = getInfo( CL_KERNEL_ARG_TYPE_NAME );
                    return std::string( vec.begin() , vec.end() );
                }
                cl_kernel_arg_type_qualifier getTypeQualifier() const
                {
                    return getUnary< cl_kernel_arg_type_qualifier >( CL_KERNEL_ARG_TYPE_QUALIFIER );
                }
                std::string getName() const
                {
                    auto vec = getInfo( CL_KERNEL_ARG_NAME );
                    return std::string( vec.begin() , vec.end() );
                }
                std::string getFull() const
                {
                    std::stringstream stream;
                    stream << AccessQualifier2Str( getAccessQualifier() );
                    stream << ' ';
                    stream << AddressQualifier2Str( getAddressQualifier() );
                    stream << ' ';
                    stream << getTypeName();
                    stream << ' ';
                    stream << getName();

                    return stream.str();
                }
            };

            auto operator [] ( cl_uint index ) const
            {
                return ArgumentWrapper{ index , handler };
            }

            template < typename T = char >
            std::vector< T > getInfo( cl_kernel_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetKernelInfo( handler , info , 0 , 0 , &num );
                CheckError( err , "clGetKernelInfo 1 : " , info );
                std::vector< T > data( num/sizeof( T ) );
                err = clGetKernelInfo( handler, info , num , data.data() , 0 );
                CheckError( err , "clGetKernelInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getUnary( cl_kernel_info info ) const
            {
                RET ret;
                cl_int err = clGetKernelInfo( handler , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , "clGetKernelInfo 3 : " , info );
                return ret;
            }
            template < typename T = char >
            std::vector< T > getWorkGroupInfo( const Device& device , cl_kernel_work_group_info info ) const
            {
                size_t num;
                cl_int err;
                err = clGetKernelWorkGroupInfo( handler , device() , info , 0 , 0 , &num );
                CheckError( err , "clGetKernelWorkGroupInfo 1 : " , info );
                std::vector< T > data( num/sizeof( T ) );
                err = clGetKernelWorkGroupInfo( handler, device() , info , num , data.data() , 0 );
                CheckError( err , "clGetKernelWorkGroupInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getWorkGroupInfoUnary( const Device& device , cl_kernel_work_group_info info ) const
            {
                RET ret;
                cl_int err = clGetKernelWorkGroupInfo( handler , device() , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , "clGetKernelWorkGroupInfo 3 : " , info );
                return ret;
            }

            std::string getName() const
            {
                auto vec = getInfo( CL_KERNEL_FUNCTION_NAME );
                return std::string( vec.begin() , vec.end() );
            }
            cl_uint getNumArgs() const
            {
                return getUnary< cl_uint >( CL_KERNEL_NUM_ARGS );
            }
            Context getContext() const
            {
                return getUnary< cl_context >( CL_KERNEL_CONTEXT );
            }
            Program getProgram() const
            {
                return getUnary< cl_program >( CL_KERNEL_PROGRAM );
            }
            std::string getAttributes() const
            {
                auto vec = getInfo( CL_KERNEL_ATTRIBUTES );
                return std::string( vec.begin() , vec.end() );
            }

            inline void operator () ( const Queue& queue ,
                                      std::initializer_list< size_t > global_size ,
                                      std::initializer_list< size_t > local_size ,
                                      std::initializer_list< size_t > global_offset ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , global_size.size() ,
                                    global_offset.begin() , global_size.begin() , local_size.begin() ,
                                    0 , 0 , 0 );
                CheckError( err , "clEnqueueNDRangeKernel" );
            }
            inline void operator () ( const Queue& queue ,
                                      std::initializer_list< size_t > global_size ,
                                      std::initializer_list< size_t > local_size ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , global_size.size() ,
                                    0 , global_size.begin() , local_size.begin() ,
                                    0 , 0 , 0 );
                CheckError( err , "clEnqueueNDRangeKernel" );
            }
            inline void operator () ( const Queue& queue ,
                                      std::initializer_list< size_t > global_size ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , global_size.size() ,
                                    0 , global_size.begin() , 0 ,
                                    0 , 0 , 0 );
                CheckError( err , "clEnqueueNDRangeKernel" );
            }
            inline void operator () ( const Queue& queue ) const
            {
                cl_int err = clEnqueueTask( queue() , handler , 0 , 0 , 0 );
                CheckError( err , "clEnqueueNDRangeKernel" );
            }
        };

    };
};
