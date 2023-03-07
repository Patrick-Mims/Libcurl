#ifndef SFTP_H
#define SFTP_H

struct sftp_t
{
  char *file;
  FILE *data;
};

size_t write_file(void *, size_t,size_t , void *);

#endif
