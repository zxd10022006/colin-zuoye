#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
	int pid;
	int i;

	int fds[2];
	if((pipe(fds))== -1) perror("pipe error");

	for(i=0;i<5;i++){
		pid = fork();
		if(pid == 0) break;

	}

	if(pid > 0){
		close(fds[1]);//close read
		char buf[4096];
		while(1){
			ssize_t n =read(fds[0],buf,sizeof(buf));
			if(n>0) {
				printf("recv from child: %s\n",buf);
			}else if(n ==0){
				printf("pipe close\n");
				break;
			}
			
		}

	}else if(pid == 0){
		close(fds[0]);
		char * msg="I living..\n";
		while(1){
			write(fds[1],msg,strlen(msg));
			sleep(1);
		}
		close(fds[1]);

	}else{
		printf("fork error\n");
		exit(0);
	}


}
