#include <curl/curl.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sftp.h"
#include "struct.h"
#include "thread.h"


/*
#define SIZE 10

#define NUM_URLS sizeof(urls)/sizeof(char *)

static size_t write_cb(char *data, size_t n, size_t l, void *userp)
{
  // take care of the data here, ignored in this example
  (void)data;
  (void)userp;
  return n*l;
}

static void add_transfer(CURLM *cm, int i, int *left)
{
  CURL *eh = curl_easy_init();
  curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(eh, CURLOPT_URL, urls[i]);
  curl_easy_setopt(eh, CURLOPT_PRIVATE, urls[i]);
  curl_multi_add_handle(cm, eh);
  (*left)++;
}
*/

curl_t allocate_memory()
{
  curl_t mem;

  if((mem = malloc(sizeof(curl_t))) == NULL)
  {
    printf("coult not allocate memory");
    exit(EXIT_FAILURE);
  }

  return mem;
}

int main(void)
{
  pthread_t id;
  pthread_t thread1, thread2;

  void *result;

  int pc, pj;

  curl_t url = allocate_memory();

  curl_global_init(CURL_GLOBAL_DEFAULT);

  if((pc = pthread_create(&thread1, NULL, thread_curl, &url)) != 0)
  {
    exit(EXIT_FAILURE);
  }

  /* Retreive thread id from utility function */
  id = thread_id();

  printf("[thread 1] %ld\n", pthread_self());
  printf("[thread_id main] %ld\n", (long)id);

  pj = pthread_join(thread1, &result);

  /*
  if(pthread_detach(thread1) != 0)
  {
    printf("pthread_detach(thread1)\n");
    exit(EXIT_FAILURE);
  }
  */

  if((pc = pthread_create(&thread2, NULL, thread_sftp, &url)) != 0)
  {
    exit(EXIT_FAILURE);
  }

  id = thread_id();

  printf("[thread 2] %ld\n", pthread_self());
  printf("[thread_id main] %ld\n", (long)id);

  pj = pthread_join(thread1, &result);

  /*
     unsigned int transfers = 0;
     int msgs_left = -1;
     int left = 0;

     curl_global_init(CURL_GLOBAL_ALL);
     cm = curl_multi_init();

     curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)SIZE);

     for(transfers = 0; transfers < SIZE && transfers < NUM_URLS;
     transfers++)
     add_transfer(cm, transfers, &left);

     do {
     int still_alive = 1;
     curl_multi_perform(cm, &still_alive);

     while((msg = curl_multi_info_read(cm, &msgs_left))) {
     if(msg->msg == CURLMSG_DONE) {
     char *url;
     CURL *e = msg->easy_handle;
     curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
     fprintf(stderr, "R: %d - %s <%s>\n",
     msg->data.result, curl_easy_strerror(msg->data.result), url);
     curl_multi_remove_handle(cm, e);
     curl_easy_cleanup(e);
     left--;
     }
     else {
     fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
     }
     if(transfers < NUM_URLS)
     add_transfer(cm, transfers++, &left);
     }
     if(left)
     curl_multi_wait(cm, NULL, 0, 1000, NULL);

     } while(left);

     curl_multi_cleanup(cm);
     curl_global_cleanup();
     */

  return 0;
}
