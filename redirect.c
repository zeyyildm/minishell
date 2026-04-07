#include "minishell.h"

int exec_redir(t_command *cmd)
{
    t_redir *tmp;
    int     fd;
    char    *filename;

    tmp = cmd->redirs;

    if (cmd->heredoc_fd != -1)
    {
        dup2(cmd->heredoc_fd, STDIN_FILENO);
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
    }

    while (tmp)
    {
        // Heredoc'u burada handle etme (exec_heredoc yapacak)
        if (tmp->type == T_HEREDOC)
        {
            tmp = tmp->next;
            continue;
        }

        filename = for_quotes(tmp->file); // Quote'ları temizle
        
        if(tmp->type == T_REDIR_IN)
        {
            fd = open(filename, O_RDONLY);
            if(fd < 0)
            {
                perror(filename);
                free(filename);
                return (-1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if(tmp->type == T_REDIR_OUT)
        {
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(fd < 0)
            {
                perror(filename);
                free(filename);
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if(tmp->type == T_REDIR_APPEND)
        {
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(fd < 0)
            {
                perror(filename);
                free(filename);
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        free(filename);
        tmp = tmp->next;
    }
    return (0);
}
