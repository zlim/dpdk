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

#ifndef _EAL_PRIVATE_H_
#define _EAL_PRIVATE_H_

/**
 * Initialize the memzone subsystem (private to eal).
 *
 * @return
 *   - 0 on success
 *   - Negative on error
 */
int rte_eal_memzone_init(void);

/**
 * Common log initialization function (private to eal).
 *
 * Called by environment-specific log initialization function to initialize
 * log history.
 *
 * @param default_log
 *   The default log stream to be used.
 * @return
 *   - 0 on success
 *   - Negative on error
 */
int rte_eal_common_log_init(FILE *default_log);

/**
 * Fill configuration with number of physical and logical processors
 *
 * This function is private to EAL.
 *
 * Parse /proc/cpuinfo to get the number of physical and logical
 * processors on the machine.
 *
 * @return
 *   0 on success, negative on error
 */
int rte_eal_cpu_init(void);

/**
 * Map memory
 *
 * This function is private to EAL.
 *
 * Fill configuration structure with these infos, and return 0 on success.
 *
 * @return
 *   0 on success, negative on error
 */
int rte_eal_memory_init(void);

/**
 * Configure HPET
 *
 * This function is private to EAL.
 *
 * Mmap memory areas used by HPET (high precision event timer) that will
 * provide our time reference.
 *
 * @return
 *   0 on success, negative on error
 */
int rte_eal_hpet_init(void);

/**
 * Init early logs
 *
 * This function is private to EAL.
 *
 * @return
 *   0 on success, negative on error
 */
int rte_eal_log_early_init(void);

/**
 * Init the default log stream
 *
 * This function is private to EAL.
 *
 * @return
 *   0 on success, negative on error
 */
int rte_eal_log_init(void);

/**
 * Init the default log stream
 *
 * This function is private to EAL.
 *
 * @return
 *   0 on success, negative on error
 */
int rte_eal_pci_init(void);

struct rte_pci_driver;
struct rte_pci_device;

/**
 * Mmap memory for single PCI device
 *
 * This function is private to EAL.
 *
 * @return
 *   0 on success, negative on error
 */
int rte_eal_pci_probe_one_driver(struct rte_pci_driver *dr,
		struct rte_pci_device *dev);

/**
 * Init tail queues for non-EAL library structures. This is to allow
 * the rings, mempools, etc. lists to be shared among multiple processes
 *
 * This function is private to EAL
 *
 * @return
 *    0 on success, negative on error
 */
int rte_eal_tailqs_init(void);

/**
 * Init interrupt handling.
 *
 * This function is private to EAL.
 *
 * @return
 *  0 on success, negative on error
 */
int rte_eal_intr_init(void);

/**
 * Init alarm mechanism. This is to allow a callback be called after
 * specific time.
 *
 * This function is private to EAL.
 *
 * @return
 *  0 on success, negative on error
 */
int rte_eal_alarm_init(void);

#endif /* _EAL_PRIVATE_H_ */
