/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:09:01 by jhogonca          #+#    #+#             */
/*   Updated: 2024/04/20 16:28:29 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cmd_path(t_shell *shell)
{
	t_pipes		*pipes;
	int			is_builtin;
	const char	*builtin[7];

	is_builtin = -1;
	init_builtin(builtin);
	pipes = shell->pipes;
	if (!pipes->cmds || !pipes->cmds[0])
		return ;
	while (pipes)
	{
		if (!pipes->cmds || !pipes->cmds[0])
			return ;
		is_builtin = ft_is_builtin(builtin, pipes->cmds[0]);
		if (is_builtin != -1 || ft_strcmp(pipes->cmds[0], "") == 0)
			;
		else if (pipes->cmds && pipes->cmds[0])
			ft_access(&pipes->cmds[0], shell);
		pipes = pipes->next;
	}
}

static int	validate_aux(char data, int quote)
{
	if (is_special_char(data) && data != '$' && data != '(' && \
	data != ')' && quote == NONE)
	{
		ft_printf_fd(2, MS_ERR RESET STX BOLD_WHITE \
		"`%c'"RESET, data);
		g_exit_status = 2;
	}
	return (true);
}

static bool	validate_tokens(t_shell *shell)
{
	t_token	*token;

	token = shell->tokens;
	while (token->next)
	{
		if ((token->type == PIPELINE && token->next->type == PIPELINE)
			|| (token->type == PIPELINE && token->quote != NONE))
			return (print_error(MS_ERR RESET STX BOLD_WHITE" `|'"RESET, 2));
		if (token->type == REDIR_IN && token->next->type != WORD)
			return (print_error(MS_ERR RESET STX BOLD_WHITE " `<'"RESET, 2));
		if (token->type == REDIR_OUT && token->next->type != WORD)
			return (print_error(MS_ERR RESET STX BOLD_WHITE " `>'"RESET, 2));
		if (token->type == APPEND && token->next->type != WORD)
			return (print_error(MS_ERR RESET STX BOLD_WHITE " `>>'"RESET, 2));
		if (token->type == PARENTHESIS && (token->next->type != WORD && \
			token->next->type != PIPELINE))
			return (print_error(MS_ERR RESET STX BOLD_WHITE " `('"RESET, 2));
		//if (token->type == PARENTHESIS_CLOSE && (token->next->type != WORD && \
		//	token->next->type != PARENTHESIS_CLOSE))
		//	return (print_error(MS_ERR RESET STX BOLD_WHITE " `)'"RESET, 2));
		if (token->type == HEREDOC && token->next->type != WORD
			&& token->next->type != ENV)
			return (print_error(MS_ERR RESET STX BOLD_WHITE " `<<'"RESET, 2));
		token = token->next;
	}
	return (validate_aux(token->data[0], token->quote));
}

static void	print_token(t_token *token)
{
	while (token)
	{
		ft_printf_fd(2, "type: %d, data: %s, quote: %d\n", token->type, \
		token->data, token->quote);
		token = token->next;
	}
}

void	process_parenthesis(t_shell *shell)
{
	char	*input;
	char	*tmp;
	int		count;
	int		i;
	int		j;

	//printf("PROCESS PARENTHESIS\ntoken: %s\n\n", shell->input);
	i = -1;
	count = 0;
	input = shell->input;
	while (input[++i])
	{
		if (input[i] == '(')
		{
			j = i;
			count++;
		}
		else if (input[i] == ')')
		{
			count--;
			if (count == 0)
			{
				tmp = ft_substr(shell->input, j + 1, i - j - 1);
				free(shell->input);
				shell->input = tmp;
				
				break ;
			}
		}
	}

	/* if (input[0] == '(' && input[ft_strlen(input) - 1] == ')')
	{
		tmp = ft_substr(input, 1, ft_strlen(input) - 2);
		free(input);
		shell->input = tmp;
	} */
}

bool	parser(t_shell *shell, bool is_parenthesis)
{

	/*
	Se parenteses no inicio e no fim do input entao
		retirar parentesis
	senao
		armazenar o valor todo das parentesis (inclusive as parentesis) num token

	*/
		//printf("input: %s\n", shell->input);
	if (is_parenthesis)
		process_parenthesis(shell);
	if (!lexical(shell->input, &shell->tokens))
		return (false);
	if (!process_tokens(shell))
		return (false);
	if (!validate_tokens(shell))
		return (false);
	if (!create_pipeline_node(shell))
		return (false);
	if (!process_redirections(shell))
		return (false);
	get_cmd_path(shell);
	return (true);
}
