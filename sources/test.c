#include "headers/main.h"

static size_t CallBack(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  curl_data_t *cdt = (curl_data_t*)userp;
  
  cdt->resp_data = realloc(cdt->resp_data, cdt->resp_size + realsize + 1);
  if(cdt->resp_data == NULL)
  {
    printf("--> not enough memory (realloc returned NULL)\n");
    return 0;
  }
  
  memcpy(&(cdt->resp_data[cdt->resp_size]), contents, realsize);
  cdt->resp_size += realsize;
  cdt->resp_data[cdt->resp_size] = 0;
  
  return realsize;
} 

void GetMyIpViaProxy(char *proxy)
{  
  CURL *curl = curl_easy_init();
  CURLcode res;
  
  curl_data_t cdt;
  cdt.resp_data = malloc(1);
  cdt.resp_size = 0;
  
  curl_easy_setopt(curl, CURLOPT_URL, "http://ident.me");
  curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
  curl_easy_setopt(curl, CURLOPT_HEADER, 0);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallBack);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&cdt);
  curl_easy_setopt(curl, CURLOPT_POST, 0);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  
  do
  {
    //printf("--> Sending curl request\n");
    res = curl_easy_perform(curl);
  }while(res != CURLE_OK);
  
  printf("-> MyIP: %s\n", cdt.resp_data);
  
  curl_easy_cleanup(curl);
} 
