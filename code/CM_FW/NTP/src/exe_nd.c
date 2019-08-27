
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    char cmdline[100]={0};
    char pre[]="/home/ntp/ntpd -4d -P 110 -g -n -L -I ";/*启动命令*/
    char netdev[2][6]={"eth1 &","eth2 &"}; /*选择校时服务器监听的网口*/

    int32_t ret = 0;

    strncat(cmdline,pre,strlen(pre));
    strncat(cmdline,(char*)&netdev[1],6);
    printf("%s\n",cmdline);

    ret = system(cmdline); /*启动ntp校时服务器*/

    if(ret != 0){
        perror("start ntpd failed\n");
    }
    else{
        printf("exec ok %d\n",ret);
    }

    return 0;

    ret = system("pkill ntpd");/*停止ntp校时服务器*/
    if(ret !=0){
        perror("stop ntpd failed\n");
    }
    else{
        printf("exec ok2 %d\n",ret);
    }
    return 0;
}

