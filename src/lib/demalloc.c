/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2015 Edward Haas (edwardh.dev@gmail.com). All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>

/*
 * Access glibc allocators and deleter
 */
extern __typeof (malloc) __libc_malloc;
extern __typeof (calloc) __libc_calloc;
extern __typeof (realloc) __libc_realloc;
extern __typeof (memalign) __libc_memalign;
extern __typeof (free) __libc_free;

/*
 * Pre & Post hook points
 */
void (* demalloc_malloc_prehook) (size_t *size, const void *caller) = NULL;
void (* demalloc_calloc_prehook) (size_t *size, const void *caller) = NULL;
void (* demalloc_realloc_prehook) (void **ptr, size_t *size, const void *caller) = NULL;
void (* demalloc_memalign_prehook) (size_t alignment, size_t *size, const void *caller) = NULL;
void (* demalloc_posix_memalign_prehook) (size_t alignment, size_t *size, const void *caller) = NULL;
void (* demalloc_free_prehook) (void **ptr, const void *caller) = NULL;

void (* demalloc_malloc_posthook) (void **ptr, size_t size, const void *caller) = NULL;
void (* demalloc_calloc_posthook) (void **ptr, size_t size, const void *caller) = NULL;
void (* demalloc_realloc_posthook) (void **ptr, size_t size, const void *caller) = NULL;
void (* demalloc_memalign_posthook) (void **ptr, size_t alignment, size_t size, const void *caller) = NULL;
void (* demalloc_posix_memalign_posthook) (void **ptr, size_t alignment, size_t size, const void *caller) = NULL;
void (* demalloc_free_posthook) (void **ptr, const void *caller) = NULL;

#define powerof2(val) ((((val) - 1) & (val)) == 0)

void *malloc (size_t __size)
{
    if (demalloc_malloc_prehook)
        demalloc_malloc_prehook(&__size, __builtin_return_address(0));

    void * ptr = __libc_malloc(__size);

    if (demalloc_malloc_posthook)
        demalloc_malloc_posthook(&ptr, __size, __builtin_return_address(0));

    return ptr;
}

void *calloc (size_t __nmemb, size_t __size)
{
    if (demalloc_calloc_prehook)
        demalloc_calloc_prehook(&__size, __builtin_return_address(0));

    void * ptr = __libc_calloc(__nmemb, __size);

    if (demalloc_calloc_posthook)
        demalloc_calloc_posthook(&ptr, __size, __builtin_return_address(0));

    return ptr;
}

void *realloc (void *__ptr, size_t __size)
{
    if (demalloc_realloc_prehook)
        demalloc_realloc_prehook(&__ptr, &__size, __builtin_return_address(0));

    void * ptr = __libc_realloc(__ptr, __size);

    if (demalloc_realloc_posthook)
        demalloc_realloc_posthook(&ptr, __size, __builtin_return_address(0));

    return ptr;

}

void *memalign (size_t __alignment, size_t __size)
{
    if (demalloc_memalign_prehook)
        demalloc_memalign_prehook(__alignment, &__size, __builtin_return_address(0));

    void * ptr = __libc_memalign (__alignment, __size);

    if (demalloc_memalign_posthook)
        demalloc_memalign_posthook(&ptr, __alignment, __size, __builtin_return_address(0));

    return ptr;
}

int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
{
    if (demalloc_posix_memalign_prehook)
        demalloc_posix_memalign_prehook(__alignment, &__size, __builtin_return_address(0));

    if((__alignment % sizeof (void *) != 0) ||
      (!powerof2 (__alignment / sizeof (void *)) != 0) ||
      (__alignment == 0))
        return EINVAL;

    *__memptr = __libc_memalign (__alignment, __size);

    if(*__memptr == NULL)
        return ENOMEM;

    if (demalloc_posix_memalign_posthook)
        demalloc_posix_memalign_posthook(__memptr, __alignment, __size, __builtin_return_address(0));

    return 0;
}

void free (void *__ptr)
{
    if(demalloc_free_prehook)
        demalloc_free_prehook(&__ptr, __builtin_return_address(0));

    __libc_free(__ptr);

    if (demalloc_free_posthook)
        demalloc_free_posthook(&__ptr, __builtin_return_address(0));
}


