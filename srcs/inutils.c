/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inutils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 12:17:04 by jhogonca          #+#    #+#             */
/*   Updated: 2023/10/22 12:17:04 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	print_cmds(char	**cmds)
{
	int	i;

	i = -1;
	while (cmds[++i] != NULL)
		printf(BOLD_WHITE"%s "RESET, cmds[i]);
}

void	print_redir(t_redir *head)
{
	t_redir	*current;

	current = head;
	while (current != NULL)
	{
		if (current->type == REDIR_OUT)
			printf(BOLD_WHITE"> "RESET);
		if (current->type == D_REDIR_OUT)
			printf(BOLD_WHITE">> "RESET);
		if (current->type == REDIR_IN)
			printf(BOLD_WHITE"< "RESET);
		if (current->type == HEREDOC)
			printf(BOLD_WHITE"<< "RESET);
		printf(BOLD_WHITE"%s "RESET, current->file);
		current = current->next;
	}
}

void	print_pipes(t_pipes *head)
{
	t_pipes	*current;
	int		i;
	int		pipe;

	current = head;
	pipe = 0;
	while (current != NULL)
	{
		i = 0;
		pipe++;
		printf(BOLD_GREEN"PIPELINE[%d]\n"RESET, pipe);

		print_redir(current->redir_in);
		print_cmds(current->cmds);
		print_redir(current->redir_out);

		printf("\n\n");
		printf(BOLD_RED"redir_in:\n"RESET);
		print_redir(current->redir_in);
		printf("\n");
		printf(BOLD_RED"cmds:\n"RESET);
		print_cmds(current->cmds);
		printf("\n");
		printf(BOLD_RED"redir_out:\n"RESET);
		print_redir(current->redir_out);
		printf("\n\n");
		current = current->next;
	}
}