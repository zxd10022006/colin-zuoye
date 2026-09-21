#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<regex.h>
int main(int argc,char * argv[]){
		if(argc<2){
			printf("缺少参数/n");
			return -1;
			}
		//处理映射
		
		int fd = open(argv[1],O_RDONLY);

		if(fd==-1){
			perror("open fail\n");
			return -1;
			}
		int fsize = lseek(fd,0,SEEK_END);

		char * mmap_ptr=NULL;
		mmap_ptr = mmap(NULL,fsize,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
		char * p= mmap_ptr;
		close(fd);
		//处理正则表达式

		char * regstr="<a[[:space:]]+[^>]*href=\"([^\"]+)\"[^>]*>([^<]*)<";
		regex_t reg;
		int ret=regcomp(&reg,regstr,REG_EXTENDED);
		if(ret != 0){
			char errbuf[1024];
			regerror(ret,&reg,errbuf,sizeof(errbuf));
			printf("正则编译失败: %s\n",errbuf);
			munmap(mmap_ptr,fsize);
			return -1;
			}

		//循环匹配
		int regnum = 3;
		regmatch_t match[regnum];
		char  addr[1024];
		char  title[1024];
		while((regexec(&reg,p,regnum,match,0))==0){
				
			snprintf(addr,sizeof(addr),"%.*s",match[1].rm_eo-match[1].rm_so,p+match[1].rm_so);
			
			snprintf(title,sizeof(title),"%.*s",match[2].rm_eo-match[2].rm_so,p+match[2].rm_so);

			if(p + match[0].rm_eo > mmap_ptr + fsize) break;

			p += match[0].rm_eo;
			
			printf("网址：%s    标题：%s\n",addr,title);

	
		}
		regfree(&reg);
	
	
		return 0;
	}
