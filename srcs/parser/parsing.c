/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 16:09:01 by jhogonca          #+#    #+#             */
/*   Updated: 2024/05/05 20:23:02 by jhogonca         ###   ########.fr       */
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
		if (token->type == HEREDOC && token->next->type != WORD
			&& token->next->type != ENV)
			return (print_error(MS_ERR RESET STX BOLD_WHITE " `<<'"RESET, 2));
		token = token->next;
	}
	return (validate_aux(token->data[0], token->quote));
}
static int	count_processes(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens != NULL)
	{
		if (tokens->type == AND)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void print_processes(t_process *head) {
    // Inicialize um contador para acompanhar os processos
    int process_number = 1;
    
    // Itere pela lista de processos
    t_process *current_process = head;
    while (current_process != NULL) {
        // Imprima o número do processo
        printf("Process %d:\n", process_number);
        
        // Use a função print_tokens para imprimir os tokens do processo atual
        print_tokens(current_process->tokens);
        
        // Avance para o próximo processo na lista
        current_process = current_process->next;
        
        // Incrementa o contador de processo
        process_number++;
        
        // Adicione uma linha vazia para separar a saída dos processos
        printf("\n");
    }
}

t_process *create_process(t_shell *shell);

bool	parser(t_shell *shell, bool is_parenthesis)
{
	if (!lexical(shell->input, &shell->tokens))
		return (false);
	if (!process_tokens(shell))
		return (false);
	if (!validate_tokens(shell))
		return (false);
	shell->process = create_process(shell);
	print_processes(shell->process);
	exit(0);
	if (!create_pipeline_node(shell))
		return (false);
	if (!process_redirections(shell))
		return (false);
	get_cmd_path(shell);
	print_pipes(shell->pipes);
	exit(0);
	return (true);
}
