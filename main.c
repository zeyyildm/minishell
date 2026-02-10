/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:31:21 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/10 20:14:13 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

//diyelim ki girdi ls

static int is_only_spaces(const char *s) //girilen satır boş mu veya sadece space mi
{
    int i = 0;

    if(!s)
        return 1;
    while(s[i])
    {
        if(s[i] != ' ' && s[i] != '\t')
            return 0;
        i++;
    }

    return 1;
}
/*
static int is_same(const char *a, const char *b) //iki string birebir aynı mı?
{
    int i = 0;

    if (!a || !b)
        return 0;
    while (a[i] && b[i] && a[i] == b[i])
        i++;
    return (a[i] == '\0' && b[i] == '\0');
}
*/
char *read_lines()
{
	char	*line;

	line = readline("minishell$ ");
	if (!line)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	if (*line)
		add_history(line);
	return (line);
}

int main(int ac, char **av, char **envp)
{
    char *line;
    t_token *t;
    t_command cmd;

    (void)ac;
    (void)av;
    (void)envp;

    while(1)
    {
        line = read_lines(); // =ls. readline bellek kullanır o yüzden free edilmeli
        if(is_only_spaces(line))
        {
            free(line);
            continue;
        }
        t = tokenizer(line);
        // while(t)
        // {
        //     printf(" %d %s: \n", t->type, t->value);
        //     t = t->next;
        // }
        t_command *cmd1 = parser(t,&cmd);
         int i;
        i= 0;
        while(cmd1)
        {
            while (cmd1->argv[i])
            {
                printf(" %s: \n", cmd1->argv[i]);
                i++;
                /* code */
            }
            
            cmd1 = cmd1->next;
        }
        free(line);
    }
    return (0);
}

