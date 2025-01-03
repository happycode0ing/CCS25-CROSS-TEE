/*
 * Copyright (c) 2012-2017 Roberto E. Vargas Caballero
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Portions copyright (c) 2018-2019, Arm Limited and Contributors.
 * All rights reserved.
 */

#ifndef TIME_H
#define TIME_H

#include <stddef.h>
#include <arch_helpers.h>

#define _TBIAS	((70 * 365LU + 17) * 86400)

typedef unsigned long time_t;

time_t (time)(time_t *tod);
unsigned long (difftime)(time_t t1, time_t t0);

time_t (time)(time_t *tod)
{	/* return calendar time */
	// time_t t = _Time(NULL) + (70*365LU+17)*86400;
	time_t t = read_cntpct_el0() + (70*365LU+17)*86400;

	if (tod)
	{
	
		*tod = t;
	}
	return (t);
}

/* return double type in original time.c */
unsigned long (difftime)(time_t t1, time_t t0)
{	/* compute difference in times */
	t0 -= _TBIAS, t1 -= _TBIAS;
	return (t0 <= t1 ? (unsigned long)(t1 - t0) : -(unsigned long)(t0 - t1));
}

#endif /* TIME_H */
