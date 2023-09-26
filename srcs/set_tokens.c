

#include "../includes/minishell.h"

int define_token(const char *token)
{
	if (ft_strncmp(token, "|", ft_strlen(token)) == 0)
		return PIPELINE;
	if (ft_strncmp(token, ">", ft_strlen(token)) == 0)
		return REDIR_OUT;
	if (ft_strncmp(token, ">>", ft_strlen(token)) == 0)
		return D_REDIR_OUT;
	if (ft_strncmp(token, "<", ft_strlen(token)) == 0)
		return REDIR_IN;
	if (ft_strncmp(token, "<<", ft_strlen(token)) == 0)
		return HEREDOC;
	if (token[0] == '$')
		return ENV;
	return WORD;
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
	char		*data;
	int			i;
	int		start;
	bool 	error;

	i = -1;
	tokens = NULL;
	while (input[++i])
	{
		if (input[i] == '"')
		{
			start = i;
			while (input[++i] != '"')
				;
			if (input[i] != '"')
			{
				error = true;
				break ;
			}
		}
		else if (input[i] == '\'')
		{
			start = i;
			while (input[++i] != '\'')
				;
			if (input[i] != '\'')
			{
				error = true;
				break ;
			}
		}
		if (input[i] == '>')
		{
			if (input[i + 1] == '>')
				data = ft_substr(input, start + 1, i - start - 1);
			else
				data = ft_substr(input, start + 1, i - start - 1);
		}
		else if (input[i] == '<')
		{
			if (input[i + 1] == '<')
				data = ft_substr(input, start + 1, i - start - 1);
			else
				data = ft_substr(input, start + 1, i - start - 1);
		}
		else if (input[i] == '|')
			data = ft_substr(input, start + 1, i - start - 1);
		else if (input[i] == ' ')
			data = ft_substr(input, start + 1, i - start - 1);
		else if (input[i] == '$')
			data = ft_substr(input, start + 1, i - start - 1);
		else
			data = ft_substr(input, start, i - start);
	}	
	print_tokens(tokens);
	return (tokens);
}