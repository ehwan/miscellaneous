#pragma once

#include <vector>
#include <array>
#include <cassert>
#include <cmath>

namespace EH
{
    namespace Util
    {
        namespace Tween
        {
            // input in the [ 0 , 1 ] variable t,
            // return [ 0 , 1 ] computed value
            template < typename T = float >
            class Interpolater
            {
            public:
                // if true, output will clamped to boundary[ 0 , 1 ]
                bool bound_visible;

                Interpolater(){}

                // @coeff : in time-velocity graph, the stiffness of graph.
                // if @coeff is 0 , velocity is constant ( v = 1 )
                // if @coeff is 1 , velocity slides through [ 0 , 2 ]
                //                  so that distance is in  [ 0 , 1 ]
                //
                // the full formula :
                // velocity = 2*coeff*time - coeff + 1
                constexpr Interpolater( T coeff , bool bound_visib = true ) :
                    m_coeff( coeff ) , mkp1( -coeff + T(1) ) , bound_visible( bound_visib )
                {
                }
                constexpr inline void Load( T coeff )
                {
                    m_coeff =  coeff;
                    mkp1    = -coeff + T(1);
                }

                constexpr T operator () ( T t ) const
                {
                    if( t < 0 && bound_visible ){ return 0; }
                    else if( t > 1 && bound_visible ){ return 1; }

                    return m_coeff * t*t + mkp1 * t;
                }
            protected:
                T m_coeff;

                // -coeff + 1
                T mkp1;
            };

            template < typename Container >
            struct BaseTweener
            {
                Container container;
                float sizef;
                std::size_t sizei;
            };

            template < typename T , typename Container >
            struct CubicTweener : BaseTweener< Container >
            {
                typedef BaseTweener< Container > base_type;

                constexpr T operator ()( float t )
                {
                    //assert( container.size() >= 4 );

                    float t1;
                    std::size_t index;
                    if( t < 1 )
                    {
                        index = 1;
                        t1 = t - 1;
                    }else if( t < base_type::sizef - 2 )
                    {
                        const float flr = std::floor( t );
                        index = ( decltype(index) )flr;
                        t1 = t - (float)flr;
                    }else
                    {
                        index = base_type::sizei - 3;
                        t1 = t - std::floor( t ) + 1;
                    }
                    const float t2 = t1 * t1;
                    const float t3 = t2 * t1;
                    return
                        ( - t3/6 + t2/2 + t1/3     ) * base_type::container[ index - 1 ] +
                        ( - t3/2 - t2   - t1/2 + 1 ) * base_type::container[ index     ] +
                        ( - t3/2 + t2/2 + t1       ) * base_type::container[ index + 1 ] +
                        (   t3/6 -        t1/6     ) * base_type::container[ index + 2 ];

                }
            };
            template < typename T , typename Container >
            struct MonotoneCubicTweener : BaseTweener< Container >
            {
                typedef BaseTweener< Container > base_type;

                constexpr T operator () ( float t )
                {

                    float t1;
                    std::size_t index;
                    if( t < 0.5f )
                    {
                        t1 = t - 1;
                        index = 1;
                    }else if( t > base_type::sizef - 1.5f )
                    {
                        t1 = t - (base_type::sizef - 2);
                        index = base_type::sizei - 2;
                    }else
                    {
                        const float rt = std::round(t);
                        t1 = t - rt;
                        index = ( decltype(index) )rt;
                    }
                    const float t2 = t1 * t1;

                    return
                        (   t2/2 - t1/2 ) * base_type::container[ index - 1 ] +
                        ( - t2   +  1   ) * base_type::container[ index     ] +
                        (   t2/2 + t1/2 ) * base_type::container[ index + 1 ];
                }
            };

            template < typename T , typename Container >
            struct MonotoneSquareTweener : BaseTweener< Container >
            {
                typedef BaseTweener< Container > base_type;

                constexpr T operator () ( float t )
                {

                    float t1;
                    std::size_t index;
                    if( t < 1.5f )
                    {
                        t1 = t - 1.5f;
                        index = 1;
                    }else if( t < base_type::sizef - 2.5f )
                    {
                        const float fl = std::floor(t);
                        t1 = t - ( fl + 0.5f );
                        index = ( decltype(index) )fl;
                    }else
                    {
                        t1 = t - ( base_type::sizef - 2.5f );
                        index = base_type::sizei - 3;
                    }
                    const float t2 = t1 * t1;
                    const float t3 = t2 * t1;

                    const auto m0 = 0.5f * ( base_type::container[index+1] + base_type::container[index-1] );
                    const auto m1 = 0.5f * ( base_type::container[index+2] + base_type::container[index+1] );

                    return
                         base_type::container[index]*(2.0f*t3 - 1.5f*t1 + 0.5f)
                        +base_type::container[index+1]*(-2.0f*t3 + 1.5f*t1 + 0.5f)
                        +m0*(t3 - 0.5f*t2 - 0.25f*t1 + 0.25f)
                        +m1*(t3 + 0.5f*t2 - 0.25f*t1 - 0.25f);
                }
            };
        };  // namespace Tween
    };  // namespace Util
};  // namespace EH

