#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#define NORMAL                 1
#define ABNORMAL              -1
FILE *logfp;

int getDate(char *tdate)
{
    time_t t_clock;
    struct tm *t;

    t_clock = time(0);
    t = (struct tm *)localtime(&t_clock);
    sprintf(tdate, "%4d%02d%02d%02d%02d%02d", t->tm_year+1900, t->tm_mon + 1,
                   t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    return NORMAL;
}

int logging(int gb, char *va_alist, ...)
{
    va_list args;
    char    tmp[7], sdate[21], buff[1024];
    char    *fmt;

    switch(gb)
    {
        case 0 : sprintf(tmp, "[SEND]");
                 break;
        case 1 : sprintf(tmp, "[RECV]");
                 break;
        case 9 : sprintf(tmp, "[EROR]");
                 break;
        default: sprintf(tmp, "[    ]");
                 break;
    }

    va_start(args, va_alist);
    fmt = va_arg(args, char *);
    vsprintf(buff, fmt, args);
    va_end(args);

    memset(sdate, 0x00, sizeof(sdate));
    if (getDate(sdate) == ABNORMAL) return ABNORMAL;

    fprintf(logfp, "    ][%.2s/%.2s %.2s:%.2s:%.2s]%.6s[%s]\n", &sdate[4],
            &sdate[6], &sdate[8], &sdate[10], &sdate[12], tmp, buff);
    fflush(logfp);
}

int sum(int count, ...)
{
    int res = 0;
    va_list ap;
    int i;

    va_start(ap, count);
    
    for(i=0; i<count; i++)
        res += va_arg(ap, int);

    va_end(ap);

    return res;
}

int main()
{
    printf("%d\n", sum(10, 1,2,3,4,5,6,7,8,9,10));

    return 0;
}
