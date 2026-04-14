/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:43:08 by hakalkan          #+#    #+#             */
/*   Updated: 2026/04/14 14:25:16 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

// Heredoc mini bir pipe gibi davranıyor
// Shell heredoc için geçici bir pipe açar
// fd[1] ucu = heredoc yazma ucu
// fd[0] ucu = komut okuma ucu
void    handle_heredoc(t_shell *shell, t_redir *redir, int *fd)
{
    char    *line;
    char    *delimiter;
    int     expand;

    delimiter = for_quotes(redir->file);

    // quoted ise expand YOK
    if (ft_strchr(redir->file, '\'') || ft_strchr(redir->file, '"'))
        expand = 0;
    else
        expand = 1;

    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            printf("warning: here-document delimited by end-of-file\n");
            break;
        }
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
            && ft_strlen(line) == ft_strlen(delimiter))
        {
            free(line);
            break;
        }
        if (expand)
        {
            char *tmp = expand_arg(shell, line);
            free(line);
            line = tmp;
        }
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }
    free(delimiter);
}
int exec_heredoc(t_shell *shell, t_command *cmd)
{
    t_redir    *redir;
    int        fd[2];
    pid_t      pid;

    redir = cmd->redirs;
    while (redir)
    {
        if (redir->type == T_HEREDOC)
        {
            if (pipe(fd) == -1)
                return (perror("pipe"), 1);

            pid = fork();
            if (pid < 0)
                return (perror("fork"), 1);

            if (pid == 0)
            {
                close(fd[0]);
                handle_heredoc(shell, redir, fd);
                close(fd[1]);
                exit(0);
            }
            else
            {
                close(fd[1]);
                waitpid(pid, NULL, 0);

                // eski heredoc varsa kapat
                if (cmd->heredoc_fd != -1)
                    close(cmd->heredoc_fd);

                //  BURASI DOĞRU
                cmd->heredoc_fd = fd[0];
            }
        }
        redir = redir->next;
    }
    return (0);
}