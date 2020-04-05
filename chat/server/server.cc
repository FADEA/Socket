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

        cout<<"Client<<<"<<buff;
    }   
}

int main(){
    /*声明服务器地址和客户端链接地址 */
    struct sockaddr_in serveraddr,clientaddr;

    /*声明服务器监听套接字和客户端链接套接字 */
    int listenfd,connfd;
    pid_t childpid;
    pthread_t recv_t;

    /*缓存区 */
    char buff[MAX_LINE];

    socklen_t clilen;

    /*初始化监听套接字 */
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("listenfd socket error");
        exit(1);
    }
    // cout<<listenfd<<endl;
    /*设置服务器sockaddr_in结构 */
    bzero(&serveraddr,sizeof(serveraddr));

    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serveraddr.sin_port=htons(PORT);

    /*绑定服务器端套接字和端口 */
    if(bind(listenfd,(sockaddr*)&serveraddr,sizeof(serveraddr))<0){
        perror("bind error");
        exit(1);
    }

    /*监听客户端请求 */
    if(listen(listenfd,LISTENQ)<0){
        perror("listen error");
        exit(1);
    }

    clilen=sizeof(clientaddr);
    if((connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clilen))<0){
        perror("accept error");
        exit(1);
    }

    /*创建子线程接受客户端的消息 */
    if(pthread_create(&recv_t,NULL,recv_message,&connfd)==-1){
        perror("pthread create error");
        exit(1);
    }

    cout<<"get connection from "<<inet_ntoa(clientaddr.sin_addr)<<endl;

    /*接受客户端请求 */
    char msg[MAX_LINE];
    memset(msg,0,MAX_LINE);
    while(true){
        
        if(fgets(msg,MAX_LINE,stdin)==NULL) break;

        if(strcmp(msg,"exit\n")==0){
            cout<<"byebye."<<endl;
            memset(msg,0,MAX_LINE);
            strcpy(msg,"byebye.");
            send(connfd,msg,strlen(msg),0);
            close(connfd);
            exit(1);
        }

        if(send(connfd,msg,strlen(msg),0)<0){
            perror("send error.\n");
            exit(1);
        }

    }
    close(listenfd);
}