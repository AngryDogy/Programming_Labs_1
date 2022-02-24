#include <stdio.h>
#include<string.h>
#include<time.h>
struct tm data;
char logs[2000];
time_t times[2000000];
int year, month, day, hours, minutes, seconds, parameter_time;
int k = 0, pos = 0, max_amount = 0;
int ans_error, ans_time_beggin, ans_time_end;
struct strings
{
    char s[30];
} save[2000000];
char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int main()
{
    FILE *file;
    file = fopen("access_log_Jul95.txt", "r");
    printf("Enter parametric time: ");
    scanf("%d", &parameter_time);
    while (!feof(file))
    {
        fgets(logs, 2000, file);
        for (int i = 0; logs[i] != '\0'; i++)
        {
            if (logs[i] == '[')
            {
                int ch = 0;
                for (int j = i + 1; logs[j] != ' '; j++)
                {
                    save[k].s[ch] = logs[j];
                    ch++;
                }
                day = 10 * (logs[i + 1] - '0') + (logs[i + 2] - '0');
                i = i + 4;
                char month_symbl[4];
                month_symbl[0] = logs[i];
                month_symbl[1] = logs[i + 1];
                month_symbl[2] = logs[i + 2];
                month_symbl[3] = '\0';
                for (int j = 0; j < 12; j++)
                {
                    if (strcmp(month_symbl, months[j]) == 0)
                    {
                        month = j + 1;
                    }
                }
                i = i + 4;
                year = (logs[i] - '0') * 1000 + (logs[i + 1] - '0') * 100 + (logs[i + 2] - '0') * 10 + (logs[i + 3] - '0');
                i = i + 5;
                hours = 10 * (logs[i] - '0') + (logs[i + 1] - '0');
                i = i + 3;
                minutes = 10 * (logs[i] - '0') + (logs[i + 1] - '0');
                i = i + 3;
                seconds = 10 * (logs[i] - '0') + (logs[i + 1] - '0');
                data.tm_year = year - 1900;
                data.tm_mon = month - 1;
                data.tm_mday = day;
                data.tm_hour = hours;
                data.tm_min = minutes;
                data.tm_sec = seconds;
                times[k] = mktime(&data);
                if (times[k] - times[pos] >= parameter_time)
                {
                    if (max_amount < k - pos)
                    {
                        max_amount = k - pos;
                        ans_time_beggin = pos;
                        ans_time_end = k - 1;
                    }
                    while (times[k] - times[pos] >= parameter_time)
                    {
                        pos++;
                    }
                }
            }
            if (logs[i] == '"' && logs[i - 1] != ' ')
            {
                i = i + 2;
                if (logs[i] == '5')
                {
                    ans_error++;
                    printf("%s", logs);
                }
                k++;
                break;
            }
        }
    }
    printf("%d - the amount of 5xx errors requests\n", ans_error);
    printf("%d - the maximal amount of requests during %d seconds\n", max_amount, parameter_time);
    printf("Time window: from %s to %s\n", save[ans_time_beggin].s, save[ans_time_end].s);
    return 0;
}
