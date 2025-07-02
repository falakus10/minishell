/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 12:53:14 by austunso          #+#    #+#             */
/*   Updated: 2024/10/27 19:27:47 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	size_t	a;
	size_t	b;

	a = 0;
	b = 0;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (haystack[a] != '\0' && a < n)
	{
		if (haystack[a] == needle[b])
		{
			while (haystack[a + b] == needle[b] && a + b < n)
			{
				if (needle[b + 1] == '\0')
					return ((char *)haystack + a);
				b++;
			}
			b = 0;
		}
		a++;
	}
	return (NULL);
}
