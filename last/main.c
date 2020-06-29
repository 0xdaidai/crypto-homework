/*************************************************************************
 > File Name: last/main.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Mon 29 Jun 2020 01:46:17 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "aes.h"
#include "bloom.h"
#include "murmurhash2.h"
#include "file.h"

const unsigned char iv[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
const unsigned char K[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
int file_EN(char name[],BYTE K[],BYTE IV[]);
int make_BF(int total,double error,char name[],BYTE K[],BYTE IV[]);
void _bloom_write(struct bloom *bf, char *file);
void _bloom_read(struct bloom *bf, char *file);
void delete_file(const char *path);
void files_EN(const char *path);
int init(char *path);

int main(){
    WORD key_schedule[60];
    int word_len;
    init("test_dir");
    char recv_buf[BUFFER_SIZE] = {0};
    int clientSocket = connect_to_Server((char *)"39.96.57.195", SERVER_PORT);
    // int clientSocket = connect_to_Server((char *)"127.0.0.1", SERVER_PORT);
    if (clientSocket == -1)
        return 0;
    printf("已连接到服务器...\n");

    recv(clientSocket, recv_buf, BUFFER_SIZE, 0);
    printf("%s\n", recv_buf);
    memset(recv_buf,0,BUFFER_SIZE);
    sprintf(recv_buf,"%d",1);
    send(clientSocket,recv_buf,BUFFER_SIZE,0);
    submit_Files(clientSocket);
    printf("send over!\n");
    rename("resource","test_dir");

    sleep(1);
    memset(recv_buf,0,BUFFER_SIZE);
    sprintf(recv_buf,"%d",2);
    send(clientSocket,recv_buf,BUFFER_SIZE,0);
    memset(recv_buf,0,BUFFER_SIZE);
    scanf("%[^\n]%*c",recv_buf);
    word_len = strlen(recv_buf);
    word_len = word_len%16==0?word_len:(word_len/16*16+16);
    aes_encrypt_cbc((BYTE*)recv_buf,word_len,(BYTE*)recv_buf,key_schedule,256,iv);
    send(clientSocket,recv_buf,BUFFER_SIZE,0);
    
    receive_File(clientSocket);
    
    return 0;
}

int init(char *path){
    files_EN(path);
    delete_file("./resource");
    char tmp[1024];
    sprintf(tmp,"cp %s/*.enc %s/*.bf ./resource",path,path);
    system(tmp);
    return 1;
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

int file_EN(char name[],BYTE K[],BYTE IV[]){
    char name_enc[1024] = {0};
    memcpy(name_enc,name,strlen(name));
    strcpy(name_enc,".enc");
    FILE *fp = fopen(name,"rb");
    FILE *out = fopen(name_enc,"wb");
    BYTE buf[BUFFER_SIZE]; 
    BYTE res[BUFFER_SIZE]; 
    int len_file, len_block;
    int totalBlock;
    WORD key_schedule[60];
	aes_key_setup(K, key_schedule, 256);
    fseek(fp,0,2);//将指针放到文件末尾
    len_file = ftell(fp); //统计文本的数据长度
    rewind(fp); //让指针指向文件开始位置
    printf("文件的总长度len_file:%d\n",len_file);
    totalBlock = len_file % BUFFER_SIZE == 0 ? len_file / BUFFER_SIZE : ((len_file / BUFFER_SIZE) + 1); //除最后一快的总块数
    printf("文件的总块数totalBlock:%d\n",totalBlock);
    for(int i = 1; i <= totalBlock; i++) {
        memset(buf,0,BUFFER_SIZE);
        memset(res,0,BUFFER_SIZE);
        len_block = fread(buf, 1, BUFFER_SIZE, fp);
        buf[len_block]='\0';
        aes_encrypt_cbc(buf,BUFFER_SIZE,res,key_schedule,256,IV);
        fwrite(buf,BUFFER_SIZE,1,out);
    }
    printf("file: %s is ready\n",name_enc );
    return 1;
}

void files_EN(const char *path)
{
    DIR *dir;
    struct dirent *dir_info;
    char file_path[PATH_MAX];
    if(is_file(path))
    {
        file_EN(path,K,iv);
        make_BF(1000,0.001,path,K,iv);
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
            files_EN(file_path);
        }
    }
}


int make_BF(int total,double error,char name[],BYTE K[],BYTE IV[]){
    struct bloom bf;
    bloom_init(&bf,total,error);
    char name_bf[1024] = {0};
    memcpy(name_bf,name,strlen(name));
    strcpy(name_bf,".bf");
    FILE *fp = fopen(name,"rb");
    char buf[1024*1204]={0};
    BYTE res[80];
    int len_file, len_block;
    int totalBlock;
    WORD key_schedule[60];
    const char s[2] = " ";
    char *token;
	aes_key_setup(K, key_schedule, 256);
    fseek(fp,0,2);//将指针放到文件末尾
    len_file = ftell(fp); //统计文本的数据长度
    rewind(fp); //让指针指向文件开始位置
    printf("文件的总长度len_file:%d\n",len_file);
    fread(buf,1,len_file,fp);
    int i = 0;
    while(buf[i]!='\0'){
        if((buf[i]>='a'&&buf[i]<='z')||(buf[i]>='A'&&buf[i]<='Z'))  
            continue;
        else
            buf[i] = ' ';
    }
    token = strtok(buf, s);
    int word_len;
    //init bf
    bloom_init(&bf, total, error);
    while( token != NULL ) {
        memset(res,0,80);
        word_len = strlen(token);
        word_len = word_len%16==0?word_len:(word_len/16*16+16);
        aes_encrypt_cbc((BYTE*)token,word_len,res,key_schedule,256,IV);
        //add in bf
        bloom_add(&bf, res, word_len);
        token = strtok(NULL, s);
    }
    _bloom_write(&bf,name_bf);

    return 1;
}

void _bloom_write(struct bloom *bf, char *file)
{
    FILE *fp = fopen(file, "w");
    cJSON *cbf = cJSON_CreateObject();
    cJSON *tmp = cJSON_CreateArray();
    for (int i = 0; i < bf->bytes; i++)
        cJSON_AddNumberToObject(tmp, "...", bf->bf[i]);
    cJSON_AddNumberToObject(cbf, "entries", bf->entries);
    cJSON_AddNumberToObject(cbf, "error", bf->error);
    cJSON_AddNumberToObject(cbf, "bits", bf->bits);
    cJSON_AddNumberToObject(cbf, "bytes", bf->bytes);
    cJSON_AddNumberToObject(cbf, "hashes", bf->hashes);
    cJSON_AddNumberToObject(cbf, "bpe", bf->bpe);
    cJSON_AddItemToObject(cbf, "bf", tmp);
    cJSON_AddNumberToObject(cbf, "ready", bf->ready);
    char *str = cJSON_Print(cbf);
    fputs(str, fp);
    free(str);
    fclose(fp);
    return;
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

