#include <rtthread.h>

const rt_uint8_t __lowest_bit_bitmap[] = 
{
	/* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
};

/**
 * This function find the first bit which set 1,
 * and return corresponding index
 *
 * @return the index that be set 1 first, if equal 0, return 0
 */
int __rt_ffs(int value)
{
	if(value == 0) return 0;
	
	if(value & 0xff)
		return __lowest_bit_bitmap[value & 0xff] + 1;
	
	if(value & 0xff00)
		return __lowest_bit_bitmap[(value & 0xff00) >> 8] + 9;
	
	if(value & 0xff0000)
		return __lowest_bit_bitmap[(value & 0xff0000) >> 16] + 17;
	
	return __lowest_bit_bitmap[(value & 0xff000000) >> 24] + 25;
}

/**
 * This function will copy string no more than n bytes.
 *
 * @param dst the string to copy
 * @param src the string to be copied
 * @param n the maximum copied length
 *
 * @return the result
 */
char *rt_strncpy(char *dst, const char *src, rt_ubase_t n)
{
	if(n != 0)
	{
		char *d = dst;
		const char *s = src;
		
		do
		{
			if((*d++ = *s++) == 0)
			{
				/* NUL pad the remaining n-1 bytes */
				while(--n != 0)
					*d++ = 0;
				break;
			}
		}
		while(--n != 0);
	}
	
	return(dst);
}
