#include <stdio.h>
#include <stdlib.h>

#include "sftp.h"

/*
struct sftp_t
{
  char *file;
  FILE *data;
};
*/

/*
size_t write_file(void *buffer, size_t size, size_t nmemb, void *data)
{
  struct sftp_t *ft = (struct sftp_t *)data;

  if((ft->data = fopen(ft->file, "wb")) == NULL)
  {
    exit(EXIT_FAILURE);
  }

  return fwrite(buffer, size, nmemb, ft->data);
}
*/
