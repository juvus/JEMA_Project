/**
 * ================================================================================
 * @file file_io.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with file
 * input-output.
 * @version 0.2
 * @date 2022-07-08
 * ================================================================================
 */

#ifndef FILE_IO_H_
#define FILE_IO_H_

/* Game engine includes: */
#include <utils.h>

/**
 * @brief Structure for holding the file data in memory.
 */
struct File 
{
    u8 *data;  /**< Pointer to the file data as a sequence of bytes. */
    u64 size;  /**< Size of the file in bytes. */
};
typedef struct File File_t;

/**
 * @brief Constructor of the File class.
 * @return File_t* Pointer to the File structure.
 */
File_t*
file_constructor(void);

/**
 * @brief Destructor of the File class.
 * @param file Pointer to the File structure.
 */
void
file_destructor(File_t *file);

/**
 * @brief Loading the entire raw file from disc into memory.
 * @param file Pointer to the File structure.
 * @param file_path Path to the file on the computer disc.
 * @return b32 Success check.
 */
b32
file_load_to_memory(File_t *file, char *file_path);

/**
 * @brief Writing the entire file from memory to disc.
 * @param file Pointer to the File structure.
 * @param file_path Path to the file on the computer disc.
 * @return b32 Success check.
 */
b32
file_write_to_disc(File_t *file, char *file_path);

#endif // FILE_IO_H_
