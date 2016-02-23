#pragma once

#include <iostream>
#include <string>

#ifndef EHLOG_H
#define EHLOG_H

#ifndef NDEBUG
    #define ONLY_DEBUG(x) x
#else
    #define ONLY_DEBUG(x)
#endif

#ifdef NDEBUG
    #define EH_NO_LOG
#endif


namespace EH
{
    constexpr const char *ANSI_COLOR_RED    =   "\x1b[31m";
    constexpr const char *ANSI_COLOR_GREEN  =   "\x1b[32m";
    constexpr const char *ANSI_COLOR_YELLOW =   "\x1b[33m";
    constexpr const char *ANSI_COLOR_BLUE   =   "\x1b[34m";
    constexpr const char *ANSI_COLOR_MAGENTA=   "\x1b[35m";
    constexpr const char *ANSI_COLOR_CYAN   =   "\x1b[36m";
    constexpr const char *ANSI_COLOR_RESET  =   "\x1b[0m";
    constexpr const char *ANSI_CURSOR_TOP   = "\033[1;1H";
    constexpr const char *ANSI_TERMINAL_RESET = "\033[2J";

#ifndef EH_NO_LOG
    template < typename T >
    inline void LOGR( T&& arg0 )
    {
        std::cout << std::forward< T >( arg0 );
    }
#else
    template < typename T >
    inline void LOGR( T&& arg0 )
    {
    }
#endif

    template < typename T , typename ... Ts >
    inline void LOGR( T&& arg0 , Ts&& ... args )
    {
        LOGR( std::forward< T >(arg0) );
        LOGR( std::forward< Ts >( args )... );
    }
    template < typename ... Ts >
    inline void LOG( Ts&& ... args )
    {
        LOGR( std::forward< Ts >( args )... , '\n' );
    }
    template < typename ... Ts >
    inline void ERRORR( Ts&& ... args )
    {
        LOGR( ANSI_COLOR_RED , std::forward< Ts >( args )... , ANSI_COLOR_RESET );
    }
    template < typename ... Ts >
    inline void ERROR( Ts&& ... args )
    {
        ERRORR( std::forward< Ts >( args )... , '\n' );
    }
};  // namespace EH

#endif  // EHLOG_H
