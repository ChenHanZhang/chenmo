#include "stdio.h"
#include "unistd.h"
#include "chenmo/net/InetAddress.h"
#include "chenmo/net/SocketsOps.h"
#include "chenmo/net/Socket.h"
#include "chenmo/net/EventLoop.h"
#include "chenmo/net/Acceptor.h"

using namespace chenmo;
using namespace chenmo::net;


void newConnection(int sockfd, const InetAddress& peerAdde)
{
    printf("newConnection(): accepted a new connection from %s\n",
        peerAdde.toIpPort().c_str());

    ssize_t r = ::write(sockfd, "How are you?\n", 13);
    if (r > 0)
    {
        printf("success!\n");
    }
    
    sockets::close(sockfd);
}

void newConnection2(int sockfd, const InetAddress& peerAdde)
{
    printf("newConnection(): accepted a new connection from %s\n",
        peerAdde.toIpPort().c_str());

    ssize_t r = ::write(sockfd, "Where are you?\n", 15);
    if (r > 0)
    {
        printf("ok!\n");
    }
    
    sockets::close(sockfd);
}

int main()
{
    //// 可以使用 nc localhost 2333 进行交互
    printf("main(): pid = %d\n", getpid());

    InetAddress listenAddr(2333);
    EventLoop loop;
    Acceptor acceptor(&loop, listenAddr, true);
    acceptor.setNewConnectionCallback(newConnection);
    acceptor.listen();

    InetAddress listenAddr2(6666);
    Acceptor acceptor2(&loop, listenAddr2, true);
    acceptor2.setNewConnectionCallback(newConnection2);
    acceptor2.listen();

    loop.loop();
}