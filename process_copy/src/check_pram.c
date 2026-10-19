#include<copy.h>

int check_pram(int argc,const char * srcfile,int pronum){
	if(argc < 4){
		return -1;//返回-1 表明参数不够
	}
	if((access(srcfile,F_OK))!=0){
		return -2;//返回-2 表明 源文件不存在
	}
	if(pronum < 1||pronum>100){
		return -3;//返回-3 表明进程数量不符合规定
	}
	return 0;//返回0 表明参数校验正常

}
