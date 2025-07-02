/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 09:27:59 by austunso          #+#    #+#             */
/*   Updated: 2024/10/27 19:55:24 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	if ((char)c == 0)
		return ((char *)(s + i));
	while (i - 1 >= 0)
	{
		if (s[i - 1] == (char)c)
			return ((char *)(s + (i - 1)));
		i--;
	}
	return (NULL);
}
