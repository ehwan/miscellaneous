#pragma once

#include "../EHLog.h"
#include "../EHUtil/Memory.h"
#include "../EHMatrix/EHMatrix.h"
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <tuple>

#ifdef NDEBUG
    #define EH_CL_NO_LOG
    #define EH_CL_NO_DEBUG
#endif

namespace EH
{
    namespace cl
    {
        template < typename VecType , typename CLType >
        struct alignas( alignof( CLType ) ) eh_vec_type : VecType
        {
            using parent = VecType;
            using VecType::VecType;
            using VecType::operator=;
        };

        using vec2f  = Matrix::vec2< cl_float >;
        using vec3f  = Matrix::vec3< cl_float >;
        using vec4f  = Matrix::vec4< cl_float >;
        using vec8f  = Matrix::Vector< cl_float , 8 >;
        using vec16f = Matrix::Vector< cl_float , 16 >;

        using vec2i  = Matrix::vec2< cl_int >;
        using vec3i  = Matrix::vec3< cl_int >;
        using vec4i  = Matrix::vec4< cl_int >;
        using vec8i  = Matrix::Vector< cl_int , 8 >;
        using vec16i = Matrix::Vector< cl_int , 16 >;

        using vec2ui  = Matrix::vec2< cl_uint >;
        using vec3ui  = Matrix::vec3< cl_uint >;
        using vec4ui  = Matrix::vec4< cl_uint >;
        using vec8ui  = Matrix::Vector< cl_uint , 8 >;
        using vec16ui = Matrix::Vector< cl_uint , 16 >;



        using eh_float = cl_float;
        using eh_float2  = eh_vec_type< vec2f  , cl_float2 >;
        using eh_float3  = eh_vec_type< vec3f  , cl_float3 >;
        using eh_float4  = eh_vec_type< vec4f  , cl_float4 >;
        using eh_float8  = eh_vec_type< vec8f  , cl_float8 >;
        using eh_float16 = eh_vec_type< vec16f , cl_float16 >;

        using eh_int = cl_int;
        using eh_int2  = eh_vec_type< vec2i  , cl_int2 >;
        using eh_int3  = eh_vec_type< vec3i  , cl_int3 >;
        using eh_int4  = eh_vec_type< vec4i  , cl_int4 >;
        using eh_int8  = eh_vec_type< vec8i  , cl_int8 >;
        using eh_int16 = eh_vec_type< vec16i , cl_int16 >;

        using eh_uint = cl_uint;
        using eh_uint2  = eh_vec_type< vec2ui  , cl_uint2 >;
        using eh_uint3  = eh_vec_type< vec3ui  , cl_uint3 >;
        using eh_uint4  = eh_vec_type< vec4ui  , cl_uint4 >;
        using eh_uint8  = eh_vec_type< vec8ui  , cl_uint8 >;
        using eh_uint16 = eh_vec_type< vec16ui , cl_uint16 >;

        /*
        using eh_bool = cl_bool;
        using eh_bool2 = eh_vec_type< vec2b , cl_bool2 >;
        using eh_bool3 = eh_vec_type< vec3b , cl_bool3 >;
        using eh_bool4 = eh_vec_type< vec4b , cl_bool4 >;
        */
    };
    namespace cl
    {
#ifndef EH_CL_NO_LOG
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
#ifndef EH_CL_NO_LOG
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
#ifndef EH_CL_NO_LOG
            if( err )
            {
                ERROR( std::forward< Ts >( args )... , " / Name : " , Err2String( err ) , " / Code : " , err );
            }
#endif
        }
        template < typename Handler , typename CRTP >
        class CLObject
        {
        public:
            using this_type = CLObject< Handler , CRTP >;
            using handler_type = Handler;

            handler_type handler;

            CLObject() :
                handler( 0 )

            {
            }
            CLObject( const this_type& rhs ) :
                handler( rhs.handler )
            {
                _private_retain();
            }
            explicit CLObject( const handler_type& id ) :
                handler( id )
            {
                _private_retain();
            }
            CLObject( this_type&& rhs ) :
                handler( rhs.handler )
            {
                rhs.handler = 0;
            }
            explicit CLObject( handler_type&& id ) :
                handler( id )
            {
            }
            ~CLObject()
            {
                _private_release();
            }
            CRTP& operator = ( const this_type& rhs )
            {
                _private_release();
                handler = rhs.handler;
                _private_retain();

                return static_cast< CRTP& >( *this );
            }
            CRTP& operator = ( const handler_type& id )
            {
                _private_release();
                handler = id;
                _private_retain();

                return static_cast< CRTP& >( *this );
            }
            CRTP& operator = ( this_type&& rhs )
            {
                _private_release();
                handler = rhs.handler;
                rhs.handler = 0;

                return static_cast< CRTP& >( *this );
            }
            CRTP& operator = ( handler_type&& id )
            {
                _private_release();
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

            inline handler_type operator ()() const
            {
                return handler;
            }

        protected:
            void _private_retain()
            {
                if( handler )
                {
                    static_cast< CRTP& >( *this ).retain();
                }
            }
            void _private_release()
            {
                if( handler )
                {
                    static_cast< CRTP& >( *this ).release();
                    handler = 0;
                }

            }

            void retain()
            {
            }
            void release()
            {
            }

        public:
            operator bool () const
            {
                return handler;
            }

            bool operator == ( const this_type& rhs ) const
            {
                return handler == rhs();
            }
            bool operator != ( const this_type& rhs ) const
            {
                return handler != rhs();
            }
        };
        class Device;
        class Context;
        class Kernel;
        class Program;
        class Buffer;
        class Queue;

        class Platform : public CLObject< cl_platform_id , Platform >
        {
        public:
            using parent = CLObject< cl_platform_id , Platform >;
            using parent::operator=;
            using parent::parent;

            friend class CLObject< cl_platform_id , Platform >;

            Platform() :
                parent()
            {
            }
            Platform( const Platform& rhs ) :
                parent( rhs )
            {
            }
            Platform( Platform&& rhs ) :
                parent( rhs )
            {
            }

            Platform& operator = ( const Platform& rhs )
            {
                return parent::operator = ( rhs );
            }
            Platform& operator = ( Platform&& rhs )
            {
                return parent::operator = ( rhs );
            }


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

        protected:
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
        public:
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
                    if( num != 0 )
                    {
                        LOG( num , " devices fetched" );
                        std::vector< cl_device_id > devices( num );
                        err = clGetDeviceIDs( handler , type , num , devices.data() , 0 );
                        CheckError( err , "clGetDeviceIDs 2" );
                        std::vector< std::reference_wrapper< const cl_device_id > > drefs( devices.begin() , devices.end() );
                        return std::vector< Device >( drefs.begin() , drefs.end() );
                    }
                }else if( err != CL_DEVICE_NOT_FOUND )
                {
                    CheckError( err , "GetDeviceIDs 1" );
                }
                return std::vector< Device >();
            }
        };

        class Device : public CLObject< cl_device_id , Device >
        {
        protected:
            void retain()
            {
                cl_int err = clRetainDevice( handler );
                CheckError( err , "RetainDevice" );
            }
            void release()
            {
                cl_int err = clReleaseDevice( handler );
                CheckError( err , "ReleaseDevice" );
            }
        public:
            using parent = CLObject< cl_device_id , Device >;
            using parent::parent;
            using parent::operator=;

            friend class CLObject< cl_device_id , Device >;

            Device() :
                parent()
            {
            }
            Device( const Device& rhs ) :
                parent( rhs )
            {
            }
            Device( Device&& rhs ) :
                parent( rhs )
            {
            }

            Device& operator = ( const Device& rhs )
            {
                return parent::operator = ( rhs );
            }
            Device& operator = ( Device&& rhs )
            {
                return parent::operator = ( rhs );
            }

        protected:
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
            inline std::string getInfoString( cl_device_info info ) const
            {
                auto vec = getInfo( info );
                return std::string( vec.begin() , vec.end() );
            }
        public:
            inline cl_device_type getType() const
            {
                return getUnary< cl_device_type >( CL_DEVICE_TYPE );
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
                return Device( static_cast< const cl_device_id& >( getUnary< cl_device_id >( CL_DEVICE_PARENT_DEVICE ) ) );
            }
            cl_uint getMaxWorkItemDimension() const
            {
                return getUnary< cl_uint >( CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS );
            }
            size_t getMaxWorkGroupSize() const
            {
                return getUnary< size_t >( CL_DEVICE_MAX_WORK_GROUP_SIZE );
            }
            auto getMaxWorkItemSizes() const
            {
                struct Ret
                {
                    size_t arr[ 3 ];
                };
                return getUnary< Ret >( CL_DEVICE_MAX_WORK_ITEM_SIZES );
            }
            size_t getMaxWorkItemSize( cl_uint dim ) const
            {
                struct Ret
                {
                    size_t arr[ 3 ];
                };
                return getUnary< Ret >( CL_DEVICE_MAX_WORK_ITEM_SIZES ).arr[ dim ];
            }
        };

        class ContextProperties : protected std::vector< cl_context_properties >
        {
        public:
            using parent = std::vector< cl_context_properties >;

            using typename parent::iterator;
            using typename parent::const_iterator;
            using parent::begin;
            using parent::end;
            using parent::size;
            using parent::data;

            ContextProperties() :
                parent()
            {
            }

            bool hasProperty( cl_context_properties p )
            {
                return findProperty( p ) != end();
            }
            void setProperty( cl_context_properties name , cl_context_properties data )
            {
                auto iter = findProperty( name );
                if( iter == end() ){ parent::resize( size() + 2 ); *iter = name; }
                *( ++iter ) = data;
            }
            void setPlatform( const Platform& platform )
            {
                setProperty( CL_CONTEXT_PLATFORM , ( cl_context_properties )platform() );
            }
            void setContextInteropUserSync( bool rhs )
            {
                setProperty( CL_CONTEXT_INTEROP_USER_SYNC , ( cl_context_properties )( rhs ? CL_TRUE : CL_FALSE ) );
            }
        protected:
            iterator findProperty( cl_context_properties p )
            {
                return std::find( begin() , end() , p );
            }
        };

        class Context : public CLObject< cl_context , Context >
        {
        protected:
            void retain()
            {
                cl_int err = clRetainContext( handler );
                CheckError( err , "RetainContext" );
            }
            void release()
            {
                cl_int err = clReleaseContext( handler );
                CheckError( err , "ReleaseContext" );
            }
        public:
            using parent = CLObject< cl_context, Context >;
            using parent::parent;
            using parent::operator=;

            friend class CLObject< cl_context , Context >;

            Context() :
                parent()
            {
            }
            Context( const Context& rhs ) :
                parent( rhs )
            {
            }
            Context( Context&& rhs ) :
                parent( rhs )
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
            template < typename Container , typename = decltype( std::begin( std::declval< Container >() ) ) >
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

            Context& operator = ( const Context& rhs )
            {
                return parent::operator = ( rhs );
            }
            Context& operator = ( Context&& rhs )
            {
                return parent::operator = ( rhs );
            }



        protected:
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

        public:
            cl_uint getNumDevices() const
            {
                return getUnary< cl_uint >( CL_CONTEXT_NUM_DEVICES );
            }
            std::vector< Device > getDevices() const
            {
                auto devs = getInfo< cl_device_id >( CL_CONTEXT_DEVICES );
                std::vector< std::reference_wrapper< const cl_device_id > > drefs( devs.begin() , devs.end() );
                return std::vector< Device >(
                        drefs.begin() , drefs.end()
                        );
            }
            Device getDevice() const
            {
                return Device( static_cast< const cl_device_id& >( getUnary< cl_device_id >( CL_CONTEXT_DEVICES ) ) );
            }
        };

        class Queue : public CLObject< cl_command_queue , Queue >
        {
        protected:
            void retain()
            {
                cl_int err = clRetainCommandQueue( handler );
                CheckError( err , "RetainCommandQueue" );
            }
            void release()
            {
                cl_int err = clReleaseCommandQueue( handler );
                CheckError( err , "ReleaseCommandQueue" );
            }
        public:
            using parent = CLObject< cl_command_queue , Queue >;
            using parent::parent;
            using parent::operator=;

            friend class CLObject< cl_command_queue , Queue >;

            Queue() :
                parent()
            {
            }
            Queue( const Queue& rhs ) :
                parent( rhs )
            {
            }
            Queue( Queue&& rhs ) :
                parent( rhs )
            {
            }
            explicit Queue( const Context& context , const Device& device )
            {
                cl_int err;
                handler = clCreateCommandQueue( context() , device() , 0 , &err );
                CheckError( err , "CreateCommandQueue" );
            }
            explicit Queue( const Context& context ) :
                Queue( context , context.getDevice() )
            {
            }

            Queue& operator = ( const Queue& rhs )
            {
                return parent::operator = ( rhs );
            }
            Queue& operator = ( Queue&& rhs )
            {
                return parent::operator = ( rhs );
            }

        protected:
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

        public:
            Context getContext() const
            {
                return Context( static_cast< const cl_context& >( getUnary< cl_context >( CL_QUEUE_CONTEXT ) ) );
            }
            Device getDevice() const
            {
                return Device( static_cast< const cl_device_id& >( getUnary< cl_device_id >( CL_QUEUE_DEVICE ) ) );
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
        class Buffer : public CLObject< cl_mem , Buffer >
        {
        protected:
            void retain()
            {
                cl_int err = clRetainMemObject( handler );
                CheckError( err , "clRetainMemObject" );
            }
            void release()
            {
                cl_int err = clReleaseMemObject( handler );
                CheckError( err , "clReleaseMemObject" );
            }

        public:
            using parent = CLObject< cl_mem , Buffer >;
            using parent::parent;
            using parent::operator=;

            friend class CLObject< cl_mem , Buffer >;

            Buffer() :
                parent()
            {
            }
            Buffer( const Buffer& rhs ) :
                parent( rhs )
            {
            }
            Buffer( Buffer&& rhs ) :
                parent( rhs )
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

            Buffer& operator = ( const Buffer& rhs )
            {
                return parent::operator = ( rhs );
            }
            Buffer& operator = ( Buffer&& rhs )
            {
                return parent::operator = ( rhs );
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

            struct map_deleter_s
            {
                cl_mem buffer;
                cl_command_queue queue;

                inline void operator () ( void *ptr )
                {
                    cl_int err = clEnqueueUnmapMemObject( queue , buffer , ptr , 0 , 0 , 0 );
                    CheckError( err , "clEnqueueUnmapMemObject" );
                }
            };
            template < typename T >
            Ptr< T , map_deleter_s > map( const Queue& queue ,
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
                return Ptr< T , map_deleter_s >( reinterpret_cast< T* >( ret ) , { handler , queue() } );
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

        class Program : public CLObject< cl_program , Program >
        {
        protected:
            void retain()
            {
                cl_int err = clRetainProgram( handler );
                CheckError( err , "clRetainProgram" );
            }
            void release()
            {
                cl_int err = clReleaseProgram( handler );
                CheckError( err , "clReleaseProgram" );
            }
        public:
            using parent = CLObject< cl_program , Program >;
            using parent::parent;
            using parent::operator=;

            friend class CLObject< cl_program , Program >;

            Program() :
                parent()
            {
            }
            Program( const Program& rhs ) :
                parent( rhs )
            {
            }
            Program( Program&& rhs ) :
                parent( rhs )
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
            explicit Program( const Context& context , const char *source )
            {
                cl_int err;
                handler = clCreateProgramWithSource( context() , 1 , &source , 0 , &err );
                CheckError( err , "clCreateProgramWithSource 4" );
            }
            explicit Program( const Context& context , const char *source , size_t length )
            {
                cl_int err;
                handler = clCreateProgramWithSource( context() , 1 , &source , &length , &err );
                CheckError( err , "clCreateProgramWithSource 3" );
            }

            Program& operator = ( const Program& rhs )
            {
                return parent::operator = ( rhs );
            }
            Program& operator = ( Program&& rhs )
            {
                return parent::operator = ( rhs );
            }

            void build( const Device& device , const char *options = 0 ) const
            {
                cl_int err = clBuildProgram( handler , 1 , &device.handler , options , 0 , 0 );
                CheckError( err , "clBuildProgram 1" );
                build_check( device );
            }
        protected:
            void build_check( const Device& device ) const
            {
                auto status = getBuildInfoUnary< cl_build_status >( device , CL_PROGRAM_BUILD_STATUS );
                switch( status )
                {
                    //case CL_BUILD_NONE:
                        //break;
                    case CL_BUILD_ERROR:
                    {
                        ERROR( "Build Failed" );
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
                LOG( "program build log : ---------------" );
                auto ret = getBuildInfo( device , CL_PROGRAM_BUILD_LOG );
                LOG( std::string( ret.begin() , ret.end() ) );
                LOG( "Log end; --------------------------" );
            }
        public:
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
            template < typename Container , typename = decltype( std::begin( std::declval< Container >() ) ) >
            void build( Container&& devices , const char *options = 0 ) const
            {
                build( std::begin( devices ) , std::end( devices ) , options );
            }


        protected:
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
        public:

            Context getContext() const
            {
                return Context( static_cast< const cl_context& >( getUnary< cl_context >( CL_PROGRAM_CONTEXT ) ) );
            }
            cl_uint getNumDevices() const
            {
                return getUnary< cl_uint >( CL_PROGRAM_NUM_DEVICES );
            }
            std::vector< Device > getDevices() const
            {
                auto devs = getInfo< cl_device_id >( CL_PROGRAM_DEVICES );
                std::vector< std::reference_wrapper< const cl_device_id > > drefs( devs.begin() , devs.end() );
                return std::vector< Device >( drefs.begin() , drefs.end() );
            }
            Device getDevice() const
            {
                return Device( static_cast< const cl_device_id& >( getUnary< cl_device_id >( CL_PROGRAM_DEVICES ) ) );
            }
            std::string getSource() const
            {
                auto ret = getInfo< char >( CL_PROGRAM_SOURCE );
                return std::string( ret.begin() , ret.end() );
            }
        };

        class Kernel : public CLObject< cl_kernel , Kernel >
        {
        protected:
            void retain()
            {
                cl_int err = clRetainKernel( handler );
                CheckError( err , "clRetainKernel" );
            }
            void release()
            {
                cl_int err = clReleaseKernel( handler );
                CheckError( err , "clReleaseKernel" );
            }
        public:
            using parent = CLObject< cl_kernel , Kernel >;
            using parent::parent;
            using parent::operator=;

            friend class CLObject< cl_kernel , Kernel >;

            Kernel() :
                parent()
            {
            }
            Kernel( const Kernel& rhs ) :
                parent( rhs )
            {
            }
            Kernel( Kernel&& rhs ) :
                parent( rhs )
            {
            }
            explicit Kernel( const Program& program , const char *name )
            {
                cl_int err;
                handler = clCreateKernel( program() , name , &err );
                CheckError( err , "clCreateKernel : " , name );

#ifndef NDEBUG
                const Device dev = program.getDevice();
                LOG( "KERNEL " , getName() , "() : " );
                LOG( "work_group_size : " , getWorkgroupsize( dev ) );
                LOG( "preffered_workgroupsize_multiple : " , getPreffered_workgroupsize_multiple( dev ) );
                LOG();
#endif
            }

            Kernel& operator = ( const Kernel& rhs )
            {
                return parent::operator = ( rhs );
            }
            Kernel& operator = ( Kernel&& rhs )
            {
                return parent::operator = ( rhs );
            }

            template < std::size_t ID = 0 , typename T0 >
            void SetArguments( T0&& arg0 )
            {
                this->operator[]( ID ) = arg0;
            }
            template < std::size_t ID = 0 , typename T0 , typename ... Ts >
            void SetArguments( T0&& arg0 , Ts&& ... args )
            {
                this->operator[]( ID ) = arg0;
                SetArguments< ID + 1 >( std::forward< Ts >( args )... );
            }

            struct ArgumentWrapper
            {
                cl_uint index;
                const Kernel& kernel;

                template < typename T >
                const T& operator = ( const T& arg ) const
                {
                    cl_int err = clSetKernelArg( kernel() , index , sizeof( T ) , &arg );
                    CheckError( err , kernel.getName() , " : " , "clSetKernelArg 1 : " , index );
                    return arg;
                }
                const Buffer& operator = ( const Buffer& buf ) const
                {
                    cl_int err = clSetKernelArg( kernel() , index , sizeof( cl_mem ) , &buf.handler );
                    CheckError( err , kernel.getName() , " : " , "clSetKernelArg 2 : " , index );
                    return buf;
                }
                void Set( size_t size , void *ptr = 0 ) const
                {
                    cl_int err = clSetKernelArg( kernel() , index , size , ptr );
                    CheckError( err , kernel.getName() , " : " , "clSetKernelArg 3 : " , index );
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
                    err = clGetKernelArgInfo( kernel() , index , info , 0 , 0 , &num );
                    CheckError( err , kernel.getName() , " : " , "clGetKernelArgInfo 1 : " , info );
                    std::vector< T > data( num/sizeof( T ) );
                    err = clGetKernelArgInfo( kernel() , index , info , num , data.data() , 0 );
                    CheckError( err , kernel.getName() , " : " , "clGetKernelArgInfo 2 : " , info );
                    return data;
                }
                template < typename RET >
                RET getUnary( cl_kernel_arg_info info ) const
                {
                    RET ret;
                    cl_int err = clGetKernelArgInfo( kernel() , index , info , sizeof( RET ) , &ret , 0 );
                    CheckError( err , kernel.getName() , " : " , "clGetKernelArgInfo 3 : " , info );
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
                return ArgumentWrapper{ index , *this };
            }

        protected:
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
                CheckError( err , getName() , " : " , "clGetKernelWorkGroupInfo 1 : " , info );
                std::vector< T > data( num/sizeof( T ) );
                err = clGetKernelWorkGroupInfo( handler, device() , info , num , data.data() , 0 );
                CheckError( err , getName() , " : " , "clGetKernelWorkGroupInfo 2 : " , info );
                return data;
            }
            template < typename RET >
            RET getWorkGroupInfoUnary( const Device& device , cl_kernel_work_group_info info ) const
            {
                RET ret;
                cl_int err = clGetKernelWorkGroupInfo( handler , device() , info , sizeof( RET ) , &ret , 0 );
                CheckError( err , getName() , " : " , "clGetKernelWorkGroupInfo 3 : " , info );
                return ret;
            }
        public:

            size_t getPreffered_workgroupsize_multiple( const Device& dev ) const
            {
                return getWorkGroupInfoUnary< size_t >( dev , CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE );
            }
            size_t getWorkgroupsize( const Device& dev ) const
            {
                return getWorkGroupInfoUnary< size_t >( dev , CL_KERNEL_WORK_GROUP_SIZE );
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
                return Context( static_cast< const cl_context& >( getUnary< cl_context >( CL_KERNEL_CONTEXT ) ) );
            }
            Program getProgram() const
            {
                return Program( static_cast< const cl_program& >( getUnary< cl_program >( CL_KERNEL_PROGRAM ) ) );
            }
            std::string getAttributes() const
            {
                auto vec = getInfo( CL_KERNEL_ATTRIBUTES );
                return std::string( vec.begin() , vec.end() );
            }

            using parent::operator ();
            inline void operator () ( const Queue& queue ,
                                      std::initializer_list< size_t > global_size ,
                                      std::initializer_list< size_t > local_size ,
                                      std::initializer_list< size_t > global_offset ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , global_size.size() ,
                                    global_offset.begin() , global_size.begin() , local_size.begin() ,
                                    0 , 0 , 0 );
                CheckError( err , getName() , " : " , "clEnqueueNDRangeKernel 1" );
            }
            inline void operator () ( const Queue& queue ,
                                      std::initializer_list< size_t > global_size ,
                                      std::initializer_list< size_t > local_size ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , global_size.size() ,
                                    0 , global_size.begin() , local_size.begin() ,
                                    0 , 0 , 0 );
                CheckError( err , getName() , " : " , "clEnqueueNDRangeKernel 2" );
            }
            inline void operator () ( const Queue& queue ,
                                      std::initializer_list< size_t > global_size ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , global_size.size() ,
                                    0 , global_size.begin() , 0 ,
                                    0 , 0 , 0 );
                CheckError( err , getName() , " : " , "clEnqueueNDRangeKernel 3" );
            }
            inline void operator () ( const Queue& queue , size_t global_size , size_t local_size , size_t global_offset ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , 1 ,
                                    &global_offset , &global_size , &local_size ,
                                    0 , 0 , 0 );
                CheckError( err , getName() , " : " , "clEnqueueNDRangeKernel 5" );
            }
            inline void operator () ( const Queue& queue , size_t global_size , size_t local_size ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , 1 ,
                                    0 , &global_size , &local_size ,
                                    0 , 0 , 0 );
                CheckError( err , getName() , " : " , "clEnqueueNDRangeKernel 6" );
            }
            inline void operator () ( const Queue& queue , size_t global_size ) const
            {
                cl_int err = clEnqueueNDRangeKernel( queue() , handler , 1 ,
                                    0 , &global_size , 0 ,
                                    0 , 0 , 0 );
                CheckError( err , getName() , " : " , "clEnqueueNDRangeKernel 7" );
            }
            inline void operator () ( const Queue& queue ) const
            {
                cl_int err = clEnqueueTask( queue() , handler , 0 , 0 , 0 );
                CheckError( err , getName() , " : " , "clEnqueueNDRangeKernel 4" );
            }
        };

    };
};
