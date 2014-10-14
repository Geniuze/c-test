#ifndef FUN_H
#define FUN_H

#include "def.h"

int test_init();
int test_const();
int test_ope_new();
int test_string();
int test_file_stream();
int test_delete_ope();
int test_getHostByName();
int test_arg(int argc, char **argv);
int test_dns(int argc, char ** argv);
int test_system(int argc, char ** argv);
int test_translate(int argc, char ** argv);
int test_parse_int(int argc, char **argv);
int test_sizeof(int argc, char **argv);
int test_streambuf(int argc, char ** argv);
int test_time(int argc, char ** argv);
int test_try_catch(int argc, char ** argv);
int test_urldecode(int argc, char ** argv);
int test_iconv(int argc, char ** argv);
int test_endian(int argc, char ** argv);
int test_fizzbuzzwhizz(int argc, char ** argv);
int test_fizzbuzzwhizz2(int argc, char ** argv);
int Combination(int *arr, int size);
int test_substr(int argc, char **argv);
int test_getip(int argc, char ** argv);
int test_getip_byioctl(int argc, char ** argv);
int get_ipv4route(int argc, char ** argv);
int test_md5(int argc, char **argv);
int test_broadcast(int argc, char **argv);
int test_uid(int argc, char **argv);
int test_classpfunc(int argc, char **argv);
int test_class_setvalue(int argc, char ** argv);
int test_argsFromUrl(int argc, char ** argv);
int test_timer_thread(int argc, char **argv);
int test_timer_signal(int argc, char **argv);

#endif // FUN_H
