/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file main.c
 **
 ** \brief This sample demonstrates how to use Timer6.
 **
 **   - 2018-11-30  1.0  husj first version for Device Driver Library of Timer6.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* KEY0 (SW2)*/
#define  SW2_PORT           (PortD)
#define  SW2_PIN            (Pin03)
/* KEY1 (SW4)*/
#define  SW4_PORT           (PortD)
#define  SW4_PIN            (Pin04)
/* KEY2 (SW3)*/
#define  SW3_PORT           (PortD)
#define  SW3_PIN            (Pin05)
/* KEY3 (SW5)*/
#define  SW5_PORT           (PortD)
#define  SW5_PIN            (Pin06)

/* LED0 Port/Pin definition */
#define  LED0_PORT          (PortE)
#define  LED0_PIN           (Pin06)

/* LED1 Port/Pin definition */
#define  LED1_PORT          (PortD)
#define  LED1_PIN           (Pin07)

/* LED2 Port/Pin definition */
#define  LED2_PORT          (PortB)
#define  LED2_PIN           (Pin05)

/* LED3 Port/Pin definition */
#define  LED3_PORT          (PortB)
#define  LED3_PIN           (Pin09)

/* LED0~1 toggle definition */
#define  LED0_TOGGLE()      (PORT_Toggle(LED0_PORT, LED0_PIN))
#define  LED1_TOGGLE()      (PORT_Toggle(LED1_PORT, LED1_PIN))
#define  LED2_TOGGLE()      (PORT_Toggle(LED2_PORT, LED2_PIN))
#define  LED3_TOGGLE()      (PORT_Toggle(LED3_PORT, LED3_PIN))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/


/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Callback function of external interrupt ch.0
 **
 ******************************************************************************/

void Timer6_OverFlow_CallBack(void)
{
    LED0_TOGGLE();
}

/**
 ******************************************************************************
 ** \brief  Initialize the system clock for the sample
 **
 ** \param  None
 **
 ** \return None
 ******************************************************************************/
static void SysClkIni(void)
{
    en_clk_sys_source_t       enSysClkSrc;
    stc_clk_sysclk_cfg_t      stcSysClkCfg;
    stc_clk_mpll_cfg_t        stcMpllCfg;
    stc_clk_output_cfg_t      stcOutputClkCfg;


    MEM_ZERO_STRUCT(enSysClkSrc);
    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);
    MEM_ZERO_STRUCT(stcOutputClkCfg);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv = ClkSysclkDiv1;   // 168MHz
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;  // 84MHz
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;  // 168MHz   (timer6 cnt)
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;  // 84MHz    (timer6 logic)
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;  // 42MHz
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;  // 42MHz
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;  // 84MHz
    CLK_SysClkConfig(&stcSysClkCfg);

    CLK_HrcCmd(Enable);       //Enable HRC

    /* MPLL config. */
    stcMpllCfg.pllmDiv = 2ul;   //HRC 16M / 2
    stcMpllCfg.plln    = 42ul;  //8M*42 = 336M
    stcMpllCfg.PllpDiv = 2ul;   //MLLP = 168M
    stcMpllCfg.PllqDiv = 2ul;   //MLLQ = 168M
    stcMpllCfg.PllrDiv = 2ul;   //MLLR = 168M
    CLK_SetPllSource(ClkPllSrcHRC);
    CLK_MpllConfig(&stcMpllCfg);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_4);
    EFM_Lock();

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);

    /* Wait MPLL ready. */
    while(Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
        ;
    }

    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);
}

/**
 *******************************************************************************
 ** \brief  Main function of project
 **
 ** \param  None
 **
 ** \retval int32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    stc_port_init_t                  stcPortInit;
    stc_timer6_basecnt_cfg_t         stcTIM6BaseCntCfg;
    stc_timer6_port_output_cfg_t     stcTIM6PWMxCfg;
    stc_timer6_gcmp_buf_cfg_t        stcGCMPBufCfg;
    stc_irq_regi_conf_t              stcIrqRegiConf;

    MEM_ZERO_STRUCT(stcPortInit);
    MEM_ZERO_STRUCT(stcTIM6BaseCntCfg);
    MEM_ZERO_STRUCT(stcTIM6PWMxCfg);
    MEM_ZERO_STRUCT(stcGCMPBufCfg);
    MEM_ZERO_STRUCT(stcPortInit);
    MEM_ZERO_STRUCT(stcIrqRegiConf);

    SysClkIni();

    PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM61, Enable);   //Enable Timer61 Module
    PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM62, Enable);   //Enable Timer62 Module
    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_AOS, Enable);     //Enable AOS Module
    
    stcPortInit.enPinMode = Pin_Mode_Out;
    /* LED0 Port/Pin initialization */
    PORT_Init(LED0_PORT, LED0_PIN, &stcPortInit);
    
    stcTIM6BaseCntCfg.enCntMode   = Timer6CntSawtoothMode;              //Sawtooth wave mode
    stcTIM6BaseCntCfg.enCntDir    = Timer6CntDirUp;                     //Counter counting up
    stcTIM6BaseCntCfg.enCntClkDiv = Timer6PclkDiv1;                     //Count clock: pclk0
    Timer6_Init(M4_TMR61, &stcTIM6BaseCntCfg);                          //timer6 PWM frequency, count mode and clk config
    
#ifdef __DEBUG
    M4_DBGC->MCUSTPCTL_f.TM61STP = 1;
    M4_DBGC->MCUSTPCTL_f.TM62STP = 1;
#endif /* __DEBUG */
    
    Timer6_SetPeriod(M4_TMR61, Timer6PeriodA, 0xFFFF);            //Timer61 period set

    Timer6_SetPeriod(M4_TMR62, Timer6PeriodA, 500);               //Timer62 period set
    

    Timer6_SetTriggerSrc0(EVT_TMR61_GOVF);            //Set Timer61 OVF envet as Timer6 AOS0 event.
    
    Timer6_ConfigHwCntUp(M4_TMR62, Timer6HwCntAos0);  //Set AOS0 event as Timer62 hardware count-up event
    
    
    
    /*config interrupt*/
    /* Enable timer61 GOVF interrupt */
    Timer6_ConfigIrq(M4_TMR61, Timer6INTENOVF, true);
    
    
    
    /* Enable timer62 GOVF interrupt */
    Timer6_ConfigIrq(M4_TMR62, Timer6INTENOVF, true);

    stcIrqRegiConf.enIRQn = Int002_IRQn;                    //Register INT_TMR62_GOVF Int to Vect.No.002
    stcIrqRegiConf.enIntSrc = INT_TMR62_GOVF;               //Select Event interrupt of timer61
    stcIrqRegiConf.pfnCallback = &Timer6_OverFlow_CallBack;  //Callback function
    enIrqRegistration(&stcIrqRegiConf);                     //Registration IRQ

    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);            //Clear Pending
    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);//Set priority
    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);                   //Enable NVIC


    /*start timer6*/
    Timer6_StartCount(M4_TMR62);
    Timer6_StartCount(M4_TMR61);

    while(1)
    {
        ;
    }

}




/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
