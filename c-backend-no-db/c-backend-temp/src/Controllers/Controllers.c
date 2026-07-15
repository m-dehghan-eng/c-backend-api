#include <facil/fiobj/fiobj.h>
#include <facil/http/http.h>
#include <facil/fio.h>
#include "http.h"

#include "Controllers.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>

#include <stdio.h>     
#include <string.h>  
#include "../db/db.h"

void  user_create(http_s *h) {

  us1 tmp;
  
 Field_extraction(h,&tmp);
 

    if ( Add_user(&tmp)==0) {

      char msg[] = "{\"status\": true, \"body\": {\"msg\": \"OK\"}}";
      http_send_body(h, msg, strlen(msg));
      return ;
    }

   else {
      char msg[] = "{\"status\": false, \"msg\": \"User Create Failed\"}";
      http_send_body(h, msg, strlen(msg));
    }
    return ;
  }


  void user_login(http_s *h) {

    us1 tmp;

  if (controller_struct_user1(h,&tmp) == 0) {
    char msg[] = "{\"status\": true, \"body\": {\"msg\": \"User Loged In\"}}";
    http_send_body(h, msg, strlen(msg));

  } 
   else {
    char msg[] = "{\"status\":false , \"body\": {\"msg\": \"User not found\"}}";
    http_send_body(h, msg, strlen(msg));
  }
  return;
}


 void user_delete(http_s *h) {

    if ( user_delete_byusername(h)==0) {


      char msg[] = "{\"status\":true, \"body\":{\"msg\":\"User Deleted\"}}" ;
        
          http_send_body(h, msg, strlen(msg));
    }
  
  else {
     char msg[] = "{\"status\":false,\"msg\":\"User not found\"}" ;
        
          http_send_body(h, msg, strlen(msg));
  }
   return ;
}


void  user_update(http_s *h) {

  us1 tmp;
  
 if( User_update_by_ID(h,&tmp)==0){

     char msg[] = "{\"status\":true, \"body\": {\"msg\": \"User Updated\"}}";
    http_send_body(h, msg, strlen(msg));
 }


 
 else {

    char msg[] = "{\"status\":false,\"msg\": \"User didn 't updated\"}";
    http_send_body(h, msg, strlen(msg));
 }

  return;
 
}


void user_read_by_id(http_s *h) {

 us1 tmp;

  if (controller_data_byid(h,&tmp)==0) {

    char msg[1000];
    
    snprintf(msg, 250,"{\"status\" : true, \"body\" : {\"username\" : \"%s\", \"email\" : \"%s\"} }",tmp.u, tmp.e);

    http_send_body(h, msg, strlen(msg));

  }
  else {

     char msg[] = "{\"status\":false,\"msg\": \"User not found\"}";

    http_send_body(h, msg, strlen(msg));
  
  }
  return;
}



void user_read_all(http_s *h) {

 http_set_header(
  h,
  fiobj_str_new("content-disposition", 19),
  fiobj_str_new("attachment; filename=\"all user\"", 32)
);

  if (Creating_user_files(h) == 0) {

    return;
  }

else {

 char msg[] = "{\"status\":false, \"body\": {}}";

    http_send_body(h, msg, strlen(msg));
}
return;
}

