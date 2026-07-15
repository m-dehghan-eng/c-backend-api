#pragma once
#include <facil/http/http.h>

typedef struct {
  char u[25];
  char e[255];
  char p[65];
} us1;


void Field_extraction(http_s *h, us1 *tmp);



int controller_struct_user(us1 *tmp);



int Add_user(us1 *tmp);



int controller_struct_user1(http_s *h,us1 *tmp);



int controller_data_byid(http_s *h,us1 *tmp);




int user_delete_byusername(http_s *h);




int User_update_by_ID(http_s *h,us1 *tmp) ;




int Creating_user_files(http_s *h);

    
