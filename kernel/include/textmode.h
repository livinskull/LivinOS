#ifndef _TEXTMODE_H_
#define _TEXTMODE_H_

#include <stdint.h>


#define	TEXTMODE_COLS		80
#define TEXTMODE_ROWS		25


/* colors, color 0-7 also available for background, ALL colors avail for foreground */
typedef enum {
	TextmodeColor_Black = 0,
	TextmodeColor_Blue,
	TextmodeColor_Green,
	TextmodeColor_Cyan,
	TextmodeColor_Red,
	TextmodeColor_Magenta,
	TextmodeColor_Brown,
	TextmodeColor_LightGrey,
	TextmodeColor_Grey,
	TextmodeColor_LightBlue,
	TextmodeColor_LightGreen,
	TextmodeColor_LightCyan,
	TextmodeColor_LightRed,
	TextmodeColor_LightMagenta,
	TextmodeColor_Yellow,
	TextmodeColor_White
}Textmode_Color;

// combines foregorund and background color
#define TEXTMODE_COLOR(fg,bg) ((bg << 4) | fg)


//only background color relevant when using ' '
extern void textmode_clearscreen(uint8_t byChar, uint8_t byCol);

void putchar(char ch);
void puts(const char *s);
void kprintf(char *pFormat, ...);


#endif
