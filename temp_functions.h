// temp_functions.h
#ifndef TEMP_FUNCTIONS_H
#define TEMP_FUNCTIONS_H

#define SIZE 1000 // максимальное количество записей

// Структура для хранения данных о температуре
struct sensor
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int t; // температура
};

// Прототипы функций
int LoadDataFromFile(const char *filename, struct sensor *info);
void PrintStatsByMonth(int month, struct sensor *info, int size);
void PrintYearlyStats(struct sensor *info, int size);
void PrintHelp();
int DateToInt(struct sensor *info);
void SortByDate(struct sensor *info, int n);

#endif // TEMP_FUNCTIONS_H
