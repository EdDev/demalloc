#   BSD LICENSE
#
#   Copyright(c) 2015 Edward Haas (edwardh.dev@gmail.com). All rights reserved.
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#     * Neither the name of Intel Corporation nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

APPTEST_NAME = example
APPTEST_SRC = src/app

LIBMEM_NAME = libdemalloc.so
LIBMEM_SRC = src/lib

WARNING_FLAGS = -Wall -Wextra

all: libmem apptest

apptest: $(APPTEST_SRC)/example.cpp
	g++ $(WARNING_FLAGS) -I$(LIBMEM_SRC) -L. $^ -ldemalloc -o $(APPTEST_NAME)

libmem: $(LIBMEM_SRC)/demalloc.c
	gcc $(WARNING_FLAGS) -fPIC -c $^
	gcc -shared -Wl,-soname,$(LIBMEM_NAME).1 -o $(LIBMEM_NAME).1.0 *.o
	ln -sf $(LIBMEM_NAME).1.0 $(LIBMEM_NAME)
	
install:
	cp $(LIBMEM_NAME).1.0 /usr/lib/
	ln -sf /usr/lib/$(LIBMEM_NAME).1.0 /usr/lib/$(LIBMEM_NAME).1
	ln -sf /usr/lib/$(LIBMEM_NAME).1.0 /usr/lib/$(LIBMEM_NAME)
    
clean:
	rm $(APPTEST_NAME)
	rm *.o
	rm *.so*
	
uninstall:
	rm /usr/lib/$(LIBMEM_NAME).1.0 
	rm /usr/lib/$(LIBMEM_NAME).1
	rm /usr/lib/$(LIBMEM_NAME)
	