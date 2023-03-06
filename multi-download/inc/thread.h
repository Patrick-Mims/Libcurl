#ifndef THREAD_H
#define THREAD_H

void *thread_sftp(void *);
void *thread_curl(void *);
pthread_t thread_id();

#endif
