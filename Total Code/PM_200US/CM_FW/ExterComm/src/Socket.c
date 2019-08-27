/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : CM_FW外部通信模块
*
*  文件名称    : Socket.c
*
*  功能简介    : 提供套接字相关接口函数
*
*  作者        : 王东
*
*  创建日期    : 2016-01-05
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
#include "../inc/Socket.h"
#include "../inc/LogID.h"
#include "Log/inc/log.h"

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



/*
***********************************************************************************************************************
*                                全局函数实现
***********************************************************************************************************************
*/

/*
***********************************************************************************************************************
* 函数名称: GetErrorNo
*
* 功能描述: 获取Linux API操作产生的错误码
*
* 输入参数: 无
*
* 输出参数: 无
*
* 返 回 值: 错误码
*
* 注意事项: Linux API 返回值显示出错时，在获取此错误码，不建议手动设置此错误码。
***********************************************************************************************************************
*/

int32_t GetErrorNo( void )
{
    return errno;
}

/*
***********************************************************************************************************************
* 函数名称: SetNonBlockMode
*
* 功能描述: 将套接字设置成非阻塞模式
*
* 输入参数: hSock：套接字
*
* 输出参数: 无
*
* 返 回 值: true：设置成功；false：设置失败
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t SetNonBlockMode( SOCKET hSock )
{
    bool_t bResult = false;
    int32_t iFlags = 0;
    if( hSock != (int32_t)(INVALID_SOCKET))
    {
        /* Get Flags */
        iFlags = fcntl( hSock, F_GETFL, 0 );
        if( iFlags != (int32_t)(SOCKET_ERROR))
        {
            if( fcntl( hSock, F_SETFL, iFlags | O_NONBLOCK ) != (int32_t)(SOCKET_ERROR))
            {
                bResult = true;
            }
            else
            {
                /* Record Error Info: fcntl, errno */
                DEBUG_SOCKET( "SetNonBlockMode: fcntl(set) failed-%d!\n", GetErrorNo());
                bResult = false;
            }
        }
        else
        {
            /* Record Error Info: fcntl, errno */
            DEBUG_SOCKET( "SetNonBlockMode: fcntl(get) failed-%d!\n", GetErrorNo());
            bResult = false;
        }
    }

    return bResult;
}

/*
***********************************************************************************************************************
* 函数名称: CloseSocket
*
* 功能描述: 关闭套接字
*
* 输入参数: phSocket：指向待关闭的套接字
*
* 输出参数: phSocket：指向关闭后的套接字
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void CloseSocket( SOCKET *phSocket )
{
    if( phSocket != NULL )
    {
        if( *phSocket != (int32_t)(INVALID_SOCKET))
        {
            /* Close the socket */
            if( close( *phSocket ) != (int32_t)(SOCKET_ERROR))
            {
                *phSocket = (int32_t)(INVALID_SOCKET);
            }
            else
            {
                /* Record Error Info: close, errno */
                DEBUG_SOCKET( "CloseSocket: close failed-%d!\n", GetErrorNo());
            }
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CloseListenSocket
*
* 功能描述: 关闭监听套接字
*
* 输入参数: phSocket：指向待关闭的套接字
*
* 输出参数: phSocket：指向关闭后的套接字
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void CloseListenSocket( SOCKET *phSocket )
{
    CloseSocket( phSocket );

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CloseConnSocket
*
* 功能描述: 关闭连接套接字
*
* 输入参数: phSocket：指向关闭前的连接套接字
*
* 输出参数: phSocket：指向关闭后的连接套接字
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void CloseConnSocket( SOCKET *phSocket )
{
    if( phSocket != NULL )
    {
        if( *phSocket != (int32_t)(INVALID_SOCKET))
        {
            /* Shutdown */
            if( (int32_t)(SOCKET_ERROR) == shutdown( *phSocket, SHUT_WR ))
            {
                DEBUG_SOCKET( "CloseConnSocket: shutdown failed-%d!\n", GetErrorNo());
            }

            CloseSocket( phSocket );
        }
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CreateListenSocket
*
* 功能描述: 创建监听套接字，绑定到指定端口，并开始监听
*
* 输入参数: uiIPAddr：网口IP地址；
*          usPort：待监听端口；
*          bNonBlock：是否设置成非阻塞模式；
*          phSocket：指向创建前的监听套接字。
*
* 输出参数: phSocket：指向创建后的监听套接字
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void CreateListenSocket( uint32_t uiIPAddr, uint16_t usPort, bool_t bNonBlock, SOCKET *phSocket )
{
    SOCKET hLisSock = (int32_t)(INVALID_SOCKET);
    int32_t iOptval = 1;
    SOCKADDR_IN stSockAddr;
    memset( &stSockAddr, 0, sizeof(SOCKADDR_IN));

    if((uiIPAddr != 0U) &&
       (usPort != 0U) &&
       (phSocket != NULL))
    {
        hLisSock = *phSocket;
        if((int32_t)(INVALID_SOCKET) == hLisSock )
        {
            /* Create Listen Socket */
            hLisSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
            if( hLisSock != (int32_t)(INVALID_SOCKET))
            {
                if( bNonBlock )
                {
                    if( !SetNonBlockMode( hLisSock ))
                    {
                        DEBUG_SOCKET( "CreateListenSocket: SetNonBlockMode failed!\n" );
                        CloseSocket( &hLisSock );
                    }
                }

                if( hLisSock != (int32_t)(INVALID_SOCKET))
                {
                    /* permit reuse addr */
                    iOptval = 1;
                    if( setsockopt( hLisSock, SOL_SOCKET, SO_REUSEADDR, \
                                    &iOptval, sizeof(iOptval)) != (int32_t)(SOCKET_ERROR))
                    {
                        /* Bind */
                        memset( &stSockAddr, 0, sizeof(SOCKADDR_IN));
                        stSockAddr.sin_family = AF_INET;
                        stSockAddr.sin_addr.s_addr = htonl( uiIPAddr );  /* Local Network IP Address */
                        stSockAddr.sin_port = htons( usPort );           /* Local Port */
                        if( bind( hLisSock, (SOCKADDR*)&stSockAddr, sizeof(SOCKADDR_IN)) != (int32_t)(SOCKET_ERROR))
                        {
                            /* Listen */
                            if( listen( hLisSock, SOMAXCONN ) != (int32_t)(SOCKET_ERROR))
                            {
                                *phSocket = hLisSock;
                            }
                            else
                            {
                                /* Record Error Info: Listen Failed, errno */
                                DEBUG_SOCKET( "CreateListenSocket: listen failed-%d!\n", GetErrorNo());
                                CloseSocket( &hLisSock );
                            }
                        }
                        else
                        {
                            /* Record Error Info: Bind Failed, errno */
                            DEBUG_SOCKET( "CreateListenSocket: bind failed-%d!\n", GetErrorNo());
                            CloseSocket( &hLisSock );
                        }
                    }
                    else
                    {
                        /* Record Error Info: Setsockopt Failed, errno */
                        DEBUG_SOCKET( "CreateListenSocket: setsockopt failed-%d!\n", GetErrorNo());
                        CloseSocket( &hLisSock );
                    }
                }
            }
            else
            {
                /* Record Error Info: Create failed, errno */
                DEBUG_SOCKET( "CreateListenSocket: socket failed-%d!\n", GetErrorNo());
            }
        }/* if((int32_t)(INVALID_SOCKET) == hLisSock ) */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: CreateUDPServerSocket
*
* 功能描述: 创建UDP服务器端套接字
*
* 输入参数: uiIPAddr：网口IP地址；
*          usPort：待绑定端口；
*          bNonBlock：是否设置成非阻塞模式；
*          phSocket：指向未创建前的UDP服务器端套接字。
*
* 输出参数: phSocket：指向创建后的UDP服务器端套接字
*
* 返 回 值: 无
*
* 注意事项: 无
***********************************************************************************************************************
*/

void CreateUDPServerSocket( uint32_t uiIPAddr, uint16_t usPort, bool_t bNonBlock, SOCKET *phSocket )
{
    SOCKET hUDPSerSock = (int32_t)(INVALID_SOCKET);
    SOCKADDR_IN stSockAddr;
    memset( &stSockAddr, 0, sizeof(SOCKADDR_IN));

    if((uiIPAddr != 0) &&
       (usPort != 0) &&
       (phSocket != NULL))
    {
        hUDPSerSock = *phSocket;
        if((int32_t)(INVALID_SOCKET) == hUDPSerSock )
        {
            /* Create UDP Server Socket */
            hUDPSerSock = socket( AF_INET, SOCK_DGRAM, 0 );
            if( hUDPSerSock != (int32_t)(INVALID_SOCKET))
            {
                if( bNonBlock )
                {
                    if( !SetNonBlockMode( hUDPSerSock ) )
                    {
                        DEBUG_SOCKET( "CreateUDPServerSocket: SetNonBlockMode failed!\n" );
                        CloseSocket( &hUDPSerSock );
                    }
                }

                if( hUDPSerSock != (int32_t)(INVALID_SOCKET))
                {
                    /* Bind */
                    memset( &stSockAddr, 0, sizeof(SOCKADDR_IN));
                    stSockAddr.sin_family = AF_INET;
                    stSockAddr.sin_addr.s_addr = htonl( uiIPAddr );  /* Local Network IP Address */
                    stSockAddr.sin_port = htons( usPort );           /* Local Port */
                    if( bind( hUDPSerSock, (SOCKADDR*)&stSockAddr, sizeof(SOCKADDR_IN)) != (int32_t)(SOCKET_ERROR))
                    {
                        *phSocket = hUDPSerSock;
                    }
                    else
                    {
                        /* Record Error Info: Bind Failed, errno */
                        DEBUG_SOCKET( "CreateUDPServerSocket: bind failed-%d!\n", GetErrorNo());
                        CloseSocket( &hUDPSerSock );
                    }
                }
            }
            else
            {
                /* Record Error Info: Create failed, errno */
                DEBUG_SOCKET( "CreateUDPServerSocket: socket failed-%d!\n", GetErrorNo());
            }
        }/* if((int32_t)(INVALID_SOCKET) == hLisSock ) */
    }

    return;
}

/*
***********************************************************************************************************************
* 函数名称: Connect
*
* 功能描述: 建立连接
*
* 输入参数: uiIPAddr：目标IP地址；
*          usPort：目标端口号；
*          bNonBlock：是否设置为非阻塞模式。
*
* 输出参数: phSocket：连接套接字，INVALID_SOCKET代表创建失败
*
* 返 回 值: 连接成功标志，true代表成功
*
* 注意事项: 无
***********************************************************************************************************************
*/

bool_t Connect( uint32_t uiIPAddr, uint16_t usPort, bool_t bNonBlock, SOCKET *phSocket )
{
    bool_t bSuccess = false;
    SOCKET hConnSock = (int32_t)(INVALID_SOCKET);
    SOCKADDR_IN stSockAddr;
    memset( &stSockAddr, 0, sizeof(SOCKADDR_IN));

    if((uiIPAddr != 0) &&
       (usPort != 0) &&
       (phSocket != NULL))
    {
        /* Create Connect Socket */
        hConnSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
        if( hConnSock != (int32_t)(INVALID_SOCKET))
        {
            if( bNonBlock )
            {
                if( !SetNonBlockMode( hConnSock ))
                {
                    /* SetNonBlockMode Failed: Record Info */
                    DEBUG_SOCKET( "Connect: SetNonBlockMode Failed-%d!\n", GetErrorNo());
                    CloseSocket( &hConnSock );
                }
            }

            if( hConnSock != (int32_t)(INVALID_SOCKET))
            {
                /* Connect */
                memset( &stSockAddr, 0, sizeof(SOCKADDR_IN));
                stSockAddr.sin_family = AF_INET;
                stSockAddr.sin_addr.s_addr = htonl( uiIPAddr );  /* Local Network IP Address */
                stSockAddr.sin_port = htons( usPort );           /* Local Port */
                if( connect( hConnSock, (SOCKADDR*)&stSockAddr, sizeof(SOCKADDR_IN)) != (int32_t)(SOCKET_ERROR))
                {
                    bSuccess = true;
                }
                else
                {
                    if( GetErrorNo() != EINPROGRESS )
                    {
                        /* Connect Failed */
                        DEBUG_SOCKET( "Connect To IP(0x%08X) Failed-%d!\n", uiIPAddr, GetErrorNo());
                        CloseSocket( &hConnSock );
                    }
                    else
                    {
                        /* Connect In Progress */
                        DEBUG_SOCKET( "Connect To IP(0x%08X) is in progress.\n", uiIPAddr );
                    }
                }
            }
        }
        else
        {
            /* Record Error Info: Create failed, errno */
            DEBUG_SOCKET( "Connect: Create Socket Failed-%d!\n", GetErrorNo());
        }

        *phSocket = hConnSock;
    }

    return bSuccess;
}

/*
***********************************************************************************************************************
*                                局部函数实现
***********************************************************************************************************************
*/



/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/

