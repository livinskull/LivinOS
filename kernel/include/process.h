#ifndef _PROCESS_H_
#define _PROCESS_H_


#include "inthandler.h"
#include "../config.h"


typedef enum ProcessState {
    ProcessState_Running,
    ProcessState_Blocked,
    ProcessState_Suspended
}ProcessState;



typedef struct Process {
    uint32_t ulPid;
    uint8_t szName[CONFIG_PROCESS_NAME_LENGTH];
    CpuState cpu;
    
}Process;


CpuState *schedule(CpuState *pState);



#endif
