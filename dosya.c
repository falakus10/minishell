#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

static int	ft_checker(char s1, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == s1)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	char	*ptr;
	size_t	k;

	if (s1 == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0' && ft_checker(s1[i], set) != 0)
		i++;
	j = ft_strlen(s1);
	while (j > i && ft_checker(s1[j - 1], set) != 0)
		j--;
	ptr = (char *)malloc(sizeof(char) * (j - i) + 1);
	if (ptr == NULL)
		return (NULL);
	k = 0;
	while (i < j)
	{
		ptr[k] = s1[i];
		k++;
		i++;
	}
	ptr[k] = '\0';
	return (ptr);
}

int main()
{
    char *str = "cccj7ccabcc";
    printf("%s\n",str);
    str = ft_strtrim (str, "c");
    printf("%s\n",str);

}