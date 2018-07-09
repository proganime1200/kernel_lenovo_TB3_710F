#ifndef MT6582_AUTOK_H
#define MT6582_AUTOK_H

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/sdio_func.h>

#include "mt_sd.h"

#define AUTOK_READ 0
#define AUTOK_WRITE 1

#define PROC_BUF_SIZE 512

/*************************************************************************
*           AutoK Implementation
*************************************************************************/
//#define AUTOK_DEBUG
#define USE_KERNEL_THREAD
//#define CHANGE_SCHED_POLICY
//#define SCHED_POLICY_INFO
#define PMIC_MT6323

/*TODO: just remove those def and include the correct one header*/
#define SDIO_IP_WTMDR       	(0x00B0)
#define SDIO_IP_WTMCR       	(0x00B4)
#define SDIO_IP_WTMDPCR0    	(0x00B8)
#define SDIO_IP_WTMDPCR1    	(0x00BC)
#define SDIO_IP_WPLRCR      	(0x00D4)
#define TEST_MODE_STATUS        (0x100)

struct sdio_autok_params
{
    u32 cmd_edge;
    u32 rdata_edge;
    u32 wdata_edge;
    u32 clk_drv;
    u32 cmd_drv;
    u32 dat_drv;
    u32 dat0_rd_dly;
    u32 dat1_rd_dly;
    u32 dat2_rd_dly;
    u32 dat3_rd_dly;
    u32 dat_wrd_dly;
    u32 cmd_resp_rd_dly;
    u32 cmd_rd_dly;
    u32 int_dat_latch_ck;
    u32 ckgen_msdc_dly_sel;
    u32 cmd_rsp_ta_cntr;
    u32 wrdat_crcs_ta_cntr;
    u32 pad_clk_txdly;
};

#ifdef USE_KERNEL_THREAD
struct sdio_autok_thread_data
{
	struct msdc_host *host;
	struct sdio_func *sdioFunc;
	char autok_stage1_result[PROC_BUF_SIZE];
	int len;
	char stage;
};
#else   // USE_KERNEL_THREAD
struct sdio_autok_workqueue_data
{
	struct delayed_work autok_delayed_work;
	struct msdc_host *host;
	char autok_stage1_result[PROC_BUF_SIZE];
	int len;
	char stage;
};
#endif  // USE_KERNEL_THREAD

#define LTE_MODEM_FUNC (1)
#define CMD_52         (52)
#define CMD_53         (53)

#define REQ_CMD_EIO    (0x1 << 0)
#define REQ_CMD_TMO    (0x1 << 1)
#define REQ_DAT_ERR    (0x1 << 2)

#define MSDC_READ      (0)
#define MSDC_WRITE     (1)

enum AUTOK_PARAM {
    CMD_EDGE,                       // command response sample selection (MSDC_SMPL_RISING, MSDC_SMPL_FALLING)
    RDATA_EDGE,                     // read data sample selection (MSDC_SMPL_RISING, MSDC_SMPL_FALLING)
    WDATA_EDGE,                    // write data sample selection (MSDC_SMPL_RISING, MSDC_SMPL_FALLING)
    CLK_DRV,                            // clock driving
    CMD_DRV,                          // command driving
    DAT_DRV,                           // data driving
    DAT0_RD_DLY,                   // DAT0 Pad RX Delay Line Control (for MSDC RD), Total 32 stages
    DAT1_RD_DLY,                   // DAT1 Pad RX Delay Line Control (for MSDC RD), Total 32 stages
    DAT2_RD_DLY,                   // DAT2 Pad RX Delay Line Control (for MSDC RD), Total 32 stages
    DAT3_RD_DLY,                   // DAT3 Pad RX Delay Line Control (for MSDC RD), Total 32 stages
    DAT_WRD_DLY,                  // Write Data Status Internal Delay Line Control. This register is used to fine-tune write status phase latched by MSDC internal clock. Total 32 stages
    DAT_RD_DLY,                  // Rx  Delay Line Control. Total 32 stages
    CMD_RESP_RD_DLY,          // CMD Response Internal Delay Line Control. This register is used to fine-tune response phase  latched by MSDC internal clock. Total 32 stages
    CMD_RD_DLY,                    // CMD Pad RX Delay Line Control. This register is used to fine-tune CMD pad macro respose latch timing. Total 32 stages
    DATA_DLYLINE_SEL,           // Data line delay line fine tune selection. 1'b0: All data line share one delay selection value indicated by PAD_TUNE.PAD_DAT_RD_RXDLY. 1'b1: Each data line has its own delay selection value indicated by Data line (x): DAT_RD_DLY(x).DAT0_RD_DLY
    READ_DATA_SMPL_SEL,     // Data line rising/falling latch  fine tune selection in read transaction. 1'b0: All data line share one value indicated by MSDC_IOCON.R_D_SMPL. 1'b1: Each data line has its own  selection value indicated by Data line (x): MSDC_IOCON.R_D(x)_SMPL
    WRITE_DATA_SMPL_SEL,   // Data line rising/falling latch  fine tune selection in write transaction. 1'b0: All data line share one value indicated by MSDC_IOCON.W_D_SMPL. 1'b1: Each data line has its own  selection value indicated by Data line (x): MSDC_IOCON.W_D(x)_SMPL
    INT_DAT_LATCH_CK,          // Internal MSDC clock phase selection. Total 8 stages, each stage can delay 1 clock period of msdc_src_ck
    CKGEN_MSDC_DLY_SEL,    // CKBUF in CKGEN Delay Selection. Total 32 stages
    CMD_RSP_TA_CNTR,          // CMD response turn around period. The turn around cycle = CMD_RSP_TA_CNTR + 2, Only for USH104 mode, this register should be set to 0 in non-UHS104 mode
    WRDAT_CRCS_TA_CNTR,   // Write data and CRC status turn around period. The turn around cycle = WRDAT_CRCS_TA_CNTR + 2, Only for USH104 mode,  this register should be set to 0 in non-UHS104 mode
    PAD_CLK_TXDLY,                // CLK Pad TX Delay Control. This register is used to add delay to CLK phase. Total 32 stages
    TOTAL_PARAM_COUNT
};

int msdc_autok_read(struct msdc_host *host, unsigned int u4Addr, unsigned int u4Func, void *pBuffer, unsigned int u4Len, unsigned int u4Cmd);
int msdc_autok_write(struct msdc_host *host, unsigned int u4Addr, unsigned int u4Func, void *pBuffer, unsigned int u4Len, unsigned int u4Cmd);
int msdc_autok_adjust_param(struct msdc_host *host, enum AUTOK_PARAM param, u32 *value, int rw);
int msdc_autok_stg1_cal(struct msdc_host *host, unsigned int offset_restore);
int msdc_autok_stg1_data_get(void **ppData, int *pLen);
int msdc_autok_stg2_cal(struct msdc_host *host, void *pData, int len, unsigned int vcore_uv);

int msdc_autok_apply_param(struct msdc_host *host, void *pData, int len, unsigned int vcore_uv);
#endif /* end of MT6582_AUTOK_H */
