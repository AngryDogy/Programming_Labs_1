#include <stdio.h>
#include<string.h>
#include <stdlib.h>

char pixels[10000][10000], pixels_edit[10000][10000];
int width, height, bytes_per_pixels, pixels_position;

int get_num(FILE *pic, int n)
{
    int degree = 1, ans = 0;
    char s;
    for (int i = 0; i < n; i++)
    {
        s = getc(pic);
        for (int j = 0; j < 8; j++)
        {
            if (s % 2 == 1)
            {
                ans = ans + degree;
            }
            s = s / 2;
            degree = degree * 2;
        }
    }
    return ans;
}

int check_pixel(FILE *pic, int n)
{
    char bytes[10];
    for (int i = 0; i < n; i++)
    {
        bytes[i] = getc(pic);
    }
    int check = 0;
    for (int i = 0; i < n; i++)
    {
        if (bytes[i] == -1)
        {
            check++;
        }
    }
    if (check != n)
        return 1;
    else
        return 0;
}

void get_bytes(FILE *pic, int n)
{
    char s;
    for (int i = 0; i < n; i++)
    {
        s = getc(pic);
    }
}

void get_picture(FILE *pic)
{

    get_bytes(pic, 10);
    pixels_position = get_num(pic, 4);
    get_bytes(pic, 4);
    width = get_num(pic, 4);
    height = get_num(pic, 4);
    get_bytes(pic, 2);
    bytes_per_pixels = get_num(pic, 2);
    fseek(pic, pixels_position, SEEK_SET);
    for (int i = height; i >= 1; i--)
    {
        for (int j = 1; j <= width; j++)
        {
            pixels[i][j] = check_pixel(pic, bytes_per_pixels / 8);
        }
        int skip = 0, val = width * (bytes_per_pixels / 8);
        while (val % 4 != 0)
        {
            skip++;
            val++;
        }
        get_bytes(pic, skip);
    }
}

void life_game()
{
    int alive;
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            alive = 0;
            if (pixels[i - 1][j] == 1)
            {
                alive++;
            }
            if (pixels[i + 1][j] == 1)
            {
                alive++;
            }
            if (pixels[i][j - 1] == 1)
            {
                alive++;
            }
            if (pixels[i][j + 1] == 1)
            {
                alive++;
            }
            if (pixels[i - 1][j - 1] == 1)
            {
                alive++;
            }
            if (pixels[i - 1][j + 1] == 1)
            {
                alive++;
            }
            if (pixels[i + 1][j - 1] == 1)
            {
                alive++;
            }
            if (pixels[i + 1][j + 1] == 1)
            {
                alive++;
            }
            if (pixels[i][j] == 0)
            {
                if (alive == 3)
                {
                    pixels_edit[i][j] = 1;
                }
            }
            else
            {
                if (alive < 2 || alive > 3)
                {
                    pixels_edit[i][j] = 0;
                }
            }
        }
    }
}

int create_picture(FILE *new, FILE *old)
{
    int bytes = 0, check = 0;
    char s, byte = 0;
    while (bytes < pixels_position)
    {
        s = getc(old);
        putc(s, new);
        bytes++;
    }
    for (int i = height; i >= 1; i--)
    {
        for (int j = 1; j <= width; j++)
        {
            pixels[i][j] = pixels_edit[i][j];
            if (pixels_edit[i][j] == 1)
            {
                check++;
                byte = 0;
            }
            else
            {
                byte = -1;
            }
            for (int k = 0; k < bytes_per_pixels / 8; k++)
            {
                putc(byte, new);
            }
        }
        int skip = 0, val = width * (bytes_per_pixels / 8);
        while (val % 4 != 0)
        {
            skip++;
            val++;
        }
        for (int k = 0; k < skip; k++)
        {
            putc(0, new);
        }
    }
    return check;
}

int main(int argc, char **argv)
{
    int max_iter = 500, frequency = 1, name_position;
    char input[100] = {0}, output[100] = {0};
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--input") == 0)
        {
            for (int j = 0; argv[i + 1][j] != '\0'; j++)
            {
                input[j] = argv[i + 1][j];
            }
        }
        if (strcmp(argv[i], "--output") == 0)
        {
            for (int j = 0; argv[i + 1][j] != '\0'; j++)
            {
                output[j] = argv[i + 1][j];
            }
        }
        if (strcmp(argv[i], "--max_iter") == 0)
        {
            max_iter = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "--dump_freq") == 0)
        {
            frequency = atoi(argv[i + 1]);
        }
    }
    FILE *picture = fopen(input, "r");
    if (picture == NULL)
    {
        printf("Can't open this file!\n");
        return -1;
    }
    get_picture(picture);
    fclose(picture);
    for (int i = 0; i < 100; i++)
    {
        if (output[i] == '\0')
        {
            output[i] = '/';
            output[i + 1] = '1';
            name_position = i + 1;
            output[i + 2] = '.';
            output[i + 3] = 'b';
            output[i + 4] = 'm';
            output[i + 5] = 'p';
            output[i + 6] = '\0';
            break;
        }
    }
    for (int i = 1; i <= max_iter; i++)
    {
        life_game();
        if ((i - 1) % frequency == 0)
        {
            FILE *picture = fopen(input, "r");
            FILE *new_generation = fopen(output, "w");
            if (create_picture(new_generation, picture) == 0)
            {
                break;
            }
            fclose(picture);
            fclose(new_generation);
        }
        else
        {
            for (int j = 1; j <= height; j++)
            {
                for (int k = 1; k <= width; k++)
                {
                    pixels[j][k] = pixels_edit[j][k];
                }
            }
        }
        int iteration = i, k = 0, pos = name_position;
        char digits[100];
        while (iteration != 0)
        {
            digits[k] = (iteration % 10) + '0';
            iteration = iteration / 10;
            k++;
        }
        for (int j = k - 1; j >= 0; j--)
        {
            output[pos] = digits[j];
            pos++;
        }
        output[pos] = '.';
        output[pos + 1] = 'b';
        output[pos + 2] = 'm';
        output[pos + 3] = 'p';
        output[pos + 4] = '\0';
    }

    return 0;
}
