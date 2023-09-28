/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrichard <lrichard@student.42lyon.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 19:40:26 by lrichard          #+#    #+#             */
/*   Updated: 2022/03/09 16:05:33 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_strlen(char *str)
{
	char	*sstr;

	if (!str)
		return (0);
	sstr = str;
	while (*str)
		str++;
	return (str - sstr);
}

int	ft_strcmp(char *s1, char *s2)
{
	int				i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while ((str1[i] || str2[i]) && (str1[i] == str2[i]))
		i++;
	return (str1[i] - str2[i]);
}

int	excess_spaces(char *str)
{
	int	i;
	int	len_to_trim;

	len_to_trim = 0;
	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	len_to_trim = i;
	if (!str[i])
		return (i);
	i = ft_strlen(str);
	while (--i >= 0 && str[i] && str[i] == ' ')
		len_to_trim++;
	return (len_to_trim);
}

int	trim_spaces(char **str)
{
	int		i;
	int		j;
	int		k;
	char	*nstr;

	k = ft_strlen(*str);
	if (k == 0)
		return (1);
	if (!(nmalloc((void **)&nstr, k - \
					excess_spaces(*str) + 1)))
		return (0);
	i = 0;
	j = 0;
	while ((*str)[i] && (*str)[i] == ' ')
		i++;
	while ((*str)[--k] == ' ')
		;
	while ((*str)[i] && i <= k)
		nstr[j++] = (*str)[i++];
	free(*str);
	*str = nstr;
	return (1);
}

char	*ft_strdup(char *s, int frees)
{
	int		i;
	char	*dest;

	if (!s)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!dest)
		return (NULL);
	i = -1;
	while (s[++i])
		dest[i] = s[i];
	dest[i] = 0;
	if (frees)
		free(s);
	return (dest);
}
