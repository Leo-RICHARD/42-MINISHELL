/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_part_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 19:24:50 by cguiot            #+#    #+#             */
/*   Updated: 2022/03/18 21:00:56 by lrichard         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	dup_des_trucs(t_command *cmd)
{
	if (!cmd->prev)
	{
		if (cmd->next && dup2(cmd->fd[1], STDOUT_FILENO) == -1)
			return (0);
	}
	else if (cmd->next)
	{
		if (dup2(cmd->prev->fd[0], STDIN_FILENO) == -1 || \
				dup2(cmd->fd[1], STDOUT_FILENO) == -1)
			return (0);
		close(cmd->prev->fd[0]);
	}
	else
	{
		if (dup2(cmd->prev->fd[0], STDIN_FILENO) == -1)
			return (0);
		close(cmd->prev->fd[0]);
	}
	if (cmd->next)
		close(cmd->fd[0]);
	if (cmd->next)
		close(cmd->fd[1]);
	return (1);
}

int	is_builtin(t_command *cmd)
{
	if (!ft_strcmp(cmd->command, "cd") || !ft_strcmp(cmd->command, "echo") || \
			!ft_strcmp(cmd->command, "env") || \
			!ft_strcmp(cmd->command, "unset") || \
			!ft_strcmp(cmd->command, "pwd") || \
			!ft_strcmp(cmd->command, "export") || \
			!strcmp(cmd->command, "exit"))
		return (-1);
	return (0);
}

int	exec_if_builtin(t_command *cmd, t_lst *envv, int ret, int fd)
{
	if (cmd->redir_out_fd)
		fd = cmd->redir_out_fd;
	if (!ft_strcmp(cmd->command, "cd"))
		ret = cd(cmd, envv, cmd->args);
	else if (!ft_strcmp(cmd->command, "echo"))
		ret = echo(fd, cmd->args);
	else if (!ft_strcmp(cmd->command, "env"))
		ret = env(envv, 0);
	else if (!ft_strcmp(cmd->command, "unset"))
		ret = unset(cmd, envv, cmd->args, 1);
	else if (!ft_strcmp(cmd->command, "pwd"))
		ret = aff_key(envv, "PWD");
	else if (!ft_strcmp(cmd->command, "export"))
		ret = expor(cmd, envv, cmd->args);
	else if (!strcmp(cmd->command, "exit"))
	{
		exiit(cmd, envv->first, cmd->args, 0);
		ret = 1;
	}
	else
		return (0);
	if (ret == 0)
		return (-1);
	return (ret);
}
