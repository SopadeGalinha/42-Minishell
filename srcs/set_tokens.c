

#include "../includes/minishell.h"

int define_token(const char *token)
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
	if (ft_strncmp(token, "<<", ft_strlen(token)) == 0)
		return (HEREDOC);
	if (ft_strncmp(token, " ", ft_strlen(token)) == 0 \
		|| ft_strncmp(token, "\t", ft_strlen(token)) == 0 \
		|| ft_strncmp(token, "\n", ft_strlen(token)) == 0)
		return (WHITESPACE);
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
			case D_PIPES:
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
			default:
				typeStr = "unknown";
		}
		printf("data: %-8s - type: %s\n", current->data, typeStr);
		current = current->next;
	}
}

int redir_pipe_data(char *input, int i, int start, t_token **tokens)
{
	char *data;

	if (input[i] == input[i + 1])
	{
		data = ft_substr(input, start, 2);
		i++;
	}
	else
		data = ft_substr(input, start, 1);
	addtoken(tokens, data, define_token(data), NONE);
	return (i);
}

int	quote_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;
	int		quote;
	int		ref;
	
	ref = i - 1;
	quote = SINGLE; 
	while (input[i] != '\0' && input[i] != input[ref])
		i++;
	data = ft_substr(input, start, i - start + 1);
	if (input[ref] == '"')
		quote = DOUBLE;
	addtoken(tokens, data, define_token(data), quote);
	return (i);
}

int	env_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	while (input[i] != '\0' && input[i] != ' ' && input[i] \
		!= '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
		i++;
	data = ft_substr(input, start, i - start);
	addtoken(tokens, data, define_token(data), NONE);
}

int	general_data(char *input, int i, int start, t_token **tokens)
{
	char	*data;

	while (input[i] != '\0' && input[i] != ' ' && input[i] \
		!= '>' && input[i] != '<' && input[i] != '|' && input[i] != '$')
		i++;
	data = ft_substr(input, start, i - start);
	addtoken(tokens, data, define_token(data), NONE);
}

t_token *set_tokens(char *input)
{
	t_token *tokens;
	int		i;
	int 	start;
	int 	quote;
	char	*data;
	
	i = -1;
	data = NULL;
	tokens = NULL;
	while (input[++i])
	{
		start = i;
		if (input[i] == '"' || input[i] == '\'')
			i = quote_data(input, ++i, start, &tokens);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			i = redir_pipe_data(input, i, start, &tokens);
		else if (input[i] == '$')
			i = env_data(input, ++i, start, &tokens);
		else if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n')
			continue ;
		else
			general_data(input, i, start, &tokens);	
	}
	print_tokens(tokens);
	return tokens;
}
