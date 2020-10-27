/*
 * sdram_fmc_drv.c
 *
 *  Created on: Oct 27, 2020
 *      Author: laoxi
 */
#include "main.h"
#include "sdram_fmc_drv.h"

static int SDRAM_SendCommand(uint32_t CommandMode, uint32_t Bank, uint32_t RefreshNum, uint32_t RegVal)
{
    uint32_t CommandTarget;
    FMC_SDRAM_CommandTypeDef Command;

    if (Bank == 1) {
        CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    } else if (Bank == 2) {
        CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
    }

    Command.CommandMode = CommandMode;
    Command.CommandTarget = CommandTarget;
    Command.AutoRefreshNumber = RefreshNum;
    Command.ModeRegisterDefinition = RegVal;

    if (HAL_SDRAM_SendCommand(&hsdram1, &Command, 0x1000) != HAL_OK) {
        return -1;
    }

    return 0;
}

void SDRAM_Init(void)
{
    uint32_t temp;

    /* 1. 时钟使能命令 */
    SDRAM_SendCommand(FMC_SDRAM_CMD_CLK_ENABLE, 2, 1, 0);

    /* 2. 延时，至少100us */
    HAL_Delay(1);

    /* 3. SDRAM全部预充电命令 */
    SDRAM_SendCommand(FMC_SDRAM_CMD_PALL, 2, 1, 0);

    /* 4. 自动刷新命令 */
    SDRAM_SendCommand(FMC_SDRAM_CMD_AUTOREFRESH_MODE, 2, 8, 0);

    /* 5. 配置SDRAM模式寄存器 */
    temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1            |          //设置突发长度：1
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL     |          //设置突发类型：连续
                     SDRAM_MODEREG_CAS_LATENCY_3             |          //设置CL值：3
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD   |          //设置操作模式：标准
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;              //设置突发写模式：单点访问
    SDRAM_SendCommand(FMC_SDRAM_CMD_LOAD_MODE, 2, 1, temp);

    /* 6. 设置自刷新频率 */
    /*
        SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
      = 64000(64 ms) / 4096 *108MHz - 20
      = 1667.5 取值1668
    */
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, 1668);
}
