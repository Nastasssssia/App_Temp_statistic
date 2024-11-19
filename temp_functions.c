
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "temp_functions.h"

// Функция для загрузки данных из файла
int LoadDataFromFile(const char *filename, struct sensor *info)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "File opening error %s\n", filename);
        return -1;
    }

    char line[256];
    int line_number = 0;
    int size = 0;

    while (fgets(line, sizeof(line), file))
    {
        line_number++;
        // Пропускаем пустые строки
        if (line[0] == '\n')
            continue;

        int year, month, day, hour, minute, temperature;

        // Чтение данных из строки
        int n = sscanf(line, "%d;%d;%d;%d;%d;%d", &year, &month, &day, &hour, &minute, &temperature);

        // Проверка, что все данные корректно считаны
        if (n != 6)
        {
            fprintf(stderr, "Error in the line %d: incorrect data format, 6 values were expected, but found %d.\n", line_number, n);
            continue; // Пропускаем ошибочную строку
        }

        // Проверка на диапазон температуры
        if (temperature < -99 || temperature > 99)
        {
            fprintf(stderr, "Error in the line %d: The temperature should be between -99 and 99. Meaning: %d\n", line_number, temperature);
            continue; // Пропускаем ошибочную строку
        }

        if (year < 1000 || year > 9999)
        {
            fprintf(stderr, "Error in the line %d: The year should be a 4-digit number. Meaning: %d\n", line_number, year);
            continue; // Пропускаем ошибочную строку
        }

        // Дополнительные проверки
        if (month < 1 || month > 12)
        {
            fprintf(stderr, "Error in the line %d: incorrect month %d\n", line_number, month);
            continue;
        }
        if (day < 1 || day > 31)
        {
            fprintf(stderr, "Error in the line %d: incorrect day %d\n", line_number, day);
            continue;
        }
        if (hour < 0 || hour > 23)
        {
            fprintf(stderr, "Error in the line %d: incorrect hour %d\n", line_number, hour);
            continue;
        }
        if (minute < 0 || minute > 59)
        {
            fprintf(stderr, "Error in the line %d: incorrect minute %d\n", line_number, minute);
            continue;
        }

        if (size < SIZE)
        {
            info[size].year = year;
            info[size].month = month;
            info[size].day = day;
            info[size].hour = hour;
            info[size].minute = minute;
            info[size].t = temperature;
            size++;
        }
        else
        {
            fprintf(stderr, "Error: Record limit exceeded.\n");
            break;
        }
    }

    fclose(file);
    return size;
}

// Функция для сортировки данных по дате
int DateToInt(struct sensor *info)
{
    return (info->year << 16) | (info->month << 8) | info->day;
}

void SortByDate(struct sensor *info, int n)
{
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (DateToInt(&info[i]) > DateToInt(&info[j]))
            {
                struct sensor temp = info[i];
                info[i] = info[j];
                info[j] = temp;
            }
}

// Функция для печати статистики по месяцу
void PrintStatsByMonth(int month, struct sensor *info, int size)
{
    int total = 0, count = 0, min_temp = 100, max_temp = -100;

    for (int i = 0; i < size; i++)
    {
        if (info[i].month == month)
        {
            total += info[i].t;
            if (info[i].t < min_temp)
                min_temp = info[i].t;
            if (info[i].t > max_temp)
                max_temp = info[i].t;
            count++;
        }
    }

    if (count > 0)
    {
        printf("Month %d Statistics:\n", month);
        printf("  Average Temperature: %.2f\n", total / (float)count);
        printf("  Min Temperature: %d\n", min_temp);
        printf("  Max Temperature: %d\n", max_temp);
    }
    else
    {
        printf("No data for month %d\n", month);
    }
}

// Функция для печати статистики за год
void PrintYearlyStats(struct sensor *info, int size)
{
    int total = 0, count = 0, min_temp = 100, max_temp = -100;

    // Вывод всех данных за год
    printf("Yearly Data:\n");
    for (int i = 0; i < size; i++)
    {
        printf("Date: %d-%02d-%02d %02d:%02d, Temperature: %d\n",
               info[i].year, info[i].month, info[i].day,
               info[i].hour, info[i].minute, info[i].t);
    }

    // Подсчет статистики
    for (int i = 0; i < size; i++)
    {
        total += info[i].t;
        if (info[i].t < min_temp)
            min_temp = info[i].t;
        if (info[i].t > max_temp)
            max_temp = info[i].t;
        count++;
    }

    if (count > 0)
    {
        printf("\nYearly Statistics:\n");
        printf("  Average Temperature: %.2f\n", total / (float)count);
        printf("  Min Temperature: %d\n", min_temp);
        printf("  Max Temperature: %d\n", max_temp);
    }
    else
    {
        printf("No data for the year\n");
    }
}

void PrintHelp()
{
    printf("Usage: -f <filename.csv> [-m <month>]\n");
    printf("  -f <filename.csv> : Specify the input file containing temperature data.\n");
    printf("  -m <month>        : Specify the month for which to show the statistics (1-12).\n");
    printf("  -h                : Show this help message.\n");
}
