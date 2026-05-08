/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:31:14 by hakalkan          #+#    #+#             */
/*   Updated: 2026/05/06 16:40:15 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	parse_redir(t_token **head, char *s, int *i)
{
	if (s[*i] == '<' && s[*i + 1] == '<')
	{
		t_add_back(head, new_token(T_HEREDOC, NULL));
		(*i)++;
	}
	else if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			t_add_back(head, new_token(T_REDIR_APPEND, NULL));
			(*i)++;
		}
		else
			t_add_back(head, new_token(T_REDIR_OUT, NULL));
	}
	else
		t_add_back(head, new_token(T_REDIR_IN, NULL));
}

static void	fd_redirs(t_token **h, char *s, int *i)
{
	int	t;

	t = *i;
	while (ft_isdigit(s[*i]))
		(*i)++;
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			t_add_back(h, new_token(T_REDIR_APPEND, ft_substr(s, t, *i - t)));
			(*i)++;
		}
		else
			t_add_back(h, new_token(T_REDIR_OUT, ft_substr(s, t, *i - t)));
	}
	else if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
		{
			t_add_back(h, new_token(T_HEREDOC, ft_substr(s, t, *i - t)));
			(*i)++;
		}
		else
			t_add_back(h, new_token(T_REDIR_IN, ft_substr(s, t, *i - t)));
	}
}

static void	word_token(t_token **head, char *s, int *i)
{
	int		start;
	char	quote_char;
	int		in_quote;

	start = *i;
	in_quote = 0;
	quote_char = 0;
	while (s[*i] && (in_quote || (s[*i] != ' ' && s[*i] != '\t'
				&& s[*i] != '|' && s[*i] != '<' && s[*i] != '>')))
	{
		if (!in_quote && (s[*i] == '"' || s[*i] == '\''))
		{
			in_quote = 1;
			quote_char = s[*i];
		}
		else if (in_quote && s[*i] == quote_char)
			in_quote = 0;
		(*i)++;
	}
	t_add_back(head, new_token(TWORD, ft_substr(s, start, *i - start)));
	(*i)--;
}

t_token	*tokenizer(char *s)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (s[i])
	{
		skip_spaces(s, &i);
		if (!s[i])
			break ;
		if (s[i] == '|')
			t_add_back(&head, new_token(TPIPE, NULL));
		else if (ft_isdigit(s[i]) && (s[i + 1] == '>' || s[i + 1] == '<'))
			fd_redirs(&head, s, &i);
		else if (s[i] == '<' || s[i] == '>')
			parse_redir(&head, s, &i);
		else
			word_token(&head, s, &i);
		i++;
	}
	return (head);
}
