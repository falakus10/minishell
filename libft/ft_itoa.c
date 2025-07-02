/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: austunso <austunso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:44:48 by austunso          #+#    #+#             */
/*   Updated: 2024/10/25 15:10:22 by austunso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(int number)
{
	int	counter;

	counter = 0;
	if (number == 0)
		counter++;
	if (number < 0)
	{
		number = number * (-1);
		counter++;
	}
	while (number != 0)
	{
		number = number / 10;
		counter++;
	}
	return (counter);
}

static void	make_positive(char *ptr, int *flag, long *nn)
{
	ptr[0] = '-';
	*nn *= -1;
	*flag = 1;
}

char	*ft_itoa(int n)
{
	int		len;
	int		flag;
	char	*ptr;
	long	nn;

	nn = (long)n;
	flag = 0;
	len = int_len(nn);
	ptr = (char *)malloc(sizeof(char) * (len) + 1);
	if (ptr == NULL)
		return (NULL);
	ptr[len] = '\0';
	if (nn < 0)
		make_positive(ptr, &flag, &nn);
	while (--len > 0)
	{
		ptr[len] = (nn % 10) + '0';
		nn = nn / 10;
	}
	if (flag == 0)
		ptr[0] = nn + '0';
	return (ptr);
}
