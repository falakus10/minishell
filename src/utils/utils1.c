#include "minishell.h"

char *ft_strcpy(char *dest, const char *src)
{
    int i = 0;

    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

char *ft_strcat(char *dest, const char *src)
{
    int i = 0;
    int j = 0;

    // dest'in sonuna git
    while (dest[i])
        i++;

    // src'yi dest'in sonuna kopyala
    while (src[j])
    {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
    return dest;
}
