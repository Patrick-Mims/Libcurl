#include <curl/curl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "list.h"
#include "queue.h"
#include "stack.h"
#include "struct.h"
#include "thread.h"

struct URL
{
  CURL *curl;
  CURLM *cm;
  CURLMsg *message;
  CURLcode response;
  FILE *fp;
  int num;
};

/*
void *thread_io(void *arg)
{
  struct email_list *head = (struct email_list *)arg;

  if ((head = malloc(sizeof(struct email_list *))) == NULL)
  {
    exit(EXIT_FAILURE);
  }

  const char *filename = "email-list.csv";

  FILE *in_file = NULL;
  struct stat sb;

  if ((in_file = fopen(filename, "r")) == NULL)
  {
    printf("Error: Opening File\n");

    exit(EXIT_FAILURE);
  }

  stat(filename, &sb);

  char *file_contents = malloc(sb.st_size);

  while (fscanf(in_file, "%[^\n] ", file_contents) != EOF)
  {
    add(&head, file_contents);
    printf("> %s\n", file_contents);
  }

  fclose(in_file);
  exit(EXIT_SUCCESS);

  return NULL;
}
*/

void *thread_curl(void *arg)
{
  curl_global_init(CURL_GLOBAL_ALL);

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
    curl_easy_setopt(url->curl, CURLOPT_URL, file_contents);
    curl_easy_perform(url->curl);
  }

  curl_easy_cleanup(url->curl);
  curl_global_cleanup();

  fclose(url->fp);

  return NULL;
}

pthread_t thread_id()
{
  pthread_t id;

  id = pthread_self();

  return id;
}
