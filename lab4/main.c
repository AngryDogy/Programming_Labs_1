#include <stdio.h>
#include<string.h>


char path[1000], name[1000], value[1000], bytes[10], data[4];
struct tags
{
    char id[1000];
    char inf[1000];
    int num_bytes;
} saved_tags[10000];

int get_string(char *argv, char *arr)
{
    int k = 0, check = 0;
    for (int i = 0; argv[i] != '\0'; i++)
    {
        if (check)
        {
            arr[k] = argv[i];
            k++;
        }
        if (argv[i] == '=')
        {
            check = 1;
        }
    }
    return k;
}

void get_bytes(FILE *in, int *num_bytes)
{
    for (int i = 0; i < 10; i++)
    {
        bytes[i] = getc(in);
    }
    *num_bytes = *num_bytes + 10;
}

int get_length(char *data)
{
    int bits, shift = 0, len = 0;
    for (int i = 3; i >= 0; i--)
    {
        bits = data[i];
        bits = bits << shift;
        len = len + bits;
        shift = shift + 7;
    }
    return len;
}

int get_tags(FILE *in)
{
    int header_len, len, n = 0, num_bytes = 0, k = 0;
    char s;
    get_bytes(in, &num_bytes);
    for (int i = 6; i < 10; i++)
    {
        data[k] = bytes[i];
        k++;
    }
    header_len = get_length(data);
    for (int i = 0; i < header_len; i = i + 10)
    {
        saved_tags[n].num_bytes = num_bytes;
        get_bytes(in, &num_bytes);
        if (bytes[0] == 0 && bytes[1] == 0 && bytes[2] == 0 && bytes[3] == 0)
            continue;
        for (int j = 0; j < 4; j++)
        {
            saved_tags[n].id[j] = bytes[j];
        }
        k = 0;
        for (int j = 4; j < 8; j++)
        {
            data[k] = bytes[j];
            k++;
        }
        len = get_length(data);
        k = 0;
        for (int j = 0; j < len; j++)
        {
            s = getc(in);
            i++;
            num_bytes++;
            if (j != 0)
            {
                saved_tags[n].inf[k] = s;
                k++;
            }
        }
        n++;
    }
    return n;
}

void file_edit(FILE *edit, FILE *in, int k, int *tags_number)
{
    char length[4] = {0};
    int degree, len = k;
    for (int i = 0; i < 4; i++)
    {
        degree = 1;
        for (int j = 0; j < 7; j++)
        {
            length[i] = length[i] + (len % 2) * degree;
            len = len / 2;
            degree = degree * 2;
        }
    }
    int pos = 0;
    for (int i = 0; i < *tags_number; i++)
    {
        if (strcmp(saved_tags[i].id, name) == 0)
        {
            pos = i;
            fseek(edit, saved_tags[i].num_bytes + 4, SEEK_SET);
            for (int j = 3; j >= 0; j--)
            {
                putc(length[j], edit);
            }
            break;
        }
    }
    fseek(edit, 3, SEEK_CUR);
    for (int i = 0; i < k - 1; i++)
    {
        putc(value[i], edit);
    }
    fseek(in, saved_tags[pos + 1].num_bytes, SEEK_SET);
    char s;
    while (!feof(in))
    {
        s = getc(in);
        putc(s, edit);
    }
}

int main(int argc, char **argv)
{
    get_string(argv[1], path);
    FILE *in;
    in = fopen(path, "r");
    if (in == NULL)
    {
        printf("Can't open this file!\n");
        return -1;
    }
    int tags_number = get_tags(in);
    fclose(in);
    if (strcmp(argv[2], "--show") == 0)
    {
        for (int i = 0; i < tags_number; i++)
        {
            printf("The %s containts: %s\n", saved_tags[i].id, saved_tags[i].inf);
        }
    }
    else
    {
        get_string(argv[2], name);
        if (argc == 4)
        {
            int k = get_string(argv[3], value);
            FILE *edit = fopen(path, "r+");
            FILE *in = fopen(path, "r");
            file_edit(edit, in, k + 1, &tags_number);
        }
        else
        {
            for (int i = 0; i < tags_number; i++)
            {
                if (strcmp(saved_tags[i].id, name) == 0)
                {
                    printf("The %s containts: %s\n", saved_tags[i].id, saved_tags[i].inf);
                    break;
                }
            }
        }
    }
    return 0;
}