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

int QiwiApiGetProfile(char *token)
{  
  CURL *curl = curl_easy_init();
  CURLcode res;
  
  curl_data_t cdt;
  cdt.resp_data = malloc(1);
  cdt.resp_size = 0;
  int timeout = 0;
  
  if(curl == NULL)
  {
    printf("--> Curl init ERROR\n");
    exit(-1);
  }
  
  char bearer[128];
  char *url = "https://edge.qiwi.com/person-profile/v1/profile/current?authInfoEnabled=False&contractInfoEnabled=True&userInfoEnabled=False";
  
  sprintf(bearer, "Authorization: Bearer %s", token);  
  
  struct curl_slist *slist = NULL;
  
  slist = curl_slist_append(slist, "Accept: application/json");
  slist = curl_slist_append(slist, "Content-Type: application/json");
  slist = curl_slist_append(slist, "Content-Type: application/json; charset=UTF-8");
  slist = curl_slist_append(slist, bearer);
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
  curl_easy_setopt(curl, CURLOPT_HEADER, 0);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallBack);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&cdt);
  curl_easy_setopt(curl, CURLOPT_POST, 0);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (iPhone; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5");
  
  if(core.proxy != NULL)
    curl_easy_setopt(curl, CURLOPT_PROXY, core.proxy);
  else
    printf("-> WARNING: proxy is disabled\n");
  
  do
  {
    res = curl_easy_perform(curl);
    
    if(res != CURLE_OK)
    {
      timeout++;
      sleep(timeout);
    }
  }while(res != CURLE_OK);
  
  char *answer = malloc(strlen(cdt.resp_data) + 1);
  strcpy(answer, cdt.resp_data);
  
  if(slist != NULL)
    curl_slist_free_all(slist);
  
  curl_easy_cleanup(curl);
  
  if(strstr(answer, "contractInfo"))
    return 1;
  else
    return 0;
}
