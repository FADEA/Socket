#include "config.h"

int main(){
    /*声明服务器地址和客户端链接地址 */
    struct sockaddr_in serveraddr,clientaddr;

    /*声明服务器监听套接字和客户端链接套接字 */
    int listenfd,connfd;
    pid_t childpid;

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

    /*接受客户端请求 */
    while (true){
        clilen=sizeof(clientaddr);

        if(connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clilen)<0){
            perror("accpet error");
            exit(1);
        }

        childpid=fork();
        if(childpid==0){
            close(listenfd);
            ssize_t n;
            char buff[MAX_LINE];
            while((n=read(connfd,buff,MAX_LINE))>0){
                write(connfd,buff,n);
            }
            exit(0);
        }
        close(connfd);
    }
    close(listenfd);
}