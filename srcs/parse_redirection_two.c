/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_two.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 19:01:12 by cguiot            #+#    #+#             */
/*   Updated: 2022/03/18 19:42:39 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	skip_quote_easy(t_command *cmd, int *i)
{
	if (cmd->command[*i] == '"')
	{
		(*i)++;
		while (cmd->command[*i] != '"')
			(*i)++;
		(*i)++;
	}
	if (cmd->command[*i] == '\'')
	{	
		(*i)++;
		while (cmd->command[*i] != '\'')
			(*i)++;
		(*i)++;
	}
}

int	get_path_len(char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != ' ' && line[i] != '<' && line [i] != '>')
	{
		if (line[i] == '\'' && ++i)
			while (line[i] != '\'')
				i++;
		if (line[i] == '"' && ++i)
			while (line[i] != '"')
				i++;
		i++;
	}
	return (i);
}

int	norm_heredoc(t_command	*cmd, char *line)
{
	write(cmd->redir_in_fd, line, ft_strlen(line));
	write(cmd->redir_in_fd, "\n", 1);
	close(cmd->redir_in_fd);
	cmd->redir_in_fd = open(".heredoc", O_RDWR | O_CREAT | O_APPEND, 0666);
	free(line);
	if (cmd->redir_in_fd == -1)
		return (0);
	return (1);
}

int	heredoc(t_command *cmd, char *line)
{
	unlink(".heredoc");
	if (cmd->redir_in_fd)
		close(cmd->redir_in_fd);
	cmd->redir_in_fd = open(".heredoc", O_RDWR | O_CREAT | O_APPEND, 0666);
	if (cmd->redir_in_fd == -1)
		return (0);
	while (1)
	{
		g_int[2] = 1;
		line = readline("> ");
		if (!line)
			dup2(g_int[2], 0);
		if (!line)
			return (1);
		if (!ft_strcmp(line, cmd->redir_in_path))
		{
			free(line);
			free(cmd->redir_in_path);
			cmd->redir_in_path = ft_strdup(".heredoc", 0);
			return ((cmd->redir_in_path != 0));
		}
		if (!norm_heredoc(cmd, line))
			return (0);
	}
	return (1);
}
