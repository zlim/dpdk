/*-
 *   BSD LICENSE
 * 
 *   Copyright(c) 2010-2012 Intel Corporation. All rights reserved.
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
 * 
 *  version: DPDK.L.1.2.3-3
 */

#ifndef _TEST_H_
#define _TEST_H_

/* icc on baremetal gives us troubles with function named 'main' */
#ifdef RTE_EXEC_ENV_BAREMETAL
#define main _main
#endif

#define RECURSIVE_ENV_VAR "RTE_TEST_RECURSIVE"

extern const char *prgname;

extern cmdline_parse_ctx_t main_ctx[];

void test_hexdump(const char *title, const void *buf, unsigned int len);

int main(int argc, char **argv);

int test_pci(void);
int test_memory(void);
int test_per_lcore(void);
int test_spinlock(void);
int test_rwlock(void);
int test_atomic(void);
int test_byteorder(void);
int test_prefetch(void);
int test_cycles(void);
int test_logs(void);
int test_memzone(void);
int test_ring(void);
int test_mempool(void);
int test_mbuf(void);
int test_timer(void);
int test_malloc(void);
int test_memcpy(void);
int test_hash(void);
int test_lpm(void);
int test_debug(void);
int test_errno(void);
int test_tailq(void);
int test_string_fns(void);
int test_mp_secondary(void);
int test_cpuflags(void);
int test_eal_flags(void);
int test_alarm(void);
int test_interrupt(void);
int test_version(void);
int test_pci_run;

#endif
