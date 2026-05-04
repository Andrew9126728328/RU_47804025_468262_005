/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f415_wk_config.h
  * @brief    header file of work bench config
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* define to prevent recursive inclusion -----------------------------------*/
#ifndef __AT32F415_WK_CONFIG_H
#define __AT32F415_WK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes -----------------------------------------------------------------------*/
#include "stdio.h"
#include "at32f415.h"

/* private includes -------------------------------------------------------------*/
/* add user code begin private includes */
#include "ai_appl.h"
/* add user code end private includes */

/* exported types -------------------------------------------------------------*/
/* add user code begin exported types */

/* add user code end exported types */

/* exported constants --------------------------------------------------------*/
/* add user code begin exported constants */
extern uint16_t adc_raw[AI_TOTAL_CHANNELS]; 
/* add user code end exported constants */

/* exported macro ------------------------------------------------------------*/
/* add user code begin exported macro */

/* add user code end exported macro */

/* add user code begin dma define */
/* user can only modify the dma define value */
#define DMA1_CHANNEL1_BUFFER_SIZE   (sizeof(adc_raw)/sizeof(adc_raw[0]))
#define DMA1_CHANNEL1_MEMORY_BASE_ADDR   ((uint32_t)adc_raw)
//#define DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR  0

//#define DMA1_CHANNEL2_BUFFER_SIZE   0
//#define DMA1_CHANNEL2_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL2_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL3_BUFFER_SIZE   0
//#define DMA1_CHANNEL3_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL3_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL4_BUFFER_SIZE   0
//#define DMA1_CHANNEL4_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL4_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL5_BUFFER_SIZE   0
//#define DMA1_CHANNEL5_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL5_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL6_BUFFER_SIZE   0
//#define DMA1_CHANNEL6_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL6_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL7_BUFFER_SIZE   0
//#define DMA1_CHANNEL7_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL7_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL1_BUFFER_SIZE   0
//#define DMA2_CHANNEL1_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL1_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL2_BUFFER_SIZE   0
//#define DMA2_CHANNEL2_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL2_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL3_BUFFER_SIZE   0
//#define DMA2_CHANNEL3_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL3_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL4_BUFFER_SIZE   0
//#define DMA2_CHANNEL4_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL4_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL5_BUFFER_SIZE   0
//#define DMA2_CHANNEL5_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL5_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL6_BUFFER_SIZE   0
//#define DMA2_CHANNEL6_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL6_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL7_BUFFER_SIZE   0
//#define DMA2_CHANNEL7_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL7_PERIPHERAL_BASE_ADDR   0
/* add user code end dma define */

/* Private defines -------------------------------------------------------------*/
#define COL_3_PIN    GPIO_PINS_13
#define COL_3_GPIO_PORT    GPIOC
#define HEXT_IN_PIN    GPIO_PINS_0
#define HEXT_IN_GPIO_PORT    GPIOD
#define HEXT_OUT_PIN    GPIO_PINS_1
#define HEXT_OUT_GPIO_PORT    GPIOD
#define PWRvs_PIN    GPIO_PINS_0
#define PWRvs_GPIO_PORT    GPIOA
#define COL_5_PIN    GPIO_PINS_1
#define COL_5_GPIO_PORT    GPIOA
#define ROW_5_PIN    GPIO_PINS_2
#define ROW_5_GPIO_PORT    GPIOA
#define ROW_4_PIN    GPIO_PINS_3
#define ROW_4_GPIO_PORT    GPIOA
#define ROW_3_PIN    GPIO_PINS_4
#define ROW_3_GPIO_PORT    GPIOA
#define COL_4_PIN    GPIO_PINS_5
#define COL_4_GPIO_PORT    GPIOA
#define ENC_A_PIN    GPIO_PINS_6
#define ENC_A_GPIO_PORT    GPIOA
#define ENC_B_PIN    GPIO_PINS_7
#define ENC_B_GPIO_PORT    GPIOA
#define VIBR_EN_PIN    GPIO_PINS_1
#define VIBR_EN_GPIO_PORT    GPIOB
#define COL_1_PIN    GPIO_PINS_2
#define COL_1_GPIO_PORT    GPIOB
#define VIBR_SCL_PIN    GPIO_PINS_10
#define VIBR_SCL_GPIO_PORT    GPIOB
#define VIBR_SDA_PIN    GPIO_PINS_11
#define VIBR_SDA_GPIO_PORT    GPIOB
#define PB_12_PIN    GPIO_PINS_12
#define PB_12_GPIO_PORT    GPIOB
#define VIBR_IN_PIN    GPIO_PINS_14
#define VIBR_IN_GPIO_PORT    GPIOB
#define M_TX_D_PIN    GPIO_PINS_9
#define M_TX_D_GPIO_PORT    GPIOA
#define M_RX_D_PIN    GPIO_PINS_10
#define M_RX_D_GPIO_PORT    GPIOA
#define COL_6_PIN    GPIO_PINS_11
#define COL_6_GPIO_PORT    GPIOA
#define M_SWDIO_PIN    GPIO_PINS_13
#define M_SWDIO_GPIO_PORT    GPIOA
#define M_SWCLK_PIN    GPIO_PINS_14
#define M_SWCLK_GPIO_PORT    GPIOA
#define PA_15_PIN    GPIO_PINS_15
#define PA_15_GPIO_PORT    GPIOA
#define M_SWO_PIN    GPIO_PINS_3
#define M_SWO_GPIO_PORT    GPIOB
#define PB_4_PIN    GPIO_PINS_4
#define PB_4_GPIO_PORT    GPIOB
#define ROW_2_PIN    GPIO_PINS_5
#define ROW_2_GPIO_PORT    GPIOB
#define ROW_1_PIN    GPIO_PINS_6
#define ROW_1_GPIO_PORT    GPIOB
#define COL_2_PIN    GPIO_PINS_7
#define COL_2_GPIO_PORT    GPIOB

/* exported functions ------------------------------------------------------- */
  /* system clock config. */
  void wk_system_clock_config(void);

  /* config periph clock. */
  void wk_periph_clock_config(void);

  /* nvic config. */
  void wk_nvic_config(void);

/* add user code begin exported functions */

/* add user code end exported functions */

#ifdef __cplusplus
}
#endif

#endif
