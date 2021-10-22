#include <stdio.h>
#include <time.h>
#include "common_func.h"



int  IsLeapYear(int year)
{
    return (((year % 4 == 0) &&
        (year % 100 != 0)) ||
        (year % 400 == 0));
}


int isValidDate(Date validDate)
{
    if (validDate.yyyy > MAX_YR ||
        validDate.yyyy < MIN_YR)
        return 0;
    if (validDate.mm < 1 || validDate.mm > 12)
        return 0;
    if (validDate.dd < 1 || validDate.dd > 31)
        return 0;

    if (validDate.mm == 2)
    {
        if (IsLeapYear(validDate.yyyy))
            return (validDate.dd <= 29);
        else
            return (validDate.dd <= 28);
    }

    if (validDate.mm == 4 || validDate.mm == 6 ||
        validDate.mm == 9 || validDate.mm == 11)
        return (validDate.dd <= 30);
    return 1;
}


char* ltrim(char* s)
{
    while (isspace(*s)) s++;
    return s;
}

char* rtrim(char* s)
{
    char* back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}

char* trim(char* s)
{
    return rtrim(ltrim(s));
}


int dateCompare(Date source, Date destination)
{
    int value = 0;
    if (source.yyyy > destination.yyyy)
    {
        value = 1;
    }
    else if (source.yyyy < destination.yyyy)
    {
        value = -1;
    }
    else {
        if (source.mm > destination.mm)
        {
            value = 1;
        }
        else if (source.mm < destination.mm)
        {
            value = -1;
        }
        else {
            if (source.dd > destination.dd)
            {
                value = 1;
            } 
            else if (source.dd < destination.dd)
            {
                value = -1;
            }
        }
    }
    return value;
}


int timeCompare(Time source, Time destination)
{
    int value = 0;
    if (source.hh > destination.hh)
    {
        value = 1;
    }
    else if (source.hh < destination.hh)
    {
        value = -1;
    }
    else {
        if (source.MM > destination.MM)
        {
            value = 1;
        } 
        else if (source.MM < destination.MM)
        {
            value = -1;
        }
    }
    return value;
}


int dateTimeCompare(DateTime src, DateTime des)
{
    int dateCompareResult = dateCompare(src.date, des.date);
    if (dateCompareResult != 0)
        return dateCompareResult;
    else
        return timeCompare(src.time, des.time);
}

int isValidHour(Time validTime)
{
    if (validTime.hh > 24 || validTime.hh < 0 || validTime.MM > 60  || validTime.MM < 0)
        return 0;
    return 1;
}


Date getDateFromString(char* token)
{
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    Date date = { 0 };
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, token, 2);
    date.dd = atoi(buffer);
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, &token[3], 2);
    date.mm = atoi(buffer);
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, &token[6], 4);
    date.yyyy = atoi(buffer);
    return date;
}

Time getTimeFromString(char* token)
{
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    Time time = { 0 };
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, token, 2);
    time.hh = atoi(buffer);
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, &token[3], 2);
    time.MM = atoi(buffer);
    return time;
}


int addTime(Time* des, Time src)
{
    int addDays;
    int addHours;
    des->MM += src.MM;
    addHours = (int)des->MM / 60;
    if (addHours > 0)
    {
        des->MM = des->MM % 60;
    }

    des->hh += src.hh + addHours;
    addDays = (int)des->hh / 24;
    if (addDays > 0)
    {
        des->hh = des->hh % 24;
    }
    return addDays;
}

int offsetDays(int d, int m, int y)
{
    int offset = d;

    switch (m - 1)
    {
    case 11:
        offset += 30;
    case 10:
        offset += 31;
    case 9:
        offset += 30;
    case 8:
        offset += 31;
    case 7:
        offset += 31;
    case 6:
        offset += 30;
    case 5:
        offset += 31;
    case 4:
        offset += 30;
    case 3:
        offset += 31;
    case 2:
        offset += 28;
    case 1:
        offset += 31;
    }

    if (IsLeapYear(y) && m > 2)
        offset += 1;

    return offset;
}

void revoffsetDays(int offset, int y, int* d, int* m)
{
    int month[13] = { 0, 31, 28, 31, 30, 31, 30,
                      31, 31, 30, 31, 30, 31 };

    if (IsLeapYear(y))
        month[2] = 29;

    int i;
    for (i = 1; i <= 12; i++)
    {
        if (offset <= month[i])
            break;
        offset = offset - month[i];
    }

    *d = offset;
    *m = i;
}

void addDays(Date * date, int x)
{
    int offset1 = offsetDays(date->dd, date->mm, date->yyyy);
    int remDays = 0;
    if (IsLeapYear(date->yyyy))
    {
        remDays = (366 - offset1);
    }
    else {
        remDays = (365 - offset1);
    }

    int nextYear, offset2;
    if (x <= remDays)
    {
        nextYear = date->yyyy;
        offset2 = offset1 + x;
    }

    else
    {
        x -= remDays;
        nextYear = date->yyyy + 1;
        int nextYeardays = 0;
        if (IsLeapYear(nextYear))
        {
            nextYeardays = 366;
        }
        else {
            nextYeardays = 365;
        }
        while (x >= nextYeardays)
        {
            x -= nextYeardays;
            nextYear++;
            if (IsLeapYear(nextYear))
            {
                nextYeardays = 366;
            }
            else {
                nextYeardays = 365;
            }
        }
        offset2 = x;
    }

    int nextMonth, nextDay;
    revoffsetDays(offset2, nextYear, &nextDay, &nextMonth);

    date->dd = nextDay;
    date->mm = nextMonth;
    date->yyyy = nextYear;
}