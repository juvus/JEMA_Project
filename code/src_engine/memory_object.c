/**
 * ================================================================================
 * @file src_engine/file_io.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with file input-output.
 * @version 0.2
 * @date 2022-12-04
 * ================================================================================
 */

#include "include_engine/memory_object.h"

#include <windows.h>

#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/utils.h"

MemObject*
MemObject_Constructor(void)
{
    size_t size = sizeof(MemObject);
    MemObject *mem_object = (MemObject *)HelperFcn_MemAllocate(size);
    return mem_object;
}

MemObject*
MemObject_Destructor(MemObject *mem_object)
{
    VirtualFree(mem_object->data, 0, MEM_RELEASE);
    HelperFcn_MemFree(mem_object);
    return NULL;
}

void
MemObject_InitByFile(MemObject *mem_object, char *file_path)
{
    HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, 0, 
        OPEN_EXISTING, 0, 0);
    if (file_handle == INVALID_HANDLE_VALUE) 
    {
        dbg_error("%s", "File not found!");
    }

    DWORD size = GetFileSize(file_handle, 0);
    LPVOID data = VirtualAlloc(0, size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    if (data == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    
    DWORD bytes_read;  /* Amount of bytes that was really read from the disc. */
    BOOL flag = (ReadFile(file_handle, data, size, &bytes_read, 0)) && (size == bytes_read);
    if (!flag) 
    {
        dbg_error("%s", "Error reading file into memory object!");
    }

    mem_object->size = (u64)size;
    mem_object->data = (u8 *)data;
}

void
MemObject_WriteToDisc(MemObject *mem_object, char *file_path)
{
    /* TODO: Make definition of this method later on. */
   UNUSED(mem_object);
   UNUSED(file_path);
}
