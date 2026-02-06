#include "minishell.h"

static t_token *new_token(t_token_type type, char *value)
{
    t_token *t = malloc(sizeof(*t));
    if (!t) 
        return NULL;
    t->type = type;
    t->value = value;
    t->next = NULL;
    return t;
}

static void	token_add_back(t_token **list, t_token *new)
{
	t_token	*tmp;

	if (!*list)
	{
		*list = new;
		return;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_token *tokenizer(char *s)
{
    t_token *head = NULL;
    int i;

    i = 0;
    while(s[i])
    {
        while(s[i] == ' ' || s[i] == '\t')
            i++;
        if(s[i] == '|')
            token_add_back(&head, new_token(TPIPE, NULL));
        else if(s[i] == '<')
        {
            if(s[i + 1] == '<')
            {
                token_add_back(&head, new_token(T_HEREDOC, NULL));
                i++;
            }
            else
                token_add_back(&head, new_token(T_REDIR_IN, NULL));
                
        }
        else if(s[i] == '>')
        {
            if(s[i + 1] == '>')
            {
                token_add_back(&head, new_token(T_REDIR_APPEND, NULL));
                i++;
            }
            else
                token_add_back(&head, new_token(T_REDIR_OUT, NULL));
        }
        else
        {
            int start = i;
            while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '|' && s[i] != '<' && s[i] != '>')
                i++;
            token_add_back(&head, new_token(TWORD, ft_substr(s, start, i - start)));
        }
    
        i++;
    }
    return head;
}
