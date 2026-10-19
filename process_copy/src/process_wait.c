#include<copy.h>

void process_wait(){
	pid_t zpid;
	int n=0;
	while((zpid=wait(NULL))>0) n++;
	printf("共回收 %d 个子进程\n",n);

}
