#include<common.hpp>

void error(const char *Fmt, ...){
  va_list VA;
  va_start(VA, Fmt);
  vfprintf(stderr, Fmt, VA);
  fprintf(stderr, "\n");
  // 清除VA
  va_end(VA);
  exit(1);
}