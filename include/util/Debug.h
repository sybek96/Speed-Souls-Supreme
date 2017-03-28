/// <summary>
/// @author Rafael Plugge
/// @version 1.0
/// @brief Header for Debug messaging function.
/// 
/// Based on preprocessor definitions,
/// a template debug messaging function is created.
/// </summary>

#ifndef DEBUG_H
#define DEBUG_H

// dynamically change from debug to release
#ifdef __DEBUG

// include standard output stream
#include <iostream>
#include <string>

//Define DEBUG 1 or 2 for greater level of debug messages
#define DEBUG 2
#else
#define DEBUG 0
#endif


// Template function for Debug messaging

#ifdef DEBUG


#if (DEBUG >= 1)
template<typename X>
void debugMsg(X msg)
{
	std::cout << msg << std::endl;
}

#if (DEBUG >= 2)
template<typename X, typename Y>
void debugMsg(X msgTitle, Y msgContent)
{
	debugMsg(msgTitle);
	debugMsg(msgContent);
	debugMsg("");
}
#else
template<typename X, typename Y>
void debugMsg(X msgTitle, Y msgContent)
{
	debugMsg(msgContent);
	debugMsg("");
}
#endif // (DEBUG >= 2)

#endif // (DEBUG >= 1)

#if (DEBUG == 0)
template<typename X>
void debugMsg(X msg) {}

template<typename X, typename Y>
void debugMsg(X msgT, Y msgC) {}
#endif // (DEBUG == 0)


#else // !DEBUG

template<typename X>
void debugMsg(X msg) {}

template<typename X, typename Y>
void debugMsg(X msgT, Y msgC) {}

#endif // DEBUG

#endif // !DEBUG_H