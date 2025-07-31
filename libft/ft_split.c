/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:24:44 by austunso          #+#    #+#             */
/*   Updated: 2024/10/26 16:02:19 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*del(char **ptr, int len)
{
	int	a;

	a = 0;
	while (a < len)
	{
		free(ptr[a]);
		a++;
	}
	free(ptr);
	return (NULL);
}

static int	ft_count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s != '\0' && *s == c)
			s++;
		if (*s == '\0')
			return (count);
		while (*s != '\0' && *s != c)
			s++;
		count++;
	}
	return (count);
}

static int	ft_count_chars(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		len;

	len = 0;
	if (!s)
		return (NULL);
	str = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!str)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (!*s)
			break ;
		str[len] = ft_substr(s, 0, ft_count_chars(s, c));
		if (str[len] == NULL)
			return (del(str, len));
		s += ft_count_chars(s, c);
		len++;
	}
	str[len] = NULL;
	return (str);
}
