#include <libc/rand.h>
#include <libc/shell.h>
#include <libc/cagboot.h>
#include <libc/stdio.h>

extern UINT32 sysPirGet (void);
extern int atoi(const char *str);
extern void test_test(void);


int cmd_random(int argc,char *argv[]);
int cmd_pir(int argc,char *argv[]);
int cmd_ostest(int argc,char *argv[]);


struct shell_cmd cmd_getpir= {
    .name = "pir",
    .help = "get core id",
    .usage = "pir",
    .handler = cmd_pir
};

struct shell_cmd cmd_rdm= {
    .name = "random",
    .help = "get random number",
    .usage = "random",
    .handler = cmd_random
};

struct shell_cmd cmd_ot= {
    .name = "ostest",
    .help = "os test",
    .usage = "ostest",
    .handler = cmd_ostest
};

int cmd_random(int argc,char *argv[])
{
    int ret=0;

    ret = rand();
    printf("%d\n",ret);
    return 0;
}

int cmd_pir(int argc,char *argv[])
{
    int ret = 0;

    printf("run in core %d\n",sysPirGet());

    return ret;
}

int cmd_ostest(int argc,char *argv[])
{
    int times=0,i;
    if (argc > 2 ) {
        printf("usage: ostest [times]\n");
        return -1;
    } else if (argc == 1) {
        times=1;
    } else {
        times=atoi(argv[1]);
        if(times<0) {
            printf("negative\n");
            return -1;
        } else if (times == 99999)
            times = 0xfffffff;
    }
    for(i=0; i<times; i++) {
        test_test();
    }
    return 0;
}
