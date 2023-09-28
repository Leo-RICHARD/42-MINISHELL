/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrichard <lrichard@student.42lyon.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 18:49:30 by lrichard          #+#    #+#             */
/*   Updated: 2022/03/18 20:50:01 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*get_redir_path(t_command *cmd_lst, char *line, int *i, int type)
{
	int	pathlen;

	while (line[*i] == ' ')
		(*i)++;
	pathlen = get_path_len(line + *i);
	if (type == 1)
	{
		free(cmd_lst->redir_in_path);
		if (!nmalloc((void **)&(cmd_lst->redir_in_path), pathlen + 1))
			return (0);
		ft_strncpy(cmd_lst->redir_in_path, line + *i, pathlen);
		*i += pathlen;
		return (cmd_lst->redir_in_path);
	}
	else
	{
		free(cmd_lst->redir_out_path);
		if (!nmalloc((void **)&(cmd_lst->redir_out_path), pathlen + 1))
			return (0);
		ft_strncpy(cmd_lst->redir_out_path, line + *i, pathlen);
		*i += pathlen;
		return (cmd_lst->redir_out_path);
	}
}

int	parrse_guillemets_part_two(t_command *cmd, t_lst *env, int type)
{
	if (type == 1)
	{
		if (!dollar_ptlc(env, &cmd->redir_in_path, 0))
			return (0);
		if (cmd->redir_in_fd)
			close(cmd->redir_in_fd);
		cmd->redir_in_fd = open(cmd->redir_in_path, O_RDONLY);
	}
	else
	{
		if (!dollar_ptlc(env, &cmd->redir_out_path, 0))
			return (0);
		if (cmd->redir_out_fd)
			close(cmd->redir_out_fd);
		if (cmd->redir_out_type == 1)
			cmd->redir_out_fd = open(cmd->redir_out_path, O_RDWR | O_CREAT | \
					O_TRUNC, 0666);
		else
			cmd->redir_out_fd = open(cmd->redir_out_path, O_WRONLY | O_CREAT | \
					O_APPEND, 0666);
	}
	if ((type == 1 && cmd->redir_in_fd == -1) || \
			(type == 2 && cmd->redir_out_fd == -1))
		return (0);
	return (1);
}

int	parse_guillemets_in(t_command *cmd, t_lst *env, int i, int si)
{
	int	ei;

	while (cmd->command[i])
	{
		skip_quote_easy(cmd, &i);
		if ((cmd->command)[i] == '<')
		{
			si = i;
			cmd->redir_in_type = 1 + (cmd->command[i + 1] == '<');
			i += cmd->redir_in_type;
			cmd->redir_in_path = get_redir_path(cmd, cmd->command, &i, 1);
			ei = i;
			if (!cmd->redir_in_path || !dealloc((void **)&cmd->command, si, ei))
				return (0);
			if (cmd->redir_in_type == 2 && !heredoc(cmd, 0))
				return (0);
			else if (cmd->redir_in_type == 1 && \
					!parrse_guillemets_part_two(cmd, env, 1))
				return (0);
			i = si - 1;
		}
		if (cmd->command[i] != '\'' && cmd->command[i] != '"')
			i++;
	}
	return (1);
}

int	parse_guillemets_out(t_command *cmd, t_lst *env, int i)
{
	int	si;
	int	ei;

	while (cmd->command[i])
	{
		skip_quote_easy(cmd, &i);
		if ((cmd->command)[i] == '>')
		{
			si = i;
			cmd->redir_out_type = 1 + (cmd->command[i + 1] == '<');
			i += cmd->redir_out_type;
			cmd->redir_out_path = get_redir_path(cmd, cmd->command, &i, 2);
			ei = i;
			if (!cmd->redir_out_path || \
					!dealloc((void **)&cmd->command, si, ei) || \
					!parrse_guillemets_part_two(cmd, env, 2))
				return (0);
			i = si - 1;
		}
		if (cmd->command[i] != '\'' && cmd->command[i] != '"')
			i++;
	}
	return (1);
}

int	parse_redirs(t_command *cmd_lst, t_lst *env)
{
	while (cmd_lst)
	{
		if (!parse_guillemets_in(cmd_lst, env, 0, 0) || \
				!parse_guillemets_out(cmd_lst, env, 0))
			return (0);
		cmd_lst = cmd_lst->next;
	}
	return (1);
}
