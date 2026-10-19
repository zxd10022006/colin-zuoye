#include<copy.h>

int main(int argc,char **argv){
	if(argc < 4){
		fprintf(stderr,"usage: %s <srcfile> <destfile> <pronum>\n",argv[0]);
		return -1;
	}
	int pronum=atoi(argv[3]);
	if(pronum == 0){
		pronum=1;
	}
	int err = check_pram(argc,argv[1],pronum);
	if(err != 0){
		switch(err){
		case -1: fprintf(stderr,"参数不够\n"); break;
		case -2: fprintf(stderr,"源文件不存在\n"); break;
		case -3: fprintf(stderr,"进程数量应在1~100之间\n"); break;
		}
		return err;
	}

	off_t blocksize=block_cur(argv[1],pronum);
	if(blocksize < 0){
		return -1;
	}

	process_create(argv[1],argv[2],pronum,blocksize);

	process_wait();

	return 0;

}
