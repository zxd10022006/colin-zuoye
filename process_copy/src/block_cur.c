#include<copy.h>

off_t block_cur(const char * srcfile,int pronum){
	int fd = open(srcfile,O_RDONLY);
	if(fd == -1){
		perror("open srcfile");
		return -1;
	}
	off_t filesize=lseek(fd,0,SEEK_END);
	close(fd);
	if(filesize == -1){
		perror("lseek");
		return -1;
	}

	if((filesize%pronum)==0){
		return filesize/pronum;
	}else{
		return (filesize/pronum)+1;
	}
}
