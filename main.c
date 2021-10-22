#ifndef _TrungHoang_Nguyen
#define _TrungHoang_Nguyen
#pragma warning(disable:4996)
#include <stdio.h>
#include "data_structure.h"
#include <stdlib.h>
#include <stdbool.h>


CalendarList  calendarList;



/// <summary>
/// Reset CalendarEvent
/// </summary>
/// <param name="event"></param>
void memsetEvent(CalendarEvent* event)
{
	memset(event->descrition, 0, sizeof(event->descrition));
	event->date.dd = 0;
	event->date.mm = 0;
	event->date.yyyy = 0;
	event->duration.hh = 0;
	event->duration.MM = 0;
	event->time.hh = 0;
	event->time.MM = 0;
}

/// <summary>
/// 1. initialize (empty the database)
/// </summary>
void initialize()
{
	calendarList.index = 0;
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		memsetEvent(&calendarList.array[i]);
	}
}


/// <summary>
/// 2. save database to file
/// </summary>
void saveDatabaseFile(FILE* fp)
{
	rewind(fp);
	for (int i = 0; i < calendarList.index; i++)
	{
		fprintf(fp, "%s;%d/%d/%d;%d:%d;%d:%d\n",
			calendarList.array[i].descrition,
			calendarList.array[i].date.dd,
			calendarList.array[i].date.mm,
			calendarList.array[i].date.yyyy,
			calendarList.array[i].time.hh,
			calendarList.array[i].time.MM,
			calendarList.array[i].duration.hh,
			calendarList.array[i].duration.MM
			);
	}
}

/// <summary>
/// 3. read database from file and save in struct list
/// </summary>
void readDatabaseFile(FILE* fp)
{

	char lineValue[301];
	memset(lineValue, 0, sizeof(lineValue));
	rewind(fp);
	if (fp == NULL)
	{
		printf("Unable to open file. Please check if file is exist or not\n");
		return;
	}
	initialize();
	while (fgets(lineValue, 301, fp) != NULL) {

		char* token = strtok(lineValue, ";");
		strcpy(calendarList.array[calendarList.index].descrition, token);
		token = strtok(NULL, ";");
		Date dateResult = getDateFromString(token);
		memcpy(&calendarList.array[calendarList.index].date, &dateResult, sizeof(dateResult));
		token = strtok(NULL, ";");
		Time timeResult = getTimeFromString(token);
		memcpy(&calendarList.array[calendarList.index].time, &timeResult, sizeof(timeResult));
		token = strtok(NULL, ";");
		Time durationResult = getTimeFromString(token);
		memcpy(&calendarList.array[calendarList.index].duration, &durationResult, sizeof(durationResult));
		calendarList.index++;
	}

}

/// <summary>
/// 4, add an event: user enters description, date, time and duration of an event
/// </summary>
void addAnEvent()
{
	char buffer[301];
	Date getDate = { 0 };
	Time getTime = { 0 };
	bool firstTimeEnter = true;
	if (calendarList.index >= ARRAY_SIZE)
	{
		printf("ArrayList exceeds the size, could not add more event \n");
		return;
	}
enterdes:
	memset(buffer, 0, sizeof buffer);
	printf("Please enter your event information: \n");
timeEnter:
	_flushall();
	fgets(buffer, sizeof(buffer), stdin);
	if (firstTimeEnter)
	{
		firstTimeEnter = false;
		goto timeEnter;
	}
	if (strlen(buffer) > DESCRITION_LENGTH || strlen(buffer) == 0)
	{
		printf("Please enter the event description length less than or equal %d\n", DESCRITION_LENGTH);
		goto enterdes;
	}
	strcpy(calendarList.array[calendarList.index].descrition, trim(buffer));
enterdate:
	printf("Please enter your event date in format (day/month/year) : \n");
	scanf("%d/%d/%d", &getDate.dd, &getDate.mm, &getDate.yyyy);
	if (isValidDate(getDate) != 1)
	{
		printf("Please enter the valid event date\n");
		fgets(buffer, sizeof(buffer), stdin);
		goto enterdate;
	}
	memcpy(&calendarList.array[calendarList.index].date, &getDate, sizeof(getDate));
entertime:
	printf("Please enter your event time in format (hh:mm) : \n");
	scanf("%d:%d", &getTime.hh, &getTime.MM);
	if (isValidHour(getTime) != 1)
	{
		printf("Please enter the valid event time\n");
		fgets(buffer, sizeof(buffer), stdin);
		goto entertime;
	}
	memcpy(&calendarList.array[calendarList.index].time, &getTime, sizeof(getTime));
enterduration:
	memset(getTime, 0, sizeof getTime);
	printf("Please enter your event duration in format (hh:mm): \n");
	scanf("%d:%d", &getTime.hh, &getTime.MM);
	if (isValidHour(getTime) != 1)
	{
		printf("Please enter the valid event time\n");
		fgets(buffer, sizeof(buffer), stdin);
		goto entertime;
	}
	memcpy(&calendarList.array[calendarList.index].duration, &getTime, sizeof(getTime));
	printf("You have successfull to add new event %s\n", calendarList.array[calendarList.index].descrition);
	calendarList.index += 1;
}

/// <summary>
/// 5. remove an event
/// </summary>
void removeEvent()
{
	char buffer[301];
	int foundIndex = -1;
	CalendarEvent foundEven;
	bool firstTimeEnter = true;
	if (calendarList.index <= 0)
	{
		printf("There are not any event in the list\n");
		return;
	}
	printf("Please enter your event to delete: \n");
	timeEnter:
	_flushall();
	fgets(buffer, sizeof(buffer), stdin);
	if (firstTimeEnter)
	{
		firstTimeEnter = false;
		goto timeEnter;
	}
	for (int i = 0; i < calendarList.index; i++)
	{
		if (strcmp(trim(buffer), calendarList.array[i].descrition) == 0)
		{
			foundIndex = i;
			foundEven = calendarList.array[i];
			break;
		}
	}
	if (foundIndex != -1)
	{
		if (calendarList.index == 1)
		{
			memsetEvent(&calendarList.array[0]);
		}
		else {
			for (int i = foundIndex; i < calendarList.index - 1; i++)
			{
				calendarList.array[i] = calendarList.array[i + 1];
			}
		}
		calendarList.index--;
		printf("You have successfull to delete event %s\n", buffer);
	}
	
}

/// <summary>
/// Sorting the Calendar List by date time
/// </summary>
void arrangeCalendarList()
{
	for (int i = 0; i < calendarList.index; i++) {
		for (int j = i + 1; j < calendarList.index; j++) {
			int dateCompareValue = dateCompare(calendarList.array[i].date, calendarList.array[j].date);
			if (dateCompareValue > 0) {
				CalendarEvent temp = calendarList.array[i];
				calendarList.array[i] = calendarList.array[j];
				calendarList.array[j] = temp;
			}
			else if (dateCompareValue == 0)
			{
				if (timeCompare(calendarList.array[i].time, calendarList.array[j].time) > 0)
				{
					CalendarEvent temp = calendarList.array[i];
					calendarList.array[i] = calendarList.array[j];
					calendarList.array[j] = temp;
				}
			}
		}
	}
}

/// <summary>
/// 6. Print a report which prints all events sorted by date and time
/// </summary>
void sortEventList()
{
	if (calendarList.index == 0)
	{
		printf("event list is empty for being sorted");
		return;
	}
	arrangeCalendarList();

	printf("List event after sorted ascending by date, time: \n");
	for (int i = 0; i < calendarList.index; i++)
	{
		printf("%s;%d/%d/%d;%d:%d;%d:%d\n",
			calendarList.array[i].descrition,
			calendarList.array[i].date.dd,
			calendarList.array[i].date.mm,
			calendarList.array[i].date.yyyy,
			calendarList.array[i].time.hh,
			calendarList.array[i].time.MM,
			calendarList.array[i].duration.hh,
			calendarList.array[i].duration.MM);
	}
}



/// <summary>
/// 7. search for a free time in the range of dates that user specifies
/// </summary>
void searchForFreeTime()
{
	DateTime dateFrom = { 0 };
	DateTime dateTo = { 0 };
	DateTime dateTimeRange[ARRAY_SIZE * 2];
	DateTime freeTime[ARRAY_SIZE * 2];
	
enterFromDate:
	printf("Please enter your From Date in format (day/month/year) : \n");
	scanf("%d/%d/%d", &dateFrom.date.dd, &dateFrom.date.mm, &dateFrom.date.yyyy);
	if (isValidDate(dateFrom.date) != 1)
	{
		printf("Please enter the valid From Date date\n");
		goto enterFromDate;
	}
enterToDate:
	printf("Please enter your To Date in format (day/month/year) : \n");
	scanf("%d/%d/%d", &dateTo.date.dd, &dateTo.date.mm, &dateTo.date.yyyy);
	if (isValidDate(dateTo.date) != 1)
	{
		printf("Please enter the valid To Date date\n");
		goto enterToDate;
	}
	int compareResult = dateCompare(dateFrom.date, dateTo.date);

	if (compareResult == 0)
	{
		dateTo.time.hh = 23;
		dateTo.time.MM = 59;
	}
	else if (compareResult == 1)
	{
		printf("To date must greater than From Date\n");
		goto enterToDate;
	}

	int j = 0;
	for (int i = 0; i < calendarList.index; i++)
	{
		memcpy(&dateTimeRange[j].date, &calendarList.array[i].date, sizeof(calendarList.array[i].date));
		memcpy(&dateTimeRange[j].time, &calendarList.array[i].time, sizeof(calendarList.array[i].time));
		int EndayCount = addTime(&calendarList.array[i].time, calendarList.array[i].duration);
		if (EndayCount > 0)
		{
			addDays(&calendarList.array[i].date, EndayCount);
		}
		memcpy(&dateTimeRange[j + 1].date, &calendarList.array[i].date, sizeof(calendarList.array[i].date));
		memcpy(&dateTimeRange[j+1].time, &calendarList.array[i].time, sizeof(calendarList.array[i].time));
		j += 2;
	}

	int z = 0;
	int i = 0;
	int rangePosition = -1;
	memcpy(&freeTime[z], &dateFrom, sizeof(dateFrom));
	memcpy(&freeTime[++z], &dateTo, sizeof(dateTo));
	for (int i = 0; i < z; i++)
	{

	}
}


/// <summary>
/// main function off the application
/// </summary>
/// <returns></returns>
int main()
{	
	FILE* fp;
	fp = fopen("./database.txt", "w+");
	initialize();
	while (true)
	{
		bool isBreak = false;
		int temp =  -1;
		printf("=====================Calendar event=======================\n");
		printf("Please chooses your action of the program:\n");
		printf("1,Add event\n");
		printf("2,Remove event\n");
		printf("3,Save to database file\n");
		printf("4,Read database file\n");
		printf("5,Print Report with sort date/time\n");
		printf("6,Search free time\n");
		printf("7,Exit\n");
		scanf("%d", &temp);
		switch (temp)
		{
			case 1:
			{
				addAnEvent();
				break;
			}
			case 2:
			{
				removeEvent();
				break;
			}
			case 3:
			{
				saveDatabaseFile(fp);
				break;
			}
			case 4:
			{
				readDatabaseFile(fp);
				break;
			}
			case 5:
			{
				sortEventList();
				break;
			}
			case 6:
			{
				searchForFreeTime();
				break;
			}
			case 7:
			{
				isBreak = true;
				break;
			}
			default:
				break;
		}
		if (isBreak)
			break;
	}
	fclose(fp);
	free(fp);
	return 0;
}

#endif