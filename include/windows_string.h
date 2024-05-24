//
// Created by 17579 on 5/22/2024.
//

#ifndef CALIBRATION_WINDOW_STRING_H
#define CALIBRATION_WINDOW_STRING_H

#endif //CALIBRATION_WINDOW_STRING_H

#include <cstring>

char* strsep(char** stringp, const char* delim)
{
    char* start = *stringp;
    char* p;

    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}
