#include"Ucs_Init_Set.h"
#include <msp430f5529.h>
/*
 * Ucs_Init_Set.c
 *
 *  Created on: 2015年7月17日
 *      Author: pfx
 */

void SetVcoreUp (unsigned int level);
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 25MHz*******//
void Ucs_25Mhz(void)
{
	  SetVcoreUp (0x01);
	  SetVcoreUp (0x02);
	  SetVcoreUp (0x03);

	  UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	  __bis_SR_register(SCG0);                  // Disable the FLL control loop
	  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	  UCSCTL1 = DCORSEL_7;                      // Select DCO range 50MHz operation
	  UCSCTL2 = FLLD_0 + 762;                   // Set DCO Multiplier for 25MHz
	                                            // (N + 1) * FLLRef = Fdco
	                                            // (762 + 1) * 32768 = 25MHz
	                                            // Set FLL Div = fDCOCLK/2
	  __bic_SR_register(SCG0);                  // Enable the FLL control loop

	  // Worst-case settling time for the DCO when the DCO range bits have been
	  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	  // UG for optimization.
	  // 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
	  __delay_cycles(782000);

	  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	  do
	  {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 20MHz*******//
void Ucs_20Mhz(void)
{
	  SetVcoreUp (0x01);
	  SetVcoreUp (0x02);
	  SetVcoreUp (0x03);

	  UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	  __bis_SR_register(SCG0);                  // Disable the FLL control loop
	  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	  UCSCTL1 = DCORSEL_7;                      // Select DCO range 50MHz operation
	  UCSCTL2 = FLLD_0 + 609;                   // Set DCO Multiplier for 25MHz
	                                            // (N + 1) * FLLRef = Fdco
	                                            // (609 + 1) * 32768 = 20MHz
	                                            // Set FLL Div = fDCOCLK/2
	  __bic_SR_register(SCG0);                  // Enable the FLL control loop

	  // Worst-case settling time for the DCO when the DCO range bits have been
	  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	  // UG for optimization.
	  // 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
	  __delay_cycles(782000);

	  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	  do
	  {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}
//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 8MHz*******//
void Ucs_8Mhz(void)
{
	  UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
	  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx

	  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	  do
	  {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	  __bis_SR_register(SCG0);                  // Disable the FLL control loop
	  UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
	  UCSCTL2 |= 249;                           // Set DCO Multiplier for 8MHz
	                                            // (N + 1) * FLLRef = Fdco
	                                            // (249 + 1) * 32768 = 8MHz
	  __bic_SR_register(SCG0);                  // Enable the FLL control loop

	  // Worst-case settling time for the DCO when the DCO range bits have been
	  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	  // UG for optimization.
	  // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
	  __delay_cycles(250000);
}

//***********功能：ACLK = REFO = 32kHz, MCLK = SMCLK = 12MHz*******//
void Ucs_12Mhz(void)
{
	  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
	  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	  __bis_SR_register(SCG0);                  // Disable the FLL control loop
	  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	  UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
	  UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
	                                            // (N + 1) * FLLRef = Fdco
	                                            // (374 + 1) * 32768 = 12MHz
	                                            // Set FLL Div = fDCOCLK/2
	  __bic_SR_register(SCG0);                  // Enable the FLL control loop

	  // Worst-case settling time for the DCO when the DCO range bits have been
	  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	  // UG for optimization.
	  // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
	  __delay_cycles(375000);

	  // Loop until XT1,XT2 & DCO fault flag is cleared
	  //测试晶振是否产生故障失效，并清除故障失效标志位
	  do
	  {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
}

//**************在相应管脚输出时钟信号******************//
void Sysclk_Signal_Outpt(void)
{
	  P1DIR |= BIT0;                            // ACLK set out to pins
	  P1SEL |= BIT0;
	  P2DIR |= BIT2;                            // SMCLK set out to pins
	  P2SEL |= BIT2;
	  P7DIR |= BIT7;                            // MCLK set out to pins
	  P7SEL |= BIT7;
}
void Sysclk_Init_4Mhz_XT2(void)
{
	P5SEL |= BIT2+BIT3;                       // Port select XT2

	  UCSCTL6 &= ~XT2OFF;                       // Enable XT2
	  UCSCTL3 |= SELREF_2;                      // FLLref = REFO
	                                            // Since LFXT1 is not used,
	                                            // sourcing FLL with LFXT1 can cause
	                                            // XT1OFFG flag to set
	  UCSCTL4 |= SELA_2;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO

	  // Loop until XT1,XT2 & DCO stabilizes - in this case loop until XT2 settles
	  do
	  {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	  UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
	                                            // expected frequency
	  UCSCTL4 |= SELS_5 + SELM_5;               // SMCLK=MCLK=XT2
}


//配置Ucs_25Mhz 函数需要的库函数，TI官方提供//
void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}


