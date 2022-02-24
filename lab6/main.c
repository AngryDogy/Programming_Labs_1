#include <stdio.h>
#include<string.h>

typedef long long ll;
char archive[100], command[100], files[256][100];
ll size[256];

void get_string(char *str, char *result)
{
    int i = 0;
    while (str[i] != '\0')
    {
        result[i] = str[i];
        i++;
    }
}

void create(FILE *arc, int n)
{
    putc(n, arc);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; files[i][j] != '\0'; j++)
        {
            putc(files[i][j], arc);
        }
        putc(0, arc);
        ll val = size[i];
        for (int j = 0; j < 8; j++)
        {
            char byte = 0;
            ll degree = 1;
            for (int k = 0; k < 7; k++)
            {
                if (val % 2 == 1)
                {
                    byte = byte + degree;
                }
                val = val / 2;
                degree = degree * 2;
            }
            putc(byte, arc);
        }
    }
    char s;
    for (int i = 0; i < n; i++)
    {
        FILE *file = fopen(files[i], "rb");;
        for (int j = 0; j < size[i]; j++)
        {
            s = getc(file);
            putc(s, arc);

        }
        fclose(file);
        remove(files[i]);
    }


}

void extract(FILE *arc, int n)
{
    char s;
    for (int i = 0; i < n; i++)
    {
        FILE *file = fopen(files[i], "wb");
        for (int j = 0; j < size[i]; j++)
        {
            s = getc(arc);
            putc(s, file);

        }
        fclose(file);
    }
}

void list(int n)
{
    printf("This archive contains:\n");
    for (int i = 0; i < n; i++)
    {
        printf("File's name: %s, File's size: %lu bytes\n", files[i], size[i]);
    }
}

int main(int argc, char **argv)
{
    get_string(argv[2], archive);
    get_string(argv[3], command);
    if (strcmp(command, "--create") == 0)
    {
        int k = 0;
        for (int i = 4; i < argc; i++)
        {
            get_string(argv[i], files[k]);
            FILE *file = fopen(files[k], "rb");
            if (file == NULL)
            {
                continue;
            }
            fseek(file, 0L, SEEK_END);
            size[k] = ftell(file);
            fclose(file);
            k++;
        }
        FILE *arc = fopen(archive, "wb");
        create(arc, k);
    }
    else
    {
        FILE *arc = fopen(archive, "rb");
        if (arc == NULL)
        {
            return -1;
        }
        char s = getc(arc);
        int n = s, k;
        for (int i = 0; i < n; i++)
        {
            k = 0;
            s = 1;
            while (s != 0)
            {
                s = getc(arc);
                files[i][k] = s;
                k++;
            }
            ll val = 0, shift = 0, bits;
            for (int j = 0; j < 8; j++)
            {
                s = getc(arc);
                bits = s;
                bits = bits << shift;
                val = val + bits;
                shift = shift + 7;
            }
            size[i] = val;
        }
        if (strcmp(command, "--extract") == 0)
        {
            extract(arc, n);
        }
        else
        {
            list(n);
        }
    }
    return 0;
}
