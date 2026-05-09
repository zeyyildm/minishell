/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:16:09 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/09 16:16:10 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*t;

	t = malloc(sizeof(*t));
	if (!t)
		return (NULL);
	t->type = type;
	t->value = value;
	t->next = NULL;
	return (t);
}

void	t_add_back(t_token **list, t_token *new)
{
	t_token	*tmp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	skip_spaces(char *s, int *i)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
}

int	quote_size(char *str, char c)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] == c)
			break ;
		i++;
	}
	return (i);
}
