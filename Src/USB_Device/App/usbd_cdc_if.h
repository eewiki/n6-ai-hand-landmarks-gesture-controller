/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/USB_Device/App/usbd_cdc_if.h
  * @author  MCD Application Team
  * @brief   Header for usbd_cdc_interface.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H
#define __USBD_CDC_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
typedef enum CDC_App_State
{
  CDC_APP_STATE_MENU,
  CDC_APP_STATE_GAME1,
  CDC_APP_STATE_GAME2,
  CDC_APP_STATE_MAX
} CDC_App_State_t;


extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;
extern volatile CDC_App_State_t      ext_app_state;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t CDC_Itf_Transmit(uint8_t* Buf, uint16_t Len);

#endif /* __USBD_CDC_IF_H */

