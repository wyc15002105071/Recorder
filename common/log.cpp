#include "log.h"
#include <sys/time.h>
#include <time.h>
#define LINE_SZ 1024

int os_get_env_u32(const char *name, int *value, int default_value)
{
    char *ptr = getenv(name);
    if (NULL == ptr) {
        *value = default_value;
    } else {
        char *endptr;
        int base = (ptr[0] == '0' && ptr[1] == 'x') ? (16) : (10);
        errno = 0;
        *value = strtoul(ptr, &endptr, base);
        if (errno || (ptr == endptr)) {
            errno = 0;
            *value = default_value;
        }
    }
    return 0;
}

void __log(int level,const char *tag, const char *fmt,
                      const char *fname, va_list list)
{
    char msg[LOG_MAX_LEN + 1];
    char *tmp = msg;
    const char *buf = fmt;
    size_t len_fmt  = strnlen(fmt, LOG_MAX_LEN);
    size_t len_name = (fname) ? (strnlen(fname, LOG_MAX_LEN)) : (0);
    size_t buf_left = LOG_MAX_LEN;
    size_t len_all  = len_fmt + len_name;

    if (NULL == tag)
        tag = MODULE_TAG;

    if (len_name) {
        buf = msg;
        buf_left -= snprintf(msg, buf_left, "[%s] ", fname);
        tmp += len_name + 3;
    }

    if (len_all == 0) {
        buf = msg_log_nothing;
    } else if (len_all >= LOG_MAX_LEN) {
        buf_left -= snprintf(tmp, buf_left, "%s", msg_log_warning);
        buf = msg;
    } else {
        snprintf(tmp, buf_left, "%s", fmt);
        if (fmt[len_fmt - 1] != '\n') {
            tmp[len_fmt]    = '\n';
            tmp[len_fmt + 1]  = '\0';
        }
        buf = msg;
    }

    struct timeval current_time;
    char time_str[30];
    gettimeofday(&current_time, NULL);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&current_time.tv_sec));

    char line[LINE_SZ] = {0};
    snprintf(line, sizeof(line) - 1, "%s:%s:%s: %s",time_str,level==MSG_LOG_ERROR?"E":"D",tag, msg);
    vsyslog(LOG_INFO, line, list);
    vfprintf(stdout,line,list);
}

void _log_l(int level, const char *tag, const char *fmt, const char *fname, ...)
{
    va_list args;
    int log_level;

    if (level <= MSG_LOG_UNKNOWN || level >= MSG_LOG_SILENT)
        return;
    if(r_log_level < 0) {
        os_get_env_u32("log_level", (int *)&r_log_level, 4);
    }
    log_level = r_log_level;
    if (log_level >= MSG_LOG_SILENT || log_level < MSG_LOG_UNKNOWN)
        return;

    if (level > log_level)
        return;

    va_start(args, fname);
    __log(level, tag, fmt, fname, args);
    va_end(args);
}
