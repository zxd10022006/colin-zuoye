#include<copy.h>

int main(int argc,char ** argv){
	if(argc < 5){
		fprintf(stderr,"usage: %s <srcfile> <destfile> <offset> <blocksize>\n",argv[0]);
		return 1;
	}
	off_t offset=(off_t)atoll(argv[3]);
	off_t blocksize=(off_t)atoll(argv[4]);

	int srcfd = open(argv[1],O_RDONLY);
	if(srcfd == -1){
		perror("open srcfile");
		return 1;
	}
	int destfd = open(argv[2],O_WRONLY|O_CREAT,0664);
	if(destfd == -1){
		perror("open destfile");
		return 1;
	}

	/* 跳到本进程负责的偏移处 */
	if(lseek(srcfd,offset,SEEK_SET) == -1){
		perror("lseek srcfd");
		return 1;
	}
	if(lseek(destfd,offset,SEEK_SET) == -1){
		perror("lseek destfd");
		return 1;
	}

	char buf[64*1024];
	off_t total=0;
	while(total < blocksize){
		size_t want=(size_t)(blocksize-total);
		if(want > sizeof(buf)) want=sizeof(buf);

		ssize_t len=read(srcfd,buf,want);
		if(len == -1){
			if(errno==EINTR) continue;
			perror("read");
			return 1;
		}
		if(len == 0) break; /* 已到文件尾(最后一块) */

		/* 处理短写 */
		char *p=buf;
		ssize_t left=len;
		while(left > 0){
			ssize_t n=write(destfd,p,left);
			if(n == -1){
				if(errno==EINTR) continue;
				perror("write");
				return 1;
			}
			p+=n;
			left-=n;
		}
		total+=len;
	}

	printf("offset:%lld ,block: %lld - %lld\n",
		(long long)offset,(long long)offset,(long long)(offset+total));
	return 0;
}
