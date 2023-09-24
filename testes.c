#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum to represent token types
enum TokenType {
	WORD,
	ENV,
	W_SPACE,
	PIPELINE,
	REDIR_OUT,
	D_REDIR_OUT,
	REDIR_IN,
	HEREDOC
};

// Struct to represent a token
typedef struct Token {
	char *data;
	int type;
	struct Token *next;
} Token;

// Function to add a new token to the linked list
void addToken(Token **head, char *data, int type)
{
	Token *newToken;

	newToken = (Token *)malloc(sizeof(Token));
	if (newToken == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}
	newToken->data = strdup(data);
	newToken->type = type;
	newToken->next = *head;
	*head = newToken;
}

// Function to print the tokens and their types
void printTokens(Token *head)
{
	const char *typeStr;
	
	Token *current = head;
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
			case W_SPACE:
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

// Function to free the memory used by the linked list
void freeTokens(Token *head)
{
	Token *current;

	current = head;
	while (current != NULL) {
		Token *temp = current;
		current = current->next;
		free(temp->data);
		free(temp);
	}
}

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
	} else {
		return WORD;
	}
}

int main(int ac, char **av)
{
	char	*input;
	Token	*tokenList;
	char	*token;
	int		tokenType;

	input = av[1];
	tokenList = NULL;

	token = strtok(input, " ");
	while (token != NULL)
	{
		tokenType = define_token(token);
		addToken(&tokenList, token, tokenType);
		token = strtok(NULL, " ");
	}
	printTokens(tokenList);
	freeTokens(tokenList);
	return 0;
}
