#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ft_replace(char *str, char *to_find, char *to_replace)
{
    char *new_str = NULL;
    char *tmp;
    char *start;
    int i, j, k;
    int found = 0;
    int str_len = strlen(str);
    int find_len = strlen(to_find);
    int replace_len = strlen(to_replace);

    for (i = 0; i < str_len;)
    {
        found = 1;
        for (j = 0; j < find_len; j++)
        {
            if (str[i + j] != to_find[j])
            {
                found = 0;
                break;
            }
        }

        if (found)
        {
            tmp = (char *)malloc(i + replace_len + 1);
            for (k = 0; k < i; k++)
                tmp[k] = str[k];
            for (k = 0; k < replace_len; k++)
                tmp[i + k] = to_replace[k];
            for (k = i + find_len; k < str_len; k++)
                tmp[i + replace_len + k - (i + find_len)] = str[k];
            tmp[i + replace_len + str_len - (i + find_len)] = '\0';

            if (new_str)
                free(new_str);

            new_str = tmp;
            i += replace_len;
            str_len = strlen(new_str);
        }
        else
        {
            i++;
        }
    }

    if (!new_str)
    {
        new_str = (char *)malloc(str_len + 1);
        strcpy(new_str, str);
    }

    return new_str;
}

int main(int ac, char **av)
{
    char *result = ft_replace(av[1], av[2], av[3]);
    printf("Result: %s\n", result);

    free(result);

    return 0;
}
