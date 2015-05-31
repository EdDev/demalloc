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

#ifndef SRC_LIB_DEMALLOC_H_
#define SRC_LIB_DEMALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Allocators and free replacer.
 */
extern void *malloc (size_t __size);
extern void *calloc (size_t __nmemb, size_t __size);
extern void *realloc (void *__ptr, size_t __size);
extern void *memalign (size_t __alignment, size_t __size);
extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size);
extern void free (void *__ptr);

/*
 * Pre hook points
 */
extern void (* demalloc_malloc_prehook) (size_t *size, const void *caller);
extern void (* demalloc_calloc_prehook) (size_t *size, const void *caller);
extern void (* demalloc_realloc_prehook) (void **ptr, size_t *size, const void *caller);
extern void (* demalloc_memalign_prehook) (size_t alignment, size_t *size, const void *caller);
extern void (* demalloc_posix_memalign_prehook) (size_t alignment, size_t *size, const void *caller);
extern void (* demalloc_free_prehook) (void **ptr, const void *caller);

/*
 * Post hook points
 */
extern void (* demalloc_malloc_posthook) (void **ptr, size_t size, const void *caller);
extern void (* demalloc_calloc_posthook) (void **ptr, size_t size, const void *caller);
extern void (* demalloc_realloc_posthook) (void **ptr, size_t size, const void *caller);
extern void (* demalloc_memalign_posthook) (void **ptr, size_t alignment, size_t size, const void *caller);
extern void (* demalloc_posix_memalign_posthook) (void **ptr, size_t alignment, size_t size, const void *caller);
extern void (* demalloc_free_posthook) (void **ptr, const void *caller);

#ifdef __cplusplus
}
#endif

#endif /* SRC_LIB_DEMALLOC_H_ */
