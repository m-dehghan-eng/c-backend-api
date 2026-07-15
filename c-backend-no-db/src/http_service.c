#include "Controllers/Controllers.h"
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include <facil/fiobj/fiobj.h>
#include <fiobj/fiobj_str.h>
#include <facil/http/http.h>

#include <stdio.h>      
#include <string.h>    
#include <stdlib.h>    
#include <fcntl.h>     
#include <unistd.h>    
#include <sys/stat.h>  
//******************************************
static void on_http_request(http_s *h) {


  fprintf(stderr, "METHOD = [%s]\n", fiobj_obj2cstr(h->method).data);
  fprintf(stderr, "PATH   = [%s]\n", fiobj_obj2cstr(h->path).data);

//**************************************************************** 

  //    :  با متد گت register هندل کردن روت

  if ((strcmp(fiobj_obj2cstr(h->method).data, "GET") == 0) &&
      (strspn(fiobj_obj2cstr(h->path).data, "/register") == 9)) {
    int fdr = open("src/public/index.html", O_RDONLY);
    if (fdr == -1) {
      http_send_error(h, 404);
      return;
    }
    struct stat str;
    fstat(fdr, &str);
    http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                    fiobj_str_new("text/html; charset=utf-8",
                                  strlen("text/html; charset=utf-8")));

    http_sendfile(h, fdr, str.st_size, 0);
    close(fdr);
    return;
  }

//***************************************************************** 

  //    : با متد پست usercreate   هندل کردن روت
  if (strcmp(fiobj_obj2cstr(h->method).data, "POST") == 0 &&
      (strspn(fiobj_obj2cstr(h->path).data, "/user/create") == 12)) {

    http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                    fiobj_str_new("application/json; charset=utf-8",
                                  strlen("application/json; charset=utf-8")));
    http_parse_body(h);

    user_create(h);

    return;
  }

//************************************************************************** 

  //    : با متد پست  userlogin هندل کردن روت

  if (strcmp(fiobj_obj2cstr(h->method).data, "POST") == 0 &&
      strcmp(fiobj_obj2cstr(h->path).data, "/user/login") == 0) {

    http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                    fiobj_str_new("application/json; charset=utf-8",
                                  strlen("application/json; charset=utf-8")));
    http_parse_body(h);

    user_login(h);

    return;
  }

//**************************************************************************** 

  //    : با متد پست  userdelete هندل کردن روت

  


    if (strcmp(fiobj_obj2cstr(h->method).data, "POST") == 0 &&
        strcmp(fiobj_obj2cstr(h->path).data, "/user/delete") == 0) {

      http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                      fiobj_str_new("application/json; charset=utf-8",
                                    strlen("application/json; charset=utf-8")));
      http_parse_body(h);
       user_delete(h);

          return;
    }
  

//**********************************************************************


   //    : با متد پست userupdate هندل کردن روت




    if (strcmp(fiobj_obj2cstr(h->method).data, "POST") == 0 &&
        strcmp(fiobj_obj2cstr(h->path).data, "/user/update") == 0) {

      http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                      fiobj_str_new("application/json; charset=utf-8",
                                    strlen("application/json; charset=utf-8")));
      http_parse_body(h);
       user_update(h);

          return;
    }
  
  
//***************************************************************************



 //    : با متد پست user_read_by_id هندل کردن روت


    if (strcmp(fiobj_obj2cstr(h->method).data, "POST") == 0 &&
        strcmp(fiobj_obj2cstr(h->path).data, "/user/read-by-id") == 0) {

      http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                      fiobj_str_new("application/json; charset=utf-8",
                                    strlen("application/json; charset=utf-8")));
      http_parse_body(h);
      user_read_by_id(h);

          return;
    }

//****************************************************************** 


  //    : با متد پست user_read_all هندل کردن روت


    if (strcmp(fiobj_obj2cstr(h->method).data, "POST") == 0 &&
        strcmp(fiobj_obj2cstr(h->path).data, "/user/read-all") == 0) {

      http_set_header(h, HTTP_HEADER_CONTENT_TYPE,
                      fiobj_str_new("application/json; charset=utf-8",
                                    strlen("application/json; charset=utf-8")));
      http_parse_body(h);
      user_read_all(h);
          return;
    }
    
    http_send_error(h, 404);
}
  
  
//***************************************************************** 

    
    void initialize_http_service(void) {
     
      if (http_listen("3000", "127.0.0.1", .on_request = on_http_request,
                      .max_body_size = 5 * 1024 * 1024,
                      .ws_max_msg_size = 1024 * 1024,
                      .public_folder = "src/public", .log = 1) == -1) {

        perror("ERROR: http_listen failed");
        exit(1);
      }

      printf("Server running at http://127.0.0.1:3000\n");
    }

    