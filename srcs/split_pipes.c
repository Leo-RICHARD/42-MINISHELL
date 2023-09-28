/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cut_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrichard <lrichard@student.42lyon.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 21:51:49 by lrichard          #+#    #+#             */
/*   Updated: 2022/03/09 16:02:03 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	pipe_get_index(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '|')
	{
		if (str[i] == '\'' && ++i)
			while (str[i] && str[i] != '\'')
				i++;
		if (str[i] == '"' && ++i)
			while (str[i] && str[i] != '"')
				i++;
		if (str[i])
			i++;
	}
	return (i);
}

int	get_command_chunk(t_command *cmd_lst, char *line, int *i)
{
	int	len;

	len = pipe_get_index(line);
	if (!nmalloc((void **)&cmd_lst->command, len + 1))
		return (0);
	*i += len;
	while (len--)
		cmd_lst->command[len] = line[len];
	return (1);
}

int	split_pipes(t_cmd_lst *cmd_ctrl, t_command *cmd_lst, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!cmd_lst)
			return (0);
		if (!get_command_chunk(cmd_lst, line + i, &i))
			return (0);
		cmd_ctrl->size++;
		if (line[i])
		{
			cmd_lst->next = create_new_chunk();
			cmd_lst->next->prev = cmd_lst;
			cmd_lst = cmd_lst->next;
			i++;
		}
	}
	return (1);
}
