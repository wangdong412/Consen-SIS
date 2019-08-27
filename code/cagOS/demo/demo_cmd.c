#include <libc/rand.h>
#include <libc/shell.h>
#include <libc/cagboot.h>
#include <libc/stdio.h>

extern int atoi(const char *str);

int demo_test(int argc,char *argv[]);

#define cmd_str "usage: demo_cmd <option>\n"

//定义shell 命令结构体
struct shell_cmd cmd_demo= {
    .name = "demo_cmd",
    .help = "demo",
    .usage = cmd_str,
    .handler = demo_test
};

//shell 命令实现函数
int demo_test(int argc,char *argv[])
{
    int i;
    if (argc != 2 ) {
        printf("%s",cmd_str);
        return -1;
    }
    i=atoi(argv[1]);
    switch(i) {
    case 0:
        nor_init_demo();
        do_demo_nor();
        break;
    case 1:
        iic_init_demo();
        test_iic_demo();
        break;
    case 2:
        spi_init_demo();
        spi_test_demo();
        break;
    case 3:
        test_sffs_demo();
        break;

    default :
        break;
    }
    return 0;
}

//出册shell 命令，调用该函数以后就可以在shell 里面执行命令demo_cmd
void demo_register(void)
{
    shellCmdRegister(&cmd_demo);
}
