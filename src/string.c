
#include <string.h>

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

//void *memset(void *dest, char val, size_t count)
void *memset(void *dest, uint8_t val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

//unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
uint16_t *memsetw(uint16_t *dest, uint16_t val, size_t count)
{
    uint16_t *temp = (uint16_t *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

void itoa(char *buf, int base, int d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	/* If %d is specified and D is minus, put `-' in the head. */
	if (base == 'd' && d < 0)
	{
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x' ||
			 base == 'p')
	{
		divisor = 16;
	}

	/* Divide ud by DIVISOR until ud == 0. */
	do {
		int remainder = ud % divisor;
		*p++ = (remainder < 10) ? \
				remainder + '0' : \
				remainder + 'a' - 10;
	} while (ud /= divisor);

	/* Terminate BUF. */
	*p = 0;

	/* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}
