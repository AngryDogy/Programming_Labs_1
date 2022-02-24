#include <stdio.h>
#include<string.h>
void countWords(FILE *file)
{
    int ans = 0, check = 0;
    char s;
    while ((s = getc(file)) != EOF)
    {
        if ((s == ' ' || s == '\n' || s == '\t') && check == 1)
        {
            ans++;
            check = 0;
            continue;
        }
        if (s != ' ' && s != '\n' && s != '\t')
            check = 1;
    }
    if (check == 1)
        ans++;
    printf("%d words", ans);
}
void countLines(FILE *file)
{
    int ans = 0;
    char s;
    while ((s = getc(file)) != EOF)
    {
        if (s == '\n')
        {
            ans++;
        }
    }
    ans++;
    printf("%d lines", ans);
}
void countBytes(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    int sz = ftell(file);
    printf("%d bytes", sz);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Wrong arguments! :(");
        return -1;
    }
    FILE *file;
    file = fopen(argv[2], "r");
    if (file == NULL)
    {
        printf("Can't open this file!\n");
        return -1;
    }
    else
    {
        if (strcmp(argv[1], "-w") == 0)
        {
            countWords(file);
        }
        if (strcmp(argv[1], "-l") == 0)
        {
            countLines(file);
        }
        if (strcmp(argv[1], "-c") == 0)
        {
            countBytes(file);
        }
    }
    return 0;
}