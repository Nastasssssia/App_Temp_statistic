
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "temp_functions.h"

int main(int argc, char *argv[])
{
    struct sensor info[SIZE];
    const char *filename = NULL;
    int month = 0, size = 0;

    int opt;
    while ((opt = getopt(argc, argv, "hf:m:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            PrintHelp();
            return 0;
        case 'f':
            filename = optarg;
            break;
        case 'm':
            month = atoi(optarg);
            if (month < 1 || month > 12)
            {
                fprintf(stderr, "Invalid month number. Please provide a month between 1 and 12.\n");
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Invalid option\n");
            return 1;
        }
    }

    if (!filename)
    {
        fprintf(stderr, "Input file not specified.\n");
        return 1;
    }

    size = LoadDataFromFile(filename, info);
    if (size < 0)
        return 1;

    SortByDate(info, size);

    if (month > 0)
    {
        PrintStatsByMonth(month, info, size);
    }
    else
    {
        PrintYearlyStats(info, size);
    }

    return 0;
}
