/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : CM_FW外部通信模块、上位机应用模块
*
*  文件名称    : PCSWCommon.c
*
*  功能简介    : 上位机软件与外部通讯模块的公用部分
*
*  作者        : 王东
*
*  创建日期    : 2016-02-16
*
*  版本信息    : V1.0
*
*  修订信息    : 无
*
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include "../inc/PCSWCommon.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/


/*
***********************************************************************************************************************
*                                局部函数声明
***********************************************************************************************************************
*/

static bool_t CheckAccCtrlCfgInfo( AccessCtrlConfigInfo_t const *pstAccCfgInfo );
static bool_t SwitchAccCtrlCfgInfo( AccCtrlInfo_t *pstAccInfo, AccessCtrlConfigInfo_t const *pstAccCfgInfo );

/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: GetReqActualType
*
* 功能描述: 获取请求帧实际类型
*
* 输入参数: pucReq：指向请求帧
*
* 输出参数: 无
*
* 返 回 值: 请求帧的实际类型
*
* 注意事项: 无
***********************************************************************************************************************
*/

uint16_t GetReqActualType( uint8_t const pucReq[])
{
    uint16_t usAType = 0U;

    if( pucReq != NULL )
    {
        usAType = (pucReq[APP_REQ_ATYPE_HIINDEX] << 8) | pucReq[APP_REQ_ATYPE_LOWINDEX];
    }

    return usAType;
}

/*
***********************************************************************************************************************
* 函数名称: InitAccessCtrlInfo
*
* 功能描述: 初始化本地保存的访问控制信息
*
* 输入参数: pstAccCtrlInfo：指向本地保存的访问控制信息
*
* 输出参数: 无
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void InitAccessCtrlInfo( AccCtrlInfo_t *pstAccCtrlInfo )
{
    uint8_t ucIndex = 0U;
    uint8_t ucItemIndex = 0U;
    AccessCtrlConfigInfo_t *pAccCfgInfo = NULL;
    AccessCtrlConfig_t *pAccCfgItem = NULL;

    if( pstAccCtrlInfo != NULL )
    {
        memset( pstAccCtrlInfo, 0, sizeof(AccCtrlInfo_t));
        pstAccCtrlInfo->ucIndex = 0U;

        /* CM Configuration Version */
        pstAccCtrlInfo->uiCMCfgCRC = 0U;
        pstAccCtrlInfo->uiCMCfgVer = 0U;

        /* Initialize Configuration */
        for( ucIndex = 0U; ucIndex < (uint8_t)(LOCAL_CFG_VER_CNT); ucIndex++ )
        {
            pAccCfgInfo = &( pstAccCtrlInfo->stAccCtrl[ucIndex]);

            /* Header */
            pAccCfgInfo->stAccessCtrlHead.bUpdate = false;
            pAccCfgInfo->stAccessCtrlHead.ucAccessCtrlNum = 0U;

            /* Item */
            for( ucItemIndex = 0U; ucItemIndex < (uint8_t)(MAX_ACCESS_CTRL_NUM); ucItemIndex++ )
            {
                pAccCfgItem = &( pAccCfgInfo->stAccessCtrlConfig[ucItemIndex]);
                pAccCfgItem->uiClientIPAddr = 0U;
                pAccCfgItem->uiSubnetMask = 0U;
                pAccCfgItem->ucNetwork = (uint8_t)(NW_NET1);
                pAccCfgItem->ucCS1131 = (uint8_t)(AL_ALL);
                pAccCfgItem->ucOPC = (uint8_t)(AL_ALL);
                pAccCfgItem->ucSOE = (uint8_t)(AL_ALL);
                pAccCfgItem->ucAMS = (uint8_t)(AL_ALL);
                pAccCfgItem->ucClient = (uint8_t)(AL_ALL);
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: HandleAccessCtrlInfo
*
* 功能描述: 访问控制信息版本变化时，拷贝到本地并切换
*
* 输入参数: pstAccCtrlInfo：指向本地保存的访问控制信息；
*          puiCRC：指向本地保存的CRC信息；
*          puiFileVer：指向本地保存的文件版本信息。
*
* 输出参数: 无
*
* 返 回 值: 是否发生切换
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t HandleAccessCtrlInfo( AccCtrlInfo_t *pstAccCtrlInfo )
{
    bool_t bSwitch = false;
    uint32_t uiCRC = 0U;
    uint32_t uiFileVer = 0U;
    AccessCtrlConfigInfo_t stAccCtrlCfgInfo;

    if( pstAccCtrlInfo != NULL )
    {
        /* Get CM Version */
        if( SysGetCMConfigVersion( &uiCRC, &uiFileVer ))
        {
            /* Compare Version */
            if((uiCRC != pstAccCtrlInfo->uiCMCfgCRC) ||
               (uiFileVer != pstAccCtrlInfo->uiCMCfgVer))
            {
                /* Copy To Local */
                if( SysGetAccCtrlInfo( &stAccCtrlCfgInfo ))
                {
                    /* Check Active */
                    if( ACTIVE == SysGetCMConfigInfoActiveFlag())
                    {
                        /* CM always exist: Switch when bUpdate is true */
                        /* Online download and then change CM: bUpdate true/false, Local ver is 0 */
                        if( stAccCtrlCfgInfo.stAccessCtrlHead.bUpdate ||
                            ((0U == pstAccCtrlInfo->uiCMCfgCRC ) && (0U == pstAccCtrlInfo->uiCMCfgVer)))
                        {
                            /* Check Valid */
                            if( CheckAccCtrlCfgInfo( &stAccCtrlCfgInfo ))
                            {
                                /* Switch */
                                if( SwitchAccCtrlCfgInfo( pstAccCtrlInfo, &stAccCtrlCfgInfo ))
                                {
                                    bSwitch = true;
                                    pstAccCtrlInfo->uiCMCfgCRC = uiCRC;
                                    pstAccCtrlInfo->uiCMCfgVer = uiFileVer;
                                }
                            }
                            else
                            {
                                pstAccCtrlInfo->uiCMCfgCRC = uiCRC;
                                pstAccCtrlInfo->uiCMCfgVer = uiFileVer;
                                DEBUG_RTS( "Access Control Cfg Invalid\n" );
                            }
                        }
                    }
                }
            }
        }
    }

    return bSwitch;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: CheckAccCtrlCfgInfo
*
* 功能描述: 检查访问控制信息是否有效
*
* 输入参数: pstAccCfgInfo：指向待切换的访问控制配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否有效
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t CheckAccCtrlCfgInfo( AccessCtrlConfigInfo_t const *pstAccCfgInfo )
{
    bool_t bValid = false;
    uint8_t ucItemNum = 0U;
    uint8_t ucIndex = 0U;
    AccessCtrlConfig_t const *pstAccItem = NULL;

    if( pstAccCfgInfo != NULL )
    {
        /* Header: Item Number */
        ucItemNum = pstAccCfgInfo->stAccessCtrlHead.ucAccessCtrlNum;
        if( ucItemNum < (uint8_t)(MAX_ACCESS_CTRL_NUM))
        {
            bValid = true;

            /* Item */
            for( ucIndex = 0U; ucIndex < ucItemNum; ucIndex++ )
            {
                pstAccItem = &( pstAccCfgInfo->stAccessCtrlConfig[ucIndex]);

                /* IP Address */
                if((0U == pstAccItem->uiClientIPAddr) ||
                   (0U == pstAccItem->uiSubnetMask) ||
                   ((pstAccItem->ucNetwork < (uint8_t)(NW_NONE)) || (pstAccItem->ucNetwork > (uint8_t)(NW_NET_ALL))) ||
                   ((pstAccItem->ucCS1131 < (uint8_t)(AL_DENY)) || (pstAccItem->ucCS1131 > (uint8_t)(AL_ALL))) ||
                   ((pstAccItem->ucSOE < (uint8_t)(AL_DENY)) || (pstAccItem->ucSOE > (uint8_t)(AL_ALL))) ||
                   ((pstAccItem->ucAMS < (uint8_t)(AL_DENY)) || (pstAccItem->ucAMS > (uint8_t)(AL_ALL))) ||
                   ((pstAccItem->ucOPC < (uint8_t)(AL_DENY)) || (pstAccItem->ucOPC > (uint8_t)(AL_ALL))) ||
                   ((pstAccItem->ucClient < (uint8_t)(AL_DENY)) || (pstAccItem->ucClient > (uint8_t)(AL_ALL))))
                {
                    bValid = false;
                    break;
                }
            }/* end of for */
        }
    }

    return bValid;
}

/*
***********************************************************************************************************************
* 函数名称: SwitchAccCtrlCfgInfo
*
* 功能描述: 切换访问控制信息
*
* 输入参数: pstAccInfo：指向本地保存的访问控制信息；
*          pstAccCfgInfo：指向待切换的访问控制配置信息。
*
* 输出参数: 无
*
* 返 回 值: 是否切换成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

static bool_t SwitchAccCtrlCfgInfo( AccCtrlInfo_t *pstAccInfo, AccessCtrlConfigInfo_t const *pstAccCfgInfo )
{
    bool_t bResult = false;
    uint8_t ucCfgIndex = 0U;
    AccessCtrlConfigInfo_t *pstDstAccCfgInfo = NULL;

    if((pstAccInfo != NULL) &&
       (pstAccCfgInfo != NULL))
    {
        ucCfgIndex = pstAccInfo->ucIndex;    /* Current Configuration Index */
        if( ucCfgIndex < (uint8_t)(LOCAL_CFG_VER_CNT))
        {
            /* Switch Version */
            ucCfgIndex = ( ucCfgIndex + 1U ) % (uint8_t)(LOCAL_CFG_VER_CNT);
            pstDstAccCfgInfo = &( pstAccInfo->stAccCtrl[ucCfgIndex]);

            /* Copy */
            memcpy( pstDstAccCfgInfo, pstAccCfgInfo, sizeof(AccessCtrlConfigInfo_t));

            /* Update Index */
            pstAccInfo->ucIndex = ucCfgIndex;
            bResult = true;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

