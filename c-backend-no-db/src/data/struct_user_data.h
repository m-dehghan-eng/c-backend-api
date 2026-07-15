#pragma once

#define MAX_USERS 1000
#define MAX_USERNAME 25
#define MAX_EMAIL 255
#define MAX_PASSWORD 65

  struct user {
  char username[MAX_USERNAME];
  char email[ MAX_EMAIL];
  char password[MAX_PASSWORD];
  int id;
};

extern  struct user st[MAX_USERS];
extern int users_count;
extern int ID;

