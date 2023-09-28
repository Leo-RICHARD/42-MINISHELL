/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrichard <lrichard@student.42lyon.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 00:30:46 by lrichard          #+#    #+#             */
/*   Updated: 2022/03/18 20:28:07 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_strncpy(char *dst, char *src, int len)
{
	char	*dest;

	dest = dst;
	if (dst && src)
	{
		while (*src && len--)
			*(dst++) = *(src++);
		if (*dst)
			*dst = 0;
	}
	return (dest);
}

int	ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		if (str[i++] == c)
			return (1);
	return (0);
}

char	*ft_strjoin(char *s1, char *s2, int frees1, int frees2)
{
	int		len;
	char	*nstr;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	nstr = (char *)malloc(sizeof(char) * (len + 1));
	if (!nstr)
		return (NULL);
	nstr[len] = 0;
	len = 0;
	while (*s1)
		nstr[len++] = *(s1++);
	if (frees1)
		free(s1 - len);
	i = 0;
	while (s2[i])
		nstr[len++] = s2[i++];
	if (frees2)
		free(s2);
	return (nstr);
}

void	ft_putstr_fd(int fd, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}
