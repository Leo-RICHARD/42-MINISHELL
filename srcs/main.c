/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 17:02:17 by cguiot            #+#    #+#             */
/*   Updated: 2022/03/18 20:29:55 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_int[4] = {0};

char	**rebuild_envp(t_lst *env)
{
	int		i;
	int		len;
	char	**envp;
	t_env	*environment;

	if (!(nmalloc_2d(&envp, env->size + 1)))
		return (0);
	i = 0;
	environment = env->first;
	while (environment)
	{
		len = ft_strlen(environment->key) + ft_strlen(environment->value) + 1;
		if (!nmalloc((void **)&envp[i], len + 1))
			return (0);
		len = ft_strlen(environment->key);
		ft_strncpy(envp[i], environment->key, len);
		envp[i][len] = '=';
		ft_strncpy(envp[i] + len + 1, environment->value, \
				ft_strlen(environment->value));
		environment = environment->next;
		i++;
	}
	return (envp);
}

void	prompt_part_two(t_command *cmds, t_lst *data_env, int *thefinalpid, \
		int status)
{
	int			i;
	char		**path;
	char		*tmp;

	tmp = get_value(data_env, "PATH");
	path = ft_split(tmp, ':');
	free(tmp);
	if (!path)
		error(cmds, data_env->first, errno, 1);
	i = -1;
	while (path[++i])
		path[i] = ft_strjoin(path[i], "/", 1, 0);
	while (cmds && execute(cmds, path, data_env, thefinalpid))
		cmds = cmds->next;
	i = 0;
	while (path[i])
		free(path[i++]);
	free(path);
	if (*thefinalpid != -1)
	{
		while (wait(&status) > 0)
			;
		if (g_int[0] != 130)
			g_int[0] = WEXITSTATUS(status);
	}
}

void	prompt(t_cmd_lst *cmd_ctrl, t_lst *data_env, char *prt, int thefinalpid)
{
	prt = readline("Minishell-4.2$> ");
	while (prt)
	{
		unlinkk();
		if (prt && *prt)
		{
			add_history(prt);
			if (!parse_command(cmd_ctrl, data_env, prt) || (g_int[2] && \
						g_int[2] > 1))
			{
				g_int[2] = 0;
				free(prt);
				free_cmd_lst(cmd_ctrl->first);
				prt = readline("Minishell-4.2$> ");
				continue ;
			}
			g_int[2] = 0;
			prompt_part_two(cmd_ctrl->first, data_env, &thefinalpid, 0);
			free_cmd_lst(cmd_ctrl->first);
		}
		free(prt);
		g_int[1] = 0;
		prt = readline("Minishell-4.2$> ");
	}
}

void	init_shit(char **const envr)
{
	char		*prt;
	int			size;
	t_lst		data_env;
	t_cmd_lst	cmd_ctrl;

	size = get_env_size(envr);
	prt = NULL;
	data_env = init_env_ctrl(envr);
	init_env_lst(&data_env, envr, size);
	prompt(&cmd_ctrl, &data_env, prt, -1);
}

int	main(int ac, char **av, char **const envr)
{
	g_int[1] = 0;
	(void)av;
	if (ac == 1)
	{
		listen_signox();
		init_shit(envr);
	}
	else
		error(0, 0, 30001, 1);
	return (0);
}
