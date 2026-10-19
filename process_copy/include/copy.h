#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>


int check_pram(int,const char *,int);

//off_t 默认64位 为了解决大文件偏移
off_t block_cur(const char *,int);

void process_create(const char *,const char *,int ,off_t);

void process_wait();


