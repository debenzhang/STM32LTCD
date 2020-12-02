/*
 * USBDisk.c
 *
 *  Created on: Dec 2, 2020
 *      Author: laoxi
 */

#include "USBDisk.h"
#include <stdio.h>

void MSC_Application(void) {
    FRESULT res;                             /* FatFs function common result code */
    uint32_t byteswrite;
    uint8_t wtext[] = "hello world!";

    /* Register the file system object to the FatFs module */
    if(f_mount(&USBDISKFatFs, (TCHAR const*)USBHPath, 0) != FR_OK) {
        /* FatFs Initialization Error */
        Error_Handler();
    } else {
        /* Create and Open a new text file object with write access */
        if(f_open(&MyFile, "hello.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
            /* 'STM32.TXT' file Open for write Error */
            Error_Handler();
        } else {
            res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswrite);
            if(byteswrite == 0 || (res != FR_OK)) {
            Error_Handler();
            } else {
                /* Close the open text file */
                f_close(&MyFile);
                printf("write file done.");
            }
        }
    }
}

FRESULT Explore_Disk(char *path, uint8_t recu_level) {
  FRESULT res = FR_OK;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[14];

  res = f_opendir(&dir, path);
  printf("res1: %d \r\n", res);

  for (;;) {
	  res = f_readdir(&dir, &fno);                   /* Read a directory item */
	  if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//	  if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//		  i = strlen(path);
//		  sprintf(&path[i], "/%s", fno.fname);
//		  res = scan_files(path);                    /* Enter the directory */
//		  if (res != FR_OK) break;
//		  path[i] = 0;
//	  } else {                                       /* It is a file. */
//		  printf("%s/%s\n", path, fno.fname);
//	  }
	  printf("%s/%s\n", path, fno.fname);
  }
  f_closedir(&dir);
  return res;
}

void writefile_test(void) {
    FIL fil;
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, "0:/mytest.txt", FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        return;
    }
    f_printf(&fil, "%s\n", "[USB]write txt OK!0123456789");

    /* Close the file */
    f_close(&fil);
}

void readfile_test(void) {
    FIL fil;
    FRESULT fr;
    uint8_t buff[20];
    UINT off = 0;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(&fil, "0:/mytest.txt", FA_READ);
    if (fr != FR_OK) {
        return;
    }

    f_read(&fil, buff, 16, &off);
    buff[16] = 0;

    /* Close the file */
    f_close(&fil);
}
