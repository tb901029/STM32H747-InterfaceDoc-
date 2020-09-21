/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   Please note: The implementation of this module is partially based on
*   examples that are provided within the STM32 cube firmware. In case you want
*   to adapt this module to your custom specific hardware, please adapt the
*   hardware initialization code according your needs or integrate the generated
*   initialization code created by using the tool CubeMX.
*   This template is responsible to initialize the display hardware of the board
*   and to provide the necessary access to update the display content.
*
*******************************************************************************/

#include "ewconfig.h"
#include "stm32h7xx_hal.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_bus.h"
#include "stm32h747i_discovery_lcd.h"

#include <string.h>

#include "ewrte.h"

#include "ew_bsp_graphics.h"
#include "ew_bsp_display.h"
#include "ew_bsp_clock.h"

#if EW_USE_FREE_RTOS == 1

  #include "cmsis_os.h"

  static osSemaphoreId        LcdUpdateSemaphoreId = 0;

#endif

#define LAYER_INDEX           0

#define VSYNC                 1
#define VBP                   1
#define VFP                   1
#define HSYNC                 1
#define HBP                   1
#define HFP                   1

#define NO_AREA               0
#define LEFT_AREA             1
#define RIGHT_AREA            2

#define hltdc_handle          hlcd_ltdc
#define hdsi_handle           hlcd_dsi

#define __DSI_MASK_TE()       (GPIOJ->AFR[0] &= (0xFFFFF0FFU))   /* Mask DSI TearingEffect Pin*/
#define __DSI_UNMASK_TE()     (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8)) /* UnMask DSI TearingEffect Pin*/

#if (( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 ) \
  || ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB888 ))
  #define DSI_COLOR_CODING    DSI_RGB888
  #define OTM8009A_FORMAT     OTM8009A_FORMAT_RGB888
#else
  #define DSI_COLOR_CODING    DSI_RGB565
  #define OTM8009A_FORMAT     OTM8009A_FORMAT_RBG565
#endif

extern LTDC_HandleTypeDef     hltdc_handle;
extern DSI_HandleTypeDef      hdsi_handle;

static LTDC_LayerCfgTypeDef   LayerConfig;
static volatile int32_t       ActiveArea  = NO_AREA;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

#if 0
static uint8_t pColLeft[]   = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
static uint8_t pColRight[]  = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
static uint8_t pPage[]      = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
#if EW_USE_DOUBLE_BUFFER == 1
  static uint8_t pScanCol[] = {0x01, 0xB0};             /* Scan @ 432 */
#else
  static uint8_t pScanCol[] = {0x02, 0x40};             /* Scan @ 576 */
#endif
#endif
  
uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pScanCol[]    = {0x02, 0x15};             /* Scan @ 533 */
/* Private function prototypes -----------------------------------------------*/  
  
  

#if EW_USE_FREE_RTOS == 1
void HAL_DSI_MspInit(DSI_HandleTypeDef* hdsi)
{
  HAL_NVIC_SetPriority(DSI_IRQn, 8, 0);
}
#endif

static int FirstUpdateIsDone = 0;


void LCD_MspInit(void)
{
  /** @brief Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /** @brief Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /** @brief Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();

  /** @brief Soft Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  /** @brief NVIC configuration for LTDC interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0x0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0x0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 3, 0x0);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}


static void LCD_DisplayOn( void )
{
  /* Send Display On DCS Command to display */
  HAL_DSI_ShortWrite(&hdsi_handle,
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPON,
                     0x00);
}


static void LCD_DisplayOff( void )
{
  /* Send Display On DCS Command to display */
  HAL_DSI_ShortWrite(&hdsi_handle,
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPOFF,
                     0x00);
}


/**
  * @brief  DCS or Generic short/long write command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be written
  * @param  pData pointer to a buffer of data to be write
  * @param  Size To precise command to be used (short or long)
  * @retval BSP status
  */
static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Size <= 1U)
  {
    if(HAL_DSI_ShortWrite(&hdsi_handle, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  else
  {
    if(HAL_DSI_LongWrite(&hdsi_handle, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  DCS or Generic read command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be read
  * @param  pData pointer to a buffer to store the payload of a read back operation.
  * @param  Size  Data size to be read (in byte).
  * @retval BSP status
  */
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_DSI_Read(&hdsi_handle, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  return ret;
}



void LTDC_IRQHandler( void )
{
  HAL_LTDC_IRQHandler( &hltdc_handle );
}

void LTDC_ER_IRQHandler( void )
{
 // BSP_LCD_LTDC_ER_IRQHandler();
}

void DSI_IRQHandler( void )
{
  HAL_DSI_IRQHandler( &hdsi_handle );
}


void HAL_DSI_TearingEffectCallback( DSI_HandleTypeDef* hdsi )
{
  /* Mask the TE */
  __DSI_MASK_TE();

  /* Refresh the left part of the display */
  HAL_DSI_Refresh( hdsi );
}


void HAL_DSI_EndOfRefreshCallback( DSI_HandleTypeDef* hdsi )
{
  if ( ActiveArea == LEFT_AREA )
  {
    /* Disable DSI Wrapper */
    __HAL_DSI_WRAPPER_DISABLE( hdsi );

    /* Update LTDC configuaration */
    LTDC_LAYER( &hltdc_handle, LAYER_INDEX )->CFBAR = LayerConfig.FBStartAdress + LayerConfig.ImageWidth * FRAME_BUFFER_DEPTH;
    __HAL_LTDC_RELOAD_CONFIG( &hltdc_handle );

    /* Enable DSI Wrapper */
    __HAL_DSI_WRAPPER_ENABLE( hdsi );

    HAL_DSI_LongWrite( hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight );

    /* Refresh the display */
    HAL_DSI_Refresh( hdsi );

    ActiveArea = RIGHT_AREA;
  }
  else if ( ActiveArea == RIGHT_AREA )
  {

    #if EW_USE_FREE_RTOS == 1

      osSemaphoreRelease( LcdUpdateSemaphoreId );

    #endif

    ActiveArea = NO_AREA;

    /* DSI update is done, now let the DMA2D work in parallel with CPU */
    EwBspGraphicsConcurrentOperation( 1 );
  }
}


/* helper function to wait until framebuffer transfer via DSI is completed */
static void WaitForDsiTransfer( void )
{
  if ( ActiveArea == NO_AREA )
    return;

  CPU_LOAD_SET_IDLE();

  #if EW_USE_FREE_RTOS == 1

    /* wait until framebuffer is completely transferred */
    while ( ActiveArea != NO_AREA )
      osSemaphoreWait( LcdUpdateSemaphoreId, 1000 );

  #else

    while ( ActiveArea != NO_AREA )
      ;

  #endif

  CPU_LOAD_SET_ACTIVE();
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayInit
*
* DESCRIPTION:
*   The function EwBspDisplayInit initializes the display hardware and returns
*   the display parameter.
*
* ARGUMENTS:
*   aDisplayInfo - Display info data structure.
*
* RETURN VALUE:
*   Returns 1 if successful, 0 otherwise.
*
*******************************************************************************/
int EwBspDisplayInit( XDisplayInfo* aDisplayInfo )
{
  DSI_CmdCfgTypeDef         CmdCfg;
  DSI_LPCmdTypeDef          LPCmd;
  DSI_PLLInitTypeDef        dsiPllInit;
  GPIO_InitTypeDef          GPIO_Init_Structure;
  
  DSI_PHY_TimerTypeDef       PhyTimings;  
  OTM8009A_IO_t              IOCtx;
  static OTM8009A_Object_t   OTM8009AObj;
  static void                *Lcd_CompObj;
  
  memset( &CmdCfg, 0, sizeof( CmdCfg ));
  memset( &LPCmd, 0, sizeof( LPCmd ));
  memset( &dsiPllInit, 0, sizeof( dsiPllInit ));
  memset( &GPIO_Init_Structure, 0, sizeof( GPIO_Init_Structure ));

  ActiveArea = NO_AREA;

  /* Toggle Hardware Reset of the DSI LCD using its XRES signal (active low) */
  BSP_LCD_Reset( 0 );

  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  LCD_MspInit();

  /* LCD clock configuration */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/19 = 42 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 42 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;    
  PeriphClkInitStruct.PLL3.PLL3N = 160;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;  
  PeriphClkInitStruct.PLL3.PLL3R = 19;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);     
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_handle.Instance = DSI;

  HAL_DSI_DeInit(&(hdsi_handle));

  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;

  hdsi_handle.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi_handle.Init.TXEscapeCkdiv = 0x4;
  HAL_DSI_Init(&(hdsi_handle), &(dsiPllInit));

  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_COLOR_CODING;
  CmdCfg.CommandSize           = FRAME_BUFFER_WIDTH / 2; /* screen is diveded into 2 areas! */
  CmdCfg.TearingEffectSource   = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_DISABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi_handle, &CmdCfg);

  LPCmd.LPGenShortWriteNoP     = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP    = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP    = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP      = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP     = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP     = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite         = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP     = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP    = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP      = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite         = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(&hdsi_handle, &LPCmd);

  /* De-Initialize LTDC */
  HAL_LTDC_DeInit(&hltdc_handle);

  /* Configure LTDC */
  hltdc_handle.Init.HorizontalSync     = HSYNC;
  hltdc_handle.Init.VerticalSync       = VSYNC;
  hltdc_handle.Init.AccumulatedHBP     = HSYNC + HBP;
  hltdc_handle.Init.AccumulatedVBP     = VSYNC + VBP;
  hltdc_handle.Init.AccumulatedActiveW = HSYNC + HBP + FRAME_BUFFER_WIDTH / 2; /* screen is diveded into 2 areas! */
  hltdc_handle.Init.AccumulatedActiveH = VSYNC + VBP + FRAME_BUFFER_HEIGHT;
  hltdc_handle.Init.TotalWidth         = HSYNC + HBP + FRAME_BUFFER_WIDTH / 2 + HFP; /* screen is diveded into 2 areas! */
  hltdc_handle.Init.TotalHeigh         = VSYNC + VBP + FRAME_BUFFER_HEIGHT + VFP;
  hltdc_handle.Init.Backcolor.Blue     = 0;
  hltdc_handle.Init.Backcolor.Green    = 0;
  hltdc_handle.Init.Backcolor.Red      = 0;
  hltdc_handle.Init.HSPolarity         = LTDC_HSPOLARITY_AL;
  hltdc_handle.Init.VSPolarity         = LTDC_VSPOLARITY_AL;
  hltdc_handle.Init.DEPolarity         = LTDC_DEPOLARITY_AL;
  hltdc_handle.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;
  hltdc_handle.Instance                = LTDC;

  /* Initialize LTDC */
  HAL_LTDC_Init(&hltdc_handle);

  /* Start DSI */
  HAL_DSI_Start(&(hdsi_handle));

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi_handle, &PhyTimings);     
  
  
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver) */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;
  OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx);
  Lcd_CompObj=(&OTM8009AObj);  
  OTM8009A_Init( Lcd_CompObj, OTM8009A_FORMAT, OTM8009A_ORIENTATION_LANDSCAPE );

  /* Switch-off the backlight to hide random pixel after start-up */
  LCD_DisplayOff();

  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi_handle, &LPCmd);

  HAL_DSI_ConfigFlowControl(&hdsi_handle, DSI_FLOW_CONTROL_BTA);
  HAL_DSI_ForceRXLowPower(&hdsi_handle, ENABLE);  

  /* Enable GPIOJ clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  
  /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
  /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
  /* This pin is used only when the LCD and DSI link is configured in command mode    */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);

  /* Refresh the display */
//  HAL_DSI_Refresh(&hdsi_handle);

  
  /* Disable DSI Wrapper in order to access and configure the LTDC */
  __HAL_DSI_WRAPPER_DISABLE(&hdsi_handle);  
  
  /* Layer Init */
  LayerConfig.WindowX0 = 0;
  LayerConfig.WindowX1 = FRAME_BUFFER_WIDTH / 2;
  LayerConfig.WindowY0 = 0;
  LayerConfig.WindowY1 = FRAME_BUFFER_HEIGHT;

  /* Pixel Format configuration: translate framebuffer color format into LTDC mode */
  #if ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA8888 )
    LayerConfig.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB888 )
    LayerConfig.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGBA4444 )
    LayerConfig.PixelFormat = LTDC_PIXEL_FORMAT_ARGB4444;
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_RGB565 )
    LayerConfig.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_Index8 )
    LayerConfig.PixelFormat = LTDC_PIXEL_FORMAT_L8;
  #elif ( EW_FRAME_BUFFER_COLOR_FORMAT == EW_FRAME_BUFFER_COLOR_FORMAT_LumA44 )
    LayerConfig.PixelFormat = LTDC_PIXEL_FORMAT_AL44;
  #endif

  LayerConfig.FBStartAdress = (uint32_t)FRAME_BUFFER_ADDR;
  LayerConfig.Alpha = 255;
  LayerConfig.Alpha0 = 0;
  LayerConfig.Backcolor.Blue = 0;
  LayerConfig.Backcolor.Green = 0;
  LayerConfig.Backcolor.Red = 0;
  LayerConfig.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  LayerConfig.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  LayerConfig.ImageWidth = FRAME_BUFFER_WIDTH / 2;
  LayerConfig.ImageHeight = FRAME_BUFFER_HEIGHT;

  HAL_LTDC_ConfigLayer(&hltdc_handle, &LayerConfig, LAYER_INDEX );

  /* Update pitch : the draw is done on the whole physical X Size */
  HAL_LTDC_SetPitch( &hltdc_handle, FRAME_BUFFER_WIDTH, LAYER_INDEX );

  /* Enable DSI Wrapper so DSI IP will drive the LTDC */
  __HAL_DSI_WRAPPER_ENABLE(&hdsi_handle);   

  HAL_DSI_LongWrite( &hdsi_handle, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft );
  HAL_DSI_LongWrite( &hdsi_handle, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage );
  
  HAL_DSI_LongWrite(&hdsi_handle, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pScanCol);
  
  /* short delay necessary to ensure proper DSI update... */
  HAL_Delay( 1000 );

  #if EW_USE_FREE_RTOS == 1

    osSemaphoreDef( LcdUpdateSemaphore );
    LcdUpdateSemaphoreId = osSemaphoreCreate( osSemaphore( LcdUpdateSemaphore ), 1 );

  #endif

  /* return the current display configuration */
  if ( aDisplayInfo )
  {
    memset( aDisplayInfo, 0, sizeof( XDisplayInfo ));
    aDisplayInfo->FrameBuffer   = (void*)FRAME_BUFFER_ADDR;
    aDisplayInfo->DoubleBuffer  = (void*)DOUBLE_BUFFER_ADDR;
    aDisplayInfo->BufferWidth   = FRAME_BUFFER_WIDTH;
    aDisplayInfo->BufferHeight  = FRAME_BUFFER_HEIGHT;
    aDisplayInfo->DisplayWidth  = FRAME_BUFFER_WIDTH;
    aDisplayInfo->DisplayHeight = FRAME_BUFFER_HEIGHT;
    aDisplayInfo->UpdateMode    = EW_BSP_DISPLAY_UPDATE_NORMAL;
  }
  return 1;
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayDone
*
* DESCRIPTION:
*   The function EwBspDisplayDone deinitializes the display hardware.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayDone( void )
{
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayGetUpdateArea
*
* DESCRIPTION:
*   The function EwBspDisplayGetUpdateArea returns the next update area
*   depending on the selected display mode:
*   In case of a synchroneous single-buffer, the function has to return the
*   the rectangular areas that correspond to the horizontal stripes (fields)
*   of the framebuffer.
*   In case of a scratch-pad buffer, the function has to return the subareas
*   that fit into the provided update rectangle.
*   During each display update, this function is called until it returns 0.
*
* ARGUMENTS:
*   aUpdateRect - Rectangular area which should be updated (redrawn).
*
* RETURN VALUE:
*   Returns 1 if a further update area can be provided, 0 otherwise.
*
*******************************************************************************/
int EwBspDisplayGetUpdateArea( XRect* aUpdateRect )
{
  return 0;
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayWaitForCompletion
*
* DESCRIPTION:
*   The function EwBspDisplayWaitForCompletion is called from the Graphics Engine
*   to ensure that all pending activities of the display system are completed, so
*   that the rendering of the next frame can start.
*   In case of a double-buffering system, the function has to wait until the
*   V-sync has occured and the pending buffer is used by the display controller.
*   In case of an external display controller, the function has to wait until
*   the transfer (update) of the graphics data has been completed and there are
*   no pending buffers.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayWaitForCompletion( void )
{
  #if EW_USE_DOUBLE_BUFFER == 0

    /* in case of single buffer, the DSI update has to be completed - otherwise
       content of framefuffer will be overwritten */
    WaitForDsiTransfer();

  #endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplayCommitBuffer
*
* DESCRIPTION:
*   The function EwBspDisplayCommitBuffer is called from the Graphics Engine
*   when the rendering of a certain buffer has been completed.
*   The type of buffer depends on the selected framebuffer concept.
*   If the display is running in a double-buffering mode, the function is called
*   after each buffer update in order to change the currently active framebuffer
*   address. Changing the framebuffer address should be synchronized with V-sync.
*   If the system is using an external graphics controller, this function is
*   responsible to start the transfer of the framebuffer content.
*
* ARGUMENTS:
*   aAddress - Address of the framebuffer to be shown on the display.
*   aX,
*   aY       - Origin of the area which has been updated by the Graphics Engine.
*   aWidth,
*   aHeight  - Size of the area which has been updated by the Graphics Engine.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplayCommitBuffer( void* aAddress, int aX, int aY, int aWidth, int aHeight )
{
  /* ensure that previous LCD update is finished before starting next transfer */
  WaitForDsiTransfer();

  /* prepare the update of the left display area */
  ActiveArea = LEFT_AREA;

  /* change address within layer configuration structure */
  LayerConfig.FBStartAdress = (uint32_t)aAddress;

  /* change layer configuration */
  if ( HAL_LTDC_ConfigLayer( &hltdc_handle, &LayerConfig, LAYER_INDEX ) != HAL_OK )
    EwPrint( "EwBspUpdateDisplay: Could not change layer configuration!\n" );

  /* Disable DSI Wrapper */
  __HAL_DSI_WRAPPER_DISABLE( &hdsi_handle );

  /* Update LTDC configuaration */
  LTDC_LAYER( &hltdc_handle, LAYER_INDEX )->CFBAR = LayerConfig.FBStartAdress ;
  __HAL_LTDC_RELOAD_CONFIG( &hltdc_handle );

  /* Enable DSI Wrapper */
  __HAL_DSI_WRAPPER_ENABLE( &hdsi_handle );

  HAL_DSI_LongWrite( &hdsi_handle, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft );

  /* start the update of the LCD via DSI */
  __DSI_UNMASK_TE();

  #if EW_USE_DOUBLE_BUFFER == 1

    /* reserve the memory bandwidth for the DSI update */
    EwBspGraphicsConcurrentOperation( 0 );

  #endif

  /* switch-on the backlight after first framebuffer update */
  if ( !FirstUpdateIsDone )
  {
    WaitForDsiTransfer();
    LCD_DisplayOn();
    FirstUpdateIsDone = 1;
  }
}


/*******************************************************************************
* FUNCTION:
*   EwBspDisplaySetClut
*
* DESCRIPTION:
*   The function EwBspDisplaySetClut is called from the Graphics Engine
*   in order to update the hardware CLUT of the current framebuffer.
*   The function is only called when the color format of the framebuffer is
*   Index8 or LumA44.
*
* ARGUMENTS:
*   aClut - Pointer to a color lookup table with 256 entries.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspDisplaySetClut( unsigned long* aClut )
{
  __HAL_DSI_WRAPPER_DISABLE( &hdsi_handle );

  /* short delay necessary to ensure proper DSI update... */
  HAL_Delay( 100 );

  if ( HAL_LTDC_ConfigCLUT( &hltdc_handle, (uint32_t*)aClut, 256, LAYER_INDEX ) != HAL_OK )
    EwPrint( "EwBspDisplaySetClut: Could not configure layer CLUT!\n" );
  if ( HAL_LTDC_EnableCLUT( &hltdc_handle, LAYER_INDEX ) != HAL_OK )
    EwPrint( "EwBspDisplaySetClut: Could not enable layer CLUT!\n" );

  /* short delay necessary to ensure proper DSI update... */
  HAL_Delay( 100 );

  __HAL_DSI_WRAPPER_ENABLE( &hdsi_handle );

  /* short delay necessary to ensure proper DSI update... */
  HAL_Delay( 100 );
}


/* msy */
