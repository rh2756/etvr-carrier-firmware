#include "samd11.h"

extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

void reset_handler(void);
void dummy_handler(void);
int main(void);

/* Cortex-M0+ core handlers */
void NonMaskableInt_Handler(void) __attribute__((weak, alias("dummy_handler")));
void HardFault_Handler(void) __attribute__((weak, alias("dummy_handler")));
void SVCall_Handler(void) __attribute__((weak, alias("dummy_handler")));
void PendSV_Handler(void) __attribute__((weak, alias("dummy_handler")));
void SysTick_Handler(void) __attribute__((weak, alias("dummy_handler")));

/* Peripherals handlers */
void PM_Handler(void) __attribute__((weak, alias("dummy_handler")));
void SYSCTRL_Handler(void) __attribute__((weak, alias("dummy_handler")));
void WDT_Handler(void) __attribute__((weak, alias("dummy_handler")));
void RTC_Handler(void) __attribute__((weak, alias("dummy_handler")));
void EIC_Handler(void) __attribute__((weak, alias("dummy_handler")));
void NVMCTRL_Handler(void) __attribute__((weak, alias("dummy_handler")));
void DMAC_Handler(void) __attribute__((weak, alias("dummy_handler")));
void USB_Handler(void) __attribute__((weak, alias("dummy_handler")));
void EVSYS_Handler(void) __attribute__((weak, alias("dummy_handler")));
void SERCOM0_Handler(void) __attribute__((weak, alias("dummy_handler")));
void SERCOM1_Handler(void) __attribute__((weak, alias("dummy_handler")));
void SERCOM2_Handler(void) __attribute__((weak, alias("dummy_handler")));
void TCC0_Handler(void) __attribute__((weak, alias("dummy_handler")));
void TC1_Handler(void) __attribute__((weak, alias("dummy_handler")));
void TC2_Handler(void) __attribute__((weak, alias("dummy_handler")));
void ADC_Handler(void) __attribute__((weak, alias("dummy_handler")));
void AC_Handler(void) __attribute__((weak, alias("dummy_handler")));
void DAC_Handler(void) __attribute__((weak, alias("dummy_handler")));
void PTC_Handler(void) __attribute__((weak, alias("dummy_handler")));

/* Exception Table */
__attribute__((section(".vectors"))) const DeviceVectors exception_table = {

	/* Configure Initial Stack Pointer, using linker-generated symbols */
	.pvStack = (void *)(&_estack),

	.pfnReset_Handler = (void *)reset_handler,
	.pfnNonMaskableInt_Handler = (void *)NonMaskableInt_Handler,
	.pfnHardFault_Handler = (void *)HardFault_Handler,
	.pvReservedM12 = (void *)(0UL), /* Reserved */
	.pvReservedM11 = (void *)(0UL), /* Reserved */
	.pvReservedM10 = (void *)(0UL), /* Reserved */
	.pvReservedM9 = (void *)(0UL),	/* Reserved */
	.pvReservedM8 = (void *)(0UL),	/* Reserved */
	.pvReservedM7 = (void *)(0UL),	/* Reserved */
	.pvReservedM6 = (void *)(0UL),	/* Reserved */
	.pfnSVCall_Handler = (void *)SVCall_Handler,
	.pvReservedM4 = (void *)(0UL), /* Reserved */
	.pvReservedM3 = (void *)(0UL), /* Reserved */
	.pfnPendSV_Handler = (void *)PendSV_Handler,
	.pfnSysTick_Handler = (void *)SysTick_Handler,

	/* Configurable interrupts */
	.pfnPM_Handler = (void *)PM_Handler,		   /*  0 Power Manager */
	.pfnSYSCTRL_Handler = (void *)SYSCTRL_Handler, /*  1 System Control */
	.pfnWDT_Handler = (void *)WDT_Handler,		   /*  2 Watchdog Timer */
	.pfnRTC_Handler = (void *)RTC_Handler,		   /*  3 Real-Time Counter */
	.pfnEIC_Handler = (void *)EIC_Handler,		   /*  4 External Interrupt Controller */
	.pfnNVMCTRL_Handler = (void *)NVMCTRL_Handler, /*  5 Non-Volatile Memory Controller */
	.pfnDMAC_Handler = (void *)DMAC_Handler,	   /*  6 Direct Memory Access Controller */
	.pfnUSB_Handler = (void *)USB_Handler,		   /*  7 Universal Serial Bus */
	.pfnEVSYS_Handler = (void *)EVSYS_Handler,	   /*  8 Event System Interface */
	.pfnSERCOM0_Handler = (void *)SERCOM0_Handler, /*  9 Serial Communication Interface 0 */
	.pfnSERCOM1_Handler = (void *)SERCOM1_Handler, /* 10 Serial Communication Interface 1 */
	.pfnSERCOM2_Handler = (void *)SERCOM2_Handler, /* 11 Serial Communication Interface 2 */
	.pfnTCC0_Handler = (void *)TCC0_Handler,	   /* 12 Timer Counter Control */
	.pfnTC1_Handler = (void *)TC1_Handler,		   /* 13 Basic Timer Counter 0 */
	.pfnTC2_Handler = (void *)TC2_Handler,		   /* 14 Basic Timer Counter 1 */
	.pfnADC_Handler = (void *)ADC_Handler,		   /* 15 Analog Digital Converter */
	.pfnAC_Handler = (void *)AC_Handler,		   /* 16 Analog Comparators */
	.pfnDAC_Handler = (void *)DAC_Handler,		   /* 17 Digital Analog Converter */
	.pfnPTC_Handler = (void *)PTC_Handler		   /* 18 Peripheral Touch Controller */
};

void reset_handler(void)
{
	uint32_t *src = &_sidata;
	uint32_t *dst;

	/* Copy .data from flash to RAM */
	for (dst = &_sdata; dst < &_edata;)
	{
		*dst++ = *src++;
	}

	/* Zero .bss */
	for (dst = &_sbss; dst < &_ebss;)
	{
		*dst++ = 0;
	}

	/* Set the vector table base address */
	SCB->VTOR = ((uint32_t)&exception_table & SCB_VTOR_TBLOFF_Msk);

	/* Change default QOS values to have the best performance and correct USB behaviour */
	SBMATRIX->SFR[SBMATRIX_SLAVE_HMCRAMC0].reg = 2;
	USB->DEVICE.QOSCTRL.bit.CQOS = 2;
	USB->DEVICE.QOSCTRL.bit.DQOS = 2;
	DMAC->QOSCTRL.bit.DQOS = 2;
	DMAC->QOSCTRL.bit.FQOS = 2;
	DMAC->QOSCTRL.bit.WRBQOS = 2;

	main();

	while (1)
		;
}

void dummy_handler(void)
{
	while (1)
		;
}
