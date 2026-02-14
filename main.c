/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:31:21 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/14 20:07:56 by hakalkan         ###   ########.fr       */
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
int line_quote_size(char *str , char c)
{
    int i;
    
    i = 0;
    while (str[i])
    {
        if (str[i] == c)
        {
            return i;
        }
        i++;
    }
    return 0;
}

int line_flag(char *str , char c)
{
    int i;
    
    i = 0;
    while (str[i])
    {
        if (str[i] == c)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

int line_check_quote(char *str)
{
    int i;
    char quote;

    i = 0;
    quote = 0;
    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && quote == 0)
            quote = str[i];
        else if (str[i] == quote)
            quote = 0;
        i++;
    }
    if (quote != 0)
        return 1; // hata var
    return 0;
}

int main(int ac, char **av, char **envp)
{
    char *line;
    t_token *t;
    t_command *cmd;
    t_command *cmdHead;
    cmd = NULL;
    cmdHead = NULL;

    (void)ac;
    (void)av;
    (void)envp;

    while(1)
    {
        line = read_lines(); // =ls. readline bellek kullanır o yüzden free edilmeli
        if(line_check_quote(line))
            exit(1);
        if(is_only_spaces(line))
        {
            free(line);
            continue;
        }
        t = tokenizer(line);
        cmdHead = parser(t,cmd);
        // int i;
        // while(cmdHead)
        // {
        //     i= 0;
        //     while (cmdHead->argv && cmdHead->argv[i])
        //     {
        //         printf(" %s: \n", cmdHead->argv[i]);
        //         i++;
        //     }
        //     printf("diger komut: \n");
        //     cmdHead = cmdHead->next;
        // }
        print_tokens(t);
        print_commands(cmdHead);

        free(line);
    }
    return (0);
}

