/*
 * USBDisk.h
 *
 *  Created on: Dec 2, 2020
 *      Author: laoxi
 */

#ifndef INC_USBDISK_H_
#define INC_USBDISK_H_

#include "stm32f4xx.h"
#include "fatfs.h"
#include "usbh_core.h"
#include "usb_host.h"

ApplicationTypeDef Appli_state;
USBH_HandleTypeDef hUsbHostFS;
char USBHPath[4];  				/* USBH logical drive path */
FATFS USBDISKFatFs;           	/* File system object for USB disk logical drive */
FIL   MyFile;                 	/* File object */


void MSC_Application(void);

FRESULT Explore_Disk(char *path, uint8_t recu_level);
void writefile_test(void);
void readfile_test(void);

#endif /* INC_USBDISK_H_ */
