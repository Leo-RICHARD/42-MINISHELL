/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   split_args.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lrichard <lrichard@student.42lyon.f>	   +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/02/22 18:49:21 by lrichard		  #+#	#+#			 */
/*   Updated: 2022/03/09 16:05:27 by cguiot           ###   ########lyon.fr   */
/*																			*/
/* ************************************************************************** */

#include <minishell.h>

int	iter_through_word(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	while (str[i] && str[i] != ' ')
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

int	get_arg(t_command *cmd_lst, int *i, int *y)
{
	int	x;
	int	j;
	int	len;

	while (cmd_lst->command[*i] && cmd_lst->command[*i] == ' ')
		(*i)++;
	if (!cmd_lst->command[*i])
		return (1);
	len = iter_through_word(cmd_lst->command + *i);
	if (!nmalloc((void **)&(cmd_lst->args[*y]), sizeof(char) * (len + 1)))
		return (0);
	x = 0;
	j = *i;
	while (j < *i + len)
		cmd_lst->args[*y][x++] = cmd_lst->command[j++];
	cmd_lst->args[*y][x] = 0;
	if (!dealloc((void **)&(cmd_lst->command), j - len, *i + len))
		return (0);
	*i = j - len;
	trim_spaces(&(cmd_lst->args[*y]));
	(*y)++;
	return (1);
}

int	argscount(char *str)
{
	int	i;
	int	argsn;

	i = 0;
	argsn = 0;
	while (str[i])
	{
		i += iter_through_word(str + i);
		if (str[i] == ' ')
		{
			while (str[i] && str[i] == ' ')
				i++;
			argsn += (str[i] != 0);
		}
	}
	return (argsn);
}

int	split_args(t_command *cmd_lst, int i, int y)
{
	while (cmd_lst)
	{
		if (!nmalloc_2d((char ***)&(cmd_lst->args), \
					(argscount(cmd_lst->command) + 2)))
			return (0);
		if (!argscount(cmd_lst->command))
		{
			if (!trim_spaces(&(cmd_lst->command)))
				return (0);
			cmd_lst = cmd_lst->next;
			continue ;
		}
		i = 0;
		i += iter_through_word(cmd_lst->command);
		y = 1;
		while (cmd_lst->command[i])
			if (!get_arg(cmd_lst, &i, &y))
				return (0);
		if (!trim_spaces(&(cmd_lst->command)))
			return (0);
		cmd_lst = cmd_lst->next;
	}
	return (1);
}
