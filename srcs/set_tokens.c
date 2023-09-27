

#include "../includes/minishell.h"

int define_token(const char *token)
{
	if (ft_strncmp(token, "|", ft_strlen(token)) == 0)
		return (PIPELINE);
	if (ft_strncmp(token, ">", ft_strlen(token)) == 0)
		return (REDIR_OUT);
	if (ft_strncmp(token, ">>", ft_strlen(token)) == 0)
		return (D_REDIR_OUT);
	if (ft_strncmp(token, "<", ft_strlen(token)) == 0)
		return (REDIR_IN);
	if (ft_strncmp(token, "<<", ft_strlen(token)) == 0)
		return (HEREDOC);
	if (token[0] == '$')
		return (ENV);
	return WORD;
}

void	addtoken(t_token **tokens, char *data, int type, int quote)
{
	t_token	*new;
	t_token	*last;

	new = malloc(sizeof(t_token));
	new->data = data;
	new->type = type;
	new->quote = quote;
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


t_token *set_tokens(char *input)
{
	t_token *tokens = NULL;
	int		i;
	int 	start;
	int 	quote;
	char	*data;
	
	i = -1;
	data = NULL;
	while (input[++i] != '\0')
	{
		start = i;
		if (input[i] == '"')
		{
			i++;
			quote = DOUBLE;
			while (input[i] != '\0' && input[i] != '"')
				i++;
			data = ft_substr(input, start, i - start + 1);
				// addtoken(&tokens, ft_substr(input, start + 1, i - start - 1), define_token(data), quote);
				i++; // Skip the closing quote
		}
		else if (input[i] == '\'')
		{
			i++;
			quote = SINGLE;
			while (input[i] != '\0' && input[i] != '\'')
				i++;
			data = ft_substr(input, start, i - start + 1);
				i++; // Skip the closing quote
		}
		else if (input[i] == '>')
		{
			quote = NONE;
			if (input[i + 1] == '>')
			{
				data = ft_substr(input, start, 2);
				i += 2;
			}
			else
			{
				data = ft_substr(input, start, 1);
				i+= 1;
			}
		}
		else if (input[i] == '<')
		{
			quote = NONE;
			if (input[i + 1] == '<')
			{
				data = ft_substr(input, start, 2);
				i += 2;
			}
			else
			{
				data = ft_substr(input, start, 1);
				i+= 1;
			}
		}
		else if (input[i] == '|')
		{
			quote = NONE;
			data = ft_substr(input, start, 1);
			i+= 1;
		}
		else if (input[i] == '$')
		{
			quote = NONE;
			i++;
			while (input[i] != '\0' && input[i] != ' ' && input[i] != '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
				i++;
			data = ft_substr(input, start, i - start);
		}
		else if (input[i] == ' '){
			i++;
		}
		else
		{
			quote = NONE;
			while (input[i] != '\0' && input[i] != ' ' && input[i] != '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
				i++;
			data = ft_substr(input, start, i - start);
		}
		if (data != NULL)
			printf("data: %s\n", data);
	}
	return tokens;
}
