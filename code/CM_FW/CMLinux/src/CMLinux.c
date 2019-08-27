/*
*********************************************************************************************************************** 
*                  Copyright(c) 北京康吉森技术有限公司 2015
*           Beijing Consen Technologies Co.,Ltd. All rights reserved.
*                                  
*                                  
*  项目名称    : 安全控制系统
* 
*  模块名称    : CM_FW Linux系统调用
*
*  文件名称    : CMLinux.c
*
*  功能简介    : 提供Linux系统调用相关函数
*
*  作者        : 王东
*
*  创建日期    : 2016-05-12
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

#include "../inc/CMLinux.h"

/*
***********************************************************************************************************************
*                                局部变量声明
***********************************************************************************************************************
*/

int errno;

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

/*************************** C Library ***************************/
//sizeof
void* memset(void *block, int c, __kernel_size_t size)
{
    return block;
}

void* memcpy(void *dst, const void *src, __kernel_size_t size)
{
    return dst;
}

char* strncpy(char *dst, const char *src, __kernel_size_t size )
{
    return dst;
}

int strncmp(const char *s1,const char *s2,__kernel_size_t size)
{
    return 0;
}

int sprintf( char *buffer, const char *format, ... /* arg */ )
{
    return -1;
}

__kernel_size_t strlen(const char *s)
{
    return 0;
}

/*************************** socket ***************************/
int socket(int namespace, int style, int protocol)
{
    return -1;
}

int close(int filedes)
{
    return -1;
}

int shutdown(int socket, int how)
{
    return -1;
}

int bind(int socket, struct sockaddr *addr, socklen_t length)
{
    return -1;
}

int listen(int socket, int n)
{
    return -1;
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return -1;
}

int connect(int socket, struct sockaddr *addr, socklen_t length)
{
    return -1;
}

int recvfrom(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *addrlen)
{
    return -1;
}

int recv(int socket, void *buffer, size_t size, int flags)
{
    return -1;
}

int send(int socket, const void *buffer, size_t size, int flags)
{
    return -1;
}

int sendto(int socket, const void *buffer, size_t size, int flags,
           const struct sockaddr *addr, socklen_t length)
{
    return -1;
}

int fcntl(int filedes, int command, ... /* arg */ )
{
    return -1;
}

int ioctl(int fd, unsigned long request, ... /* arg */ )
{
    return -1;
}

int setsockopt(int socket, int level, int optname, void *optval, socklen_t optlen)
{
    return -1;
}

uint32_t htonl(uint32_t hostlong)
{
    return 0;
}

uint16_t htons(uint32_t hostshort)
{
    return 0;
}

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout)
{
    return -1;
}

void FD_CLR(int fd, fd_set *set)
{

}

int  FD_ISSET(int fd, fd_set *set)
{
    return -1;
}

void FD_SET(int fd, fd_set *set)
{

}

void FD_ZERO(fd_set *set)
{

}

/*************************** File ***************************/
int open(const char *pathname, int flags, ... /* arg */)
{
    return -1;
}

ssize_t write(int fd, const void *buf, size_t count)
{
    return -1;
}

ssize_t read(int fd, void *buf, size_t count)
{
    return -1;
}

off_t lseek(int fd, off_t offset, int whence)
{
    return -1;
}

int unlink(const char *pathname)
{
    return -1;
}

/*************************** Serial ***************************/
int tcgetattr( int filedes, struct termios *termios )
{
    return -1;
}

int cfsetispeed(struct termios *termios, speed_t speed)
{
    return -1;
}

int cfsetospeed(struct termios *termios, speed_t speed)
{
    return -1;
}

int tcflush(int filedes, int queue)
{
    return -1;
}

int tcsetattr(int filedes, int when, const struct termios *termios)
{
    return -1;
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

