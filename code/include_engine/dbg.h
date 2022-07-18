/**
 * ================================================================================
 * @file dbg.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Header file containing a set of useful debug macros. The code has been 
 * coppied from the book "Learn C Hard Way".
 * @version 0.2
 * @date 2022-07-17
 * ================================================================================
 */

#ifndef DBG_H_
#define DBG_H_

/* Standard library includes: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definition of useful debug macros. */
/* Debug macro. Prints the debug information. */
#define dbg_debug(M, ...) { printf("[DEBUG] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); }

/* Error macro. Prints the error message and exits the program. */
#define dbg_error(M, ...) { printf("[ERROR] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); exit(1); }

/* Warn macro. Prints the warning message. */
#define dbg_warn(M, ...) { printf("[WARN] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); }

/* Info macro. Prints the info message. */
#define dbg_info(M, ...) { printf("[INFO] | %s | %s | %d | ", __FILE__, __func__, __LINE__); \
    printf(M "\n", ##__VA_ARGS__); }

/* Check macro. Analog to assert. Check for the truth of A condition.  */
#define dbg_check(A, M, ...) if(!(A)) { dbg_error(M, ##__VA_ARGS__); }

#endif // DBG_H_
