/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrichard <lrichard@student.42lyon.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 19:06:46 by lrichard          #+#    #+#             */
/*   Updated: 2022/03/18 21:04:07 by lrichard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	free_tmp(char *tmp)
{
	free(tmp);
	return (0);
}

int	exec_cmd_part_two(t_command *cmd_lst, char **path, char **envp, int i)
{
	int		joined;
	char	*tmp;

	tmp = ft_strdup(cmd_lst->command, 0);
	while (path[i])
	{
		joined = 0;
		if (!ft_strchr(tmp, '/'))
		{
			tmp = ft_strjoin(path[i], tmp, 0, 1);
			if (!tmp)
				return (0);
			joined = 1;
		}
		if (execve(tmp, cmd_lst->args, envp) == -1)
		{
			if (joined && !dealloc((void **)&tmp, 0, \
						ft_strlen(path[i])))
				return (0);
			i++;
		}
		else
			return (!free_tmp(tmp));
	}
	return (free_tmp(tmp));
}

int	exec_cmd(t_command *cmd, t_lst *env, char **path, char **envp)
{
	int	ret;

	if (!dup_des_trucs(cmd))
		return (0);
	if (g_int[1] && cmd->redir_in_fd && \
			dup2(cmd->redir_in_fd, STDIN_FILENO) == -1 && \
			close(cmd->redir_in_fd))
		return (0);
	if (g_int[1] && cmd->redir_out_fd && \
			dup2(cmd->redir_out_fd, STDOUT_FILENO) == -1 && \
			close(cmd->redir_out_fd))
		return (0);
	ret = exec_if_builtin(cmd, env, 1, 1);
	if (ret != -1 && ret != 0 && \
			!(!strcmp(cmd->command, "exit") && cmd->args[1]))
		g_int[0] = 0;
	if (ret == -1 || (ret == 0 && cmd->args[0][0] && \
				!exec_cmd_part_two(cmd, path, envp, 0)))
		return (0);
	if (g_int[1])
		exit(0);
	return (1);
}

int	exec_norm(t_command *cmd, int *pid)
{
	if (cmd->next || cmd->prev || !is_builtin(cmd))
	{
		g_int[1] = 1;
		*pid = fork();
		if (*pid == -1)
			return (0);
		else if (!*pid)
			g_int[1] = 1;
	}
	return (1);
}

int	execute(t_command *cmd, char **path, t_lst *env, int *thefinalpid)
{
	int		i;
	int		pid;
	char	**envp;

	if (cmd->next && pipe(cmd->fd) == -1)
		return (error(cmd, env->first, errno, 0));
	envp = rebuild_envp(env);
	if (!envp)
		return (error(cmd, env->first, errno, 0));
	pid = 0;
	if (!exec_norm(cmd, &pid))
		return (error(cmd, env->first, errno, 0));
	if (pid)
		*thefinalpid = pid;
	else if (!exec_cmd(cmd, env, path, envp))
		error(cmd, 0, errno, g_int[1]);
	if (cmd->next)
		close(cmd->fd[1]);
	if (cmd->prev)
		close(cmd->prev->fd[0]);
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
	return (1);
}
