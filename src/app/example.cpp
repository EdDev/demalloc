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

/* Override initializing hook from the C library. */
//void (*__malloc_initialize_hook) (void) = dbg_mem_init_hook;

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

