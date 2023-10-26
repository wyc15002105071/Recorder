#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <stdarg.h>

#define MSG_LOG_UNKNOWN     0
#define MSG_LOG_ERROR       1
#define MSG_LOG_WARN        2
#define MSG_LOG_INFO        3
#define MSG_LOG_DEBUG       4
#define MSG_LOG_SILENT      5

#ifndef MODULE_TAG
#define MODULE_TAG              NULL
#endif

#define LOG_MAX_LEN 256

static int r_log_level = -1;
static const char *msg_log_nothing = "\n";
static const char *msg_log_warning = "log message is long\n";

#define RLOGD(format,...) _log_l(MSG_LOG_DEBUG,MODULE_TAG,format,__FUNCTION__ ,##__VA_ARGS__)
#define RLOGE(format,...) _log_l(MSG_LOG_ERROR,MODULE_TAG,format,__FUNCTION__ ,##__VA_ARGS__)


int os_get_env_u32(const char *name, int *value, int default_value);

void __log(int level,const char *tag, const char *fmt,
                      const char *fname, va_list list);

void _log_l(int level, const char *tag, const char *fmt, const char *fname, ...);

#endif // LOG_H
