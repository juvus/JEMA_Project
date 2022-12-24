/**
 * ================================================================================
 * @file dbg.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Header file containing a set of useful debug macros. The code has been 
 * coppied from the book "Learn C Hard Way".
 * @version 0.3
 * @date 2022-07-17
 * ================================================================================
 */

#ifndef JEMA_ENGINE_DBG_H_
#define JEMA_ENGINE_DBG_H_

/* Standard library includes: */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* Game engine includes: */
#include "utils.h"

/* Static functions. */
/**
 * @brief Termination of the program.
 * @param exit_code descr 
 * @return void.
 */
static void 
terminate(u32 exit_code);

/* Definition of useful debug macros. */
/* Debug macro. Prints the debug information. */
#define dbg_debug(M, ...) \
if (true) \
{ \
    printf("[DEBUG] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); \
} \
else (void)0

/* Error macro. Prints the error message and exits the program. */
#define dbg_error(M, ...) \
if (true) \
{ \
    printf("[ERROR] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); \
    terminate(1); \
} \
else (void)0

/* Warn macro. Prints the warning message. */
#define dbg_warn(M, ...) \
if (true) \
{ \
    printf("[WARN] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); \
} \
else (void)0

/* Info macro. Prints the info message. */
#define dbg_info(M, ...) \
if (true) \
{ \
    printf("[INFO] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); \
} \
else (void)0

/* Check macro. Analog to assert. Check for the truth of A condition.  */
#define dbg_check(A, M, ...) \
if (!(A)) \
{ \
    dbg_error(M, ##__VA_ARGS__); \
} \
else (void)0

static void 
terminate(u32 exit_code)
{
    exit(exit_code);
}

#endif  /* JEMA_ENGINE_DBG_H_ */
