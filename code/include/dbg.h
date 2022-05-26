/**
 * ================================================================================
 * @file dbg.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Header file containing a set of useful debug macros. It was got from
 * the book "Learn C Hard Way".
 * @version 0.1
 * @date 2021-12-18
 * @copyright JuvuSoft (c) 2021
 * ================================================================================
 */

#ifndef DBG_H_
#define DBG_H_

#include <stdio.h>
#include <string.h>

/* Definition of useful debug macroses */
#define debug(M, ...) { printf("[DEBUG] | %s | %s | %d | ", __FILE__, __func__, __LINE__); printf(M "\n", ##__VA_ARGS__); }
#define error(M, ...) { printf("[ERROR] | %s | %s | %d | ", __FILE__, __func__, __LINE__); printf(M "\n", ##__VA_ARGS__); }
#define warn(M, ...) { printf("[WARN] | %s | %s | %d | ", __FILE__, __func__, __LINE__); printf(M "\n", ##__VA_ARGS__); }
#define info(M, ...) { printf("[INFO] | %s | %s | %d | ", __FILE__, __func__, __LINE__); printf(M "\n", ##__VA_ARGS__); }

#define check(A, M, ...) if(!(A)) { error(M, ##__VA_ARGS__); }

#endif // DBG_H_
