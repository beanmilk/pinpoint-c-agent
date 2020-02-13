﻿#include <gtest/gtest.h>
#include "TransLayer.h"
#include <unistd.h>
#include <errno.h>
#include<functional>
#include<iostream>
using namespace testing;


#define unix_socket  "./pinpoint_test.sock"
const static char* g_msg = "Hello pinpoint!!!";
bool run = true;

int facke_server()
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un address = {0};
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, unix_socket, sizeof(address.sun_path) -1);
    remove(unix_socket);
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        EXPECT_TRUE(0)<<"bind server socket failed"<<strerror(errno);
    }

    listen(fd, 10);
    while (true)
    {
        char buffer[1024]={0};
        struct sockaddr addr;
        socklen_t addrlen;
        int cfd = accept(fd, &addr, &addrlen);
        pp_trace("recv cfd:%d",cfd);

        strcpy((char*)buffer+sizeof(Header),g_msg);
        int len = strlen(g_msg);
        Header* header = (Header*)buffer;
        header->type = htonl(RESPONSE_AGENT_INFO);
        header->length = htonl(len);
        int ret = send(cfd,buffer,len+sizeof(*header),0);
        pp_trace("send[%d] %s [%d] %s ",cfd,buffer,ret,strerror(errno));
        sleep(2);
        close(cfd);
    }
    
}

void handle_agent_info(int type,const char* buf,size_t len)
{
    printf("%s",buf);
    EXPECT_EQ(type,RESPONSE_AGENT_INFO);
    EXPECT_STREQ(buf,g_msg);
    run = false;
}



TEST(translayer, unix_socket_layer)
{
    pid_t pid = fork();
    if(pid == 0){
        facke_server();
        exit(0);
    }

    TransLayer layer("unix:" unix_socket,10);
    using namespace std::placeholders;
    layer.registerPeerMsgCallback(std::bind(handle_agent_info,_1,_2,_3));
    while(run){
        layer.trans_layer_pool();
    }

    kill(pid,SIGQUIT);
    waitpid(pid,0,0);

}