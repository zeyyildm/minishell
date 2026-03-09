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
void init_env(t_shell *shell)
{
    shell->env = NULL;
    get_env(shell);
}
void free_lists(t_shell *shell)
{
    
}

int main(int ac, char **av, char **envp)
{
    char *line;
    t_shell shell;
    shell.envp = envp;
    shell.env = NULL;
    init_env(&shell);
    shell.tokens = NULL;
    shell.commands = NULL;
    t_command *cmdHead;
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
        shell.tokens = tokenizer(line);
        if(shell.tokens->type == TPIPE)
            exit(1); 
        cmdHead = parser(shell.tokens, shell.commands);
        shell.commands = cmdHead;
        if (cmdHead)
            expanded(&shell);
        if (cmdHead && cmdHead->next)
            execute_pipe(&shell, cmdHead, -1);
        else if (cmdHead)
        {
            // Parent builtin ise (cd, export, unset, exit)
            if (is_parent_builtin(cmdHead->argv[0]))
            {
                int saved_stdin = dup(STDIN_FILENO);
                int saved_stdout = dup(STDOUT_FILENO);
                
                if (exec_redir(cmdHead) == 0)
                    init_builtin_ex(&shell, cmdHead);
                
                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
            }
            else
            {
                // Diğer komutlar fork ile çalışır (echo, pwd, env, external)
                int ret = init_builtin_ex(&shell, cmdHead);
                if (ret == -1)
                    execute_basic(&shell, cmdHead);
            }
        }
        free_lists();
        free(line);
    }
    return (0);
}
