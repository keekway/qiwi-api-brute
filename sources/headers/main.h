#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <curl/curl.h>
#include <pthread.h>

typedef struct curl_data
{
  char *resp_data;
  int resp_size;
}curl_data_t; 

typedef struct core
{
  char *proxy;
  int threads;
}core_t;

core_t core;

void GetMyIpViaProxy(char *proxy);

int SaveDatas(char *action, char *data);
int CheckDublicateToken(char *token);
char *GenerateNewToken();
int QiwiApiGetProfile(char *token);

int MakeBrute();
