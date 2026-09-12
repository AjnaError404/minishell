/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_something.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laaubry <laaubry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 20:14:41 by ykandous          #+#    #+#             */
/*   Updated: 2026/09/11 20:14:53 by laaubry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_str_is_something(char *str)
{
	if (!str)
		return (TYPE_ERROR);
	if (ft_isall_word(str))
		return (TYPE_WORD);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_EOF)
		return (TYPE_EOF);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_PIPE)
		return (TYPE_PIPE);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_REDIR_IN)
		return (TYPE_REDIR_IN);
	if (ft_strlen(str) == 1 && ft_char_is_something(str[0]) == TYPE_REDIR_OUT)
		return (TYPE_REDIR_OUT);
	if (ft_strlen(str) == 2 && ft_char_is_something(str[0]) == TYPE_REDIR_OUT
		&& ft_char_is_something(str[1]) == TYPE_REDIR_OUT)
		return (TYPE_APPEND);
	if (ft_strlen(str) == 2 && ft_char_is_something(str[0]) == TYPE_REDIR_IN
		&& ft_char_is_something(str[1]) == TYPE_REDIR_IN)
		return (TYPE_HEREDOC);
	return (TYPE_UNDEFINE);
}

int	ft_check_valid(char c)
{
	if (ft_char_is_something(c) == TYPE_UNDEFINE)
		return (0);
	return (1);
}

void	print_type(int type)
{
	if (type < TYPE_WORD || type > TYPE_UNDEFINE)
		return ;
	ft_printf("\"");
	if (type == TYPE_WORD)
		ft_printf("WORD");
	else if (type == TYPE_REDIR_IN)
		ft_printf("R-IN");
	else if (type == TYPE_REDIR_OUT)
		ft_printf("R-OUT");
	else if (type == TYPE_HEREDOC)
		ft_printf("HDOC");
	else if (type == TYPE_APPEND)
		ft_printf("APPD");
	else if (type == TYPE_PIPE)
		ft_printf("PIPE");
	else if (type == TYPE_UNDEFINE)
		ft_printf("UNDEF");
	ft_printf("\"");
}

int	is_cmd_opprt(int type)
{
	if (type >= TYPE_REDIR_IN && type <= TYPE_PIPE)
		return (1);
	return (0);
}
