/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:43:08 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/16 13:47:18 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//heredoc mini bir pipe gibi davraniyor
//shell heredoc icin gecici bir pipe acar
// fd[1] ucu = heredoc yazma ucu
// fd[0] ucu = komut okuma ucu

//diyelim ki cat << EOF

void exec_heredoc(t_shell *shell, t_command *cmd)
{
    t_command *tmp;

    tmp = cmd;
    while (tmp)
    {
        if(tmp->redirs && tmp->redirs->type == T_HEREDOC) //redir varsa ve bu redir de heredocsa
        {
            int     fd[2];
            pid_t   pid;
            if(pipe(fd) == -1)
            {
                perror("heredoc hatasiii");
                return;
            }



            
            //bu arada write ile heredoc satirlarini yazdirmamiz lazim






            pid = fork();
            if(pid < 0)
            {
                perror("heredoc child hatasii"); 
                return;
            }
            if(pid == 0)
            {
                dup2(fd[0], STDIN_FILENO); //girisi heredoc pipeindan
                close(fd[0]);
                //tmp şu an zaten cati tutuyor cati çalıştırmamiz lazim
                if(init_builtin_ex(shell, tmp) == -1)
                    execute_basic(shell, tmp);
                exit(1);
            }
    
            close(fd[0]); //parent icin de fd kapanmali
            waitpid(pid, NULL, 0);

        }
        tmp = tmp->next;
    }
    
}