/*************************************************************************
 > File Name: last/server.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Mon 29 Jun 2020 07:06:04 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "bloom.h"
#include "murmurhash2.h"
#include "file.h"

void delete_file(const char *path);
int copy_file(char* srcPath,char *destPath);
void _bloom_read(struct bloom *bf, char *file);

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
            printf("word: %s\n", recv_buf);
            delete_file("./temp");
            check_and_send(recv_buf);
            printf("check over!\n");
            submit_Files(client,"./temp");
        }
        else
            printf("Wrong msg!\n");
    }
    close(serverSocket);
    return 0;
}

void check_and_send(char s[])
{
    char path[] = "./resource";
    char cp_path[1024];
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
            if(strstr(file_path,".bf") == NULL)
                continue;
            printf("check %s\n",file_path );
            struct bloom bf;
            _bloom_read(&bf,file_path);
            if (bloom_check(&bf, s, BUFFER_SIZE)){
                char dst[80] = "./temp/";
                strcat(dst,dir_info->d_name);
                int tmp = strlen(dst);
                dst[tmp-2] = 'e';
                dst[tmp-1] = 'n';
                dst[tmp] = 'c';
                printf("dst: %s\n",dst );
                tmp = strlen(file_path);
                memset(cp_path,0,1024);
                memcpy(cp_path,file_path,tmp-3);
                strcat(cp_path,".enc");
                copy_file(cp_path,dst);
            }
        }
    }

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


int copy_file(char* srcPath,char *destPath)
{
	char Buf[1024] = {0};
	int count_read = 0;
	long fp_src_ltell = 0,fp_src_atell = 0;
	FILE* fp_src = fopen(srcPath,"r");//只读方式打开
	FILE* fp_dst = fopen(destPath,"w");//只写方式打开
	if(fp_dst ==NULL || fp_src == NULL)
	{
		printf("文件打开有问题\n");
		return -1;
	}
	while(1)
	{
		memset(Buf,0,sizeof(Buf));
		fp_src_ltell = ftell(fp_src); //上一次文件指针位置
		count_read = fread(Buf,sizeof(Buf),1,fp_src);
		fp_src_atell = ftell(fp_src); //当前文件指针位置
		if(count_read<1) //异常或到达末尾结束
		{
			if(feof(fp_src))
			{
				long temp = fp_src_atell - fp_src_ltell;
				fwrite(Buf,temp,1,fp_dst); //成功
				return 0;
			}
			else if(ferror(fp_src))
			{
				perror("fread error:");
				return -1;
			}
		}
		fwrite(Buf,sizeof(Buf),1,fp_dst);
	}
	return 0;
}

void _bloom_read(struct bloom *bf, char *file)
{
    FILE *fp = fopen(file, "rb");
    int filesize = 0;
    struct stat buf;
    stat(file, &buf);
    filesize = buf.st_size;
    char *str = (char *)malloc(sizeof(char) * filesize);
    fread(str, 1, filesize, fp);
    cJSON *root = cJSON_Parse(str), *tmp, *t;
    tmp = cJSON_GetObjectItem(root, "bits");
    bf->bits = tmp->valueint;
    tmp = cJSON_GetObjectItem(root, "bpe");
    bf->bpe = tmp->valuedouble;
    tmp = cJSON_GetObjectItem(root, "bytes");
    bf->bytes = tmp->valueint;
    tmp = cJSON_GetObjectItem(root, "entries");
    bf->entries = tmp->valueint;
    tmp = cJSON_GetObjectItem(root, "error");
    bf->error = tmp->valuedouble;
    tmp = cJSON_GetObjectItem(root, "hashes");
    bf->hashes = tmp->valueint;
    tmp = cJSON_GetObjectItem(root, "ready");
    bf->ready = tmp->valueint;
    bf->bf = (unsigned char *)malloc(sizeof(char) * bf->bytes);
    t = cJSON_GetObjectItem(root, "bf");
    for (int i = 0; i < bf->bytes; i++)
    {
        tmp = cJSON_GetArrayItem(t, i);
        bf->bf[i] = tmp->valueint;
    }
    free(str);
    cJSON_Delete(root);
    fclose(fp);
    return;
}


