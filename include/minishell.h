/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cguiot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 17:02:50 by cguiot            #+#    #+#             */
/*   Updated: 2022/03/17 20:04:11 by cguiot           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define SYNTAX_ERROR 30000

extern int	g_int[4];

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;

}				t_env;

typedef struct s_command
{
	char				*command;
	char				**args;
	int					redir_in_type;
	char				*redir_in_path;
	int					redir_in_fd;
	int					redir_out_type;
	char				*redir_out_path;
	int					redir_out_fd;
	int					fd[2];
	struct s_command	*next;
	struct s_command	*prev;
}				t_command;

typedef struct s_cmd_lst
{
	t_command	*first;
	t_command	*last;
	int			size;
}			t_cmd_lst;

typedef struct s_lst
{
	t_env	*first;
	t_env	*last;
	int		size;
}				t_lst;

//env
int						get_env_size(char **envr);
t_lst					init_env_ctrl(char **envr);
void					init_env_lst(t_lst *lst, char **envr, int size);
t_env					*create_env_elem(char *path);
int						aff_key(t_lst *data, char *str);
char					*get_value(t_lst *data, char *key);
t_env					*get_key(t_lst *data, char *key);

//main	
int						main(int ac, char **av, char **const	envr);
char					**rebuild_envp(t_lst *env);
//utils
void					ft_putstr_fd(int fd, char *str);
void					ft_putstr(char *str);
int						ft_strlen(char *str);
int						ft_strcmp(char *s1, char *s2);
char					*ft_strjoin(char *s1, char *s2, int frees1, int frees2);
char					*ft_strdup(char *s, int frees1);
int						trim_spaces(char **str);
char					*ft_strncpy(char *dest, char *src, int len);
char					**ft_split(char *str, char c);
int						ft_strchr(char *str, char c);
unsigned long long int	ft_atoi_custom(char *str);
char					*ft_itoa(int n);
void					unlinkk(void);

// norm_alloc_utils
void					*nmalloc(void **var, int size);
void					*insalloc(void **ptr, void *mem_to_add, int starti, \
		int endi);
void					*dealloc(void **ptr, int starti, int endi);
void					*nmalloc_2d(char ***var, int sizey);

//initlst
char					*key(char *str);
char					*value(char *str);
void					init_env_lst(t_lst *lst, char **envr, int size);

//builtinS
//

int						env(t_lst *data, int ntm);
int						echo(int fd, char **args);
int						unset(t_command *cmd, t_lst *data, char **key, \
		int test);
int						expor(t_command *cmd, t_lst *data, char **path);
int						cd(t_command *cmds, t_lst *data, char **args);
void					exiit(t_command *cmd_lst, t_env *env, char **args, \
		unsigned long long int ret);
int						exec_if_builtin(t_command *cmd, t_lst *envv, int ret, \
		int fd);

//parsing
t_command				*create_new_chunk(void);
int						parse_command(t_cmd_lst *cmd_ctrl, t_lst *env, \
		char *line);
int						parse_redirs(t_command *cmd_lst, t_lst *env);
int						split_pipes(t_cmd_lst *cmd_ctrl, t_command *cmd_lst, \
		char *line);
int						split_args(t_command *cmd_lst, int i, int y);
int						expand_dollars(t_lst *env, t_command *cmd_lst);
int						dollar_ptlc(t_lst *env, char **str, int i);

//exec
int						execute(t_command *cmd_lst, char **path, t_lst *envp, \
		int *thefinalpid);

int						get_arg(t_command *cmd_lst, int *i, int *y);

//error
int						error(t_command *cmd_lst, t_env *env, int errnum, \
		int exit);

//free

void					free_cmd_lst(t_command *cmd);
void					free_env_lst(t_env *env);

// siignox
int						listen_signox(void);

// heredoc.c

void					skip_quote_easy(t_command *cmd, int *i);
int						get_path_len(char *line);
int						heredoc(t_command *cmd, char *line);
int						dup_des_trucs(t_command *cmd);
int						is_builtin(t_command *cmd);
int						exec_if_builtin(t_command *cmd, t_lst *envv, int ret, \
		int fd);
#endif
