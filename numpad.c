/*
 * numpad.c
 *
 *  Created on: Jul 10, 2017
 *      Author: Jeremy P.
 */


#include "numpad.h"
#include <stdbool.h>

struct numpad numpad_create(
	struct button *k_0,
	struct button *k_1,
	struct button *k_2,
	struct button *k_3,
	struct button *k_4,
	struct button *k_5,
	struct button *k_6,
	struct button *k_7,
	struct button *k_8,
	struct button *k_9,
	struct button *k_dec,
	struct button *k_clr
)  {
	struct numpad np;
	np.k_0 = k_0;
	np.k_1 = k_1;
	np.k_2 = k_2;
	np.k_3 = k_3;
	np.k_4 = k_4;
	np.k_5 = k_5;
	np.k_6 = k_6;
	np.k_7 = k_7;
	np.k_8 = k_8;
	np.k_9 = k_9;
	np.k_dec = k_dec;
	np.k_clr = k_clr;
	return np;
}

bool numpad_poll_num(struct numpad np, int x, int y, int *num) {
	if (CHECK_BUTTON_BOUNDS(np.k_0, x, y)) {
		*num = 0;
	} else if (CHECK_BUTTON_BOUNDS(np.k_1, x, y)) {
		*num = 1;
	} else if (CHECK_BUTTON_BOUNDS(np.k_2, x, y)) {
		*num = 2;
	} else if (CHECK_BUTTON_BOUNDS(np.k_3, x, y)) {
		*num = 3;
	} else if (CHECK_BUTTON_BOUNDS(np.k_4, x, y)) {
		*num = 4;
	} else if (CHECK_BUTTON_BOUNDS(np.k_5, x, y)) {
		*num = 5;
	} else if (CHECK_BUTTON_BOUNDS(np.k_6, x, y)) {
		*num = 6;
	} else if (CHECK_BUTTON_BOUNDS(np.k_7, x, y)) {
		*num = 7;
	} else if (CHECK_BUTTON_BOUNDS(np.k_8, x, y)) {
		*num = 8;
	} else if (CHECK_BUTTON_BOUNDS(np.k_9, x, y)) {
		*num = 9;
	} else {
		return false;
	}

	return true;
}

bool numpad_poll_dec(struct numpad np, int x, int y) {
	return CHECK_BUTTON_BOUNDS(np.k_dec, x, y);
}

bool numpad_poll_clr(struct numpad np, int x, int y) {
	return CHECK_BUTTON_BOUNDS(np.k_clr, x, y);
}
