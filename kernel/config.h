#ifndef _CONFIG_H_
#define _CONFIG_H_


/* general */
#define CONFIG_SYSTEM_FREQUENCY			100		// system frequency in Hz


/* textmode.c */
#define CONFIG_TEXTMODE_TAB_SIZE		8		// number of spaces for a tab

/* task management */
#define CONFIG_PROCESS_TIMESLICE            1   // with 100Hz this is 10ms
#define CONFIG_PROCESS_NAME_LENGTH         64   // max length of process name       


#endif
