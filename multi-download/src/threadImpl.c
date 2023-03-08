#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "list.h"
#include "queue.h"
#include "sftp.h"
#include "stack.h"
#include "struct.h"
#include "thread.h"

struct URL
{
  CURL *curl;
  CURLM *cm;
  CURLMsg *message;
  CURLcode response;
  char *file;
  FILE *data;
  FILE *fp;
  int num;
};

void *thread_curl(void *arg)
{
  struct URL *url = (struct URL *)arg;

  url->curl = curl_easy_init();

  const char *filename = "url-list.txt";

  struct stat sb;

  if ((url->fp = fopen(filename, "r")) == NULL){
    printf("Error! opening file");
    exit(EXIT_FAILURE);
  }

  stat(filename, &sb);

  char *file_contents = malloc(sb.st_size);

  fscanf(url->fp,"%d", &url->num);

  while (fscanf(url->fp, "%[^\n] ", file_contents) != EOF)
  {
    printf("[%s]\n", file_contents);
  //  curl_easy_setopt(url->curl, CURLOPT_URL, file_contents);
 //   curl_easy_perform(url->curl);
  }

  curl_easy_cleanup(url->curl);
  curl_global_cleanup();

  fclose(url->fp);

  return NULL;
}


/* size_t
 * is a type which is used to represent the size of objects in
 * bytes and is therefore used as the return type by the sizeof operator.
 * */
size_t write_file(void *buffer, size_t size, size_t nmemb, void *data)
{
  struct URL *ft = (struct URL *)data;

  printf("Inside of write_file\n");

  if((ft->data = fopen(ft->file, "wb")) == NULL)
  {
    exit(EXIT_FAILURE);
  }

  return fwrite(buffer, size, nmemb, ft->data);
}



void *thread_sftp(void *arg)
{
  struct Node {
    char *fileName;
  };

  struct Node *ftpNode = NULL;

  if((ftpNode = malloc(sizeof(struct Node))) == NULL)
  {
    printf("Cannot Allocate Memory");
    exit(EXIT_FAILURE);
  }

  printf("Inside thread_sftp\n");

  struct URL *ftp = (struct URL *)arg;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  ftp->curl = curl_easy_init();

  if(ftp->curl) {
    printf("ftp->curl is true...\n");
    curl_easy_setopt(ftp, CURLOPT_URL, "sftp://192.168.1.167/home/patrick/test.txt");
    curl_easy_setopt(ftp, CURLOPT_WRITEFUNCTION, write_file);
    curl_easy_setopt(ftp, CURLOPT_WRITEDATA, &ftpNode);
    curl_easy_setopt(ftp, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_AGENT);
    curl_easy_setopt(ftp, CURLOPT_VERBOSE, 1L);

    ftp->response = curl_easy_perform(ftp);

    curl_easy_cleanup(ftp->curl);

    if(CURLE_OK != ftp->response)
    {
      fprintf(stderr, "Error: %d\n", ftp->response);
    }
  }

  if(ftp->data)
    fclose(ftp->data);

  curl_global_cleanup();

  return NULL;
}

pthread_t thread_id()
{
  pthread_t id;

  id = pthread_self();

  return id;
}















