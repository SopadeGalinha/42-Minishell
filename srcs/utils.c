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

bool	ft_isspace_str(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] != 32 && (str[i] < 9 || str[i] > 13))
			return (false);
	return (true);
}

int	define_token(const char *token)
{
	if (ft_strncmp(token, "|", ft_strlen("|")) == 0 && ft_strlen(token) == 1)
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
	if (ft_isspace_str((char *)token))
		return (WHITESPACE);
	if (token[0] == '-' && ft_isalpha(token[1]))
		return (OPTION);
	if (token[0] == '$')
		return (ENV);
	return (WORD);
}

void	init_shell(t_shell *shell, char **env)
{
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
            typeStr = "OR";
            break;
        case PIPELINE:
            typeStr = "pipeline";
			break;
        case REDIR_OUT:
            typeStr = "redir_out";
            break;
        case D_REDIR_OUT:
            typeStr = "D_REDIREC_OUT";
            break;
        case REDIR_IN:
            typeStr = "REDIR_IN";
            break;
        case HEREDOC:
            typeStr = "HEREDOC";
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
			typeStr = "AND";
			break;
		case SEMICOLON:
			typeStr = "SEMICOLON";
			break;
		case OPTION:
			typeStr = "OPTION";
			break;
		case WHITESPACE:
			typeStr = "WHITESPACE";
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
			printf(BOLD_RED" %-8s\n\n"RESET, quoteStr);
		else
			printf(BOLD_CYAN" NONE\n"RESET);
		if (current->error != NO_ERROR)
		{
			switch (current->error)
			{
				case UNCLOSED_QUOTE:
					printf(BOLD_PURPLE" - Error: %s\n"RESET, "UNCLOSED_QUOTE");
					break;
				case BACKGROUND_NOT_SUPPORTED:
					printf(BOLD_PURPLE" - Error: %s\n"RESET, "BACKGROUND_NOT_SUPPORTED");
					break;
				case D_PIPELINE_NOT_SUPPORTED:
					printf(BOLD_PURPLE" - Error: %s\n"RESET, "D_PIPELINE_NOT_SUPPORTED");
					break;
				case SEMICOLON_NOT_SUPPORTED:
					printf(BOLD_PURPLE" - Error: %s\n"RESET, "SEMICOLON_NOT_SUPPORTED");
					break;
				default:
					printf("\n");
			}
		}
		current = current->next;
	}
}