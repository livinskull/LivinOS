#include <textmode.h>
#include <string.h>
#include "config.h"


uint32_t g_Pos = 0;		// current position in video memory
static uint16_t *pScreen = (uint16_t *) 0xb8000;
static uint16_t s_Attribute = 0x0700;	// current attribute byte


#define GETX	(g_Pos % TEXTMODE_COLS)
#define GETY	((g_Pos - GETX) / TEXTMODE_COLS)



void scroll_down() {
	uint32_t tmp;
	
	if (GETY >= TEXTMODE_ROWS) {
		tmp = (GETY - TEXTMODE_ROWS) + 1;
		memcpy((void *) pScreen, (void *) ((uint32_t) pScreen + tmp*TEXTMODE_COLS*2), (TEXTMODE_ROWS - tmp) * TEXTMODE_COLS * 2);
		memset((void *) ((uint32_t) pScreen + (TEXTMODE_ROWS - tmp) * TEXTMODE_COLS * 2), 0, TEXTMODE_COLS * 2);
		g_Pos = 1920;
	}
}




void putchar(char ch) {
	switch (ch) {
		case '\n': 
			g_Pos -= GETX;
			g_Pos += TEXTMODE_COLS;
			break;
		case '\r':
			g_Pos -= GETX;
			break;
		case '\t':
			/* go until the next multiple of TAB_SIZE */
			g_Pos += CONFIG_TEXTMODE_TAB_SIZE;
			g_Pos -= g_Pos % CONFIG_TEXTMODE_TAB_SIZE;
			
			//g_Pos += (GETX < TEXTMODE_COLS - CONFIG_TEXTMODE_TAB_SIZE - 1) ? 
			//	CONFIG_TEXTMODE_TAB_SIZE : 0;
			break;
		
		default:
			pScreen[g_Pos++] = (uint16_t) ch | s_Attribute;
	}
	
	scroll_down();
}

/**
 * Output a zero-terminated string to the screen
 * @param uint8_t *s Pointer to string to output
 */
void puts(const char *s) {
	while(*s) 
		putchar(*(s++));
}


/**
 * Kernel printf; prints text formatted
 *     format specifiers:
 * 			%s	zero-terminated string
 * 			%c	single character
 * 			%x	lowercase hex
 * 			%X  uppercase hex
 * 			%d or %i   signed int
 * 			%u  unsigned int
 * 		modifiers:
 * 			l 	=> %lu => 64bit    => needs special div function (undefined reference to __umoddi3
 * @param uint8_t *format format string
 * @param ... arguments to format string
 */
void kprintf(char *pFormat, ...) {
	static char hexmap[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	static char hexmap_low[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};
	uint32_t *pArgs = ((uint32_t *) &pFormat) + 1;
	char *s, d[11], xx[9];
	uint32_t curArg = 0, u;
	int n, i,big=0;
	
	xx[8] = 0;
	
	
	if (pFormat == 0)
		return;
	
	while (*pFormat) {
		if (*pFormat == '%') {
			++pFormat;
			big = 0;
			
			if (*pFormat == 0) 
				break;
			
			if (*pFormat == 'l') {
				big=1;
				++pFormat;
			}
			
			switch (*pFormat) {
				case 's':	/* string */
					s = (char *) pArgs[curArg];
					if (s) {
						while (*s)
							putchar(*s++);
					} else {
						puts("(null)");
					}
					break;
				
				case 'c':	/* single character */
					putchar((char) pArgs[curArg]);
					break;
				
				case 'X':
				case 'x':	/* lowercase hex */
					i = 7;
					u = pArgs[curArg];
					do {
						xx[i--] = (*pFormat == 'x') ? hexmap_low[u & 0x0F] : hexmap[u & 0x0F];
						u >>= 4;
                    } while (u && i >= 0);
					
                    while (++i < 8)
						putchar(xx[i]);
					
					break;
					
				case 'i':
				case 'd':	/* signed int */
					n = pArgs[curArg];
					if (n < 0) {
						u = -n;
						putchar('-');
					} else 
						u = n;
					
					i=9;
					do {
						d[i--] = (u%10) + '0';
						u /= 10;
					} while (u && i>=0);
					while (++i < 10) 
						putchar(d[i]);
					break;
					
				case 'u': 	/* unsigned int */
					u = pArgs[curArg];
					i=9;
					do {
						d[i--] = (u%10) + '0';
						u /= 10;
					} while (u && i>=0);
					while (++i < 10) 
						putchar(d[i]);
					break;
						
				
				
				default:
					putchar(*pFormat);
			}
			++curArg;
			
		} else {
			/* no format specifier, just output */
			putchar(*pFormat);
		}
		++pFormat;
	}
	
}