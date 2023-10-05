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
	if (ft_strncmp(token, "|", ft_strlen(token)) == 0)
		return (PIPELINE);
	if (ft_strncmp(token, "||", ft_strlen(token)) == 0)
		return (D_PIPES);
	if (ft_strncmp(token, ">", ft_strlen(token)) == 0)
		return (REDIR_OUT);
	if (ft_strncmp(token, "<", ft_strlen(token)) == 0)
		return (REDIR_IN);
	if (ft_strncmp(token, ">>", ft_strlen(token)) == 0)
		return (D_REDIR_OUT);
	if (ft_strncmp(token, "2>", ft_strlen(token)) == 0)
		return (REDIR_ERR);
	if (ft_strncmp(token, "<<", ft_strlen(token)) == 0)
		return (HEREDOC);
	if (token[0] == '$')
		return (ENV);
	return (WORD);
}

void	print_tokens(t_token *head)
{
	char	*typeStr;
	t_token	*current;
	

	current = head;
	while (current != NULL)
	{
		switch (current->type) {
			case WORD:
				typeStr = "word";
			break ;
			case ENV:
				typeStr = "env";
			break ;
			case D_PIPES:
				typeStr = "d_pipeline";
			break ;
			case PIPELINE:
				typeStr = "pipeline";
			break ;
			case REDIR_OUT:
				typeStr = "redir_out";
			break ;
			case D_REDIR_OUT:
				typeStr = "d_redir_out";
			break ;
			case REDIR_IN:
				typeStr = "redir_in";
			break ;
			case HEREDOC:
				typeStr = "heredoc";
			break;
			case CMD:
				typeStr = "cmd";
			break ;
			case REDIR_ERR:
				typeStr = "redir_err";
			break ;
			default:
				typeStr = "unknown";
		}
		printf("data: %-8s - type: %s\n", current->data, typeStr);
		current = current->next;
	}
}
