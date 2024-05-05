#include "../../includes/minishell.h"

size_t count_processes(t_token *tokens)
{
	size_t count = 0;

	while (tokens != NULL)
	{
		if (tokens->type == AND)
			count++;
		tokens = tokens->next;
	}
	return count;
}


t_process *create_process_from_tokens(t_token **current)
{
    t_process *new_process = (t_process *)malloc(sizeof(t_process));
    if (new_process == NULL) {
        return NULL;  // Retorna NULL em caso de falha na alocação
    }
    new_process->tokens = *current;
    new_process->type = 0;  // Defina o tipo de acordo com os operadores `AND` ou `OR`
    new_process->next = NULL;

    // Avança os tokens até o próximo operador `AND` ou `OR`
    while (*current != NULL && (*current)->type != AND && (*current)->type != OR) {
        *current = (*current)->next;
    }

    if (*current != NULL) {
        new_process->type = (*current)->type;
        *current = (*current)->next;
    }

    return new_process;
}

void add_process_to_list(t_process **head, t_process *new_process) 
{

	if (*head == NULL) {
		*head = new_process;
	} else {
		t_process *current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_process;
	}
}

t_process *create_process(t_shell *shell)
{
	t_process *head = NULL;
    t_process *new_process;
    t_token *current = shell->tokens;

    while (current != NULL)
	{
        new_process = create_process_from_tokens(&current);
        if (new_process == NULL) {
            // Trate erro de alocação de memória
            return NULL;
        }
        add_process_to_list(&head, new_process);
    }
    return head;
}
