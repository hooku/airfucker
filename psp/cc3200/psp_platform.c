/* platform specific - cc3200 */
/* Stdlib includes */
#include "../../af_common.h"

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
#include "gpio.h"

#include "../psp.h"

extern uVectorEntry __vector_table;

const platform_pin_mapping_t gpio_mapping[] =
{
    /* Common GPIOs for internal use */
    [PSP_POWER_LED]              = {GPIOA1_BASE, 0x8     , GPIO_DIR_MODE_OUT     }, 
    [PSP_SYS_LED]                = {GPIOA1_BASE, 0x4     , GPIO_DIR_MODE_OUT     }, 
    [PSP_RF_LED]                 = {GPIOA1_BASE, 0x2     , GPIO_DIR_MODE_OUT     },
    [PSP_EasyLink_BUTTON]        = {GPIOA2_BASE, 0x40    , GPIO_DIR_MODE_IN      }, 
    [PSP_WakeUp_BUTTON]          = {GPIOA1_BASE, 0x20    , GPIO_DIR_MODE_IN      }, 
};

static void psp_fault_isr(void)
{
    // 1. save a copy of stack pointer:
    asm volatile(
        " movs r0,#4            \n"
        " movs r1, lr           \n"
        " tst r0, r1            \n"     // test whether msp/psp
        " beq.n _MSP            \n"
        " mrs r0, psp           \n"
        " b.n _HALT             \n"
      "_MSP:                    \n"
        " mrs r0, msp           \n"
      "_HALT:                   \n"
        " ldr r1, [r0, #0x14]   \n"     // sp+6
        " bkpt #0               \n"     // breakpoint
          );
  
    // 2. print out error:
    UART_PRINT("HARD FAULT:\r\n");
    //UART_PRINT("%x", );
    
    // 3. halt (like BSOD):
    LOOP_FOREVER();
    
    return ;
}

int8_t psp_init_platform(void)
{
    int8_t ret = FAILURE;

    /* config interrupt */
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
    
    MAP_IntPrioritySet(INT_GPIOA0, INT_PRIORITY_LVL_1);
    MAP_IntPrioritySet(INT_GPIOA1, INT_PRIORITY_LVL_1);
    MAP_IntPrioritySet(INT_GPIOA2, INT_PRIORITY_LVL_1);
    MAP_IntPrioritySet(INT_GPIOA3, INT_PRIORITY_LVL_1);
    
    IntRegister(FAULT_HARD, psp_fault_isr);

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
    
    ret = SUCCESS;

    return ret;
}

void psp_sleep(uint32_t ms)
{
    UtilsDelay(80*10*ms);
    return ;
}

int8_t psp_gpio_init(psp_gpio_t gpio)
{
    int8_t ret = FAILURE;

    //MAP_GPIOIntClear(gpio_mapping[EasyLink_BUTTON].gpio_base, 0xFF);
    
    MAP_IntPendClear(INT_GPIOA1);
    MAP_IntEnable(INT_GPIOA1);

    ret = SUCCESS;
    
    return ret;
}

int8_t psp_gpio_set(psp_gpio_t gpio, uint8_t val)
{
    int8_t ret = FAILURE;
    
    long gpio_val;
    
    gpio_val = MAP_GPIOPinRead(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin);

    if (val)
    {
        gpio_val |= gpio_mapping[gpio].gpio_pin;
    }
    else
    {
        gpio_val &= (~gpio_mapping[gpio].gpio_pin);
    }
    
    MAP_GPIOPinWrite(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin, gpio_val);

    ret = SUCCESS;
    
    return ret;
}

bool psp_gpio_get(psp_gpio_t gpio)
{
    bool ret = false;
  
    long gpio_val;
    
    psp_gpio_t common_gpio = gpio;
    
    if (common_gpio > PSP_GPIO_UNUSED)
    {
        gpio_val = MAP_GPIOPinRead(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin);
        
        if (gpio_val == gpio_mapping[gpio].gpio_pin)
        {
            ret = true;
        }
        else
        {
            ret = false;
        }
    }
    
    return ret;
}

//int8_t psp_GpioEnableIRQ(mico_gpio_t gpio, mico_gpio_irq_trigger_t trigger, mico_gpio_irq_handler_t handler, void *arg)
//{
//    int8_t ret = FAILURE;
//    
//    if (gpio == EasyLink_BUTTON)
//    {
//        // save callback handler:
//        gpio_int_handler = handler;
//        
//        MAP_GPIOIntTypeSet(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin, GPIO_BOTH_EDGES);
//        
//        MAP_GPIOIntRegister(gpio_mapping[gpio].gpio_base, &mico_gpio_int_handler);
//        
////        MAP_GPIOIntDisable(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin);
//        MAP_GPIOIntClear(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin);
//        MAP_GPIOIntEnable(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin);   // should be refined
//#if 0
//        long gpio_val;
//        
//        while (1)
//        {
//            gpio_val = MAP_GPIOPinRead(gpio_mapping[gpio].gpio_base, gpio_mapping[gpio].gpio_pin);
//            
//            if (gpio_val == gpio_mapping[gpio].gpio_pin)
//            {
//                while (1)
//                    ;
//            }
//        }
//#endif
//    }
//    
//    return ret;
//}
