#ifndef __UTIL_H__
#define __UTIL_H__

/*
 * Method 1 (Using Arithmetic Operators):
 *		(a) The idea is to get sum in one of the two given numbers.
 *			The numbers can then be swapped using the sum and subtraction from sum.
 *		(b) Multiplication and division can also be used for swapping. 
 *
 * Method 2 (Using Bitwise XOR):
 *		The bitwise XOR operator can be used to swap two variables.
 *		The XOR of two numbers a and b returns a number which has all the bits
 *		as 1 wherever bits of a and b differ.  For example -
 *				XOR of 10 (1010) and 5 (0101) is (1111) and
 *				XOR of  7 (0111) and 5 (0101) is (0010).
 *
 */

/*
 *	Problems with above methods
 * =============================
 *		(1) The multiplication and division based approach doesn't work
 *			if one of the numbers is 0 as the product becomes 0 irrespective
 *			of the other number.
 *		(2) Both Arithmetic solutions may cause arithmetic overflow.
 *			If x and y are too large, addition and multiplication
 *			may go out of integer range.
 *		(3) When we use pointers to variable and make a function swap,
 *			all of the above methods fail when both pointers point to
 *			the same variable. Let's take a look what will happen
 *			in this case if both are pointing to the same variable.
 *
 * 			Bitwise XOR based method
 *			x = x ^ x; // x becomes 0
 *			x = x ^ x; // x remains 0
 *			x = x ^ x; // x remains 0
 *
 *			Arithmetic based method
 *			x = x + x; // x becomes 2x
 *			x = x . x; // x becomes 0
 *			x = x . x; // x remains 0
 *
 */

#define swap_arithmatic_1(__a, __b)  \
do {                    \
__a = __a + __b;		\
__b = __a - __b;		\
__a = __a - __b;		\
} while(0)

#define swap_arithmatic_2(__a, __b)  \
do {                    \
__a = __a * __b;		\
__b = __a / __b;		\
__a = __a / __b;		\
} while(0)

#define swap_xor(__a, __b)  \
do {                    \
    __a = __a ^ __b;    \
    __b = __a ^ __b;    \
    __a = __a ^ __a;    \
} while(0)

#ifndef swap
#define swap swap_xor
#endif /* swap */

#endif /* __UTIL_H__ */

