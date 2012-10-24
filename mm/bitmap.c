/*
 * mm/bitmap.c
 * Copyright(C) 2003 Alexander Blessing
 */

#include <memory.h>

inline int bitmap_is_bit_set(unsigned long *bitmap, unsigned long bit)
{
	return ((bitmap[BITMAP_OFFSET(bit)] & BITMAP_MASK(bit)) == BITMAP_MASK(bit));
}

inline int bitmap_is_bit_clear(unsigned long *bitmap, unsigned long bit)
{
	return !(bitmap_is_bit_set(bitmap, bit));
}

inline void bitmap_set_bit(unsigned long *bitmap, unsigned long bit)
{
	bitmap[BITMAP_OFFSET(bit)] |= BITMAP_MASK(bit);
}

inline void bitmap_clear_bit(unsigned long *bitmap, unsigned long bit)
{
	bitmap[BITMAP_OFFSET(bit)] &= ~BITMAP_MASK(bit);
}

inline int bitmap_test_and_set_bit(unsigned long *bitmap, unsigned long bit)
{
	int result;

	result = bitmap_is_bit_set(bitmap, bit);
	bitmap_set_bit(bitmap, bit);
	return result;
}

inline int bitmap_test_and_clear_bit(unsigned long *bitmap, unsigned long bit)
{
	int result;

	result = bitmap_is_bit_set(bitmap, bit);
	bitmap_clear_bit(bitmap, bit);

	return result;
}

/* bitmap_find_first_zero_bit()
 * looks for the first zero bit in a bitmap
 *
 * returns num_bits if no zero bits were found
 */
inline unsigned long bitmap_find_first_zero_bit(unsigned long *bitmap,
						unsigned long num_bits)
{
	unsigned long first = 0;
	unsigned long *tmp = bitmap;

	while ((0xFFFFFFFF == *tmp) && (first < num_bits)) {
		tmp++;
		first += 32;
	}
	while (!bitmap_is_bit_clear(bitmap, first) && (first < num_bits)) 
		first++;

	return first;
}

/* bitmap_search()
 * @bitmap: bitmap to search
 * @start: start bit
 * @num_bits: number of bits to search
 * @type: type of search
 *
 * returns start + num_bits if operation fails
 */
inline unsigned long bitmap_search(unsigned long *bitmap,
				   unsigned long start,
				   unsigned long num_bits,
				   unsigned long type)
{
	unsigned long ret;

	switch(type)
	{
		/* only FIRST_ZERO_BIT is needed now
		 * more operations may come later, but not necessary for the moment
		 */
		case FIRST_ZERO_BIT:
			ret = bitmap_find_first_zero_bit(bitmap + (start / BITS_PER_ELEMENT),
							 num_bits);
			break;
		default:
			break;
	}
	return start + ret;
}
