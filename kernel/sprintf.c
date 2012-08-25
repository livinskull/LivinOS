#include <util.h>


uint32_t sprintf(char *buff, const char *format,...) {
	static uint8_t hexmap[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	static uint8_t hexmap_low[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};
	uint32_t *args = ((uint32_t*) &format) + 1;
	char *startbuff, *t, d[11], xx[9];
	uint8_t aktarg=0;
	uint32_t u;
	int n, i;

	if (buff == 0)
		return -1;

	startbuff = buff;   
	xx[8] = 0;   

	/* first argument in args[0] */
	/* FIXME somehow check if args[i] for % is available! */
	while (*format) {
		if (*format == '%') {
			++format;
			if (*format == 0)
				break;
			
			/* TODO check flags, width, precision and length modifiers here */

			switch (*format) {
				case 's':    /* null-terminated string */
					t = (char *) args[aktarg];
					if (t) {
						while (*t)
							*buff++ = *t++;
					}
					break;
				
				case 'c':    /* single character */
					*buff++ = (char) args[aktarg];
					break;
					
				case 'X':   
				case 'x':    /* lower case hex */
					u = args[aktarg];
					i = 8;

					do {
						xx[i--] = (*format == 'x') ? hexmap_low[u & 0x0F] : hexmap[u & 0x0F];
						u >>= 4;
					} while (u && i >= 0);
					while (++i < 9)
						*buff++ = xx[i];
					break;
					
				case 'i':
				case 'd':    /* sint32_t */
					n = args[aktarg];
					if (n < 0) {
						u = -n;
						*buff++ = '-';
					} else {
						u = n;
					}
					
					i=9;    /* signed 32bit int is max 10 numbers */
					do {
						d[i--] = (u%10) + '0';
						u /= 10;
					} while (u && i>=0);
					while (++i < 10)
						*buff++ = d[i];
					break;
					
				case 'u':     /* unsigned integer */
					u = args[aktarg];
					i=9;
					do {
						d[i--] = (u%10) + '0';
						u /= 10;
					} while (u && i>=0);
					while (++i < 10)
						*buff++ = d[i];
					break;
				
				default:
					*buff++ = *format;
			}
			++aktarg;
		} else {
			*buff = *format;
			++buff;
		}
		++format;
	}

	*buff = 0;
	return buff - startbuff;
}

