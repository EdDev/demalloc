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
#include <stdio.h>
#include <malloc.h>

#include "demalloc.h"

#define UNUSED(v) (void(v))
namespace
{

const int TEST_MEMSIZE = 1024;
const int TEST_MEMALIGNSIZE = 256;

}


static void report_allocator_prehook(const char * allocator_name, void *ptr, size_t size, const void *caller)
{
    printf("%s pre-hook, ptr=%p, reqSize=%lu [caller:%p]\n", allocator_name, ptr, size, caller);
}

static void report_allocator_posthook(const char * allocator_name, void *ptr, size_t size, const void *caller)
{
    printf("%s post-hook, ptr=%p, reqSize=%lu [caller:%p]\n", allocator_name, ptr, size, caller);
}

void dbg_mem_malloc_prehook(size_t *size, const void *caller)
{
    report_allocator_prehook("malloc", NULL, *size, caller);
}

static void dbg_mem_calloc_prehook(size_t *size, const void *caller)
{
    report_allocator_prehook("calloc", NULL, *size, caller);
}

static void dbg_mem_realloc_prehook(void **ptr, size_t *size, const void *caller)
{
    report_allocator_prehook("realloc", *ptr, *size, caller);
}

static void dbg_mem_memalign_prehook(size_t alignment, size_t *size, const void *caller)
{
    UNUSED(alignment);
    report_allocator_prehook("memalign", NULL, *size, caller);
}

static void dbg_mem_posix_memalign_prehook(size_t alignment, size_t *size, const void *caller)
{
    UNUSED(alignment);
    report_allocator_prehook("posix_memalign", NULL, *size, caller);
}

static void dbg_mem_free_prehook(void **ptr, const void *caller)
{
    report_allocator_prehook("free", *ptr, 0, caller);
}

void dbg_mem_malloc_posthook(void **ptr, size_t size, const void *caller)
{
    report_allocator_posthook("malloc", *ptr, size, caller);
}

static void dbg_mem_calloc_posthook(void **ptr, size_t size, const void *caller)
{
    report_allocator_posthook("calloc", *ptr, size, caller);
}

static void dbg_mem_realloc_posthook(void **ptr, size_t size, const void *caller)
{
    report_allocator_posthook("realloc", *ptr, size, caller);
}

static void dbg_mem_memalign_posthook(void **ptr, size_t alignment, size_t size, const void *caller)
{
    UNUSED(alignment);
    report_allocator_posthook("memalign", *ptr, size, caller);
}

static void dbg_mem_posix_memalign_posthook(void **ptr, size_t alignment, size_t size, const void *caller)
{
    UNUSED(alignment);
    report_allocator_posthook("posix_memalign", *ptr, size, caller);
}

static void dbg_mem_free_posthook(void **ptr, const void *caller)
{
    report_allocator_posthook("free", *ptr, 0, caller);
}

void dbg_mem_init_hook()
{
    demalloc_malloc_prehook = dbg_mem_malloc_prehook;
    demalloc_calloc_prehook = dbg_mem_calloc_prehook;
    demalloc_realloc_prehook = dbg_mem_realloc_prehook;
    demalloc_memalign_prehook = dbg_mem_memalign_prehook;
    demalloc_posix_memalign_prehook = dbg_mem_posix_memalign_prehook;
    demalloc_free_prehook = dbg_mem_free_prehook;

    demalloc_malloc_posthook = dbg_mem_malloc_posthook;
    demalloc_calloc_posthook = dbg_mem_calloc_posthook;
    demalloc_realloc_posthook = dbg_mem_realloc_posthook;
    demalloc_memalign_posthook = dbg_mem_memalign_posthook;
    demalloc_posix_memalign_posthook = dbg_mem_posix_memalign_posthook;
    demalloc_free_posthook = dbg_mem_free_posthook;
}

void dbg_mem_uninit_hook()
{
    demalloc_malloc_prehook = NULL;
    demalloc_calloc_prehook = NULL;
    demalloc_realloc_prehook = NULL;
    demalloc_memalign_prehook = NULL;
    demalloc_posix_memalign_prehook = NULL;
    demalloc_free_prehook = NULL;

    demalloc_malloc_posthook = NULL;
    demalloc_calloc_posthook = NULL;
    demalloc_realloc_posthook = NULL;
    demalloc_memalign_posthook = NULL;
    demalloc_posix_memalign_posthook = NULL;
    demalloc_free_posthook = NULL;
}

class MemDbg_init
{
public:
    MemDbg_init()
    {
        dbg_mem_init_hook();
    }

    ~MemDbg_init()
    {
        dbg_mem_uninit_hook();
    }
} memdbg;

/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = dbg_mem_init_hook;

static void report_allocator_result(const char * allocator_name, void * memptr)
{
    printf("%s returned memptr=%p\n", allocator_name, memptr);
}


static void chk_malloc()
{
    void * memptr = malloc(TEST_MEMSIZE);
    report_allocator_result("malloc", memptr);
    free(memptr);
}

static void chk_calloc()
{
    void * memptr = calloc(1, TEST_MEMSIZE);
    report_allocator_result("calloc", memptr);
    free(memptr);
}

static void chk_memalign()
{
    void * memptr = memalign(TEST_MEMALIGNSIZE, TEST_MEMSIZE);
    report_allocator_result("memalign", memptr);
    free(memptr);
}

static void chk_posix_memalign()
{
    void * memptr = NULL;
    posix_memalign(&memptr, TEST_MEMALIGNSIZE, TEST_MEMSIZE);
    report_allocator_result("poxis_memalign", memptr);
    free(memptr);
}

static void chk_new()
{
    char * memptr = new char[TEST_MEMSIZE];
    report_allocator_result("new", memptr);
    delete(memptr);
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    printf("Starting the example program %s...\n", argv[0]);

    chk_malloc();
    chk_calloc();
    chk_memalign();
    chk_posix_memalign();
    chk_new();

    exit(0);
}

