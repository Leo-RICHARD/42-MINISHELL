/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 18:43:40 by cguiot            #+#    #+#             */
/*   Updated: 2022/03/17 18:44:18 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

unsigned long long int	ft_atoi_custom(char *str)
{
	int						i;
	int						sign;
	unsigned long long int	n;

	i = 0;
	sign = 1;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-' && i++)
		sign = -1;
	else if (str[i] == '+')
		i++;
	n = 0;
	while (str[i] > 47 && str[i] < 58)
	{
		n *= 10;
		n += str[i] - 48;
		if (n > 9223372036854775807LL)
			return (9223372036854775808ULL);
		i++;
	}
	if (str[i])
		return (9223372036854775808ULL);
	return (n * sign);
}

char	*ft_itoa(int n)
{
	int		i;
	int		ntmp;
	char	*str;

	if (!nmalloc((void **)&str, 4))
		return (ft_strdup("", 0));
	i = 0;
	ntmp = n;
	while (ntmp >= 10)
	{
		i++;
		ntmp /= 10;
	}
	str[i + 1] = 0;
	while (i > -1)
	{
		str[i] = n % 10 + 48;
		n /= 10;
		i--;
	}
	return (str);
}
