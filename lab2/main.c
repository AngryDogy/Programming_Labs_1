#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

typedef unsigned int ui;
struct uint1024_t
{
    ui bytes[32];
};

struct uint1024_t from_uint(ui x)
{
    struct uint1024_t result = {0};
    result.bytes[31] = x;
    return result;
}

void printf_value(struct uint1024_t x)
{
    char result[512], degree[512];
    ui bytes, value, carry;
    for (int i = 0; i < 512; i++)
    {
        result[i] = '0';
        degree[i] = '0';
    }
    degree[511] = '1';
    for (int i = 31; i >= 0; i--)
    {
        bytes = x.bytes[i];
        for (int k = 0; k < 32; k++)
        {
            if (bytes % 2 == 1)
            {
                carry = 0;
                for (int j = 511; j >= 0; j--)
                {
                    value = (result[j] - '0') + (degree[j] - '0') + carry;
                    if (value > 9)
                    {
                        result[j] = (value % 10) + '0';
                        carry = value / 10;
                    }
                    else
                    {
                        result[j] = value + '0';
                        carry = 0;
                    }
                }
            }
            carry = 0;
            for (int j = 511; j >= 0; j--)
            {
                value = (degree[j] - '0') + (degree[j] - '0') + carry;
                if (value > 9)
                {
                    degree[j] = (value % 10) + '0';
                    carry = value / 10;
                }
                else
                {
                    degree[j] = value + '0';
                    carry = 0;
                }
            }
            bytes = bytes / 2;
        }
    }
    int check = 0;
    for (int i = 0; i < 512; i++)
    {
        if (result[i] == '0' && check == 0)
        {
            continue;
        }
        else
        {
            printf("%c", result[i]);
            check = 1;
        }
    }
    if (check == 0)
        printf("%d", 0);
    printf("\n");
}

struct uint1024_t add_op(struct uint1024_t x, struct uint1024_t y)
{
    struct uint1024_t result;
    ui carry = 0;
    for (int i = 31; i >= 0; i--)
    {
        result.bytes[i] = x.bytes[i] + y.bytes[i] + carry;
        if (UINT_MAX - max(x.bytes[i], y.bytes[i]) < min(x.bytes[i], y.bytes[i]))
        {
            carry = 1;
        }
        else
        {
            carry = 0;
        }
    }
    return result;
}

struct uint1024_t subtr_op(struct uint1024_t x, struct uint1024_t y)
{
    struct uint1024_t result;
    int carry = 0;
    for (int i = 31; i >= 0; i--)
    {
        result.bytes[i] = x.bytes[i] - y.bytes[i] + carry;
        if (x.bytes[i] <= y.bytes[i] && carry == -1)
            continue;
        if (x.bytes[i] < y.bytes[i])
        {
            carry = -1;
        }
        else
        {
            carry = 0;
        }
    }
    return result;
}

struct uint1024_t mult_op(struct uint1024_t x, struct uint1024_t y)
{
    struct uint1024_t result = {0};
    ui bytes, carry;
    for (int i = 31; i > 0; i--)
    {
        bytes = y.bytes[i];
        for (int k = 0; k < 32; k++)
        {
            if (bytes % 2 == 1)
            {
                result = add_op(result, x);
            }
            bytes = bytes / 2;
            carry = 0;
            for (int j = 31; j >= 0; j--)
            {
                if ((x.bytes[j] & (1 << 31)) >= (1 << 31))
                {
                    x.bytes[j] = x.bytes[j] - (1 << 31);
                    x.bytes[j] = (x.bytes[j] << 1) + carry;
                    carry = 1;
                }
                else
                {
                    x.bytes[j] = (x.bytes[j] << 1) + carry;
                    carry = 0;
                }
            }
        }
    }
    return result;
}

void scanf_value(struct uint1024_t *x)
{
    char s[512] = {'0'}, digit;
    size_t size = 0;
    scanf("%s", &s);
    int i = 0, k, val, carry = 0;
    while (s[i] <= '9' && s[i] >= '0')
    {
        size++;
        i++;
    }
    size--;
    k = 0;
    i = 31;
    while (i != 0)
    {
        digit = s[size];
        if (digit == '1' || digit == '3' || digit == '5' || digit == '7' || digit == '9')
        {

            x->bytes[i] = x->bytes[i] + (1 << k);
        }
        k++;
        if (k == 32)
        {
            k = 0;
            i--;
        }
        carry = 0;
        for (int j = 0; j <= size; j++)
        {
            val = s[j] - '0' + carry * 10;
            if (val % 2 != 0)
            {
                carry = 1;
            }
            else
            {
                carry = 0;
            }
            val = val / 2;
            s[j] = val + '0';
        }
    }
}

int main()
{
    struct uint1024_t x = from_uint(0), y = from_uint(0);
    scanf_value(&x);
    scanf_value(&y);
    struct uint1024_t add_result = add_op(x, y);
    printf("Add result: ");
    printf_value(add_result);
    struct uint1024_t subtr_result = subtr_op(x, y);
    printf("Subtruction result: ");
    printf_value(subtr_result);
    struct uint1024_t mult_result = mult_op(x, y);
    printf("Multiplication result: ");
    printf_value(mult_result);
    return 0;
}
