/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:31:14 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/10 16:31:15 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
static void skip_spaces(char *s, int *i)
{
    while (s[*i] == ' ' || s[*i] == '\t')
        (*i)++;
}

static void parse_redir(t_token **head, char *s, int *i)
{
    if (s[*i] == '<' && s[*i + 1] == '<')
    {
        token_add_back(head, new_token(T_HEREDOC, NULL));
        (*i)++;
    }
    else if (s[*i] == '>')
    {
        if (s[*i + 1] == '>')
        {
            token_add_back(head, new_token(T_REDIR_APPEND, NULL));
            (*i)++;;
        }
        else
        {
            token_add_back(head, new_token(T_REDIR_OUT, NULL));
        }
    }
    else
    {
        token_add_back(head, new_token(T_REDIR_IN, NULL)); 
    }
}


t_token *tokenizer(char *s)
{
    t_token *head = NULL;
    int i;

    i = 0;
    while(s[i])
    {
        skip_spaces(s, &i);
        if (!s[i])
            break;
        if(s[i] == '|')
            token_add_back(&head, new_token(TPIPE, NULL));
        else if(s[i] == '<' || s[i] == '>')
            parse_redir(&head, s, &i);
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
