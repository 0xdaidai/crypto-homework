/*************************************************************************
 > File Name: last/naim.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Mon 29 Jun 2020 07:06:04 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "file.h"

void delete_file(const char *path);

int main() {
    FILE *fp;
    int choice;
    int serverSocket;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    char send_buf[BUFFER_SIZE] = {0}, recv_buf[BUFFER_SIZE] = {0};

    serverSocket = init_Server(SERVER_PORT);
    if(serverSocket < 0) {
        printf("服务器初始化失败！\n");
        return 0;
    }
    printf("服务器初始化成功，监听端口：%d\n", SERVER_PORT);
    //等待用户连接
    client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
    if(client < 0) {
        printf("服务端accept失败，程序退出。\n");
        return 0;
    }

    printf("客户已连接：\n");
    printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));//inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
    printf("Port is %d\n", htons(clientAddr.sin_port));
    printf("等待消息...\n");
    sprintf(send_buf,"Hello, this is super invincible Lin\nYou have the following options:\n1: Send files\n2: Make a query\n0:Bye\n\nSo what do you need?\n");
    send(client,send_buf,BUFFER_SIZE,0);
    while(1){
        //将客户端发送过来的消息写入文件
        if((recv(client,recv_buf,BUFFER_SIZE ,0))!= -1)
	        printf("..........接收成功..........\n");
        else
	        printf("接收失败!\n");
        printf("choice: %s\n", recv_buf);
        choice = atoi(recv_buf);
        if(!choice)
            break;
        else if(choice == 1){
            receive_File(client);
        }
        else if(choice == 2){
            if((recv(client,recv_buf,BUFFER_SIZE ,0))!= -1)
	            printf("..........接收成功..........\n");
            else
	            printf("接收失败!\n");
            check_and_send(recv_buf);
        }
        else
            printf("Wrong msg!\n");
    }
    close(serverSocket);
    return 0;
}

void check_and_send(char s[])
{
    
}

//判断是否为目录
int is_dir(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//lstat返回文件的信息，文件信息存放在stat结构中
    {
        return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIR宏，判断文件类型是否为目录
    }
    return 0;
}

//判断是否为常规文件
int is_file(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)
        return S_ISREG(statbuf.st_mode) != 0;//判断文件是否为常规文件
    return 0;
}

//判断是否是特殊目录
int is_special_dir(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//生成完整的文件路径
void get_file_path(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

void delete_file(const char *path)
{
    DIR *dir;
    struct dirent *dir_info;
    char file_path[PATH_MAX];
    if(is_file(path))
    {
        remove(path);
        return;
    }
    if(is_dir(path))
    {
        if((dir = opendir(path)) == NULL)
            return;
        while((dir_info = readdir(dir)) != NULL)
        {
            get_file_path(path, dir_info->d_name, file_path);
            if(is_special_dir(dir_info->d_name))
                continue;
            delete_file(file_path);
            rmdir(file_path);
        }
    }
}

