#include "config.h"

/*接收服务器消息函数 */
void * recv_message(void *fd){
    int sockfd=*(int*) fd;
    while(true){
        char buff[MAX_LINE];
        memset(buff,0,MAX_LINE);
        int n;
        n=recv(sockfd,buff,MAX_LINE,0);
        if(n==-1){
            perror("recv error.\n");
            exit(1);
        }
        buff[n]='\0';

        if(strcmp(buff,"byebye.")==0){
            cout<<"Server is closed"<<endl;
            close(sockfd);
            exit(0);
        }

        cout<<"Server<<<"<<buff;
    }   
}
int main(int argc,char** argv){

    int sockfd;
    pthread_t recv_t;
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
    
    if(pthread_create(&recv_t,NULL,recv_message,&sockfd)==-1){
        perror("pthread create error");
        exit(1);
    }

    //消息处理
    char msg[MAX_LINE];
    memset(msg,0,MAX_LINE);
    char sendLine[MAX_LINE],recvLine[MAX_LINE];
    while(true){
        if(fgets(msg,MAX_LINE,stdin)==NULL) break;

        if(strcmp(msg,"exit\n")==0){
            cout<<"byebye."<<endl;
            memset(msg,0,MAX_LINE);
            strcpy(msg,"byebye.");
            send(sockfd,msg,strlen(msg),0);
            close(sockfd);
            exit(1);
        }

        if(send(sockfd,msg,strlen(msg),0)<0){
            perror("send error.\n");
            exit(1);
        }

    }

    close(sockfd); /*关闭套接字 */
}