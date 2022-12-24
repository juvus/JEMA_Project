/**
 * ================================================================================
 * @file include_engine/memory_object.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with objects
 * stored in memory.
 * @version 0.2
 * @date 2022-12-04
 * ================================================================================
 */

#ifndef JGE_ENGINE_MEMORY_OBJECT_H_
#define JGE_ENGINE_MEMORY_OBJECT_H_

#include "include_engine/utils.h"

/**
 * @brief Structure for holding the object data in memory.
 */
struct MemObject_ 
{
    u8 *data;  /**< Pointer to the memory object data as a sequence of bytes. */
    u64 size;  /**< Size of the memory object in bytes. */
};
typedef struct MemObject_ MemObject;

/**
 * @brief Object constructor.
 * @return MemObject* Pointer to the MemObject structure.
 */
MemObject*
MemObject_Constructor(void);

/**
 * @brief Object destructor.
 * @param mem_object Pointer to the MemObject structure.
 */
void
MemObject_Destructor(MemObject *mem_object);

/**
 * @brief Initialization of the memory object by raw data of the file loaded 
 * from the disc (loading raw file data to memory). 
 * @param mem_object Pointer to the MemObject structure.
 * @param file_path Path to the file on the computer disc.
 */
void
MemObject_InitByFile(MemObject *mem_object, char *file_path);

/**
 * @brief Writing the entire data of the memory object to disc.
 * @param mem_object Pointer to the MemObject structure.
 * @param file_path Path to the file on the computer disc.
 */
void
MemObject_WriteToDisc(MemObject *mem_object, char *file_path);

#endif  /* JGE_ENGINE_MEMORY_OBJECT_H_ */
