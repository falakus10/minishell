/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:30:13 by austunso          #+#    #+#             */
/*   Updated: 2024/10/27 16:05:20 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
