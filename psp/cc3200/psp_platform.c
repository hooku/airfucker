/* platform specific - cc3200 */
/* Stdlib includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* Driverlib includes */
#include "hw_types.h"
#include "hw_ints.h"
#include "interrupt.h"
#include "utils.h"
#include "uart.h"
#include "hw_memmap.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"

/* Common interface includes */
#include "common.h"
#include "uart_if.h"

extern uVectorEntry __vector_table;

int8_t psp_init_platform(void)
{
    int8_t result = FAILURE;

    /* config interrupt */
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
    
    MAP_IntPrioritySet(INT_GPIOA0, INT_PRIORITY_LVL_1);
    MAP_IntPrioritySet(INT_GPIOA1, INT_PRIORITY_LVL_1);
    MAP_IntPrioritySet(INT_GPIOA2, INT_PRIORITY_LVL_1);
    MAP_IntPrioritySet(INT_GPIOA3, INT_PRIORITY_LVL_1);
    
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    /* config simplelink */
    sl_DeviceDisable();
    sl_DeviceEnable();

    /* config prcm */
    PRCMCC3200MCUInit();
    
    /* config iomux */
    PinMuxConfig();

    /* config terminal */
    InitTerm();
    ClearTerm();
    
    result = SUCCESS;

    return result;
}
