#include<copy.h>

void process_create(const char *srcfile,const char *destfile,int pronum,off_t blocksize){
	int i;
	pid_t pid = -1;

	/* 父进程先创建目标文件并截断到源文件大小,
	   也防止旧文件尾部残留 */
	int srcfd = open(srcfile,O_RDONLY);
	if(srcfd == -1){
		perror("open srcfile");
		return;
	}
	off_t filesize = lseek(srcfd,0,SEEK_END);
	close(srcfd);
	if(filesize == -1){
		perror("lseek");
		return;
	}
	int destfd = open(destfile,O_WRONLY|O_CREAT,0664);
	if(destfd == -1){
		perror("open destfile");
		return;
	}
	//提前一次性申请磁盘块，而不是进程写的时候一点点申请 拷贝过程更快
	if(ftruncate(destfd,filesize) == -1){
		perror("ftruncate destfile");
		close(destfd);
		return;
	}
	close(destfd);

	for(i=0;i<pronum;i++){
		pid=fork();
		if(pid==0) break;
		if(pid==-1){
			perror("fork");
			exit(1);
		}
	}
	if(pid>0){
		/* 父进程:子进程创建完毕,返回后由 main 统一回收 */
	}else if(pid==0){
		off_t offset = (off_t)i*blocksize;
		char  str_offset[32];
		char  str_blocksize[32];
		snprintf(str_offset,sizeof(str_offset),"%lld",(long long)offset);
		snprintf(str_blocksize,sizeof(str_blocksize),"%lld",(long long)blocksize);
		printf("pid: %d \n",getpid());
		fflush(stdout);/* execl 会替换进程映像,必须先刷掉缓冲区,否则重定向时这行会丢 */
		execl("/home/colin/five/process_copy/copy","copy",srcfile,destfile,str_offset,str_blocksize,(char*)NULL);
		perror("execl");
		exit(1);
	}else{
		fprintf(stderr,"没有创建任何进程\n");
		return;
	}


}
