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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/queue.h>
#include <cmdline_parse.h>

#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_memory.h>
#include <rte_random.h>
#include <rte_branch_prediction.h>
#include <rte_ip.h>
#include <time.h>

#ifdef RTE_LIBRTE_LPM

#include "rte_lpm.h"
#include "test_lpm_routes.h"

#include "test.h"

#define ITERATIONS (1 << 20)
#define BATCH_SIZE (1 << 13)

#define TEST_LPM_ASSERT(cond) do {                                            \
	if (!(cond)) {                                                        \
		printf("Error at line %d: \n", __LINE__);                     \
		return -1;                                                    \
	}                                                                     \
} while(0)



typedef int32_t (* rte_lpm_test)(void);

static int32_t test0(void);
static int32_t test1(void);
static int32_t test2(void);
static int32_t test3(void);
static int32_t test4(void);
static int32_t test5(void);
static int32_t test6(void);
static int32_t test7(void);
static int32_t test8(void);
static int32_t test9(void);
static int32_t test10(void);
static int32_t test11(void);
static int32_t test12(void);
static int32_t test13(void);
static int32_t test14(void);
static int32_t test15(void);
static int32_t test16(void);
static int32_t test17(void);
static int32_t test18(void);

rte_lpm_test tests[] = {
/* Test Cases */
	test0,
	test1,
	test2,
	test3,
	test4,
	test5,
	test6,
	test7,
	test8,
	test9,
	test10,
	test11,
	test12,
	test13,
	test14,
	test15,
	test16,
	test17,
	test18
};

#define NUM_LPM_TESTS (sizeof(tests)/sizeof(tests[0]))
#define MAX_DEPTH 32
#define MAX_RULES 256
#define PASS 0

/*
 * TEST 0
 *
 * Check that rte_lpm_create fails gracefully for incorrect user input
 * arguments
 */
int32_t
test0(void)
{
	struct rte_lpm *lpm = NULL;

	/* rte_lpm_create: lpm name == NULL */
	lpm = rte_lpm_create(NULL, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm == NULL);

	/* rte_lpm_create: max_rules = 0 */
	/* Note: __func__ inserts the function name, in this case "test0". */
	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, 0, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm == NULL);

	/* rte_lpm_create: mem_location is not RTE_LPM_HEAP or not MEMZONE */
	/* Note: __func__ inserts the function name, in this case "test0". */
	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, 2);
	TEST_LPM_ASSERT(lpm == NULL);

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, -1);
	TEST_LPM_ASSERT(lpm == NULL);

	/* socket_id < -1 is invalid */
	lpm = rte_lpm_create(__func__, -2, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm == NULL);

	return PASS;
}

/* TEST 1
 *
 * Create lpm table then delete lpm table 100 times
 * Use a slightly different rules size each time
 * */
int32_t
test1(void)
{
	struct rte_lpm *lpm = NULL;
	int32_t i;

	/* rte_lpm_free: Free NULL */
	for (i = 0; i < 100; i++) {
		lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES - i,
				RTE_LPM_HEAP);
		TEST_LPM_ASSERT(lpm != NULL);

		rte_lpm_free(lpm);
	}

	/* Can not test free so return success */
	return PASS;
}

/* TEST 2
 *
 * Call rte_lpm_free for NULL pointer user input. Note: free has no return and
 * therefore it is impossible to check for failure but this test is added to
 * increase function coverage metrics and to validate that freeing null does
 * not crash.
 */
int32_t
test2(void)
{
	struct rte_lpm *lpm = NULL;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	rte_lpm_free(lpm);
	rte_lpm_free(NULL);
	return PASS;
}

/* TEST 3
 *
 * Check that rte_lpm_add fails gracefully for incorrect user input arguments
 */
int32_t
test3(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip = IPv4(0, 0, 0, 0);
	uint8_t depth = 24, next_hop = 100;
	int32_t status = 0;

	/* rte_lpm_add: lpm == NULL */
	status = rte_lpm_add(NULL, ip, depth, next_hop);
	TEST_LPM_ASSERT(status < 0);

	/*Create vaild lpm to use in rest of test. */
	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	/* rte_lpm_add: depth < 1 */
	status = rte_lpm_add(lpm, ip, 0, next_hop);
	TEST_LPM_ASSERT(status < 0);

	/* rte_lpm_add: depth > MAX_DEPTH */
	status = rte_lpm_add(lpm, ip, (MAX_DEPTH + 1), next_hop);
	TEST_LPM_ASSERT(status < 0);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 4
 *
 * Check that rte_lpm_delete fails gracefully for incorrect user input
 * arguments
 */
int32_t
test4(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip = IPv4(0, 0, 0, 0);
	uint8_t depth = 24;
	int32_t status = 0;

	/* rte_lpm_delete: lpm == NULL */
	status = rte_lpm_delete(NULL, ip, depth);
	TEST_LPM_ASSERT(status < 0);

	/*Create vaild lpm to use in rest of test. */
	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	/* rte_lpm_delete: depth < 1 */
	status = rte_lpm_delete(lpm, ip, 0);
	TEST_LPM_ASSERT(status < 0);

	/* rte_lpm_delete: depth > MAX_DEPTH */
	status = rte_lpm_delete(lpm, ip, (MAX_DEPTH + 1));
	TEST_LPM_ASSERT(status < 0);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 5
 *
 * Check that rte_lpm_lookup fails gracefully for incorrect user input
 * arguments
 */
int32_t
test5(void)
{
#if defined(RTE_LIBRTE_LPM_DEBUG)
	struct rte_lpm *lpm = NULL;
	uint32_t ip = IPv4(0, 0, 0, 0);
	uint8_t next_hop_return = 0;
	int32_t status = 0;

	/* rte_lpm_lookup: lpm == NULL */
	status = rte_lpm_lookup(NULL, ip, &next_hop_return);
	TEST_LPM_ASSERT(status < 0);

	/*Create vaild lpm to use in rest of test. */
	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	/* rte_lpm_lookup: depth < 1 */
	status = rte_lpm_lookup(lpm, ip, NULL);
	TEST_LPM_ASSERT(status < 0);

	rte_lpm_free(lpm);
#endif
	return PASS;
}



/* TEST 6
 *
 * Call add, lookup and delete for a single rule with depth <= 24
 */
int32_t
test6(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip = IPv4(0, 0, 0, 0);
	uint8_t depth = 24, next_hop_add = 100, next_hop_return = 0;
	int32_t status = 0;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 7
 *
 * Call add, lookup and delete for a single rule with depth > 24
 */

int32_t
test7(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip = IPv4(0, 0, 0, 0);
	uint8_t depth = 32, next_hop_add = 100, next_hop_return = 0;
	int32_t status = 0;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 8
 *
 * Use rte_lpm_add to add rules which effect only the second half of the lpm
 * table. Use all possible depths ranging from 1..32. Set the next hop = to the
 * depth. Check lookup hit for on every add and check for lookup miss on the
 * first half of the lpm table after each add. Finally delete all rules going
 * backwards (i.e. from depth = 32 ..1) and carry out a lookup after each
 * delete. The lookup should return the next_hop_add value related to the
 * previous depth value (i.e. depth -1).
 */
int32_t
test8(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip1 = IPv4(127, 255, 255, 255), ip2 = IPv4(128, 0, 0, 0);
	uint8_t depth, next_hop_add, next_hop_return;
	int32_t status = 0;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	/* Loop with rte_lpm_add. */
	for (depth = 1; depth <= 32; depth++) {
		/* Let the next_hop_add value = depth. Just for change. */
		next_hop_add = depth;

		status = rte_lpm_add(lpm, ip2, depth, next_hop_add);
		TEST_LPM_ASSERT(status == 0);

		/* Check IP in first half of tbl24 which should be empty. */
		status = rte_lpm_lookup(lpm, ip1, &next_hop_return);
		TEST_LPM_ASSERT(status == -ENOENT);

		status = rte_lpm_lookup(lpm, ip2, &next_hop_return);
		TEST_LPM_ASSERT((status == 0) &&
			(next_hop_return == next_hop_add));
	}

	/* Loop with rte_lpm_delete. */
	for (depth = 32; depth >= 1; depth--) {
		next_hop_add = (uint8_t) (depth - 1);

		status = rte_lpm_delete(lpm, ip2, depth);
		TEST_LPM_ASSERT(status == 0);

		status = rte_lpm_lookup(lpm, ip2, &next_hop_return);

		if (depth != 1) {
			TEST_LPM_ASSERT((status == 0) &&
				(next_hop_return == next_hop_add));
		}
		else {
			TEST_LPM_ASSERT(status == -ENOENT);
		}

		status = rte_lpm_lookup(lpm, ip1, &next_hop_return);
		TEST_LPM_ASSERT(status == -ENOENT);
	}

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 9
 *
 * - Add & lookup to hit invalid TBL24 entry
 * - Add & lookup to hit valid TBL24 entry not extended
 * - Add & lookup to hit valid extended TBL24 entry with invalid TBL8 entry
 * - Add & lookup to hit valid extended TBL24 entry with valid TBL8 entry
 *
 */
int32_t
test9(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip, ip_1, ip_2;
	uint8_t depth, depth_1, depth_2, next_hop_add, next_hop_add_1,
		next_hop_add_2, next_hop_return;
	int32_t status = 0;

	/* Add & lookup to hit invalid TBL24 entry */
	ip = IPv4(128, 0, 0, 0);
	depth = 24;
	next_hop_add = 100;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	/* Add & lookup to hit valid TBL24 entry not extended */
	ip = IPv4(128, 0, 0, 0);
	depth = 23;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	depth = 24;
	next_hop_add = 101;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	depth = 24;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	depth = 23;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	/* Add & lookup to hit valid extended TBL24 entry with invalid TBL8
	 * entry */
	ip = IPv4(128, 0, 0, 0);
	depth = 32;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	ip = IPv4(128, 0, 0, 5);
	depth = 32;
	next_hop_add = 101;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	ip = IPv4(128, 0, 0, 0);
	depth = 32;
	next_hop_add = 100;

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	/* Add & lookup to hit valid extended TBL24 entry with valid TBL8
	 * entry */
	ip_1 = IPv4(128, 0, 0, 0);
	depth_1 = 25;
	next_hop_add_1 = 101;

	ip_2 = IPv4(128, 0, 0, 5);
	depth_2 = 32;
	next_hop_add_2 = 102;

	next_hop_return = 0;

	status = rte_lpm_add(lpm, ip_1, depth_1, next_hop_add_1);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip_1, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add_1));

	status = rte_lpm_add(lpm, ip_2, depth_2, next_hop_add_2);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip_2, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add_2));

	status = rte_lpm_delete(lpm, ip_2, depth_2);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip_2, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add_1));

	status = rte_lpm_delete(lpm, ip_1, depth_1);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip_1, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_free(lpm);

	return PASS;
}


/* TEST 10
 *
 * - Add rule that covers a TBL24 range previously invalid & lookup (& delete &
 *   lookup)
 * - Add rule that extends a TBL24 invalid entry & lookup (& delete & lookup)
 * - Add rule that extends a TBL24 valid entry & lookup for both rules (&
 *   delete & lookup)
 * - Add rule that updates the next hop in TBL24 & lookup (& delete & lookup)
 * - Add rule that updates the next hop in TBL8 & lookup (& delete & lookup)
 * - Delete a rule that is not present in the TBL24 & lookup
 * - Delete a rule that is not present in the TBL8 & lookup
 *
 */
int32_t
test10(void)
{

	struct rte_lpm *lpm = NULL;
	uint32_t ip;
	uint8_t depth, next_hop_add, next_hop_return;
	int32_t status = 0;

	/* Add rule that covers a TBL24 range previously invalid & lookup
	 * (& delete & lookup) */
	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	ip = IPv4(128, 0, 0, 0);
	depth = 16;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	ip = IPv4(128, 0, 0, 0);
	depth = 25;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	rte_lpm_delete_all(lpm);

	/* Add rule that extends a TBL24 valid entry & lookup for both rules
	 * (& delete & lookup) */

	ip = IPv4(128, 0, 0, 0);
	depth = 24;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	ip = IPv4(128, 0, 0, 10);
	depth = 32;
	next_hop_add = 101;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	ip = IPv4(128, 0, 0, 0);
	next_hop_add = 100;

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	ip = IPv4(128, 0, 0, 0);
	depth = 24;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	ip = IPv4(128, 0, 0, 10);
	depth = 32;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	/* Add rule that updates the next hop in TBL24 & lookup
	 * (& delete & lookup) */

	ip = IPv4(128, 0, 0, 0);
	depth = 24;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	next_hop_add = 101;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	/* Add rule that updates the next hop in TBL8 & lookup
	 * (& delete & lookup) */

	ip = IPv4(128, 0, 0, 0);
	depth = 32;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	next_hop_add = 101;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	/* Delete a rule that is not present in the TBL24 & lookup */

	ip = IPv4(128, 0, 0, 0);
	depth = 24;
	next_hop_add = 100;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status < 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_delete_all(lpm);

	/* Delete a rule that is not present in the TBL8 & lookup */

	ip = IPv4(128, 0, 0, 0);
	depth = 32;
	next_hop_add = 100;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status < 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 11
 *
 * Add two rules, lookup to hit the more specific one, lookup to hit the less
 * specific one delete the less specific rule and lookup previous values again;
 * add a more specific rule than the existing rule, lookup again
 *
 * */
int32_t
test11(void)
{

	struct rte_lpm *lpm = NULL;
	uint32_t ip;
	uint8_t depth, next_hop_add, next_hop_return;
	int32_t status = 0;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	ip = IPv4(128, 0, 0, 0);
	depth = 24;
	next_hop_add = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	ip = IPv4(128, 0, 0, 10);
	depth = 32;
	next_hop_add = 101;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	ip = IPv4(128, 0, 0, 0);
	next_hop_add = 100;

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add));

	ip = IPv4(128, 0, 0, 0);
	depth = 24;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	ip = IPv4(128, 0, 0, 10);
	depth = 32;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 12
 *
 * Add an extended rule (i.e. depth greater than 24, lookup (hit), delete,
 * lookup (miss) in a for loop of 1000 times. This will check tbl8 extension
 * and contraction.
 *
 * */

int32_t
test12(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip, i;
	uint8_t depth, next_hop_add, next_hop_return;
	int32_t status = 0;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	ip = IPv4(128, 0, 0, 0);
	depth = 32;
	next_hop_add = 100;

	for (i = 0; i < 1000; i++) {
		status = rte_lpm_add(lpm, ip, depth, next_hop_add);
		TEST_LPM_ASSERT(status == 0);

		status = rte_lpm_lookup(lpm, ip, &next_hop_return);
		TEST_LPM_ASSERT((status == 0) &&
				(next_hop_return == next_hop_add));

		status = rte_lpm_delete(lpm, ip, depth);
		TEST_LPM_ASSERT(status == 0);

		status = rte_lpm_lookup(lpm, ip, &next_hop_return);
		TEST_LPM_ASSERT(status == -ENOENT);
	}

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 13
 *
 * Add a rule to tbl24, lookup (hit), then add a rule that will extend this
 * tbl24 entry, lookup (hit). delete the rule that caused the tbl24 extension,
 * lookup (miss) and repeat for loop of 1000 times. This will check tbl8
 * extension and contraction.
 *
 * */

int32_t
test13(void)
{
	struct rte_lpm *lpm = NULL;
	uint32_t ip, i;
	uint8_t depth, next_hop_add_1, next_hop_add_2, next_hop_return;
	int32_t status = 0;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	ip = IPv4(128, 0, 0, 0);
	depth = 24;
	next_hop_add_1 = 100;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add_1);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT((status == 0) && (next_hop_return == next_hop_add_1));

	depth = 32;
	next_hop_add_2 = 101;

	for (i = 0; i < 1000; i++) {
		status = rte_lpm_add(lpm, ip, depth, next_hop_add_2);
		TEST_LPM_ASSERT(status == 0);

		status = rte_lpm_lookup(lpm, ip, &next_hop_return);
		TEST_LPM_ASSERT((status == 0) &&
				(next_hop_return == next_hop_add_2));

		status = rte_lpm_delete(lpm, ip, depth);
		TEST_LPM_ASSERT(status == 0);

		status = rte_lpm_lookup(lpm, ip, &next_hop_return);
		TEST_LPM_ASSERT((status == 0) &&
				(next_hop_return == next_hop_add_1));
	}

	depth = 24;

	status = rte_lpm_delete(lpm, ip, depth);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip, &next_hop_return);
	TEST_LPM_ASSERT(status == -ENOENT);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST 14
 *
 * Fore TBL8 extension exhaustion. Add 256 rules that require a tbl8 extension.
 * No more tbl8 extensions will be allowed. Now add one more rule that required
 * a tbl8 extension and get fail.
 * */
int32_t
test14(void)
{

	/* We only use depth = 32 in the loop below so we must make sure
	 * that we have enough storage for all rules at that depth*/

	struct rte_lpm *lpm = NULL;
	uint32_t ip;
	uint8_t depth, next_hop_add, next_hop_return;
	int32_t status = 0;

	/* Add enough space for 256 rules for every depth */
	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, 256 * 32, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	ip = IPv4(0, 0, 0, 0);
	depth = 32;
	next_hop_add = 100;

	/* Add 256 rules that require a tbl8 extension */
	for (ip = 0; ip <= IPv4(0, 0, 255, 0); ip += 256) {
		status = rte_lpm_add(lpm, ip, depth, next_hop_add);
		TEST_LPM_ASSERT(status == 0);

		status = rte_lpm_lookup(lpm, ip, &next_hop_return);
		TEST_LPM_ASSERT((status == 0) &&
				(next_hop_return == next_hop_add));
	}

	/* All tbl8 extensions have been used above. Try to add one more and
	 * we get a fail */
	ip = IPv4(1, 0, 0, 0);
	depth = 32;

	status = rte_lpm_add(lpm, ip, depth, next_hop_add);
	TEST_LPM_ASSERT(status < 0);

	rte_lpm_free(lpm);

	return PASS;
}

/* TEST test15
 *
 * Lookup performance test using Mae West Routing Table
 */
static inline uint32_t
depth_to_mask(uint8_t depth) {
	return (int)0x80000000 >> (depth - 1);
}

static uint32_t
rule_table_check_for_duplicates(const struct route_rule *table, uint32_t n){
	unsigned i, j, count;

	count = 0;
	for (i = 0; i < (n - 1); i++) {
		uint8_t depth1 = table[i].depth;
		uint32_t ip1_masked = table[i].ip & depth_to_mask(depth1);

		for (j = (i + 1); j <n; j ++) {
			uint8_t depth2 = table[j].depth;
			uint32_t ip2_masked = table[j].ip &
					depth_to_mask(depth2);

			if ((depth1 == depth2) && (ip1_masked == ip2_masked)){
				printf("Rule %u is a duplicate of rule %u\n",
						j, i);
				count ++;
			}
		}
	}

	return count;
}

static int32_t
rule_table_characterisation(const struct route_rule *table, uint32_t n){
	unsigned i, j;

	printf("DEPTH		QUANTITY (PERCENT)\n");
	printf("--------------------------------- \n");
	/* Count depths. */
	for(i = 1; i <= 32; i++) {
		unsigned depth_counter = 0;
		double percent_hits;

		for (j = 0; j < n; j++) {
			if (table[j].depth == (uint8_t) i)
				depth_counter++;
		}

		percent_hits = ((double)depth_counter)/((double)n) * 100;

		printf("%u	-	%5u (%.2f)\n",
				i, depth_counter, percent_hits);
	}

	return 0;
}

static inline uint64_t
div64(uint64_t dividend, uint64_t divisor)
{
	return ((2 * dividend) + divisor) / (2 * divisor);
}

int32_t
test15(void)
{
	struct rte_lpm *lpm = NULL;
	uint64_t begin, end, total_time, lpm_used_entries = 0;
	unsigned avg_ticks, i, j;
	uint8_t next_hop_add = 0, next_hop_return = 0;
	int32_t status = 0;

	printf("Using Mae West routing table from www.oiforum.com\n");
	printf("No. routes = %u\n", (unsigned) NUM_ROUTE_ENTRIES);
	printf("No. duplicate routes = %u\n\n", (unsigned)
		rule_table_check_for_duplicates(mae_west_tbl, NUM_ROUTE_ENTRIES));
	printf("Route distribution per prefix width: \n");
	rule_table_characterisation(mae_west_tbl,
			(uint32_t) NUM_ROUTE_ENTRIES);
	printf("\n");

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, 1000000,
			RTE_LPM_MEMZONE);
	TEST_LPM_ASSERT(lpm != NULL);

	next_hop_add = 1;

	/* Add */
	/* Begin Timer. */
	begin = rte_rdtsc();

	for (i = 0; i < NUM_ROUTE_ENTRIES; i++) {
		/* rte_lpm_add(lpm, ip, depth, next_hop_add) */
		status += rte_lpm_add(lpm, mae_west_tbl[i].ip,
				mae_west_tbl[i].depth, next_hop_add);
	}
	/* End Timer. */
	end = rte_rdtsc();

	TEST_LPM_ASSERT(status == 0);

	/* Calculate average cycles per add. */
	avg_ticks = (uint32_t) div64((end - begin),
			(uint64_t) NUM_ROUTE_ENTRIES);

	uint64_t cache_line_counter = 0;
	uint64_t count = 0;

	/* Obtain add statistics. */
	for (i = 0; i < RTE_LPM_TBL24_NUM_ENTRIES; i++) {
		if (lpm->tbl24[i].valid)
			lpm_used_entries++;

		if (i % 32 == 0){
			if (count < lpm_used_entries) {
				cache_line_counter++;
				count = lpm_used_entries;
			}
		}
	}

	printf("Number of table 24 entries = 	%u\n",
			(unsigned) RTE_LPM_TBL24_NUM_ENTRIES);
	printf("Used table 24 entries = 	%u\n",
			(unsigned) lpm_used_entries);
	printf("Percentage of table 24 entries used = %u\n",
			(unsigned) div64((lpm_used_entries * 100) ,
					RTE_LPM_TBL24_NUM_ENTRIES));
	printf("64 byte Cache entries used = %u \n",
			(unsigned) cache_line_counter);
	printf("Cache Required = %u bytes\n\n",
			(unsigned) cache_line_counter * 64);

	printf("Average LPM Add:	%u cycles\n", avg_ticks);

	/* Lookup */

	/* Choose random seed. */
	rte_srand(0);
	total_time = 0;
	status = 0;
	for (i = 0; i < (ITERATIONS / BATCH_SIZE); i ++) {
		static uint32_t ip_batch[BATCH_SIZE];
		uint64_t begin_batch, end_batch;

		/* Generate a batch of random numbers */
		for (j = 0; j < BATCH_SIZE; j ++) {
			ip_batch[j] = rte_rand();
		}

		/* Lookup per batch */
		begin_batch = rte_rdtsc();

		for (j = 0; j < BATCH_SIZE; j ++) {
			status += rte_lpm_lookup(lpm, ip_batch[j],
					&next_hop_return);
		}

		end_batch = rte_rdtsc();
		printf("status = %d\r", next_hop_return);
		TEST_LPM_ASSERT(status < 1);

		/* Accumulate batch time */
		total_time += (end_batch - begin_batch);

		TEST_LPM_ASSERT((status < -ENOENT) ||
					(next_hop_return == next_hop_add));
	}

	avg_ticks = (uint32_t) div64(total_time, ITERATIONS);
	printf("Average LPM Lookup: 	%u cycles\n", avg_ticks);

	/* Delete */
	status = 0;
	begin = rte_rdtsc();

	for (i = 0; i < NUM_ROUTE_ENTRIES; i++) {
		/* rte_lpm_delete(lpm, ip, depth) */
		status += rte_lpm_delete(lpm, mae_west_tbl[i].ip,
				mae_west_tbl[i].depth);
	}

	end = rte_rdtsc();

	TEST_LPM_ASSERT(status == 0);

	avg_ticks = (uint32_t) div64((end - begin), NUM_ROUTE_ENTRIES);

	printf("Average LPM Delete: 	%u cycles\n", avg_ticks);

	rte_lpm_delete_all(lpm);
	rte_lpm_free(lpm);

	return PASS;
}



/*
 * Sequence of operations for find existing fbk hash table
 *
 *  - create table
 *  - find existing table: hit
 *  - find non-existing table: miss
 *
 */
int32_t test16(void)
{
	struct rte_lpm *lpm = NULL, *result = NULL;

	/* Create lpm  */
	lpm = rte_lpm_create("lpm_find_existing", SOCKET_ID_ANY, 256 * 32, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	/* Try to find existing lpm */
	result = rte_lpm_find_existing("lpm_find_existing");
	TEST_LPM_ASSERT(result == lpm);

	/* Try to find non-existing lpm */
	result = rte_lpm_find_existing("lpm_find_non_existing");
	TEST_LPM_ASSERT(result == NULL);

	/* Cleanup. */
	rte_lpm_delete_all(lpm);
	rte_lpm_free(lpm);

	return PASS;
}

/*
 * test failure condition of overloading the tbl8 so no more will fit
 * Check we get an error return value in that case
 */
static int32_t
test17(void)
{
	uint32_t ip;
	struct rte_lpm *lpm = rte_lpm_create(__func__, SOCKET_ID_ANY,
			256 * 32, RTE_LPM_HEAP);

	printf("Testing filling tbl8's\n");

	/* ip loops through all positibilities for top 24 bits of address */
	for (ip = 0; ip < 0xFFFFFF; ip++){
		/* add an entrey within a different tbl8 each time, since
		 * depth >24 and the top 24 bits are different */
		if (rte_lpm_add(lpm, (ip << 8) + 0xF0, 30, 0) < 0)
			break;
	}

	if (ip != RTE_LPM_TBL8_NUM_GROUPS) {
		printf("Error, unexpected failure with filling tbl8 groups\n");
		printf("Failed after %u additions, expected after %u\n",
				(unsigned)ip, (unsigned)RTE_LPM_TBL8_NUM_GROUPS);
	}

	rte_lpm_free(lpm);
	return 0;
}

/*
 * Test 18
 * Test for overwriting of tbl8:
 *  - add rule /32 and lookup
 *  - add new rule /24 and lookup
 *	- add third rule /25 and lookup
 *	- lookup /32 and /24 rule to ensure the table has not been overwritten.
 */
int32_t
test18(void)
{
	struct rte_lpm *lpm = NULL;
	const uint32_t ip_10_32 = IPv4(10, 10, 10, 2);
	const uint32_t ip_10_24 = IPv4(10, 10, 10, 0);
	const uint32_t ip_20_25 = IPv4(10, 10, 20, 2);
	const uint8_t d_ip_10_32 = 32,
			d_ip_10_24 = 24,
			d_ip_20_25 = 25;
	const uint8_t next_hop_ip_10_32 = 100,
			next_hop_ip_10_24 = 105,
			next_hop_ip_20_25 = 111;
	uint8_t next_hop_return = 0;
	int32_t status = 0;

	lpm = rte_lpm_create(__func__, SOCKET_ID_ANY, MAX_RULES, RTE_LPM_HEAP);
	TEST_LPM_ASSERT(lpm != NULL);

	status = rte_lpm_add(lpm, ip_10_32, d_ip_10_32, next_hop_ip_10_32);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip_10_32, &next_hop_return);
	TEST_LPM_ASSERT(status == 0);
	TEST_LPM_ASSERT(next_hop_return == next_hop_ip_10_32);

	status = rte_lpm_add(lpm, ip_10_24, d_ip_10_24,	next_hop_ip_10_24);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip_10_24, &next_hop_return);
	TEST_LPM_ASSERT(status == 0);
	TEST_LPM_ASSERT(next_hop_return == next_hop_ip_10_24);

	status = rte_lpm_add(lpm, ip_20_25, d_ip_20_25, next_hop_ip_20_25);
	TEST_LPM_ASSERT(status == 0);

	status = rte_lpm_lookup(lpm, ip_20_25, &next_hop_return);
	TEST_LPM_ASSERT(status == 0);
	TEST_LPM_ASSERT(next_hop_return == next_hop_ip_20_25);

	status = rte_lpm_lookup(lpm, ip_10_32, &next_hop_return);
	TEST_LPM_ASSERT(status == 0);
	TEST_LPM_ASSERT(next_hop_return == next_hop_ip_10_32);

	status = rte_lpm_lookup(lpm, ip_10_24, &next_hop_return);
	TEST_LPM_ASSERT(status == 0);
	TEST_LPM_ASSERT(next_hop_return == next_hop_ip_10_24);

	rte_lpm_free(lpm);

	printf("%s PASSED\n", __func__);
	return PASS;
}


/*
 * Do all unit and performance tests.
 */

int
test_lpm(void)
{
	unsigned test_num;
	int status, global_status;

	printf("Running LPM tests...\n"
	       "Total number of test = %u\n", (unsigned) NUM_LPM_TESTS);

	global_status = 0;

	for (test_num = 0; test_num < NUM_LPM_TESTS; test_num++) {

		status = tests[test_num]();

		printf("LPM Test %u: %s\n", test_num,
				(status < 0) ? "FAIL" : "PASS");

		if (status < 0) {
			global_status = status;
		}
	}

	return global_status;
}

#else

int
test_lpm(void)
{
	printf("The LPM library is not included in this build\n");
	return 0;
}

#endif