#include <chrono>
#include <ostream>

namespace EH
{
    template < std::size_t Num , std::size_t Den >
    struct RatioName
    {
        constexpr static auto name = "";
    };
    template <>
    struct RatioName< 1 , 1000 >
    {
        constexpr static auto name = "milli";
        constexpr static auto short_name = "milli";
    };
    template <>
    struct RatioName< 1 , 1000000 >
    {
        constexpr static auto name = "micro";
        constexpr static auto short_name = "micro";
    };
    template <>
    struct RatioName< 1 , 1000000000 >
    {
        constexpr static auto name = "nano";
        constexpr static auto short_name = "nano";
    };
    template <>
    struct RatioName< 1000 , 1 >
    {
        constexpr static auto name = "kilo";
        constexpr static auto short_name = "K";
    };
    template <>
    struct RatioName< 1000000 , 1 >
    {
        constexpr static auto name = "mega";
        constexpr static auto short_name = "M";
    };
    template <>
    struct RatioName< 1000000000 , 1 >
    {
        constexpr static auto name = "giga";
        constexpr static auto short_name = "G";
    };
    template <>
    struct RatioName< 1000000000000 , 1 >
    {
        constexpr static auto name = "tera";
        constexpr static auto short_name = "T";
    };


    template < typename Ref , typename Ratio = std::ratio< 1 , 1 > >
    struct GapWrapper
    {
        using duration_type = std::chrono::duration< Ref , Ratio >;

        duration_type duration;

        template < typename Ref2 , typename Ratio2 >
        GapWrapper( std::chrono::duration< Ref2 , Ratio2 > dur ) :
            duration( std::chrono::duration_cast< duration_type >( dur ) )
        {
        }
    };
    struct Timer
    {
        using clock_type = std::chrono::steady_clock;
        //using clock_type = std::chrono::system_clock;

        using time_point_type = clock_type::time_point;

        time_point_type point;
        time_point_type::duration saved_duration;
        bool running;

        Timer() :
            point( clock_type::now() ) ,
            saved_duration( time_point_type::duration::zero() ) ,
            running( true )
        {
        }

        template < typename Ref , typename Ratio = std::ratio< 1 , 1 > >
        auto count()
        {
            time_point_type::duration gap = saved_duration;
            saved_duration = time_point_type::duration::zero();
            auto now = clock_type::now();
            if( running )
            {
                gap += ( now - point );
            }else
            {
                running = true;
            }
            point = now;
            return GapWrapper< Ref , Ratio >( gap );
        }

        void stop()
        {
            running = false;
            auto now = clock_type::now();
            auto gap = now - point;
            saved_duration += gap;
        }
        void start()
        {
            running = true;
            point = clock_type::now();
        }
    };

    template < typename Ref , typename Ratio >
    std::ostream& operator << ( std::ostream& stream , GapWrapper< Ref , Ratio >&& gap )
    {
        stream << "Timer elapsed : " << gap.duration.count() << ' ' << RatioName< Ratio::num , Ratio::den >::name << " seconds";
        return stream;
    }

};
