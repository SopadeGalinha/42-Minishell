/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhogonca <jhogonca@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 23:48:05 by jhogonca          #+#    #+#             */
/*   Updated: 2023/09/24 23:48:05 by jhogonca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int define_token(const char *token)
{
	if (strcmp(token, "|") == 0) {
		return PIPELINE;
	} else if (strcmp(token, ">") == 0) {
		return REDIR_OUT;
	} else if (strcmp(token, ">>") == 0) {
		return D_REDIR_OUT;
	} else if (strcmp(token, "<") == 0) {
		return REDIR_IN;
	} else if (strcmp(token, "<<") == 0) {
		return HEREDOC;
	} else if (token[0] == '$') {
		return ENV;
	} 
	else if (ft_isspace(token)) {
		return WHITESPACE;
	}
	else {
		return WORD;
	}
}

void	addtoken(t_token **tokens, char *data, int type)
{
	t_token	*new;
	t_token	*last;

	new = malloc(sizeof(t_token));
	new->data = data;
	new->type = type;
	new->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new;
		return ;
	}
	last = *tokens;
	while (last->next != NULL)
		last = last->next;
	last->next = new;
}

void print_tokens(t_token *head)
{
	const char *typeStr;
	
	t_token *current = head;
	while (current != NULL)
	{

		// Determine the string representation of the token type
		switch (current->type) {
			case WORD:
				typeStr = "word";
				break;
			case ENV:
				typeStr = "env";
				break;
			case WHITESPACE:
				typeStr = "w_space";
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
			default:
				typeStr = "unknown";
		}

		printf("data: %-8s - type: %s\n", current->data, typeStr);
		current = current->next;
	}
}


t_token	*set_tokens(char *input)
{
	t_token		*tokens;
	char		**split;
	int			i;

	tokens = NULL;
	split = ft_split(input, ' ');
	i = -1;
	while (split[++i])
		addtoken(&tokens, split[i], define_token(split[i]));

	print_tokens(tokens);
	ft_free_array(split);
	exit(0);
	return (tokens);
}