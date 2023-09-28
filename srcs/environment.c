/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrichard <lrichard@student.42lyon.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 19:28:02 by lrichard          #+#    #+#             */
/*   Updated: 2022/03/18 21:09:21 by lrichard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_env_size(char **envr)
{
	int	i;

	i = 0;
	while (envr[i])
		i++;
	return (i);
}

int	aff_key(t_lst *data, char *str)
{
	t_env	*env;

	env = data->first;
	while (env != NULL && ft_strcmp(str, env->key))
		env = env->next;
	if (env != NULL && !ft_strcmp(str, env->key))
		printf("%s\n", env->value);
	return (1);
}

char	*get_value(t_lst *data, char *key)
{
	char	*dollarquestion;
	t_env	*env;
	int		i;

	if (!strcmp(key, "?"))
		dollarquestion = ft_itoa(g_int[0]);
	if (!strcmp(key, "?"))
		return (dollarquestion);
	i = 0;
	env = data->first;
	while (env && key[i])
	{
		if (key[i] != env->key[i])
		{
			env = env->next;
			i = -1;
		}
		i++;
	}
	if (env && ft_strlen(key) != ft_strlen(env->key))
		return (ft_strdup("", 0));
	if (env)
		return (ft_strdup(env->value, 0));
	else
		return (ft_strdup("", 0));
}

t_env	*get_key(t_lst *data, char *key)
{
	t_env	*env;
	int		i;

	i = 0;
	env = data->first;
	while (env && key[i])
	{
		if (key[i] == '=')
			break ;
		if (key[i] != env->key[i])
		{
			env = env->next;
			i = -1;
		}
		i++;
	}
	if (env && (i != ft_strlen(env->key)))
		return (NULL);
	return (env);
}
