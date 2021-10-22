#ifndef _common_func_
#define _common_func_

#define MAX_YR  9999
#define MIN_YR  1900
#define MAX_HH 24
#define MAX_MM 60

typedef struct
{
    int yyyy;
    int mm;
    int dd;
} Date;


typedef struct
{
    int hh;
    int MM;
} Time;


typedef struct
{
    Date date;
    Time time;
} DateTime;


int isValidDate(Date validDate);

int isValidHour(Time validTime);

int dateCompare(Date source, Date destination);
int timeCompare(Time source, Time destination);

Time getTimeFromString(char* token);

Date getDateFromString(char* token);

void addDays(Date* date, int x);

int dateTimeCompare(DateTime src, DateTime des);

char* trim(char* s);

#endif
