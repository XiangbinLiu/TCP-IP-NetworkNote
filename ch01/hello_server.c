#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    //调用 socket 函数创建套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    //调用 bind 函数分配ip地址和端口号
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    //调用 listen 函数将套接字转为可接受连接状态
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    //调用 accept 函数受理连接请求。如果在没有连接请求的情况下调用该函数，则不会返回，直到有连接请求为止
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");
    //稍后要将介绍的 write 函数用于传输数据，若程序经过 accept 这一行执行到本行，则说明已经有了连接请求
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

//这是我读的第一篇代码，所以注释很多。
/**总的流程：创建IP地址，打开server文件。服务器文件绑定IP。开始监听
 * （listen and accept，在accept步阻塞）。在accpet打开了clinet文件。
 * 消息写入client文件。大功告成。（万物皆文件。）
 * （1）：关于socket函数：
 * domain:地址族（ipv4，ipv6，local）
 * type：面向连接或者不面向连接
 * protocal：填0，默认和type一致
 * 
 * （2）：关于socketaddr_in结构体：
 * struct sockaddr_in
  {
    __SOCKADDR_COMMON (sin_);   //地址族
    in_port_t sin_port;			//16位端口
    struct in_addr sin_addr;	//32位IP地址
    unsigned char sin_zero[sizeof (struct sockaddr)   //不用的区段
			   - __SOCKADDR_COMMON_SIZE
			   - sizeof (in_port_t)
			   - sizeof (struct in_addr)];
  };
  （3）：用于转化大小端的函数：（网络传输用大端，CPU用小端）
  hstonl：host to net long
  hstons：host to net short
  （4）：关于listen函数函数 listen(int sockfd, int backlog)：
        监听的对象，队列长度的log值
    
 * 
*/
