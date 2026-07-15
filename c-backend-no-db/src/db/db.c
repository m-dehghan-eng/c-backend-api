
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <facil/fiobj/fiobj.h>
#include <facil/http/http.h>
#include <fiobj/fiobj_ary.h>
#include <fiobj/fiobj_hash.h>
#include <fiobj/fiobj_json.h>
#include <fiobj/fiobj_str.h>

#include "data/struct_user_data.h"

#include "../db/db.h"
//*************************************
// گذاشتن قفل روی اطلاعات برای جلوگیری از روی هم نوشته شدن اطلاعات
static pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;
#define USER_LOCK() pthread_mutex_lock(&user_mutex)
#define USER_UNLOCK() pthread_mutex_unlock(&user_mutex)
//***************************************
// پارس کردن جیسون
static FIOBJ get_param_str(http_s *h, const char *key) {
  if (!h->params || !FIOBJ_TYPE_IS(h->params, FIOBJ_T_HASH))
    return FIOBJ_INVALID;
  FIOBJ k = fiobj_str_new(key, strlen(key));
  FIOBJ v = fiobj_hash_get(h->params, k);
  fiobj_free(k);
  return v;
}
//**************************************************
// تابع برای استخراج فیلد ها و برسی معتبر بدون طول فیلد ها
void Field_extraction(http_s *h, us1 *tmp) {

  FIOBJ us = get_param_str(h, "username");
  FIOBJ em = get_param_str(h, "email"); // .استخراج فیلد و  ذخیره شناسه فیلد ها
                                        // درون یک متغیر از نوع ابجکت
  FIOBJ pa = get_param_str(h, "password");

  if (!us || !em || !pa) {
    http_send_error(h, 400); // .برسی وجود  یا عدم وحود داده ها
    return;
  }

  int len1 = strlen(fiobj_obj2cstr(us).data);
  if (len1 >= sizeof(tmp->u)) {
    http_send_error(h, 400);
    return;
  }

  int len2 = strlen(
      fiobj_obj2cstr(em).data); // استخراج طول هر فیلد برای برسی معتبر بودن
                                // بازه و جلو گیری از خطر سر ریز حافظه
  if (len2 >= sizeof(tmp->e)) {
    http_send_error(h, 400);
    return;
  }

  int len3 = strlen(fiobj_obj2cstr(pa).data);
  if (len3 >= sizeof(tmp->p)) {
    http_send_error(h, 400);
    return;
  }

  snprintf(tmp->u, sizeof(tmp->u), "%s", fiobj_obj2cstr(us).data);
  snprintf(tmp->e, sizeof(tmp->e), "%s",
           fiobj_obj2cstr(em).data); // .ذخیره فیلد های استراج شده در حافظه موقت
  snprintf(tmp->p, sizeof(tmp->p), "%s", fiobj_obj2cstr(pa).data);

  return;
}
//**************************************************************
// تابع برای چک کردن وجود یا عدم وجود کاربر با اطلاعات پیشین

int controller_struct_user(
    us1 *tmp) { // .فرستادن داده های ذخیره شده در حافظه موقت به تابع

  for (int i = 0; i < users_count; i++) {

    if (strcmp(st[i].username, tmp->u) == 0 &&
        strcmp(st[i].email, tmp->e) == 0) {
      // برسی وجود یا عدم وجود کاربر دیگر با با اطلعات مشابه برای تصمیم
      // .گیری برای ایجاد یک کاربر جدید

      return 0;
    }
  }

  return 1;
}
//***************************************
// تابع برای افزودن کاربر جدید

int Add_user(us1 *tmp) { //. ارسال داده های ذخیره شده در حافظه موقت به تابع

  USER_LOCK(); // قفل کردن اطلاعات فعلی حافظه موقت برای جلوگیری از نوشته شدن
               // اطلاعات کابر دیگر در حافظه موقت

  if (controller_struct_user(tmp) == 1 &&
      users_count < 1000) { //  چک کردن صحت عدم وجود کاربر پیشین با این اطلاعات
                            //  فعلی و چک کردن پر نبودن حافظه سرور

    snprintf(st[users_count].username, sizeof(st[users_count].username), "%s",
             tmp->u);
    snprintf(st[users_count].email, sizeof(st[users_count].email),
             "%s", // کپی کردن اطلاعات حافظه موقت در حافظه اصلی سرور
             tmp->e);
    snprintf(st[users_count].password, sizeof(st[users_count].password), "%s",
             tmp->p);

    st[users_count].id = ID; // اختصاص یک ایدی عددی به کاربر

    ID++; // افزایش ایدی و اماده کردن ان برای کاربر بعدی

    users_count++; // افزایش تعداد کاربر ها

    USER_UNLOCK(); // باز کردن قفل روی داده ها برای ازاد کردن حافظه موقت در صورت
                   // نیاز یک کاربر دیگر به ان حافظه
    return 0;
  } else {
    USER_UNLOCK();

    return 1;
  }
}
//*******************************************
// تابع برای چک کردن صحت وجود یک کاربر خاص با اطلاعات وارد شده

int controller_struct_user1(
    http_s *h, us1 *tmp) { // ارسال در خواست فرانت و حافظه موقت به ورودی تابع

  FIOBJ us = get_param_str(h, "username"); // استخراج فیلد ها
  FIOBJ pa = get_param_str(h, "password");

  if (!us || !pa) {
    http_send_error(h, 400); // برسی صحت وجود داده
    return -1;
  }

  USER_LOCK(); // گذاشتن قفل رو حافظه موقت

  snprintf(tmp->u, sizeof(tmp->u), "%s", fiobj_obj2cstr(us).data);
  snprintf(tmp->p, sizeof(tmp->p), "%s",
           fiobj_obj2cstr(pa).data); // ریختن اطلاعات درون حافظه موقت

  for (int i = 0; i < users_count;
       i++) { // برسی وجود یا عدم وجود کاربر پیشین با اطلاعات ارسالی
    if (strcmp(st[i].username, tmp->u) == 0 &&
        strcmp(st[i].password, tmp->p) == 0) {

      USER_UNLOCK(); // برداشتن قفل از روی حافظه موقت برای درخواست کاربر بعدی

      return 0;
    }
  }
  USER_UNLOCK();
  return 1;
}
//******************************************************
// تابع برای استخراج اطلاعات یک کاربر به وسیله ی ایدی ان کاربر

int controller_data_byid(http_s *h, us1 *tmp) {

  FIOBJ Id = get_param_str(h, "id"); // استخراج فیلد ایدی

  if (!Id) {
    http_send_error(h, 400); // برسی صحت وجود داده
    return -1;
  }

  int64_t iD = fiobj_obj2num(Id); // استخراج فیلد ایدی به صورت اینتیجر

  USER_LOCK();

  for (int i = 0; i < users_count; i++) {
    if (st[i].id ==
        iD) { // پیدا کردن محل ذخیره اطلاعات کاربر خاص با ایدی مد نظر

      snprintf(tmp->u, sizeof(tmp->u), "%s",
               st[i].username); // ریختن اطلاعات در حافظه موقت
      snprintf(tmp->e, sizeof(tmp->e), "%s", st[i].email);
      USER_UNLOCK();
      return 0;
    }
  }
  USER_UNLOCK();

  return 1;
}

//*******************************************************
//  مد نظر username تابع برای حذف یک کاربر خاص با

int user_delete_byusername(http_s *h) {

  FIOBJ uname_obj = get_param_str(h, "username"); // username استخراج فیلد

  if (!uname_obj ||
      !FIOBJ_TYPE_IS(uname_obj, FIOBJ_T_STRING)) { // برسی صحت وجود داده
    http_send_error(h, 400);
    return -1;
  }

  const char *username_to_delete =
      fiobj_obj2cstr(uname_obj).data; // ایجاد یک  اشاره گر به متغیر ثابت

  USER_LOCK(); // گذاشتن قفل

  int found_index = -1; // خاص username  فلگ برای برسی دیدن یا ندیدن ان

  for (int i = 0; i < users_count;
       i++) { // پیدا کردن ادرس خانه مربوط به ان کاربر خاص
    if (strcmp(st[i].username, username_to_delete) == 0) {
      found_index = i;
      break;
    }
  }

  if (found_index == -1) { // در صورت نبود کاربر
    USER_UNLOCK();
    return 1;
  }

  for (int i = found_index; i < users_count - 1; i++) { // درصورت وجود کاربر
    st[i] =
        st[i + 1]; // ارایه مربط به کاربران یکی شیفت  داده میشود و فشرده میشود
  }

  users_count--; // کم کردن تعداد کاربر ها

  USER_UNLOCK(); // برداشتن قفل

  return 0;
}
//*********************************************
// تابع برای اپدیت کردن یک کاربر خاص با ایدی مد نظر
int User_update_by_ID(http_s *h, us1 *tmp) {

  FIOBJ Id = get_param_str(h, "id"); // استخراج ایدی

  if (!Id) {
    http_send_error(h, 400); //  برسی صحت وجود داده
    return -1;
  }

  int64_t iD = fiobj_obj2num(Id); // استخراج فیلد ایدی بیه صورت اینتیجر
  Field_extraction(h, tmp); // استخراخ اطلاعات جدید کاربر

  USER_LOCK(); // قفل حافظه موقت

  for (int i = 0; i < users_count;
       i++) { // پیداکردن ادرس خانه مربوط اطلاعات کاربر

    if (st[i].id == iD) {

      snprintf(st[i].username, sizeof(st[i].username), "%s", tmp->u);
      snprintf(st[i].email, sizeof(st[i].email), "%s",
               tmp->e); // باز نویسی اطلاعات جدید کاربر
      snprintf(st[i].password, sizeof(st[i].password), "%s", tmp->p);

      USER_UNLOCK(); // برداشتن قفل
      return 0;
    }
  }
  USER_UNLOCK();
  return 1;
}
//**************************************************
// نابعی برای نوشتن یک فایل جیسون از اطلاعات کل کاربران
int Creating_user_files(http_s *h) {

  USER_LOCK();

  FIOBJ arr = fiobj_ary_new(); // ایجاد یه ارایه

  for (int i = 0; i < users_count; i++) {

    FIOBJ obj = fiobj_hash_new(); // ایجاد یک هش جدید

    FIOBJ k1 = fiobj_str_new("id", 2);
    fiobj_hash_set(obj, k1,
                   fiobj_num_new(st[i].id)); // ایجاد کلید ایدی  وتنظیم مقدار ان
    fiobj_free(k1);

    FIOBJ k2 = fiobj_str_new("username", 8);
    fiobj_hash_set(
        obj, k2,
        fiobj_str_new(
            st[i].username,
            strlen(st[i].username))); // و تنظیم مقدار ان usrename  ایجاد کلید
    fiobj_free(k2);

    FIOBJ k3 = fiobj_str_new("email", 5);
    fiobj_hash_set(
        obj, k3,
        fiobj_str_new(
            st[i].email,
            strlen(st[i].email))); // ایجاد کلید ایمیل و تنظیم مقدار ان
    fiobj_free(k3);

    fiobj_ary_push(arr, obj); // اضافه کردن هش به ارایه
  }

  USER_UNLOCK();
  FIOBJ json = fiobj_obj2json(arr, 1); // تبدیل اریه به فرم جیسون
  fio_str_info_s info = fiobj_obj2cstr(json);
  http_send_body(h, info.data, info.len); // ارسال جیسون

  fiobj_free(json); // ازاد کردن جیسون
  fiobj_free(arr);  // ازاد کردن ارایه

  return 0;
}

//**********************************************************************
// تابع برای احراز هویت کاربر
//void Authentication() {

  //Field_extraction(h, &tmp);
  //srand(time(NULL));
  //int code = rand() % 1000 + 1000;

//}
