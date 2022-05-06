#include <terminal.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>


int isspace(const char str) {
    if (str == '\n' || str == ' ' || str == '\t') return 0;
    return 1;
}

size_t strlen(const char *str)
{
    if (str == NULL) return 0;
    size_t length = 0;
    while(str[length]) length++;
    return length;
}

char *strcpy(char *destination, const char *source)
{
    if (destination == NULL) return NULL;
    char *ptr = destination;
    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
    return ptr;
}

char *strncpy(char *destination, const char *source, size_t n)
{
    if (destination == NULL) return NULL;
    char *ptr = destination;
    for (size_t i = 0; i < n && *source != '\0'; i++)
    {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
    return ptr;
}

char *strcat(char *destination, const char *source)
{
    if (destination == NULL) return NULL;
    char *ptr = destination + strlen(destination);
    while (*source != '\0') *ptr++ = *source++;
    return destination;
}

char *strchr(const char *str, char ch)
{
    if (str == NULL || ch == 0) return NULL;
    while (*str && *str != ch) ++str;
    return (char*)(ch == *str ? str : NULL);
}

int strcmp(const char *a, const char *b)
{
    if (a == NULL || b == NULL) return 1;
    while (*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return *a - *b;
}

int strncmp(const char *a, const char *b, size_t n)
{
    if (a == NULL || b == NULL) return 1;
    for (size_t i = 0; i < n; i++)
    {
        if (a[i] != b[i]) return 1;
    }
    return 0;
}

/*
char *strrm(char *str, const char *substr)
{
    if (str == NULL || substr == NULL) return NULL;
    char *p, *q, *r;
    if (*substr && (q = r = strstr(str, substr)) != NULL)
    {
        size_t len = strlen(substr);
        while ((r = strstr(p = r + len, substr)) != NULL)
        {
            while (p < r) *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0') continue;
    }
    return str;
}*/

/*
char *strdup(const char *src)
{
    size_t len = strlen(src) + 1;
    char *s = new char[len];
    if (s == nullptr) return nullptr;

    memcpy(s, const_cast<char*>(src), len);

    return s;
}*/

void strrev(unsigned char *str)
{
    unsigned char a;
    unsigned len = strlen((const char*)(str));

    for (size_t i = 0, j = len - 1; i < j; i++, j--)
    {
        a = str[i];
        str[i] = str[j];
        str[j] = a;
    }
}

/*
long strtol(const char *nPtr, char **endPtr, int base)
{
    if((base < 2 || base > 36) && base != 0) return 0;

    long number = 0;
    const char * divider;
    int currentdigit, sign, cutlim;
    enum sign
    {
        NEGATIVE,
        POSITIVE
    };
    unsigned long cutoff;
    bool correctconversion = true;

    divider = nPtr;

    while (isspace(* divider)) divider++;

    if (* divider == '+')
    {
        sign = POSITIVE;
        divider++;
    }
    else if (* divider == '-')
    {
        sign = NEGATIVE;
        divider++;
    }
    else sign = POSITIVE;

    if (*divider == 0)
    {
        *endPtr = (char*)(divider);
        return 0;
    }

    if (*divider < '0' || (*divider > '9' && *divider < 'A') || (*divider > 'z')) return 0;

    if ((base == 8) && (*divider == '0'))
    {
        divider++;
        if (*divider == 'o' || *divider == 'O') divider++;
    }
    else if (base == 16)
    {
        if (*divider == '0')
        {
            divider++;
            if (*divider == 'x' || *divider == 'X')
            {
                divider++;
                if (*divider > 'f' || *divider > 'F')
                {
                    divider--;
                    *endPtr = (char*)(divider);
                    return 0;
                }
            }
            else divider--;
        }
    }
    else if (base == 0)
    {
        if (*divider == '0')
        {
            divider++;
            if (*divider == 'o' || *divider == 'O')
            {
                base = 8;
                divider++;
                if (*divider > '7')
                {
                    divider--;
                    *endPtr = (char*)(divider);
                    return 0;
                }
            }
            else if (*divider == 'x' || *divider == 'X')
            {
                base = 16;
                divider++;
                if (*divider > 'f' || * divider > 'F')
                {
                    divider--;
                    *endPtr = (char*)(divider);
                    return 0;
                }
            }
            else if (*divider <= '7') base = 8;
            else
            {
                *endPtr = (char*)(divider);
                return 0;
            }
        }
        else if (*divider >= '1' && *divider <= '9') base = 10;
    }

    if (sign) cutoff = LONG_MAX / (unsigned long)(base);
    else cutoff = (unsigned long)(LONG_MIN) / (unsigned long)(base);

    cutlim = cutoff % (unsigned long)(base);

    while (*divider != 0)
    {
        if (isdigit(*divider)) currentdigit = * divider - '0';
        else
        {
            if (isalpha(*divider))
            {
                if (islower(*divider) && (*divider - 'a') + 10 < base) currentdigit = (*divider - 'a') + 10;
                else if (!islower(*divider) && (*divider - 'A') + 10 < base) currentdigit = (*divider - 'A') + 10;
                else break;
            }
            else break;
        }
        if (!correctconversion || number > (long)(cutoff) || (number == (long)(cutoff) && (int)(currentdigit) > cutlim))
        {
            correctconversion = false;
            divider++;
        }
        else
        {
            correctconversion = true;
            number = (number * base) + currentdigit;
            divider++;
        }
    }
    if (!correctconversion)
    {
        if (sign) number = LONG_MAX;
        else number = LONG_MIN;
    }
    if (sign == NEGATIVE) number *= -1;
    if (endPtr != NULL)
    {
        if (isspace(*divider)) divider++;
        *endPtr = (char*)(divider);
    }
    return number;
}*/