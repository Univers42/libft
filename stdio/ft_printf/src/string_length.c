#include "string_length.h"

size_t	hex_len(size_t n)
{
	size_t	len = 1;
	while (n >= 16)
	{
		n /= 16;
		len++;
	}
	return len;
}

size_t	uint_len(unsigned int n)
{
	size_t	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return len;
}
