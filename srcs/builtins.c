/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 16:34:44 by cguiot            #+#    #+#             */
/*   Updated: 2022/03/18 20:30:27 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	echo(int fd, char **args)
{
	int	i;
	int	tirayn;

	i = 1;
	tirayn = 0;
	while (args[i] && !strcmp(args[i], "-n"))
	{
		tirayn = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(fd, args[i]);
		if (args[i + 1])
			ft_putstr_fd(fd, " ");
		i++;
	}
	if (!tirayn)
		ft_putstr_fd(fd, "\n");
	return (1);
}

int	cd(t_command *cmds, t_lst *data, char **args)
{
	t_env	*env;
	t_env	*tmp;

	env = data->first;
	if (!args[1])
		chdir("~");
	else if (args[2] || chdir(args[1]) == -1)
	{
		errno = 2;
		return (-1);
	}
	env = get_key(data, "OLDPWD");
	tmp = get_key(data, "PWD");
	free(env->value);
	env->value = tmp->value;
	if (args[1])
		tmp->value = getcwd(NULL, 0);
	else if (get_key(data, "HOME"))
		tmp->value = ft_strdup((get_key(data, "HOME"))->value, 0);
	else
		error(cmds, data->first, 30002, 0);
	if (!tmp->value)
		return (-1);
	return (1);
}

int	expor(t_command *cmd, t_lst *data, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '/'))
		{
			errno = 30003;
		}
		else if (ft_strchr(args[i], '='))
		{
			unset(cmd, data, args + i - 1, 0);
			if (data->last)
			{
				data->last->next = create_env_elem(args[i]);
				data->last->next->prev = data->last;
				data->last = data->last->next;
			}
			else
				data->last = create_env_elem(args[i]);
		}
		i++;
	}
	return (1);
}

int	unset(t_command *cmd, t_lst *data, char **args, int test)
{
	t_env	*env;
	int		i;

	i = 0;
	while (args[++i])
	{
		if (ft_strchr(args[i], '=') == 1 && test == 1 && !error(cmd, 0, 2, 0))
			return (1);
		env = get_key(data, args[i]);
		if (env)
		{
			if (env == data->last)
				data->last = env->prev;
			free(env->key);
			free(env->value);
			if (env->prev)
				(env->prev)->next = env->next;
			if (env->next)
				(env->next)->prev = env->prev;
		}
		free(env);
	}
	return (1);
}

int	env(t_lst *data, int ntm)
{
	t_env	*env;

	env = data->first;
	if (ntm == 0)
	{
		while (env != NULL)
		{
			ft_putstr(env->key);
			ft_putstr("=");
			ft_putstr(env->value);
			ft_putstr("\n");
			env = env->next;
		}
	}
	return (1);
}
