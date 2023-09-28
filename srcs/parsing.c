/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 17:02:27 by cguiot            #+#    #+#             */
/*   Updated: 2022/03/17 18:47:41 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	sqs(char *line, int *i, char quote)
{
	(*i)++;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	if (line[*i])
		(*i)++;
}

int	sss(char *line, int *i)
{
	if (line[*i] == '<' || line[*i] == '>' || line[*i] == '|')
		(*i)++;
	while (line[*i] == ' ')
		(*i)++;
	return ((line[*i] != 0));
}

int	check_quotes(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
		{
			i++;
			while (line[i] && line[i] != '\'')
				i++;
			if (!line[i])
				return (0);
		}
		else if (line[i] && line[i] == '"')
		{
			i++;
			while (line[i] && line[i] != '"')
				i++;
			if (!line[i])
				return (0);
		}
		i++;
	}
	return (1);
}

int	check_syntax(char *s)
{
	int	i;

	if (!check_quotes(s))
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			sqs(s, &i, s[i]);
		if (s[i] == '>')
			if (!sss(s, &i) || (s[i] == '>' && i && s[i - 1] == ' ') \
					|| s[i] == '<' || s[i] == '|' || \
					!sss(s, &i) || s[i] == '>' || s[i] == '|' || s[i] == '<')
				return (0);
		if (s[i] == '<')
			if (!sss(s, &i) || (s[i] == '<' && i && s[i - 1] == ' ') || \
					s[i] == '>' || s[i] == '|' || !sss(s, &i) || \
						s[i] == '<' || s[i] == '|' || s[i] == '>')
				return (0);
		if (s[i] == '|' && (s[i + 1] == '|' || !sss(s, &i)))
			return (0);
		i++;
	}
	return (1);
}

int	parse_command(t_cmd_lst *cmd_ctrl, t_lst *env, char *line)
{
	t_command	*cmd_lst;

	(void)env;
	cmd_lst = create_new_chunk();
	cmd_ctrl->size = 1;
	cmd_ctrl->first = cmd_lst;
	if (!cmd_lst)
		return ((error(cmd_lst, env->first, errno, 1)));
	if (!check_syntax(line))
		return (error(cmd_lst, env->first, SYNTAX_ERROR, 0));
	if (!split_pipes(cmd_ctrl, cmd_lst, line) || \
			!parse_redirs(cmd_lst, env) || \
			!split_args(cmd_ctrl->first, 0, 0))
		return (error(cmd_lst, env->first, errno, 1));
	if (!expand_dollars(env, cmd_ctrl->first))
		return (error(cmd_lst, env->first, errno, 1));
	while (cmd_lst)
	{
		cmd_lst->args[0] = cmd_lst->command;
		cmd_lst = cmd_lst->next;
	}
	return (1);
}
