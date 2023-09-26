#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenType {
    WORD,
    ENV,
    PIPELINE,
    REDIR_OUT,
    D_REDIR_OUT,
    REDIR_IN,
    HEREDOC,
    WHITESPACE
};

enum QuoteType {
    NONE,
    SINGLE,
    DOUBLE
};

typedef struct s_token {
    char *data;
    int type;
    int quoted;
    struct s_token *next;
} t_token;

int define_token(const char *token) {
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
    return WORD;
}

void addtoken(t_token **tokens, const char *data, int type, int quoted) {
    t_token *new = malloc(sizeof(t_token));
    new->data = strdup(data); // Allocate memory and copy the data
    new->type = type;
    new->quoted = quoted;
    new->next = NULL;
    
    if (*tokens == NULL) {
        *tokens = new;
    } else {
        t_token *last = *tokens;
        while (last->next != NULL)
            last = last->next;
        last->next = new;
    }
}

void print_tokens(t_token *head) {
    const char *typeStr;
    t_token *current = head;
    
    while (current != NULL) {
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
        printf("data: %-8s - type: %s - quoted: %s\n", current->data, typeStr, current->quoted ? "true" : "false");
        current = current->next;
    }
}

t_token *set_tokens(const char *input) {
    t_token *tokens = NULL;
    int i = 0;
    
    while (input[i] != '\0') {
        // Skip leading whitespace
        while (input[i] == ' ') {
            i++;
        }

        int start = i;
        int quoted = NONE; // Track the current quote state

        // Handle quotes (single and double)
        if (input[i] == '\'' || input[i] == '"') {
            quoted = (input[i] == '\'') ? SINGLE : DOUBLE;
            i++; // Skip the opening quote
            start = i;
            while (input[i] != '\0' && input[i] != input[start]) {
                i++;
            }
            if (input[i] == input[start]) {
                i++; // Skip the closing quote
            } else {
                // Handle unterminated quote error
                break;
            }
        }

        // Tokenize operators
        if (input[i] == '|' || input[i] == '>' || input[i] == '<') {
            char operator[3] = {input[i], '\0', '\0'};
            if (input[i + 1] == '>' || input[i + 1] == '<') {
                operator[1] = input[i + 1];
                i++;
            }
            addtoken(&tokens, operator, define_token(operator), quoted);
            i++;
        }
        // Tokenize environment variables
        else if (input[i] == '$') {
            i++;
            while (isalnum(input[i]) || input[i] == '_') {
                i++;
            }
            char var[256]; // Adjust the size as needed
            strncpy(var, input + start, i - start);
            var[i - start] = '\0';
            addtoken(&tokens, var, define_token(var), quoted);
        }
        // Tokenize words
        else {
            while (input[i] != '\0' && !(quoted == NONE && input[i] == ' ') && input[i] != '|' && input[i] != '>' && input[i] != '<') {
                i++;
            }
            char word[256]; // Adjust the size as needed
            strncpy(word, input + start, i - start);
            word[i - start] = '\0';
            addtoken(&tokens, word, define_token(word), quoted);
        }
    }

    print_tokens(tokens);
    return tokens;
}

int main() {
    const char *input = "ls | grep 'file name' > output.txt";
    t_token *tokens = set_tokens(input);

    // Free memory for tokens
    t_token *current = tokens;
    while (current != NULL) {
        t_token *temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }

    return 0;
}
