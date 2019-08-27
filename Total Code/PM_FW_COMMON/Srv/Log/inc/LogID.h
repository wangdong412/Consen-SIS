/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*
*
*  项目名称    : 安全控制系统
*
*  模块名称    :
*
*  文件名称    : LogID.h
*
*  功能简介    :
*
*  作者            : 李琦
*
*  创建日期    : 2016-10-19
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

#ifndef LOGID_H_
#define LOGID_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include "inc/DataType.h"
#include "inc/Hardware.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

/*
*  前10位基址在该文件定义，后22位在其它.h文件中定义 (注意： 按照图中的格式对后22位进行使用)
* ______________________________________________________________________________
* |      log ID 格式规划                                                                                                                                                                                |
* |_____________________________________________________________________________|
* |  主模块号（8bit）                |错误等级 （2bit）|文件号（6bit） |                 错误号 （16bit）       |
* |-----------------------------------------------------------------------------|
* | COMMON: 0x00~0x3F | 0-致命   1-严重    |           |                               |
* | CORE0: 0x40~0x7F  | 2-一般   3-提示    |           |                               |
* | CORE1: 0x80~0xDF  |             |           |                               |
* | USER CORE0: 0xE0  |             |           |                               |
* | USER CORE1: OxF0  |             |           |                               |
* |___________________|_____________|___________|_______________________________|
*/

/* ------------CORE0 AND CORE1 COMMON LOG ID---------------------------------------------------------------*/
#define LOG_ID_COMMON_BASE                                              (0x00000000U)

#define LOG_ID_SRV_ALGRITHM_BASE                                        (0x00000000U)
    #define LOG_ID_SRV_ALGRITHM_LEVEL0                                  (0x00000000U)
    #define LOG_ID_SRV_ALGRITHM_LEVEL1                                  (0x00400000U)
    #define LOG_ID_SRV_ALGRITHM_LEVEL2                                  (0x00800000U)
    #define LOG_ID_SRV_ALGRITHM_LEVEL3                                  (0x00C00000U)

#define LOG_ID_SRV_DIAG_BASE                                            (0x01000000U)
    #define LOG_ID_SRV_DIAG_LEVEL0                                      (0x01000000U)
    #define LOG_ID_SRV_DIAG_LEVEL1                                      (0x01400000U)
    #define LOG_ID_SRV_DIAG_LEVEL2                                      (0x01800000U)
    #define LOG_ID_SRV_DIAG_LEVEL3                                      (0x01C00000U)

#define LOG_ID_SRV_IPBUS_BASE                                           (0x02000000U)
    #define LOG_ID_SRV_IPBUS_LEVEL0                                     (0x02000000U)
    #define LOG_ID_SRV_IPBUS_LEVEL1                                     (0x02400000U)
    #define LOG_ID_SRV_IPBUS_LEVEL2                                     (0x02800000U)
    #define LOG_ID_SRV_IPBUS_LEVEL3                                     (0x02C00000U)

#define LOG_ID_SRV_LOG_BASE                                             (0x03000000U)
    #define LOG_ID_SRV_LOG_LEVEL0                                       (0x03000000U)
    #define LOG_ID_SRV_LOG_LEVEL1                                       (0x03400000U)
    #define LOG_ID_SRV_LOG_LEVEL2                                       (0x03800000U)
    #define LOG_ID_SRV_LOG_LEVEL3                                       (0x03C00000U)

#define LOG_ID_SRV_PMBUS_BASE                                           (0x04000000U)
    #define LOG_ID_SRV_PMBUS_LEVEL0                                     (0x04000000U)
    #define LOG_ID_SRV_PMBUS_LEVEL1                                     (0x04400000U)
    #define LOG_ID_SRV_PMBUS_LEVEL2                                     (0x04800000U)
    #define LOG_ID_SRV_PMBUS_LEVEL3                                     (0x04C00000U)

#define LOG_ID_SRV_SAFTYPROTOCOL_BASE                                   (0x05000000U)
    #define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL0                             (0x05000000U)
    #define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL1                             (0x05400000U)
    #define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL2                             (0x05800000U)
    #define LOG_ID_SRV_SAFTYPROTOCOL_LEVEL3                             (0x05C00000U)

#define LOG_ID_SRV_OS_BASE                                              (0x06000000U)
    #define LOG_ID_SRV_OS_LEVEL0                                        (0x06000000U)
    #define LOG_ID_SRV_OS_LEVEL1                                        (0x06400000U)
    #define LOG_ID_SRV_OS_LEVEL2                                        (0x06800000U)
    #define LOG_ID_SRV_OS_LEVEL3                                        (0x06C00000U)

#define LOG_ID_SRV_FS_BASE                                              (0x07000000U)
    #define LOG_ID_SRV_FS_LEVEL0                                        (0x07000000U)
    #define LOG_ID_SRV_FS_LEVEL1                                        (0x07400000U)
    #define LOG_ID_SRV_FS_LEVEL2                                        (0x07800000U)
    #define LOG_ID_SRV_FS_LEVEL3                                        (0x07C00000U)

#define LOG_ID_SRV_SHAREDMEM_BASE                                       (0x08000000U)
    #define LOG_ID_SRV_SHAREDMEM_LEVEL0                                 (0x08000000U)
    #define LOG_ID_SRV_SHAREDMEM_LEVEL1                                 (0x08400000U)
    #define LOG_ID_SRV_SHAREDMEM_LEVEL2                                 (0x08800000U)
    #define LOG_ID_SRV_SHAREDMEM_LEVEL3                                 (0x08C00000U)

#define LOG_ID_SRV_SYS_BASE                                             (0x09000000U)
    #define LOG_ID_SRV_SYS_LEVEL0                                       (0x09000000U)
    #define LOG_ID_SRV_SYS_LEVEL1                                       (0x09400000U)
    #define LOG_ID_SRV_SYS_LEVEL2                                       (0x09800000U)
    #define LOG_ID_SRV_SYS_LEVEL3                                       (0x09C00000U)

#define LOG_ID_SRV_VER_BASE                                             (0x0A000000U)
    #define LOG_ID_SRV_VER_LEVEL0                                       (0x0A000000U)
    #define LOG_ID_SRV_VER_LEVEL1                                       (0x0A400000U)
    #define LOG_ID_SRV_VER_LEVEL2                                       (0x0A800000U)
    #define LOG_ID_SRV_VER_LEVEL3                                       (0x0AC00000U)

/* ------------CORE0 LOG ID--------------------------------------------------------------------------------*/
#define LOG0_ID_CORE0_BASE                                              (0x40000000U)

#define LOG0_ID_APP_DATA_PROCESS_BASE                                   (0x40000000U)

#define LOG0_ID_APP_DATA_PROCESS_LEVEL0                                 (0x40000000U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL0_NO_RIGHT                        (0x40000000U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL0_INPUTDATA_TO                    (0x40000001U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL0_NO_IOCONFIG                     (0x40000002U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL0_INPUTDATA_ERR1                  (0x40000003U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL0_INPUTDATA_ERR2                  (0x40000004U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL0_SAFETYDATA_ERR                  (0x40000005U)

#define LOG0_ID_APP_DATA_PROCESS_LEVEL1                                 (0x40400000U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL2                                 (0x40800000U)
#define LOG0_ID_APP_DATA_PROCESS_LEVEL3                                 (0x40C00000U)

#define LOG0_ID_APP_DIAG_BASE                                           (0x41000000U)
#define LOG0_ID_APP_DIAG_LEVEL0                                         (0x41000000U)
#define LOG0_ID_APP_DIAG_LEVEL1                                         (0x41400000U)
#define LOG0_ID_APP_DIAG_LEVEL2                                         (0x41800000U)
#define LOG0_ID_APP_DIAG_LEVEL3                                         (0x41C00000U)

#define LOG0_ID_APP_MAIN_BASE                                           (0x42000000U)

#define LOG0_ID_APP_MAIN_LEVEL0                                         (0x42000000U)
#define LOG0_ID_APP_MAIN_LEVEL0_OUTPUT_TO                               (0x42000000U)
#define LOG0_ID_APP_MAIN_LEVEL0_MAIN_TO                                 (0x42000001U)
#define LOG0_ID_APP_MAIN_LEVEL0_INPUT_TO                                (0x42000002U)

#define LOG0_ID_APP_MAIN_LEVEL1                                         (0x42400000U)
#define LOG0_ID_APP_MAIN_LEVEL2                                         (0x42800000U)
#define LOG0_ID_APP_MAIN_LEVEL3                                         (0x42C00000U)

#define LOG0_ID_APP_MANAGER_BASE                                        (0x43000000U)
#define LOG0_ID_APP_MANAGER_LEVEL0                                      (0x43000000U)
#define LOG0_ID_APP_MANAGER_LEVEL1                                      (0x43400000U)
#define LOG0_ID_APP_MANAGER_LEVEL2                                      (0x43800000U)
#define LOG0_ID_APP_MANAGER_LEVEL3                                      (0x43C00000U)

#define LOG0_ID_APP_MONITOR_BASE                                        (0x44000000U)
#define LOG0_ID_APP_MONITOR_LEVEL0                                      (0x44000000U)
#define LOG0_ID_APP_MONITOR_LEVEL1                                      (0x44400000U)
#define LOG0_ID_APP_MONITOR_LEVEL2                                      (0x44800000U)
#define LOG0_ID_APP_MONITOR_LEVEL3                                      (0x44C00000U)

#define LOG0_ID_APP_RETAIN_DATA_BASE                                    (0x45000000U)
#define LOG0_ID_APP_RETAIN_DATA_LEVEL0                                  (0x45000000U)
#define LOG0_ID_APP_RETAIN_DATA_LEVEL1                                  (0x45400000U)
#define LOG0_ID_APP_RETAIN_DATA_LEVEL2                                  (0x45800000U)
#define LOG0_ID_APP_RETAIN_DATA_LEVEL3                                  (0x45C00000U)

#define LOG0_ID_APP_SOFT_SOE_BASE                                       (0x46000000U)
#define LOG0_ID_APP_SOFT_SOE_LEVEL0                                     (0x46000000U)
#define LOG0_ID_APP_SOFT_SOE_LEVEL1                                     (0x46400000U)
#define LOG0_ID_APP_SOFT_SOE_LEVEL2                                     (0x46800000U)
#define LOG0_ID_APP_SOFT_SOE_LEVEL3                                     (0x46C00000U)

#define LOG0_ID_SRV_EXTLIB_BASE                                         (0x60000000U)
#define LOG0_ID_SRV_EXTLIB_LEVEL0                                       (0x60000000U)
#define LOG0_ID_SRV_EXTLIB_LEVEL1                                       (0x60400000U)
#define LOG0_ID_SRV_EXTLIB_LEVEL2                                       (0x60800000U)
#define LOG0_ID_SRV_EXTLIB_LEVEL3                                       (0x60C00000U)

#define LOG0_ID_SRV_SYS_RESOURCE_BASE                                   (0x61000000U)
#define LOG0_ID_SRV_SYS_RESOURCE_LEVEL0                                 (0x61000000U)
#define LOG0_ID_SRV_SYS_RESOURCE_LEVEL1                                 (0x61400000U)
#define LOG0_ID_SRV_SYS_RESOURCE_LEVEL2                                 (0x61800000U)
#define LOG0_ID_SRV_SYS_RESOURCE_LEVEL3                                 (0x61C00000U)

#define LOG0_ID_EXP_BASE                                                (0x71000000U)
#define LOG0_ID_EXP_LEVEL0                                              (0x71000000U)
#define LOG0_ID_EXP_LEVEL0_SPE                                          (0x71000000U)
#define LOG0_ID_EXP_LEVEL0_SPE1                                         (0x71000001U)
#define LOG0_ID_EXP_LEVEL0_SPE2                                         (0x71000002U)
#define LOG0_ID_EXP_LEVEL0_SPE3                                         (0x71000003U)

#define LOG0_ID_EXP_LEVEL1                                              (0x71400000U)
#define LOG0_ID_EXP_LEVEL2                                              (0x71800000U)
#define LOG0_ID_EXP_LEVEL3                                              (0x71C00000U)

/* ------------CORE1 LOG ID--------------------------------------------------------------------------------*/
#define LOG1_ID_CORE1_BASE                                              (0x80000000U)

#define LOG1_ID_APP_COMM_BASE                                           (0x80000000U)
#define LOG1_ID_APP_COMM_LEVEL0                                         (0x80000000U)
#define LOG1_ID_APP_COMM_LEVEL1                                         (0x80400000U)
#define LOG1_ID_APP_COMM_LEVEL2                                         (0x80800000U)
#define LOG1_ID_APP_COMM_LEVEL3                                         (0x80C00000U)

#define LOG1_ID_APP_DIAG_BASE                                           (0x81000000U)
#define LOG1_ID_APP_DIAG_LEVEL0                                         (0x81000000U)
#define LOG1_ID_APP_DIAG_LEVEL1                                         (0x81400000U)
#define LOG1_ID_APP_DIAG_LEVEL2                                         (0x81800000U)
#define LOG1_ID_APP_DIAG_LEVEL3                                         (0x81C00000U)

#define LOG1_ID_APP_FILE_MANAGER_BASE                                   (0x82000000U)
#define LOG1_ID_APP_FILE_MANAGER_LEVEL0                                 (0x82000000U)
#define LOG1_ID_APP_FILE_MANAGER_LEVEL1                                 (0x82400000U)
#define LOG1_ID_APP_FILE_MANAGER_LEVEL2                                 (0x82800000U)
#define LOG1_ID_APP_FILE_MANAGER_LEVEL3                                 (0x82C00000U)

#define LOG1_ID_APP_LED_MANGER_BASE                                     (0x83000000U)
#define LOG1_ID_APP_LED_MANGER_LEVEL0                                   (0x83000000U)
#define LOG1_ID_APP_LED_MANGER_LEVEL1                                   (0x83400000U)
#define LOG1_ID_APP_LED_MANGER_LEVEL2                                   (0x83800000U)
#define LOG1_ID_APP_LED_MANGER_LEVEL3                                   (0x83C00000U)

#define LOG1_ID_APP_MAIN_BASE                                           (0x84000000U)
#define LOG1_ID_APP_MAIN_LEVEL0                                         (0x84000000U)
#define LOG1_ID_APP_MAIN_LEVEL1                                         (0x84400000U)
#define LOG1_ID_APP_MAIN_LEVEL2                                         (0x84800000U)
#define LOG1_ID_APP_MAIN_LEVEL3                                         (0x84C00000U)

#define LOG1_ID_APP_MANAGER_BASE                                        (0x85000000U)
#define LOG1_ID_APP_MANAGER_LEVEL0                                      (0x85000000U)
#define LOG1_ID_APP_MANAGER_LEVEL1                                      (0x85400000U)
#define LOG1_ID_APP_MANAGER_LEVEL2                                      (0x85800000U)
#define LOG1_ID_APP_MANAGER_LEVEL3                                      (0x85C00000U)

#define LOG1_ID_APP_MONITOR_BASE                                        (0x86000000U)
#define LOG1_ID_APP_MONITOR_LEVEL0                                      (0x86000000U)
#define LOG1_ID_APP_MONITOR_LEVEL1                                      (0x86400000U)
#define LOG1_ID_APP_MONITOR_LEVEL2                                      (0x86800000U)
#define LOG1_ID_APP_MONITOR_LEVEL3                                      (0x86C00000U)

#define LOG1_ID_APP_PROJECT_CONFIG_BASE                                 (0x87000000U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0                               (0x87000000U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_CHECK_CRC_ERR                 (0x87000000U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_CM_CFG_FAIL                   (0x87000001U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_MMC_CFG_FAIL                  (0x87000002U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_MSC_CFG_FAIL                  (0x87000003U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_SC_CFG_FAIL                   (0x87000004U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_SOE_CFG_FAIL                  (0x87000005U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_P2P_CFG_FAIL                  (0x87000006U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_VOT_CFG_FAIL                  (0x87000007U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_IOC_CFG_FAIL                  (0x87000008U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_RTN_CFG_FAIL                  (0x87000009U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL0_PRG_CFG_FAIL                  (0x8700000aU)



#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL1                               (0x87400000U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL2                               (0x87800000U)
#define LOG1_ID_APP_PROJECT_CONFIG_LEVEL3                               (0x87C00000U)

#define LOG1_ID_APP_SYNC_BASE                                           (0x88000000U)
#define LOG1_ID_APP_SYNC_LEVEL0                                         (0x88000000U)
#define LOG1_ID_APP_SYNC_LV0_SEND_NEXT_ERR                              (0x88000000U)
#define LOG1_ID_APP_SYNC_LV0_SEND_PRE_ERR                               (0x88000001U)
#define LOG1_ID_APP_SYNC_LV0_RECV_NEXT_NUM_ERR                          (0x88000002U)
#define LOG1_ID_APP_SYNC_LV0_RECV_PRE_NUM_ERR                           (0x88000003U)
#define LOG1_ID_APP_SYNC_LV0_RECV_NEXT_PMSTAT_ERR                       (0x88000004U)
#define LOG1_ID_APP_SYNC_LV0_RECV_PRE_PMSTAT_ERR                        (0x88000005U)
#define LOG1_ID_APP_SYNC_LV0_RECV_NEXT_LINK_ERR                         (0x88000006U)
#define LOG1_ID_APP_SYNC_LV0_RECV_PRE_LINK_ERR                          (0x88000007U)
/*收发文件*/
#define LOG1_ID_APP_SYNC_LV0_DDR_WR_FILE_ERR                            (0x88000100U)
#define LOG1_ID_APP_SYNC_LV0_RECV_NEW_FILE                              (0x88000101U)
#define LOG1_ID_APP_SYNC_LV0_RECV_SINGLE_FILE_END                       (0x88000102U)
#define LOG1_ID_APP_SYNC_LV0_RECV_FILE_END                              (0x88000103U)
#define LOG1_ID_APP_SYNC_LV0_FLASH_WR_FILE_START_ERR                    (0x88000104U)
#define LOG1_ID_APP_SYNC_LV0_FLASH_WR_FILE_CONT_ERR                     (0x88000105U)
#define LOG1_ID_APP_SYNC_LV0_FLASH_WR_FILE_END_ERR                      (0x88000106U)
#define LOG1_ID_APP_SYNC_LV0_SEND_NEW_FILE                              (0x88000107U)
#define LOG1_ID_APP_SYNC_LV0_DDR_RD_FILE_STAT_ERR                       (0x88000108U)
#define LOG1_ID_APP_SYNC_LV0_DDR_RD_FILE_ERR                            (0x88000109U)
#define LOG1_ID_APP_SYNC_LV0_SEND_SINGLE_FILE_END                       (0x8800010aU)
#define LOG1_ID_APP_SYNC_LV0_SEND_FILE_END                              (0x8800010bU)
#define LOG1_ID_APP_SYNC_LV0_RECV_ACK_TO                                (0x8800010cU)
#define LOG1_ID_APP_SYNC_LV0_RECV_ACK_TYPE_ERR                          (0x8800010dU)
#define LOG1_ID_APP_SYNC_LV0_RECV_ACK_ERR                               (0x8800010eU)
/*收发实时数据*/
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_AREA_ERR                         (0x88000200U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_G_LEN_ERR                        (0x88000201U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_I_LEN_ERR                        (0x88000202U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_Q_LEN_ERR                        (0x88000203U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_TASKID_ERR                       (0x88000204U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_G_AREA                           (0x88000205U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_I_AREA                           (0x88000206U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_Q_AREA                           (0x88000207U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_A_AREA                           (0x88000208U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_A_CRC_ERR                        (0x88000209U)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_A_LEN_ERR                        (0x8800020aU)
#define LOG1_ID_APP_SYNC_LV0_RECV_DATA_END                              (0x8800020bU)
#define LOG1_ID_APP_SYNC_LV0_SEND_DATA_G_AREA                           (0x8800020cU)
#define LOG1_ID_APP_SYNC_LV0_SEND_DATA_I_AREA                           (0x8800020dU)
#define LOG1_ID_APP_SYNC_LV0_SEND_DATA_Q_AREA                           (0x8800020eU)
#define LOG1_ID_APP_SYNC_LV0_SEND_DATA_A_AREA                           (0x8800020fU)
#define LOG1_ID_APP_SYNC_LV0_SEND_DATA_END                              (0x88000210U)
/*同步命令*/
#define LOG1_ID_APP_SYNC_LV0_BROAD_SRV_LEN_ERR                          (0x88000300U)
#define LOG1_ID_APP_SYNC_LV0_BROAD_SRV_ACK_OK                           (0x88000301U)
#define LOG1_ID_APP_SYNC_LV0_BROAD_SRV_NEXT_ACK_TO                      (0x88000302U)
#define LOG1_ID_APP_SYNC_LV0_BROAD_SRV_PRE_ACK_TO                       (0x88000303U)
#define LOG1_ID_APP_SYNC_LV0_BROAD_CTRL_SRV                             (0x88000304U)
#define LOG1_ID_APP_SYNC_LV0_RECV_CTRL_SRV                              (0x88000305U)



#define LOG1_ID_APP_SYNC_LEVEL1                                         (0x88400000U)
#define LOG1_ID_APP_SYNC_LEVEL2                                         (0x88800000U)
#define LOG1_ID_APP_SYNC_LEVEL3                                         (0x88C00000U)

#define LOG1_ID_APP_SYS_STATE_MNG_BASE                                  (0x89000000U)
#define LOG1_ID_APP_SYS_STATE_MNG_LEVEL0                                (0x89000000U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_CHANGED                      (0x89000000U)
/*系统状态：非稳态*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_1                            (0x89000001U)/*S_O_W*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_2                            (0x89000002U)/*S_W_O*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_3                            (0x89000003U)/*S_W_W*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_4                            (0x89000004U)/*W_O_S*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_5                            (0x89000005U)/*W_S_O*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_6                            (0x89000006U)/*W_S_W*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_7                            (0x89000007U)/*W_W_S*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_8                            (0x89000008U)/*S_S_W*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_9                            (0x89000009U)/*S_W_S*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_10                           (0x8900000aU)/*W_S_S*/
/*系统状态：稳态*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_11                           (0x8900000bU)/*S_O_O*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_12                           (0x8900000cU)/*S_O_S*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_13                           (0x8900000dU)/*S_S_O*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_14                           (0x8900000eU)/*S_S_S*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_15                           (0x8900000fU)/*W_O_O*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_16                           (0x89000010U)/*W_O_W*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_17                           (0x89000011U)/*W_W_O*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_STAT_18                           (0x89000012U)/*W_W_W*/
/*添加同步任务*/
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_SENDPRJ                           (0x89000100U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_RECVPRJ                           (0x89000101U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_UP1_SENDDATA                      (0x89000102U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_UP1_RECVDATA                      (0x89000103U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_UP2_SENDDATA                      (0x89000104U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_UP2_RECVDATA                      (0x89000105U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_SYNC_TO                           (0x89000106U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_DOWNLOAD_TO                       (0x89000107U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_PRJ_SWITCH                        (0x89000108U)
#define LOG1_ID_APP_SYS_STATE_MNG_LV0_IPBUS_DEV_CFG_ERR                 (0x89000109U)


#define LOG1_ID_APP_SYS_STATE_MNG_LEVEL1                                (0x89400000U)
#define LOG1_ID_APP_SYS_STATE_MNG_LEVEL2                                (0x89800000U)
#define LOG1_ID_APP_SYS_STATE_MNG_LEVEL3                                (0x89C00000U)

#define LOG1_ID_APP_UPDATER_BASE                                        (0x8A000000U)
#define LOG1_ID_APP_UPDATER_LEVEL0                                      (0x8A000000U)
#define LOG1_ID_APP_UPDATER_LEVEL1                                      (0x8A400000U)
#define LOG1_ID_APP_UPDATER_LEVEL2                                      (0x8A800000U)
#define LOG1_ID_APP_UPDATER_LEVEL3                                      (0x8AC00000U)

#define LOG1_ID_SRV_COMM_PROTOCOL_BASE                                  (0xA0000000U)
#define LOG1_ID_SRV_COMM_PROTOCOL_LEVEL0                                (0xA0000000U)
#define LOG1_ID_SRV_COMM_PRO_L0_REV_ERR                                 (0xA0000000U)

#define LOG1_ID_SRV_COMM_PROTOCOL_LEVEL1                                (0xA0400000U)
#define LOG1_ID_SRV_COMM_PROTOCOL_LEVEL2                                (0xA0800000U)
#define LOG1_ID_SRV_COMM_PROTOCOL_LEVEL3                                (0xA0C00000U)

#define LOG1_ID_SRV_SYS_RESOURCE_BASE                                   (0xA1000000U)
#define LOG1_ID_SRV_SYS_RESOURCE_LEVEL0                                 (0xA1000000U)
#define LOG1_ID_SRV_SYS_RESOURCE_LEVEL1                                 (0xA1400000U)
#define LOG1_ID_SRV_SYS_RESOURCE_LEVEL2                                 (0xA1800000U)
#define LOG1_ID_SRV_SYS_RESOURCE_LEVEL3                                 (0xA1C00000U)

#define LOG1_ID_EXP_BASE                                                (0xB1000000U)
#define LOG1_ID_EXP_LEVEL0                                              (0xB1000000U)
#define LOG1_ID_EXP_LEVEL0_SPE                                          (0xB1000000U)
#define LOG1_ID_EXP_LEVEL0_SPE1                                         (0xB1000001U)
#define LOG1_ID_EXP_LEVEL0_SPE2                                         (0xB1000002U)

#define LOG1_ID_EXP_LEVEL1                                              (0xB1400000U)
#define LOG1_ID_EXP_LEVEL2                                              (0xB1800000U)
#define LOG1_ID_EXP_LEVEL3                                              (0xB1C00000U)

/* ------------CORE1 USER LOG ID---------------------------------------------------------------------------*/
#define LOG1_ID_CORE1_USER_BASE                                         (0xE0000000U)

#define LOG1_ID_USER_LEVEL0                                       (0xE0000000U)
#define LOG1_ID_USER_LEVEL1                                       (0xE0400000U)
#define LOG1_ID_USER_LEVEL2                                       (0xE0800000U)
#define LOG1_ID_USER_LEVEL3                                       (0xE0C00000U)

#define LOG1_ID_USER_L0_KEY_INIT                                  (0xE0000000U)
#define LOG1_ID_USER_L0_KEY_PROG                                  (0xE0000001U)
#define LOG1_ID_USER_L0_KEY_STRT                                  (0xE0000002U)
#define LOG1_ID_USER_L0_KEY_STOP                                  (0xE0000003U)
#define LOG1_ID_USER_L0_CS1131_WRITEFILE_START                    (0xE0000004U)
#define LOG1_ID_USER_L0_CS1131_WRITEFILE_END                      (0xE0000005U)
#define LOG1_ID_USER_L0_CS1131_DOWNLOAD_START                     (0xE0000006U)
#define LOG1_ID_USER_L0_CS1131_START_RUN                          (0xE0000007U)
#define LOG1_ID_USER_L0_CS1131_STOP_RUN                           (0xE0000008U)
#define LOG1_ID_USER_L0_CS1131_PAUSE_RUN                          (0xE0000009U)
#define LOG1_ID_USER_L0_CS1131_WRITE_VAR                          (0xE000000AU)
#define LOG1_ID_USER_L0_CS1131_SINGLE_STEP                        (0xE000000BU)
#define LOG1_ID_USER_L0_CS1131_FORCE_VAR                          (0xE000000CU)
#define LOG1_ID_USER_L0_CS1131_DISABLE_VAR                        (0xE000000DU)
#define LOG1_ID_USER_L0_CS1131_CHECK_EXTERLIB                     (0xE000000EU)
#define LOG1_ID_USER_L0_CS1131_READ_PRJINFO                       (0xE000000FU)
#define LOG1_ID_USER_L0_CS1131_READ_FORCE                         (0xE0000010U)
#define LOG1_ID_USER_L0_CS1131_DOWNLOAD_COMFIR                    (0xE0000011U)
#define LOG1_ID_USER_L0_CS1131_READFILE_START                     (0xE0000012U)
#define LOG1_ID_USER_L0_NEXT_PM_OFFLINE                           (0xE0000013U)
#define LOG1_ID_USER_L0_PRE_PM_OFFLINE                            (0xE0000014U)
#define LOG1_ID_USER_L0_NEXT_PM_ONLINE                            (0xE0000015U)
#define LOG1_ID_USER_L0_PRE_PM_ONLINE                             (0xE0000016U)
#define LOG1_ID_USER_L0_CM_OFFLINE                                (0xE0000017U)
#define LOG1_ID_USER_L0_CM_ONLINE                                 (0xE0000018U)
#define LOG1_ID_USER_L0_SYS_START                                 (0xE0000019U)
#define LOG1_ID_USER_L0_INIT_TASK                                 (0xE000001AU)
#define LOG1_ID_USER_L0_INIT_END                                  (0xE000001BU)

#define LOG1_ID_USER_L0_SELF_TEST                                 (0xE000F000U)

/* ------------IO ERR LOG ID-------------------------------------------------------------------------------*/
#define LOG1_ID_CORE1_IO_USER_BASE                                      (0xF0000000U)

#define LOG1_ID_CORE1_IO_USER_LEVEL0                                    (0xF0000000U)
#define LOG1_ID_CORE1_IO_USER_LEVEL1                                    (0xF0400000U)
#define LOG1_ID_CORE1_IO_USER_LEVEL2                                    (0xF0800000U)
#define LOG1_ID_CORE1_IO_USER_LEVEL3                                    (0xF0C00000U)
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/


#endif /* LOGID_H_ */

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

