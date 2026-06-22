/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/USB_Device/App/usb_device.c
  * @author  MCD Application Team
  * @brief   This file implements the USB Device
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

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_composite_builder.h"
#include "usbd_cdc_if.h"

#include "main.h"

/* Private variables ---------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB1_OTG_HS;

uint8_t HID_Keyboard_Buffer[8];

/* Classes ID */
uint8_t CDC_InstID, HID_InstID = 0;

/* HID Endpoint Address */
uint8_t HID_EpAdress = HID_EPIN_ADDR;
uint8_t CDC_EpAdd_Inst1[3] = {CDC_IN_EP, CDC_OUT_EP, CDC_CMD_EP}; /* CDC Endpoint Address First Instance */

/* Private function prototypes -----------------------------------------------*/
extern void SystemClockConfig_Resume(void);

/* USB Device Core handle declaration. */
USBD_HandleTypeDef hUsbDeviceHS;

extern USBD_DescriptorsTypeDef HID_Desc;

/*
 * -- Insert your variables declaration here --
 */
/*
 * -- Insert your external function declaration here --
 */


/**
 * @breif Depress only the keys given in the key_buff array
 * @param key_buff Array of key codes
 * @param len Length of key_buff (must not exceed 6 keys)
 * @retval None
 */
void USB_Device_DepressOnly(uint8_t *key_buff, uint8_t len)
{
  uint8_t i, j;
  uint8_t flag_remove;
  uint8_t flag_update;

  if (len < 0)
  {
    return;
  }

  /* 6 keys may be specified at the most */
  if (len > 6)
  {
    len = 6;
  }

  flag_update = 0;

  /* remove currently depressed keys not in the list and un-list any keys currently depressed*/
  for (j = 2; j < 8; j++)
  {
    if (HID_Keyboard_Buffer[j] != KEY_NONE)
    {
      flag_remove = 1;
      for (i = 0; i < len; i++)
      {
        if (HID_Keyboard_Buffer[j] == key_buff[i])
        {
          key_buff[i] = KEY_NONE;
          flag_remove = 0;
          // don't break to handle duplicate items in list
        }
      }
      if (flag_remove == 1)
      {
        HID_Keyboard_Buffer[j] = KEY_NONE;
        flag_update = 1;
      }
    }
  }

  /* Now there's no overlap. Add remaining item in list to be depressed. */
  for (i = 0; i < len; i++)
  {
    if (key_buff[i] != KEY_NONE)
    {
      for (j = 2; j < 8; j++)
      {
        if (HID_Keyboard_Buffer[j] == KEY_NONE)
        {
          HID_Keyboard_Buffer[j] = key_buff[i];
          flag_update = 1;
          break;
        }
      }
    }
  }

  if (flag_update == 1)
  {
    USBD_HID_SendReport(&hUsbDeviceHS, HID_Keyboard_Buffer, 8, HID_InstID);
  }
}


/**
  * Init USB device Library, add supported classes and start the library
  * @retval None
  */
void USB_Device_Init(void)
{
  /* Init Device Library */
  if (USBD_Init(&hUsbDeviceHS, &CMPST_Desc, DEVICE_HS) != USBD_OK)
  {
    assert(0);
  }

  /* Store HID instance Class ID */
  HID_InstID = hUsbDeviceHS.classId;

  /* Register the HID  class */
  if (USBD_RegisterClassComposite(&hUsbDeviceHS, USBD_HID_CLASS, CLASS_TYPE_HID, &HID_EpAdress) != USBD_OK)
  {
    assert(0);
  }

  /* Store CDC instance Class ID */
  CDC_InstID = hUsbDeviceHS.classId;

  /* Register CDC class  */
  if (USBD_RegisterClassComposite(&hUsbDeviceHS, USBD_CDC_CLASS, CLASS_TYPE_CDC, CDC_EpAdd_Inst1) != USBD_OK)
  {
    assert(0);
  }

  /* Add CDC Interface */
  if (USBD_CMPSIT_SetClassID(&hUsbDeviceHS, CLASS_TYPE_CDC, 0) != 0xFF)
  {
    if (USBD_CDC_RegisterInterface(&hUsbDeviceHS, &USBD_CDC_fops) != USBD_OK)
    {
      assert(0);
    }
  }

  if (USBD_Start(&hUsbDeviceHS) != USBD_OK)
  {
    assert(0);
  }

  memset(HID_Keyboard_Buffer, 0x00, sizeof(HID_Keyboard_Buffer));
}

/**
  * @}
  */

/**
  * @}
  */

