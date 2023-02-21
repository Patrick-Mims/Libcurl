#ifndef STRUCT_H
#define STRUCT_H

struct curl_t
{
  CURL *curl;
  CURLcode response;
};

#endif
