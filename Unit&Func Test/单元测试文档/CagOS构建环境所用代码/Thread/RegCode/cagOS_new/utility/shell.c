/* shell.c - minimal debug shell for boot loader */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:  shell cmd code for PPC85XX.
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 03jun15,Jeff created
 *
 * DESCRIPTION
 * This file contains shell library.
 */

#include <libc/cagboot.h>
#include <libc/string.h>
#include <libc/stdio.h>
#include <libc/shell.h>
#include <libc/assert.h>
#include <libc/ctype.h>
#include <libc/stddef.h>
#include <config.h>
#include <drv_config.h>


extern void fsCmdReg(void);

#define ETX 0x3  /* break, Ctrl + C */
#define BS  0x8  /* backspace, Ctrl + H */

/* shell command name max length */

#define SHELL_CMD_NAME_MAX_LEN 10

/* shell buffer length */

#define SHELL_CMD_LEN 256

extern void boardReset (void);
extern char ns16550InputChar (void);
extern void ns16550OutputChar (char c);
unsigned long strtoul(const char *, char **, int);

static char shellBuffer[SHELL_CMD_LEN];

#if 0
#define CMD_HISTORY     3
static char cmdHistory[CMD_HISTORY][SHELL_CMD_LEN];
static int  cmdNum=0;
static int  cmdTmp=0;
static int  cmdEndpos=0;
#endif
static int shellCounterPos;
static int shellCounter;
static int shellCmdNameMaxLen = 0;

static int shellParseAndExecute(void);

/* maximum supported shell commands */

#define MAX_SHELL_CMD 32
static const struct shell_cmd * __shell_cmds[MAX_SHELL_CMD];

/* These are shell built-in commands, no configuration options for built-in command */

static int help(int argc, char * argv[]);
static int hexdump(int argc, char * argv[]);
static int do_reset(int argc, char * argv[]);
static void shellPrompt(void);
static void shellInit(void);

static struct shell_cmd cmd_help = {
    .name =    "help",
    .help =    "print help message",
    .usage =   "help <cmd> - print command usage, if paramter <cmd> is null\n"\
    "             it will show the help messages of all the registered commands\n",
    .handler = help
};

static struct shell_cmd cmd_hexdump = {
    .name =    "dump",
    .help =    "display address content",
    .usage =   "dump  <addr>,<num>,<width>",
    .handler = hexdump
};

static struct shell_cmd cmd_reset = {
    .name =    "reset",
    .help =    "reset the board",
    .usage =   "call do_reset",
    .handler = do_reset
};

/*******************************************************************************
*
* shellGetChar - get a character from underlying device
*
* This routine gets a character from underlying device and puts it into
* shell's internal buffer.
*
* RETURNS: 1, if end of line(\n, \r or \n\r) or break(ctrl + c) is received
*          0, otherwise
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static unsigned char arrow[3]= {0};
static unsigned int arrow_pos = 0;

static int shellGetChar(void)
{
    char tmp = (char)ns16550InputChar();

    //check arrow button
    if (tmp==0x1b) {
        memset(arrow,0,sizeof(arrow));
        arrow[0]=tmp;
        arrow_pos=1;
        return 0;
    } else if (arrow_pos == 1 && tmp==0x5b) {
        arrow[1]=tmp;
        arrow_pos=2;
        return 0;
    } else if (arrow_pos == 2 && (tmp == 0x41|| tmp == 0x42 || tmp == 0x43||tmp == 0x44)) {
        memset(arrow,0,sizeof(arrow));
        tmp=tmp-0x41+0xfa;
        // return 0;
    } else {
        memset(arrow,0,sizeof(arrow));
        arrow_pos=0;
    }


    unsigned char tmpc;
    switch (tmp) {
    case 0xfa:
    case 0xfb:
        return 0;
    case 0xfc:
        //right
        if (shellCounterPos < shellCounter) {
            tmpc=shellBuffer[shellCounterPos];
            (void)ns16550OutputChar(tmpc);
            shellCounterPos++;
        }
        return 0;
    case 0xfd:
        //left
        if (shellCounterPos > 0) {
            shellCounterPos --;
            (void)ns16550OutputChar('\b');
        }
        return 0;

    case '\n':
    case '\r':
        (void)ns16550OutputChar('\r');
        (void)ns16550OutputChar('\n');
        shellCounterPos=0;
        return 1;
    case BS:
        if (shellCounter > 0) {
            shellCounter--;
            shellCounterPos=shellCounter;
            shellBuffer[shellCounter] = 0;
            (void)ns16550OutputChar('\b');
            (void)ns16550OutputChar(' ');
            (void)ns16550OutputChar('\b');
        }
        return 0;
    case ETX:
        //ctrl+c
        shellCounter = 0;
        shellCounterPos = 0;
        shellBuffer[0] = '\0';
        (void)ns16550OutputChar('\n');
        (void)ns16550OutputChar('\r');
        return 1;
    default:
        if (shellCounter >= (SHELL_CMD_LEN - 1))
            return 0;
        shellBuffer[shellCounter] = tmp;
        shellCounter++;
        shellCounterPos=shellCounter;
        /* coverity[negative_returns] */
        (void)ns16550OutputChar(tmp);
        return 0;
    }
    return 0;
}

/*******************************************************************************
*
* shellGetLine - read a line
*
* This routine reads a line into shell's internal buffer.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static void shellGetLine(void)
{
    while (shellGetChar() != 1)
        ;
}

/*******************************************************************************
*
* shellExec - execute shell command
*
* This routine executes shell command according to the input parameter.
*
* RETURNS: 0 on the command executed correctly, -1 otherwise
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static int shellExec
(
    int argc,
    char *argv[]
)
{
    int i, ret;

    for (i = 0; i < MAX_SHELL_CMD; i++) {
        const struct shell_cmd *c = __shell_cmds[i];
        if (c && !strcmp(argv[0], c->name)) {
            ret = c->handler(argc, argv);
            if (ret) {
                (void)printf("error executing command: %s\n", argv[0]);
                return -1;
            } else
                break;
        }
    }
    if (i == MAX_SHELL_CMD) {
        (void)printf("invalid command %s\n", argv[0]);
        return -1;
    }
    return 0;
}

/*******************************************************************************
*
* shellParseAndExecute - parse and execute shell command
*
* This routine parses the line read and executes shell command accordingly.
*
* RETURNS: 0 is all commands are executed correctly, -1 otherwise
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static int shellParseAndExecute(void)
{
    int argc;
    char *cmd, *t, *s, *ss;
    char *argv[32];

    if (shellCounter <= 0)
        return -1;

    s = shellBuffer;
    while ((*s != '\0') && (isspace(*s) || (*s == ';')))
        s++;

    if (*s == '\0')
        return 0;

    for (;;) {
        ss = s;
        while ((*s != '\0') && (*s != ';')) {
            if (*s == '"') {
                ++s;
                while ((*s != '\0') && (*s != '"'))
                    s++;
                if (*s == '\0') {
                    (void)printf("unbalanced string quote\n");
                    return -1;
                }
            }
            s++;
        }

        if (*s != '\0')
            *s++ = '\0';

        cmd = ss;
        for (argc = 0;;) {
            while ((*cmd != '\0') && (cmd < s) && isspace(*cmd))
                cmd++;

            if ((*cmd == '\0') || cmd >= s)
                break;

            t = cmd;
            while ((*cmd != '\0') && !isspace(*cmd) && (cmd < s)) {
                if (*cmd == '"') {
                    cmd++;
                    while ((*cmd != '\0') && (*cmd != '"'))
                        cmd++;
                    if (*cmd == '\0') {
                        (void)printf("unbalanced string quote\n");
                        return -1;
                    }
                }
                cmd++;
            }

            if (isspace(*cmd))
                *cmd++ = '\0';

            /* remove the quote mark if needed */

            if (*t == '"') {
                *t++ = ' ';
                t[strlen(t) - 1] = '\0';
            }

            argv[argc++] = t;
        }

        if (shellExec(argc, argv) < 0)
            return -1;

        if (*s == '\0')
            break;
    }
    return 0;
}

/*******************************************************************************
*
* shellPrompt - output shell prompt
*
* This routine prints shell prompt
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static void shellPrompt(void)
{
    (void)printf(_CAG_CONFIG_SHELL_PROMPT);
}

/*******************************************************************************
*
* shellInit - initialize shell buffer
*
* This routine initializes shell's internal buffer
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static void shellInit(void)
{
    shellCounter = 0;
    memset(shellBuffer, 0, SHELL_CMD_LEN);
}

/*******************************************************************************
*
* shellMainLoop - shell main loop
*
* This is the shell main loop routine
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static void shellMainLoop(void)
{
    for (;;) {
        shellInit();
        shellPrompt();
        shellGetLine();
        (void)shellParseAndExecute();
    }
}

/*******************************************************************************
*
* shellCmdRegister - register a shell command
*
* This routine registers a shell command
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void shellCmdRegister
(
    const struct shell_cmd * cmd
)
{
    int i, len;

    if (cmd) {
        assert(cmd->name);
        assert(cmd->handler);

        len = strlen(cmd->name);
        if (len > MAX_SHELL_CMD) {
            (void)printf("warnings - cmd name length exceeds maximum: %s, the allowed length is %u\n",
                         cmd->name, SHELL_CMD_NAME_MAX_LEN);
            return;
        }
        if (len > shellCmdNameMaxLen)
            shellCmdNameMaxLen = len;

        for (i = 0; i < MAX_SHELL_CMD; i++) {
            if (__shell_cmds[i] == NULL) {
                __shell_cmds[i] = cmd;
                return;
            }
        }
    }
    (void)printf("warning - shell cmd %s not registered\n",
                 cmd == NULL ? "NULL" : cmd->name);
}

/*******************************************************************************
*
* do_reset - shell's built-in 'reset' command
*
* This routine reset the target
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static int do_reset(int argc, char * argv[])
{
    printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
    printf("-> You already reset the target by youself:)\n");
    printf("-> will enter the U-Boot env\n");
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
    boardReset();
    return 0;
}

/*******************************************************************************
*
* hexdump - shell's built-in 'd' command
*
* This routine dump the memory contents
*
* RETURNS: 0 on success, or -1 otherwise
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static int hexdump
(
    int argc,
    char * argv[]
)
{
    int i, j;
    char *sep, *cmd, *start, *end;
    size_t bytes;

    static char *addr = NULL;
    static UINT32 num = 32;
    static UINT32 width = 4;

    if (argc >= 2) {
        cmd = strtok_r(argv[1], ", ", &sep);
        if (!cmd)
            return 0;
        addr = (void *)(UINT32)strtoul(cmd, NULL, 16);
        cmd = strtok_r(NULL, ", ", &sep);
        if (cmd)
            num = strtoul(cmd, NULL, 10);
        else
            num = 4;
        cmd = strtok_r(NULL, ", ", &sep);
        if (cmd)
            width = strtoul(cmd, NULL, 10);
        else
            width = 1;
    }

    if (addr == NULL) {
        (void)(void)printf("Trying to access NULL\n");
        return -1;
    }

    switch (width) {
    case 1:
        break;
    case 2:
        if ((UINT32)addr & 0x1)
            return -1;
        break;
    case 4:
        if ((UINT32)addr & 0x3)
            return -1;
        break;
    default:
        (void)(void)printf("Accepted with: 1, 2, 4\n");
        return -1;
    }

    bytes = num * width;
    start  = (void *)((UINT32)addr &~ 15);
    end = (void *)(UINT32)ROUND_UP(((UINT32)((char *)addr + bytes )), 16);
    end = (char *)addr + bytes;
    bytes = ROUND_UP((end - start), 16);

    if (width == 1) {
        for (i = 0; i < bytes / 16; i++) {
            cmd = start;
            (void)printf("0x%x: ", start);
            for (j = 0; j < 16; j++) {
                if (start < addr || start >= end)
                    (void)printf("   ");
                else
                    (void)printf("%02x ", *(UINT8 *)start);
                start = __address_add_offset(start, 1);
            }

            for (j = 0; j < 16; j++) {
                if (cmd < addr || cmd > end)
                    (void)printf(" ");
                else if (isprint(*cmd))
                    (void)printf("%c", *cmd);
                else
                    (void)printf(".");
                cmd++;
            }
            (void)printf("\n");
        }
    } else if (width == 2) {
        for (i = 0; i < bytes / 16; i++) {
            cmd = start;
            (void)printf("0x%x: ", start);
            for (j = 0; j < 8; j++) {
                if (start < addr || start >= end)
                    (void)printf("     ");
                else
                    (void)printf("%04x ", *(UINT16 *)start);
                start = __address_add_offset(start, 2);
            }

            for (j = 0; j < 16; j++) {
                if (cmd < addr || cmd > end)
                    (void)printf(" ");
                else if (isprint(*cmd))
                    (void)printf("%c", *cmd);
                else
                    (void)printf(".");
                cmd++;
            }
            (void)printf("\n");
        }
    } else {
        for (i = 0; i < bytes / 16; i++) {
            cmd = start;
            (void)printf("0x%x: ", start);
            for (j = 0; j < 4; j++) {
                if (start < addr || start >= end)
                    (void)printf("         ");
                else
                    (void)printf("%08x ", *(UINT32 *)start);
                start = __address_add_offset(start, 4);
            }

            for (j = 0; j < 16; j++) {
                if (cmd < addr || cmd > end)
                    (void)printf(" ");
                else if (isprint(*cmd))
                    (void)printf("%c", *cmd);
                else
                    (void)printf(".");
                cmd++;
            }
            (void)printf("\n");
        }
    }

    addr = __address_add_offset(addr, num * width);
    return 0;
}

/*******************************************************************************
*
* help - shell's built-in help command
*
* This routine implements shell's built-in help command. The help command
* is always available.
*
* RETURNS: 0, always
*
* ERRNO: N/A
*
* \NOMANUAL
*/

static int help
(
    int argc,
    char * argv[]
)
{
    int i, j, len;
    char * cmd = NULL;

    if (argc >= 2)
        cmd = argv[1];

    if (!cmd) {
        for (i = 0; i < MAX_SHELL_CMD; i++) {
            if (__shell_cmds[i]) {
                len = (int)strlen(__shell_cmds[i]->name);
                j = shellCmdNameMaxLen - len;
                while (j-- > 0)
                    (void)ns16550OutputChar(' ');

                (void)printf("->?%s - %s\n", __shell_cmds[i]->name,
                             __shell_cmds[i]->help == NULL ? "NULL" :
                             __shell_cmds[i]->help);
            }
        }
    } else {
        for (i = 0; i < MAX_SHELL_CMD; i++) {
            if (__shell_cmds[i] && strcmp(cmd, __shell_cmds[i]->name) == 0) {
                if (__shell_cmds[i]->usage)
                    (void)printf("%s\n", __shell_cmds[i]->usage);
                break;
            }
        }
        if (i >= MAX_SHELL_CMD)
            (void)printf("Unknown command: %s\n", cmd);
    }
    return 0;
}

/*******************************************************************************
*
* usrShell - start shell
*
* This routine starts the debug shell. This is called from usrRoot.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

extern struct shell_cmd cmd_ot;
void usrShell(void)
{
    shellCmdRegister(&cmd_help);
    shellCmdRegister(&cmd_hexdump);
    shellCmdRegister(&cmd_reset);
#if 0
    shellCmdRegister(&cmd_runtime);
    shellCmdRegister(&cmd_task);
#endif

#ifdef CONFIG_FS
    fsCmdReg();
#endif
#ifdef CONFIG_TEST
    shellCmdRegister(&cmd_ot);
#endif

    shellMainLoop();
}
