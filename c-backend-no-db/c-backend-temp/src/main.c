#define _GNU_SOURCE
#include <unistd.h>
#include <facil/fio.h>
#include <facil/http/http.h>

#include "http_service.h"

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;

  initialize_http_service();

  fio_start(.threads = 1);

  return 0;
}
