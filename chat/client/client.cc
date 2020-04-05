#include "config.h"

/*讲fd中maxlen长度的字节读到vptr中 */
int readLine(int fd,char *vptr,int maxlen){
    int i,rc;
    char c,*ptr;

    ptr=vptr;
    for(i=0;i<maxlen;i++){
        rc=read(fd,&c,1);
        if(rc==1){
            *ptr++=c;
            if(c=='\n') break;
        }else if(rc==0){
            *ptr=0;
            return (i-1);
        }else{
            return -1;
        }
    }

    *ptr=0;
    return i;
}
int main(int argc,char** argv){

    int sockfd;
    struct sockaddr_in servaddr;

    if(argc!=2){
        perror("usage:tcpli <IPaddress>");
        exit(1);
    }
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);

    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<0){ //转换编码
        printf("转码失败");
        exit(1);
    }

    if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0){ //发送连接请求
        perror("连接失败");
        exit(1);
    }
    
    //消息处理
    char sendLine[MAX_LINE],recvLine[MAX_LINE];
    while(fgets(sendLine,MAX_LINE,stdin)!=NULL){

        write(sockfd,sendLine,strlen(sendLine));
        
        if(readLine(sockfd,recvLine,MAX_LINE)==0){
            perror("服务器终止");
            exit(1);
        }

        if(fputs(recvLine,stdout)==EOF){ /*输出从服务器接收的消息 */
            perror("输出错误");
            exit(1);
        }
    }

    close(sockfd); /*关闭套接字 */
}