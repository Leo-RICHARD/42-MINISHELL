/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrichard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 20:20:57 by lrichard          #+#    #+#             */
/*   Updated: 2022/03/09 16:04:31 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	count_word(char *str, char c)
{
	int		i;
	int		find;
	int		count;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		find = 0;
		while ((str[i] == c) && str[i])
			i++;
		while (str[i] != c && str[i])
		{
			i++;
			find = 1;
		}
		count = count + find;
	}
	return (count + (str[0] == 0));
}

static int	get_word_length(char *str, char c, int i)
{
	int		len;

	len = 0;
	while (str[i] && str[i] != c)
	{
		i++;
		len++;
	}
	return (len);
}

static char	**ft_free(int i, char **res)
{
	while (i != 0)
	{
		free(res[i]);
		res[i] = NULL;
		i--;
	}
	res = NULL;
	return (NULL);
}

char	**ft_split(char *s, char c)
{
	int		word;
	int		i;
	int		j;
	int		l;
	char	**res;

	j = 0;
	l = 0;
	word = count_word(s, c);
	if (!nmalloc_2d((char ***)&res, word + 1))
		return (0);
	while (l < word)
	{
		i = 0;
		while (s[j] == c)
			j++;
		if (!nmalloc((void **)&(res[l]), get_word_length((char *)s, c, j) + 1))
			return (ft_free(l, res));
		while (s[j] != c && s[j])
			res[l][i++] = s[j++];
		res[l++][i] = '\0';
	}
	res[l] = NULL;
	return (res);
}
