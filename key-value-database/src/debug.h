//
// Created by snipy on 27.03.18.
//

#ifndef KEY_VALUE_DATABASE_DEBUG_H
#define KEY_VALUE_DATABASE_DEBUG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

/*
 * Configuration
 */

// #define NDEBUG
#define LOG_LEVEL LOG_LEVEL_DEBUG

/*
 * Definition
 */

// -- Log level
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_INFO  3
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERR   1
#define LOG_LEVEL_NONE  0

// -- Color
#define RED    "\e[31m"
#define GREEN  "\e[32m"
#define YELLOW "\e[33m"
#define WHITE  "\e[1m"

// reset color
#define COLOR_X "\e[m"

// -- Utils
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// -- Logging
#if !defined(NDEBUG) && (LOG_LEVEL >= LOG_LEVEL_ERR)
    #define log_err(M, ...) fprintf(stderr, "[ERR] (%s:%d: errno: %s): " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#else
    #define log_err(M, ...)
#endif

#if !defined(NDEBUG) && (LOG_LEVEL >= LOG_LEVEL_WARN)
    #define log_warn(M, ...) fprintf(stderr, "[WAR] (%s:%d: errno: %s): " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#else
    #define log_warn(M, ...)
#endif

#if !defined(NDEBUG) && (LOG_LEVEL >= LOG_LEVEL_INFO)
    #define log_info(M, ...)         fprintf(stderr, GREEN "[INF]: " M "\n", ##__VA_ARGS__)
    #define log_info_full(M, ...)    fprintf(stderr, GREEN "[INF] (%s:%d): " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
    #define log_info_mul(io_statements) do {\
                                fprintf(stderr, GREEN "[INF] ");\
                                io_statements\
                                fprintf(stderr, COLOR_X);\
                                }while(0);
    #define log_info_print(M, ...)    fprintf(stderr, M, ##__VA_ARGS__)
    #define log_info_nl log_info("\n");
#else
    #define log_info(M, ...)
    #define log_info_full(M, ...)
    #define log_info_mul(io_statements)
    #define log_info_print(M, ...)
    #define log_info_nl
#endif

#if !defined(NDEBUG) && (LOG_LEVEL >= LOG_LEVEL_DEBUG)
    #define debug(M, ...)      fprintf(stderr, "[DBG]: " M "\n", ##__VA_ARGS__)
    #define debug_full(M, ...) fprintf(stderr, "[DBG] %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define debug(M, ...)
    #define debug_full(M, ...)
#endif

// Some utility macro

#define check_mem(PTR, STATEMENTS) do {\
    if((PTR) == NULL) {\
        log_err(MEM_ALLOC_ERR,__LINE__, __FILE__);\
        STATEMENTS;\
    }} while(0);

#define check_mem_and_exit(PTR) check_mem(PTR, exit(EXIT_FAILURE));

// -- Some messages

#define MEM_ALLOC_ERR "Can't allocate memory for the sentinel at line %d in file %s\n"

#endif //KEY_VALUE_DATABASE_DEBUG_H
