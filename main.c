/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 16:31:21 by hakalkan          #+#    #+#             */
/*   Updated: 2026/04/16 18:06:36 by hakalkan         ###   ########.fr       */
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
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs->next;
		free(redirs->file);
		free(redirs);
		redirs = tmp;
	}
}
void	free_commands(t_command *cmd)
{
	t_command	*tmp;
	int	i;

	while (cmd)
	{
		tmp = cmd->next;

		if (cmd->argv)
		{
			i = 0;
			while (cmd->argv[i])
			{
				free(cmd->argv[i]);
				i++;
			}
			free(cmd->argv);
		}

		free_redirs(cmd->redirs);
        if (cmd->heredoc_fd != -1)
            close(cmd->heredoc_fd);
		free(cmd);
		cmd = tmp;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void	free_lists(t_shell *shell)
{
	if (!shell)
		return ;

	free_tokens(shell->tokens);
	free_commands(shell->commands);
	//free_env(shell->env);
	//free_envp(shell->envp);
}
int heredoc_search(t_command *cmd)
{
    t_redir *tmp;

    tmp = cmd->redirs;
    while (tmp)
    {
        if (tmp->type == T_HEREDOC)
        {
            return (1);
        }
        tmp = tmp->next;
    }
    return (0);
}
t_token *last_token(t_token **token)
{
    t_token *tmp;

    tmp = *token;
    while (tmp->next)
    {
        tmp = tmp->next;
    }
    
    return tmp;
}
int main(int ac, char **av, char **envp)
{
    char *line;
    t_shell shell;
    shell.envp = envp;
    shell.env = NULL;
    shell.last_exit_status = 0;
    init_env(&shell);
    shell.tokens = NULL;
    shell.commands = NULL;
    t_command *cmdHead;
    cmdHead = NULL;
    t_token *tmp;

    (void)ac;
    (void)av;
    (void)envp;

    while(1)
    {
        line = read_lines(); // =ls. readline bellek kullanır o yüzden free edilmeli
        if(line_check_quote(line))
        {
            ft_putstr_fd("minishell: unclosed quote\n", 2);
            shell.last_exit_status = 2;
            free(line);
            continue;
        }
        if(is_only_spaces(line))
        {
            free(line);
            continue;
        }
        shell.tokens = tokenizer(line);
        tmp = last_token(&shell.tokens);
        if(shell.tokens->type == TPIPE || tmp->type == TPIPE)
        {
            printf("zsh: parse error near `|'\n");
            shell.last_exit_status = 2;
            free_lists(&shell);
            free(line);
            continue;
        }
        cmdHead = parser(shell.tokens, shell.commands);
        shell.commands = cmdHead;
        if (cmdHead)
            expanded(&shell);
        if (cmdHead)
        {
            // HER ZAMAN önce heredoc
            if (heredoc_search(cmdHead))
            {
                if (exec_heredoc(&shell, cmdHead) != 0)
                    return 1;
            }
            //  sonra execution
            if (cmdHead->next)
            {
                // Parent builtin pipe'dan gelmişse error
                if (is_parent_builtin(cmdHead->argv[0]))
                {
                    ft_putstr_fd("minishell: ", 2);
                    ft_putstr_fd(cmdHead->argv[0], 2);
                    ft_putstr_fd(": command cannot be used with pipes\n", 2);
                    shell.last_exit_status = 1;
                    free_lists(&shell);
                    free(line);
                    continue;
                }
                execute_pipe(&shell, cmdHead, -1);
            }
            else
            {
                // Parent builtin ise (cd, export, unset, exit)
                if (is_parent_builtin(cmdHead->argv[0]))
                {
                    int saved_stdin = dup(STDIN_FILENO);
                    int saved_stdout = dup(STDOUT_FILENO);
                    int ret;
                    
                    if (exec_redir(cmdHead) == 0)
                    {
                        ret = init_builtin_ex(&shell, cmdHead);         
                        shell.last_exit_status = ret;
                    }
                    else
                        shell.last_exit_status = 1;
                    
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin); 
                    close(saved_stdout);
                }
                else
                {
                    int ret = init_builtin_ex(&shell, cmdHead);
                    if (ret == -1)
                        execute_basic(&shell, cmdHead);
                    else
                        shell.last_exit_status = ret;
                }
            }
        }
        free_lists(&shell);
        free(line);
    }
    return (0);
}
