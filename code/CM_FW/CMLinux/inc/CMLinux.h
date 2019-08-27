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
*  文件名称    : CMLinux.h
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

#ifndef CMLINUX_H_
#define CMLINUX_H_

/*
***********************************************************************************************************************
*                                包含头文件
***********************************************************************************************************************
*/

#include "Sys/inc/DataType.h"

/*
***********************************************************************************************************************
*                                宏定义
***********************************************************************************************************************
*/

/* fcntl.h  */
#define O_ACCMODE   00000003
#define O_RDONLY    00000000
#define O_WRONLY    00000001
#define O_RDWR      00000002
#ifndef O_CREAT
#define O_CREAT     00000100    /* not fcntl */
#endif
#ifndef O_EXCL
#define O_EXCL      00000200    /* not fcntl */
#endif
#ifndef O_NOCTTY
#define O_NOCTTY    00000400    /* not fcntl */
#endif
#ifndef O_TRUNC
#define O_TRUNC     00001000    /* not fcntl */
#endif
#ifndef O_APPEND
#define O_APPEND    00002000
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK  00004000
#endif
#ifndef O_DSYNC
#define O_DSYNC     00010000    /* used to be O_SYNC, see below */
#endif
#ifndef FASYNC
#define FASYNC      00020000    /* fcntl, for BSD compatibility */
#endif
#ifndef O_DIRECT
#define O_DIRECT    00040000    /* direct disk access hint */
#endif
#ifndef O_LARGEFILE
#define O_LARGEFILE 00100000
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY 00200000    /* must be a directory */
#endif
#ifndef O_NOFOLLOW
#define O_NOFOLLOW  00400000    /* don't follow links */
#endif
#ifndef O_NOATIME
#define O_NOATIME   01000000
#endif
#ifndef O_CLOEXEC
#define O_CLOEXEC   02000000    /* set close_on_exec */
#endif

/*
 * Before Linux 2.6.33 only O_DSYNC semantics were implemented, but using
 * the O_SYNC flag.  We continue to use the existing numerical value
 * for O_DSYNC semantics now, but using the correct symbolic name for it.
 * This new value is used to request true Posix O_SYNC semantics.  It is
 * defined in this strange way to make sure applications compiled against
 * new headers get at least O_DSYNC semantics on older kernels.
 *
 * This has the nice side-effect that we can simply test for O_DSYNC
 * wherever we do not care if O_DSYNC or O_SYNC is used.
 *
 * Note: __O_SYNC must never be used directly.
 */
#ifndef O_SYNC
#define __O_SYNC    04000000
#define O_SYNC      (__O_SYNC|O_DSYNC)
#endif

#ifndef O_PATH
#define O_PATH      010000000
#endif

#ifndef __O_TMPFILE
#define __O_TMPFILE 020000000
#endif

/* a horrid kludge trying to make sure that this will fail on old kernels */
#define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#define O_TMPFILE_MASK (__O_TMPFILE | O_DIRECTORY | O_CREAT)

#ifndef O_NDELAY
#define O_NDELAY    O_NONBLOCK
#endif

#define F_DUPFD     0   /* dup */
#define F_GETFD     1   /* get close_on_exec */
#define F_SETFD     2   /* set/clear close_on_exec */
#define F_GETFL     3   /* get file->f_flags */
#define F_SETFL     4   /* set file->f_flags */
#ifndef F_GETLK
#define F_GETLK     5
#define F_SETLK     6
#define F_SETLKW    7
#endif
#ifndef F_SETOWN
#define F_SETOWN    8   /* for sockets. */
#define F_GETOWN    9   /* for sockets. */
#endif
#ifndef F_SETSIG
#define F_SETSIG    10  /* for sockets. */
#define F_GETSIG    11  /* for sockets. */
#endif

#ifndef CONFIG_64BIT
#ifndef F_GETLK64
#define F_GETLK64   12  /*  using 'struct flock64' */
#define F_SETLK64   13
#define F_SETLKW64  14
#endif
#endif

#ifndef F_SETOWN_EX
#define F_SETOWN_EX 15
#define F_GETOWN_EX 16
#endif

#ifndef F_GETOWNER_UIDS
#define F_GETOWNER_UIDS 17
#endif

#define F_OWNER_TID 0
#define F_OWNER_PID 1
#define F_OWNER_PGRP    2

/* socket.h */
/* For setsockopt(2) */
#define SOL_SOCKET  1

#define SO_DEBUG    1
#define SO_REUSEADDR    2
#define SO_TYPE     3
#define SO_ERROR    4
#define SO_DONTROUTE    5
#define SO_BROADCAST    6
#define SO_SNDBUF   7
#define SO_RCVBUF   8
#define SO_SNDBUFFORCE  32
#define SO_RCVBUFFORCE  33
#define SO_KEEPALIVE    9
#define SO_OOBINLINE    10
#define SO_NO_CHECK 11
#define SO_PRIORITY 12
#define SO_LINGER   13
#define SO_BSDCOMPAT    14
#define SO_REUSEPORT    15
#ifndef SO_PASSCRED /* powerpc only differs in these */
#define SO_PASSCRED 16
#define SO_PEERCRED 17
#define SO_RCVLOWAT 18
#define SO_SNDLOWAT 19
#define SO_RCVTIMEO 20
#define SO_SNDTIMEO 21
#endif

#define O_FSYNC       O_SYNC
#define O_ASYNC       020000

/* stat.h */
#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

/* fs.h */
#define SEEK_SET    0   /* seek relative to beginning of file */
#define SEEK_CUR    1   /* seek relative to current file position */
#define SEEK_END    2   /* seek relative to end of file */
#define SEEK_DATA   3   /* seek to the next data */
#define SEEK_HOLE   4   /* seek to the next hole */
#define SEEK_MAX    SEEK_HOLE

/* socket.h */
/* Supported address families. */
#define AF_UNSPEC   0
#define AF_UNIX     1   /* Unix domain sockets      */
#define AF_LOCAL    1   /* POSIX name for AF_UNIX   */
#define AF_INET     2   /* Internet IP Protocol     */
#define AF_AX25     3   /* Amateur Radio AX.25      */
#define AF_IPX      4   /* Novell IPX           */
#define AF_APPLETALK    5   /* AppleTalk DDP        */
#define AF_NETROM   6   /* Amateur Radio NET/ROM    */
#define AF_BRIDGE   7   /* Multiprotocol bridge     */
#define AF_ATMPVC   8   /* ATM PVCs         */
#define AF_X25      9   /* Reserved for X.25 project    */
#define AF_INET6    10  /* IP version 6         */
#define AF_ROSE     11  /* Amateur Radio X.25 PLP   */
#define AF_DECnet   12  /* Reserved for DECnet project  */
#define AF_NETBEUI  13  /* Reserved for 802.2LLC project*/
#define AF_SECURITY 14  /* Security callback pseudo AF */
#define AF_KEY      15      /* PF_KEY key management API */
#define AF_NETLINK  16
#define AF_ROUTE    AF_NETLINK /* Alias to emulate 4.4BSD */
#define AF_PACKET   17  /* Packet family        */
#define AF_ASH      18  /* Ash              */
#define AF_ECONET   19  /* Acorn Econet         */
#define AF_ATMSVC   20  /* ATM SVCs         */
#define AF_RDS      21  /* RDS sockets          */
#define AF_SNA      22  /* Linux SNA Project (nutters!) */
#define AF_IRDA     23  /* IRDA sockets         */
#define AF_PPPOX    24  /* PPPoX sockets        */
#define AF_WANPIPE  25  /* Wanpipe API Sockets */
#define AF_LLC      26  /* Linux LLC            */
#define AF_IB       27  /* Native InfiniBand address    */
#define AF_CAN      29  /* Controller Area Network      */
#define AF_TIPC     30  /* TIPC sockets         */
#define AF_BLUETOOTH    31  /* Bluetooth sockets        */
#define AF_IUCV     32  /* IUCV sockets         */
#define AF_RXRPC    33  /* RxRPC sockets        */
#define AF_ISDN     34  /* mISDN sockets        */
#define AF_PHONET   35  /* Phonet sockets       */
#define AF_IEEE802154   36  /* IEEE802154 sockets       */
#define AF_CAIF     37  /* CAIF sockets         */
#define AF_ALG      38  /* Algorithm sockets        */
#define AF_NFC      39  /* NFC sockets          */
#define AF_VSOCK    40  /* vSockets         */
#define AF_MAX      41  /* For now.. */

/* Protocol families, same as address families. */
#define PF_UNSPEC   AF_UNSPEC
#define PF_UNIX     AF_UNIX
#define PF_LOCAL    AF_LOCAL
#define PF_INET     AF_INET
#define PF_AX25     AF_AX25
#define PF_IPX      AF_IPX
#define PF_APPLETALK    AF_APPLETALK
#define PF_NETROM   AF_NETROM
#define PF_BRIDGE   AF_BRIDGE
#define PF_ATMPVC   AF_ATMPVC
#define PF_X25      AF_X25
#define PF_INET6    AF_INET6
#define PF_ROSE     AF_ROSE
#define PF_DECnet   AF_DECnet
#define PF_NETBEUI  AF_NETBEUI
#define PF_SECURITY AF_SECURITY
#define PF_KEY      AF_KEY
#define PF_NETLINK  AF_NETLINK
#define PF_ROUTE    AF_ROUTE
#define PF_PACKET   AF_PACKET
#define PF_ASH      AF_ASH
#define PF_ECONET   AF_ECONET
#define PF_ATMSVC   AF_ATMSVC
#define PF_RDS      AF_RDS
#define PF_SNA      AF_SNA
#define PF_IRDA     AF_IRDA
#define PF_PPPOX    AF_PPPOX
#define PF_WANPIPE  AF_WANPIPE
#define PF_LLC      AF_LLC
#define PF_IB       AF_IB
#define PF_CAN      AF_CAN
#define PF_TIPC     AF_TIPC
#define PF_BLUETOOTH    AF_BLUETOOTH
#define PF_IUCV     AF_IUCV
#define PF_RXRPC    AF_RXRPC
#define PF_ISDN     AF_ISDN
#define PF_PHONET   AF_PHONET
#define PF_IEEE802154   AF_IEEE802154
#define PF_CAIF     AF_CAIF
#define PF_ALG      AF_ALG
#define PF_NFC      AF_NFC
#define PF_VSOCK    AF_VSOCK
#define PF_MAX      AF_MAX

/* Maximum queue length specifiable by listen.  */
#define SOMAXCONN   128

/* Flags we can use with send/ and recv.
   Added those for 1003.1g not all are supported yet
 */

#define MSG_OOB     1
#define MSG_PEEK    2
#define MSG_DONTROUTE   4
#define MSG_TRYHARD     4       /* Synonym for MSG_DONTROUTE for DECnet */
#define MSG_CTRUNC  8
#define MSG_PROBE   0x10    /* Do not send. Only probe path f.e. for MTU */
#define MSG_TRUNC   0x20
#define MSG_DONTWAIT    0x40    /* Nonblocking io        */
#define MSG_EOR         0x80    /* End of record */
#define MSG_WAITALL 0x100   /* Wait for a full request */
#define MSG_FIN         0x200
#define MSG_SYN     0x400
#define MSG_CONFIRM 0x800   /* Confirm path validity */
#define MSG_RST     0x1000
#define MSG_ERRQUEUE    0x2000  /* Fetch message from error queue */
#define MSG_NOSIGNAL    0x4000  /* Do not generate SIGPIPE */
#define MSG_MORE    0x8000  /* Sender will send more */
#define MSG_WAITFORONE  0x10000 /* recvmmsg(): block until 1+ packets avail */
#define MSG_SENDPAGE_NOTLAST 0x20000 /* sendpage() internal : not the last page */
#define MSG_EOF         MSG_FIN

#define MSG_FASTOPEN    0x20000000  /* Send data in TCP SYN */
#define MSG_CMSG_CLOEXEC 0x40000000 /* Set close_on_exit for file
                       descriptor received through
                       SCM_RIGHTS */
#if defined(CONFIG_COMPAT)
#define MSG_CMSG_COMPAT 0x80000000  /* This message needs 32 bit fixups */
#else
#define MSG_CMSG_COMPAT 0       /* We never have 32 bit fixups */
#endif

/* errno.h */
#define EPERM     1 /* Operation not permitted */

#define ENOENT    2 /* No such file or directory */

#define ESRCH     3 /* No such process */

#define EINTR     4 /* Interrupted system call */

#define EIO       5 /* I/O error */

#define ENXIO     6 /* No such device or address */

#define E2BIG     7 /* Arg list too long */

#define ENOEXEC   8 /* Exec format error */

#define EBADF     9 /* Bad file number */

#define ECHILD   10 /* No child processes */

#define EAGAIN   11 /* Try again */

#define ENOMEM   12 /* Out of memory */

#define EACCES   13 /* Permission denied */

#define EFAULT   14 /* Bad address */

#define ENOTBLK  15 /* Block device required */

#define EBUSY    16 /* Device or resource busy */

#define EEXIST   17 /* File exists */

#define EXDEV    18 /* Cross-device link */

#define ENODEV   19 /* No such device */

#define ENOTDIR  20 /* Not a directory */

#define EISDIR   21 /* Is a directory */

#define EINVAL   22 /* Invalid argument */

#define ENFILE   23 /* File table overflow */

#define EMFILE   24 /* Too many open files */

#define ENOTTY   25 /* Not a typewriter */

#define ETXTBSY  26 /* Text file busy */

#define EFBIG    27 /* File too large */

#define ENOSPC   28 /* No space left on device */

#define ESPIPE   29 /* Illegal seek */

#define EROFS    30 /* Read-only file system */

#define EMLINK   31 /* Too many links */

#define EPIPE    32 /* Broken pipe */

#define EDOM     33 /* Math argument out of domain of func */

#define ERANGE   34 /* Math result not representable */

#define EDEADLK  35 /* Resource deadlock would occur */

#define ENAMETOOLONG 36 /* File name too long */

#define ENOLCK       37 /* No record locks available */

#define ENOSYS       38 /* Function not implemented */

#define ENOTEMPTY    39 /* Directory not empty */

#define ELOOP        40 /* Too many symbolic links encountered */

#define EWOULDBLOCK EAGAIN /* Operation would block */

#define ENOMSG       42 /* No message of desired type */

#define EIDRM        43 /* Identifier removed */

#define ECHRNG       44 /* Channel number out of range */

#define EL2NSYNC     45 /* Level 2 not synchronized */

#define EL3HLT       46 /* Level 3 halted */

#define EL3RST       47 /* Level 3 reset */

#define ELNRNG       48 /* Link number out of range */

#define EUNATCH      49 /* Protocol driver not attached */

#define ENOCSI       50 /* No CSI structure available */

#define EL2HLT       51 /* Level 2 halted */

#define EBADE        52 /* Invalid exchange */

#define EBADR        53 /* Invalid request descriptor */

#define EXFULL       54 /* Exchange full */

#define ENOANO       55 /* No anode */

#define EBADRQC      56 /* Invalid request code */

#define EBADSLT      57 /* Invalid slot */

#define EDEADLOCK EDEADLK

#define EBFONT       59 /* Bad font file format */

#define ENOSTR       60 /* Device not a stream */

#define ENODATA      61 /* No data available */

#define ETIME        62 /* Timer expired */

#define ENOSR 63 /* Out of streams resources */

#define ENONET 64 /* Machine is not on the network */

#define ENOPKG 65 /* Package not installed */

#define EREMOTE 66 /* Object is remote */

#define ENOLINK 67 /* Link has been severed */

#define EADV 68 /* Advertise error */

#define ESRMNT 69 /* Srmount error */

#define ECOMM 70 /* Communication error on send */

#define EPROTO 71 /* Protocol error */

#define EMULTIHOP 72 /* Multihop attempted */

#define EDOTDOT 73 /* RFS specific error */

#define EBADMSG 74 /* Not a data message */

#define EOVERFLOW 75 /* Value too large for defined data type */

#define ENOTUNIQ 76 /* Name not unique on network */

#define EBADFD 77 /* File descriptor in bad state */

#define EREMCHG 78 /* Remote address changed */

#define ELIBACC 79 /* Can not access a needed shared library */

#define ELIBBAD 80 /* Accessing a corrupted shared library */

#define ELIBSCN 81 /* .lib section in a.out corrupted */

#define ELIBMAX 82 /* Attempting to link in too many shared libraries */

#define ELIBEXEC 83 /* Cannot exec a shared library directly */

#define EILSEQ 84 /* Illegal byte sequence */

#define ERESTART 85 /* Interrupted system call should be restarted */

#define ESTRPIPE 86 /* Streams pipe error */

#define EUSERS 87 /* Too many users */

#define ENOTSOCK 88 /* Socket operation on non-socket */

#define EDESTADDRREQ 89 /* Destination address required */

#define EMSGSIZE 90 /* Message too long */

#define EPROTOTYPE 91 /* Protocol wrong type for socket */

#define ENOPROTOOPT 92 /* Protocol not available */

#define EPROTONOSUPPORT 93 /* Protocol not supported */

#define ESOCKTNOSUPPORT 94 /* Socket type not supported */

#define EOPNOTSUPP 95 /* Operation not supported on transport endpoint */

#define EPFNOSUPPORT 96 /* Protocol family not supported */

#define EAFNOSUPPORT 97 /* Address family not supported by protocol */

#define EADDRINUSE 98 /* Address already in use */

#define EADDRNOTAVAIL 99 /* Cannot assign requested address */

#define ENETDOWN 100 /* Network is down */

#define ENETUNREACH 101 /* Network is unreachable */

#define ENETRESET 102 /* Network dropped connection because of reset */

#define ECONNABORTED 103 /* Software caused connection abort */

#define ECONNRESET 104 /* Connection reset by peer */

#define ENOBUFS 105 /* No buffer space available */

#define EISCONN 106 /* Transport endpoint is already connected */

#define ENOTCONN 107 /* Transport endpoint is not connected */

#define ESHUTDOWN 108 /* Cannot send after transport endpoint shutdown */

#define ETOOMANYREFS 109 /* Too many references: cannot splice */

#define ETIMEDOUT 110 /* Connection timed out */

#define ECONNREFUSED 111 /* Connection refused */

#define EHOSTDOWN 112 /* Host is down */

#define EHOSTUNREACH 113 /* No route to host */

#define EALREADY 114 /* Operation already in progress */

#define EINPROGRESS 115 /* Operation now in progress */

#define ESTALE 116 /* Stale NFS file handle */

#define EUCLEAN 117 /* Structure needs cleaning */

#define ENOTNAM 118 /* Not a XENIX named type file */

#define ENAVAIL 119 /* No XENIX semaphores available */

#define EISNAM 120 /* Is a named type file */

#define EREMOTEIO 121 /* Remote I/O error */

#define EDQUOT 122 /* Quota exceeded */

#define ENOMEDIUM 123 /* No medium found */

#define EMEDIUMTYPE 124 /* Wrong medium type */

/* sockios.h */
/* Socket configuration controls. */
#define SIOCSIFADDR 0x8916      /* set PA address       */
#define SIOCSIFNETMASK  0x891c      /* set network PA mask      */

/* route.h */
#define RTF_UP      0x0001      /* route usable         */
#define RTF_GATEWAY 0x0002      /* destination is a gateway */
#define RTF_HOST    0x0004      /* host entry (net otherwise)   */
#define RTF_REINSTATE   0x0008      /* reinstate route after tmout  */
#define RTF_DYNAMIC 0x0010      /* created dyn. (by redirect)   */
#define RTF_MODIFIED    0x0020      /* modified dyn. (by redirect)  */
#define RTF_MTU     0x0040      /* specific MTU for this route  */
#define RTF_MSS     RTF_MTU     /* Compatibility :-(        */
#define RTF_WINDOW  0x0080      /* per route window clamping    */
#define RTF_IRTT    0x0100      /* Initial round trip time  */
#define RTF_REJECT  0x0200      /* Reject route         */

/* sockios.h */
/* Routing table calls. */
#define SIOCADDRT   0x890B      /* add routing table entry  */
#define SIOCDELRT   0x890C      /* delete routing table entry   */
#define SIOCRTMSG   0x890D      /* call to routing system   */
/*
***********************************************************************************************************************
*                                数据结构定义
***********************************************************************************************************************
*/

/* posix_types.h */
#ifndef __kernel_long_t
typedef long        __kernel_long_t;
typedef unsigned long   __kernel_ulong_t;
#endif

#ifndef __kernel_suseconds_t
typedef __kernel_long_t     __kernel_suseconds_t;
#endif

typedef __kernel_long_t __kernel_time_t;

/* types.h */
typedef unsigned int    size_t;
typedef unsigned int    __kernel_size_t;
typedef __kernel_size_t size_t;
typedef size_t          socklen_t;

typedef int               __kernel_ssize_t;
typedef __kernel_ssize_t  ssize_t;

typedef long        __kernel_long_t;
typedef __kernel_long_t __kernel_off_t;
typedef __kernel_off_t      off_t;

#define __FD_SETSIZE    1024
typedef struct {
    unsigned long fds_bits[__FD_SETSIZE / (8 * sizeof(long))];
} __kernel_fd_set;

typedef __kernel_fd_set     fd_set;

#ifndef _TIME_T
#define _TIME_T
typedef __kernel_time_t     time_t;
#endif

typedef __kernel_suseconds_t    suseconds_t;

/* socket.h */
typedef unsigned short __kernel_sa_family_t;
typedef __kernel_sa_family_t    sa_family_t;
struct sockaddr {
    sa_family_t sa_family;  /* address family, AF_xxx   */
    char        sa_data[14];    /* 14 bytes of protocol address */
};

struct timeval {
    __kernel_time_t     tv_sec;     /* seconds */
    __kernel_suseconds_t    tv_usec;    /* microseconds */
};

/* int-l64.h */
typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

typedef __signed__ long __s64;
typedef unsigned long __u64;

/* types.h */
#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif
#ifdef __CHECK_ENDIAN__
#define __bitwise __bitwise__
#else
#define __bitwise
#endif

typedef __u16 __bitwise __le16;
typedef __u16 __bitwise __be16;
typedef __u32 __bitwise __le32;
typedef __u32 __bitwise __be32;
typedef __u64 __bitwise __le64;
typedef __u64 __bitwise __be64;

typedef __u16 __bitwise __sum16;
typedef __u32 __bitwise __wsum;

/* compiler.h */
#ifdef __CHECKER__
# define __user     __attribute__((noderef, address_space(1)))
# define __kernel   __attribute__((address_space(0)))
# define __safe     __attribute__((safe))
# define __force    __attribute__((force))
# define __nocast   __attribute__((nocast))
# define __iomem    __attribute__((noderef, address_space(2)))
# define __must_hold(x) __attribute__((context(x,1,1)))
# define __acquires(x)  __attribute__((context(x,0,1)))
# define __releases(x)  __attribute__((context(x,1,0)))
# define __acquire(x)   __context__(x,1)
# define __release(x)   __context__(x,-1)
# define __cond_lock(x,c)   ((c) ? ({ __acquire(x); 1; }) : 0)
# define __percpu   __attribute__((noderef, address_space(3)))
#ifdef CONFIG_SPARSE_RCU_POINTER
# define __rcu      __attribute__((noderef, address_space(4)))
#else
# define __rcu
#endif
extern void __chk_user_ptr(const volatile void __user *);
extern void __chk_io_ptr(const volatile void __iomem *);
#else
# define __user
# define __kernel
# define __safe
# define __force
# define __nocast
# define __iomem
# define __chk_user_ptr(x) (void)0
# define __chk_io_ptr(x) (void)0
# define __builtin_warning(x, y...) (1)
# define __must_hold(x)
# define __acquires(x)
# define __releases(x)
# define __acquire(x) (void)0
# define __release(x) (void)0
# define __cond_lock(x,c) (c)
# define __percpu
# define __rcu
#endif

/* in.h */
/* Internet address. */
struct in_addr {
    __be32  s_addr;
};

/* Structure describing an Internet (IP) socket address. */
#define __SOCK_SIZE__   16      /* sizeof(struct sockaddr)  */
struct sockaddr_in {
  __kernel_sa_family_t  sin_family; /* Address family       */
  __be16        sin_port;   /* Port number          */
  struct in_addr    sin_addr;   /* Internet address     */

  /* Pad to size of `struct sockaddr'. */
  unsigned char     __pad[__SOCK_SIZE__ - sizeof(short int) -
            sizeof(unsigned short int) - sizeof(struct in_addr)];
};
#define sin_zero    __pad       /* for BSD UNIX comp. -FvK  */

/* net.h */
enum sock_type {
    SOCK_STREAM = 1,
    SOCK_DGRAM  = 2,
    SOCK_RAW    = 3,
    SOCK_RDM    = 4,
    SOCK_SEQPACKET  = 5,
    SOCK_DCCP   = 6,
    SOCK_PACKET = 10,
};

/* if.h */
#define IFNAMSIZ    16
#define IFALIASZ    256

/* ioctl.h */
typedef struct {
    unsigned int clock_rate; /* bits per second */
    unsigned int clock_type; /* internal, external, TX-internal etc. */
    unsigned short loopback;
} sync_serial_settings;          /* V.35, V.24, X.21 */

typedef struct {
    unsigned int clock_rate; /* bits per second */
    unsigned int clock_type; /* internal, external, TX-internal etc. */
    unsigned short loopback;
    unsigned int slot_map;
} te1_settings;                  /* T1, E1 */

typedef struct {
    unsigned short encoding;
    unsigned short parity;
} raw_hdlc_proto;

typedef struct {
    unsigned int t391;
    unsigned int t392;
    unsigned int n391;
    unsigned int n392;
    unsigned int n393;
    unsigned short lmi;
    unsigned short dce; /* 1 for DCE (network side) operation */
} fr_proto;

typedef struct {
    unsigned int dlci;
} fr_proto_pvc;          /* for creating/deleting FR PVCs */

typedef struct {
    unsigned int dlci;
    char master[IFNAMSIZ];  /* Name of master FRAD device */
}fr_proto_pvc_info;     /* for returning PVC information only */

typedef struct {
    unsigned int interval;
    unsigned int timeout;
} cisco_proto;


/* if.h */
/*
 *  Device mapping structure. I'd just gone off and designed a
 *  beautiful scheme using only loadable modules with arguments
 *  for driver options and along come the PCMCIA people 8)
 *
 *  Ah well. The get() side of this is good for WDSETUP, and it'll
 *  be handy for debugging things. The set side is fine for now and
 *  being very small might be worth keeping for clean configuration.
 */

struct ifmap {
    unsigned long mem_start;
    unsigned long mem_end;
    unsigned short base_addr;
    unsigned char irq;
    unsigned char dma;
    unsigned char port;
    /* 3 bytes spare */
};

struct if_settings {
    unsigned int type;  /* Type of physical device or protocol */
    unsigned int size;  /* Size of the data allocated by the caller */
    union {
        /* {atm/eth/dsl}_settings anyone ? */
        raw_hdlc_proto      __user *raw_hdlc;
        cisco_proto     __user *cisco;
        fr_proto        __user *fr;
        fr_proto_pvc        __user *fr_pvc;
        fr_proto_pvc_info   __user *fr_pvc_info;

        /* interface settings */
        sync_serial_settings    __user *sync;
        te1_settings        __user *te1;
    } ifs_ifsu;
};

/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */

struct ifreq {
#define IFHWADDRLEN 6
    union
    {
        char    ifrn_name[IFNAMSIZ];        /* if name, e.g. "en0" */
    } ifr_ifrn;

    union {
        struct  sockaddr ifru_addr;
        struct  sockaddr ifru_dstaddr;
        struct  sockaddr ifru_broadaddr;
        struct  sockaddr ifru_netmask;
        struct  sockaddr ifru_hwaddr;
        short   ifru_flags;
        int ifru_ivalue;
        int ifru_mtu;
        struct  ifmap ifru_map;
        char    ifru_slave[IFNAMSIZ];   /* Just fits the size */
        char    ifru_newname[IFNAMSIZ];
        void __user *   ifru_data;
        struct  if_settings ifru_settings;
    } ifr_ifru;
};

/* route.h */
/* This structure gets passed by the SIOCADDRT and SIOCDELRT calls. */
struct rtentry {
    unsigned long   rt_pad1;
    struct sockaddr rt_dst;     /* target address       */
    struct sockaddr rt_gateway; /* gateway addr (RTF_GATEWAY)   */
    struct sockaddr rt_genmask; /* target network mask (IP) */
    unsigned short  rt_flags;
    short       rt_pad2;
    unsigned long   rt_pad3;
    void        *rt_pad4;
    short       rt_metric;  /* +1 for binary compatibility! */
    char __user *rt_dev;    /* forcing the device at add    */
    unsigned long   rt_mtu;     /* per route MTU/Window     */
#ifndef __KERNEL__
#define rt_mss  rt_mtu          /* Compatibility :-(            */
#endif
    unsigned long   rt_window;  /* Window clamping      */
    unsigned short  rt_irtt;    /* Initial RTT          */
};

/* termbits.h */
typedef unsigned char   cc_t;
typedef unsigned int    speed_t;
typedef unsigned int    tcflag_t;

#define NCCS 19
struct termios {
    tcflag_t c_iflag;       /* input mode flags */
    tcflag_t c_oflag;       /* output mode flags */
    tcflag_t c_cflag;       /* control mode flags */
    tcflag_t c_lflag;       /* local mode flags */
    cc_t c_line;            /* line discipline */
    cc_t c_cc[NCCS];        /* control characters */
};

/* c_cc characters */
#define VINTR 0
#define VQUIT 1
#define VERASE 2
#define VKILL 3
#define VEOF 4
#define VTIME 5
#define VMIN 6
#define VSWTC 7
#define VSTART 8
#define VSTOP 9
#define VSUSP 10
#define VEOL 11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE 14
#define VLNEXT 15
#define VEOL2 16

/* c_iflag bits */
#define IGNBRK  0000001
#define BRKINT  0000002
#define IGNPAR  0000004
#define PARMRK  0000010
#define INPCK   0000020
#define ISTRIP  0000040
#define INLCR   0000100
#define IGNCR   0000200
#define ICRNL   0000400
#define IUCLC   0001000
#define IXON    0002000
#define IXANY   0004000
#define IXOFF   0010000
#define IMAXBEL 0020000
#define IUTF8   0040000

/* c_oflag bits */
#define OPOST   0000001
#define OLCUC   0000002
#define ONLCR   0000004
#define OCRNL   0000010
#define ONOCR   0000020
#define ONLRET  0000040
#define OFILL   0000100
#define OFDEL   0000200
#define NLDLY   0000400
#define   NL0   0000000
#define   NL1   0000400
#define CRDLY   0003000
#define   CR0   0000000
#define   CR1   0001000
#define   CR2   0002000
#define   CR3   0003000
#define TABDLY  0014000
#define   TAB0  0000000
#define   TAB1  0004000
#define   TAB2  0010000
#define   TAB3  0014000
#define   XTABS 0014000
#define BSDLY   0020000
#define   BS0   0000000
#define   BS1   0020000
#define VTDLY   0040000
#define   VT0   0000000
#define   VT1   0040000
#define FFDLY   0100000
#define   FF0   0000000
#define   FF1   0100000

/* c_cflag bit meaning */
#define CBAUD   0010017
#define  B0 0000000     /* hang up */
#define  B50    0000001
#define  B75    0000002
#define  B110   0000003
#define  B134   0000004
#define  B150   0000005
#define  B200   0000006
#define  B300   0000007
#define  B600   0000010
#define  B1200  0000011
#define  B1800  0000012
#define  B2400  0000013
#define  B4800  0000014
#define  B9600  0000015
#define  B19200 0000016
#define  B38400 0000017
#define EXTA B19200
#define EXTB B38400
#define CSIZE   0000060
#define   CS5   0000000
#define   CS6   0000020
#define   CS7   0000040
#define   CS8   0000060
#define CSTOPB  0000100
#define CREAD   0000200
#define PARENB  0000400
#define PARODD  0001000
#define HUPCL   0002000
#define CLOCAL  0004000
#define CBAUDEX 0010000
#define    BOTHER 0010000
#define    B57600 0010001
#define   B115200 0010002
#define   B230400 0010003
#define   B460800 0010004
#define   B500000 0010005
#define   B576000 0010006
#define   B921600 0010007
#define  B1000000 0010010
#define  B1152000 0010011
#define  B1500000 0010012
#define  B2000000 0010013
#define  B2500000 0010014
#define  B3000000 0010015
#define  B3500000 0010016
#define  B4000000 0010017
#define CIBAUD    002003600000  /* input baud rate */
#define CMSPAR    010000000000  /* mark or space (stick) parity */
#define CRTSCTS   020000000000  /* flow control */

#define IBSHIFT   16        /* Shift from CBAUD to CIBAUD */

/* c_lflag bits */
#define ISIG    0000001
#define ICANON  0000002
#define XCASE   0000004
#define ECHO    0000010
#define ECHOE   0000020
#define ECHOK   0000040
#define ECHONL  0000100
#define NOFLSH  0000200
#define TOSTOP  0000400
#define ECHOCTL 0001000
#define ECHOPRT 0002000
#define ECHOKE  0004000
#define FLUSHO  0010000
#define PENDIN  0040000
#define IEXTEN  0100000
#define EXTPROC 0200000

/* tcflow() and TCXONC use these */
#define TCOOFF      0
#define TCOON       1
#define TCIOFF      2
#define TCION       3

/* tcflush() and TCFLSH use these */
#define TCIFLUSH    0
#define TCOFLUSH    1
#define TCIOFLUSH   2

/* tcsetattr uses these */
#define TCSANOW     0
#define TCSADRAIN   1
#define TCSAFLUSH   2


/*
***********************************************************************************************************************
*                                全局函数声明
***********************************************************************************************************************
*/

/* c library: string.h */
void* memset(void *block, int c, __kernel_size_t size);
void* memcpy(void *dst, const void *src, __kernel_size_t size);
char* strncpy(char *dst, const char *src, __kernel_size_t size );
int strncmp(const char *s1,const char *s2,__kernel_size_t size);
int sprintf( char *buffer, const char *format, ... /* arg */ );
__kernel_size_t strlen(const char *s);

/* socket */
int socket(int namespace, int style, int protocol);
int close(int filedes);
int shutdown(int socket, int how);
int bind(int socket, struct sockaddr *addr, socklen_t length);
int listen(int socket, int n);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int connect(int socket, struct sockaddr *addr, socklen_t length);
int recvfrom(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *addrlen);
int recv(int socket, void *buffer, size_t size, int flags);
int send(int socket, const void *buffer, size_t size, int flags);
int sendto(int socket, const void *buffer, size_t size, int flags,
           const struct sockaddr *addr, socklen_t length);
int fcntl(int filedes, int command, ... /* arg */ );
int ioctl(int fd, unsigned long request, ... /* arg */ );
int setsockopt(int socket, int level, int optname, void *optval, socklen_t optlen);
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint32_t hostshort);
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);

/* file */
int open(const char *pathname, int flags, ... /* arg */);
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);
int unlink(const char *pathname);

/* serial */
int tcgetattr(int filedes, struct termios *termios);
int cfsetispeed(struct termios *termios, speed_t speed);
int cfsetospeed(struct termios *termios, speed_t speed);
int tcflush(int filedes, int queue);
int tcsetattr(int filedes, int when, const struct termios *termios);

extern int errno;

#endif /*CMLINUX_H_*/

/*
***********************************************************************************************************************
                                 文件结束
***********************************************************************************************************************
*/


