/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 19:43:34 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/28 19:43:34 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	define_token(const char *token)
{
	if (ft_strncmp(token, "|", ft_strlen("|")) == 0)
		return (PIPELINE);
	if (ft_strncmp(token, ">", ft_strlen(">")) == 0)
		return (REDIR_OUT);
	if (ft_strncmp(token, "<", ft_strlen("<")) == 0)
		return (REDIR_IN);
	if (ft_strncmp(token, "||", ft_strlen("||")) == 0)
		return (OR);
	if (ft_strncmp(token, ">>", ft_strlen(">>")) == 0)
		return (D_REDIR_OUT);
	if (ft_strncmp(token, "<<", ft_strlen("<<")) == 0)
		return (HEREDOC);
	if (ft_strncmp(token, "2>", ft_strlen("2>")) == 0)
		return (REDIR_ERR);
	if (ft_strncmp(token, "$?", ft_strlen("$?")) == 0)
		return (EXIT_STATUS);	
	if (ft_strncmp(token, "export", ft_strlen("export")) == 0)
		return (CMD);
	if (ft_strncmp(token, "&&", ft_strlen("&&")) == 0)
		return (AND);
	if (ft_strncmp(token, ";", ft_strlen(";")) == 0)
		return (SEMICOLON);
	if (token[0] == '$')
		return (ENV);
	return (WORD);
}

void	init_shell(t_shell *shell, char **env)
{
	rl_initialize();
	using_history();
	ft_handle_signals();
	shell->error = NO_ERROR;
	shell->env = init_env(env);
	shell->exp = init_export(shell->env);
	shell->path_env = ft_getenv(env, "PATH");
	if (shell->path_env == NULL)
		exit(ft_printf_fd(2, "PATH not found\n"));
}

bool	get_input(t_shell *shell)
{
	free(shell->input);
	shell->input = readline(MINISHELL);
	if (shell->input == NULL)
		return (false);
	return (true);
}

void	print_tokens(t_token *head)
{
	char	*typeStr;
	char	*quoteStr;
	t_token	*current;
	

	current = head;
	while (current != NULL)
	{
		switch (current->type) {
        case WORD:
            typeStr = "word";
            break;
        case ENV:
            typeStr = "env";
            break;
        case OR:
            typeStr = "d_pipeline";
            break;
        case PIPELINE:
            typeStr = "pipeline";
			break;
        case REDIR_OUT:
            typeStr = "redir_out";
            break;
        case D_REDIR_OUT:
            typeStr = "d_redir_out";
            break;
        case REDIR_IN:
            typeStr = "redir_in";
            break;
        case HEREDOC:
            typeStr = "heredoc";
            break;
        case CMD:
            typeStr = "cmd";
            break;
        case REDIR_ERR:
            typeStr = "redir_err";
            break;
		case EXIT_STATUS:
			typeStr = "exit_status";
			break;
		case AND:
			typeStr = "and";
			break;
		case SEMICOLON:
			typeStr = "semicolon";
			break;
        default:
            typeStr = "unknown";
   		}
		switch (current->quote) {
			case SINGLE:
				quoteStr = "S_QUOTE";
				break;
			case DOUBLE:
				quoteStr = "D_QUOTE";
				break;
			default:
				quoteStr = "none";
		}
		
		printf(BOLD_ORANGE"data: "RESET"%-8s"BOLD_BLUE" type:"RESET" %-2s", current->data, typeStr);
		if (current->quote != NONE)
			printf(BOLD_RED" %-8s"RESET, quoteStr);
		else
			printf(BOLD_CYAN" NONE"RESET);
		if (current->error != NO_ERROR)
			printf(BOLD_PURPLE" - Error: %s\n"RESET, "UNCLOSED_QUOTE");
		else
			printf("\n");
		current = current->next;
	}
}
