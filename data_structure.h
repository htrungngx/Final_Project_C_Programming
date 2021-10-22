#ifndef _data_structure_
#define _data_structure_

#include <stdio.h>
#include <stdlib.h>
#include "common_func.h"

#define ARRAY_SIZE 1000
#define DESCRITION_LENGTH 100
#define LINE_LENGTH = 201
#define BUFFER_LENGTH = 300
#define DATE_LENGTH = 10
#define TIME_LENGTH = 10


/*typedef struct {
    char date[DATE_LENGTH];
    char time[TIME_LENGTH];
} FreeTimeEntity;*/

typedef struct {
    char descrition[DESCRITION_LENGTH];
    Date date;
    Time time;
    Time duration;
} CalendarEvent;

typedef struct   {
    int index;
    CalendarEvent array[ARRAY_SIZE];
}CalendarList;



#endif // !_data_structure_





