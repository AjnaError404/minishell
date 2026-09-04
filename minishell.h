/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 12:23:29 by ykandous          #+#    #+#             */
/*   Updated: 2026/08/30 19:08:42 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// clang-format off
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <fcntl.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
// clang-format on
# define TYPE_ERROR -2

extern int			g_exit_status;

enum				e_bool
{
	FALSE,
	TRUE
};

enum				e_fsm_state
{
	STATE_OUT_QUOTE,
	STATE_SGL_QUOTE,
	STATE_DBL_QUOTE
};

enum				e_type
{
	TYPE_WORD,
	TYPE_QUOTE,
	TYPE_DBLQUOTE,
	TYPE_REDIR_IN,
	TYPE_REDIR_OUT,
	TYPE_HEREDOC,
	TYPE_APPEND,
	TYPE_PIPE,
	TYPE_EOF,
	TYPE_UNDEFINE
};

typedef struct s_tab
{
	int				*tab;
	int				size;
}					t_tab;

typedef struct s_charcmd
{
	int				type;
	char			value;
}					t_charcmd;

typedef struct s_strcmd
{
	int				type;
	char			*value;
}					t_strcmd;

typedef struct s_lexeme
{
	int				type;
	char			*value;
	struct s_lexeme	*next;
	struct s_lexeme	*prev;
}					t_lexeme;

typedef struct s_rumba
{
	void			*wilfreed;
	int				type;
	struct s_rumba	*next;
	struct s_rumba	*prev;
}					t_rumba;

typedef struct s_tree
{
	char			*cmd_name;
	char			**args;
	int				type;
	int				fd_in;
	int				fd_out;
	struct s_tree	*r_child;
	struct s_tree	*l_child;
}					t_tree;

// init.c
t_charcmd			*init_charcmd(char value, t_rumba **rumba_mk1);
t_lexeme			*init_lexeme(char *value, t_rumba **rumba_mk1);
t_tree				*init_tree(char *value, int type, t_rumba **rumba_mk1);
t_tab				*init_tab(int size, t_rumba **rumba_mk1);

// rumba_mk1.c
t_rumba				*init_rumba_mk1(void *wilfreed);
t_rumba				*add_rumba(t_rumba **rumba_mk1, void *wilfreed);
int					cmpr_data(void *data_one, void *data_two, int n);
int					extract_mid_or_end_node(t_rumba **rumba_mk1);
void				*malloc_rumba(size_t size, t_rumba **rumba_mk1);

// rumba_del.c
int					del_all_rumba(t_rumba **rumba_mk1);
int					del_one_rumba(t_rumba **rumba_mk1, void *wilfreed);

// is_something.c
int					ft_char_is_something(char c);
int					ft_str_is_something(char *c);
void				print_type(int type);
int					is_cmd_opprt(int type);

// ft_split.c
char				**ft_split(char *s, char **oprts);

// fs_automaton.c
char				**fls_split(char *str, char **set, int add_set,
						t_rumba **rumba_mk1);

// fs_automaton_utils.c
char				*is_charset(char *c, char **set);
int					update_fsa_state(char c, int state);
int					update_state_n_count(char c, int state, ssize_t *count);
void				fls_countword_initvat(size_t *i, ssize_t *count,
						size_t *word, int *state);
int					increment_count_or_not(char c, int state);

// ft_utils.c
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strjoin_gp(char *s1, char *s2, t_rumba **rumba_mk1);
char				*graft_str_gp(char *dest, t_tab *index_tab, char *grafted,
						t_rumba **rumba_mk1);
char				*ft_substr_gp(char *s, unsigned int start, size_t len,
						t_rumba **rumba_mk1);

// ft_itoa.c
char				*ft_itoa_gp(int n, t_rumba **rumba_mk1);

// lexer.c
t_lexeme			*create_lexer(char *args, t_rumba **rumba_mk1);
char				**fill_str_from_lex(t_lexeme *lexer, t_rumba **rumba_mk1);

// tree.c
t_tree				*lex_to_tree(t_lexeme *lexer, t_rumba **rumba_mk1);
t_tree				*create_tree(t_lexeme *lexer, char **envp,
						t_rumba **rumba_mk1);
int					is_cmd_opprt(int type);

// char_type.c
int					ft_isall_word(char *str);
int					ft_isdigit(char c);
int					ft_isalpha(char c);
int					ft_isvarname_char(char *str);

// ft_printf
int					ft_isallnum(char *params);
int					ft_putstr_fd(char *str, int fd);
int					ft_putchar_fd(int c, int fd);
void				ft_put_pos_base_fd(unsigned long nb, char *base, int *cmp,
						int fd);
int					ft_putbase_fd(int nb, char *base, int fd);
int					ft_put_ubase_fd(unsigned int nb, char *base, int fd);
int					ft_put_ptr_fd(void *ptr, int fd);
void				ft_switch_case_fd(char convert, va_list params, int *cmp,
						int fd);
int					ft_printf_fd(int fd, const char *format, ...);
int					ft_printf(const char *format, ...);

// heredoc
int					heredoc_magic(char *delimiter, t_rumba **rumba_mk1);

// redir.c
int					is_cmd_redir(int type);
int					switch_open(char *pathname, int redir_type);
char				**cmd_to_args(char *cmd, t_rumba **rumba_mk1);
int					shift_2left_str_tab(char **strs, int nshift);

// fill_args_fd.c
int					tree_update_args_n_fd(t_tree **tree, t_rumba **rumba_mk1);
int					switch_redir(char *name, char *symbol, t_rumba **rumba_mk1);
int					treenode_add_fd(t_tree **tree_node, t_rumba **rumba_mk1,
						int i);
int					treenode_add_args_n_fd(t_tree **tree_node,
						t_rumba **rumba_mk1);

// expander_utils.c
char				*find_var(char *arg, int *i);
char				*pick_var_name(char *str, t_rumba **rumba_mk1);
char				*find_envvar(char **envp, char *var_name);

// expander.c
int					tree_expand_all(t_tree **tree, char **envp,
						t_rumba **rumba_mk1);

// remove_quote.c
int					remove_quote(t_tree **tree, t_rumba **rumba_mk1);

// builtins1.c

int					pwd(void);
int					cd(t_tree *cmd);
int					echo(t_tree *cmd);
int					env(char **envp);
int					is_numeric(char *str);

// builtins2.c
int					ft_exit(t_tree *cmd);
int					ft_unset(char **env, t_tree *cmd);
int					update_env_var(char **env, char *arg, int len);
char				**add_env_var(char **env, char *arg);
char				**ft_export(char **env, t_tree *cmd);

// exec_core.c

void				execute_node(t_tree *node, char ***envp,
						t_rumba **rumba_mk1);
void				execute_command(t_tree *cmd, char ***envp,
						t_rumba **rumba_mk1);
void				process_line(char *line, char ***my_env,
						t_rumba **rumba_mk1);
void				execute_simple_command(t_tree **cmd, char **envp,
						t_rumba **rumba_mk1);

// exec_redir.c
void				setup_redir(t_tree *tree);
void				exec_left_child(t_tree *node, char **envp, int pipefd[2],
						t_rumba **rumba_mk1);
void				exec_right_child(t_tree *node, char **envp, int pipefd[2],
						t_rumba **rumba_mk1);
void				execute_pipe(t_tree *node, char **envp,
						t_rumba **rumba_mk1);

// signals.c

void				handle_sigint(int sig);
void				init_signals(void);

// utils.c
int					count_variables(char **envp);
int					ft_strlen(const char *str);
int					ft_atoi(const char *str);
char				*ft_strdup(const char *s1);
char				**copy_env(char **envp);

// test

void				test_simple_cmd(char **envp);
void				test_pipe_cmd(char **envp);

#endif
