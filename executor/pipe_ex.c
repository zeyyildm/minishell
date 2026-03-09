#include "../minishell.h"

// ls | wc 
void execute_pipe(t_shell *shell, t_command *cmd, int prev_fd) //bu if else olarak iki fonska bölünecek
{
    int		fd[2]; //okuma ve yazma ucu
    pid_t	pid;
	int		status;

	if(cmd->next == NULL) //pipe yoksa
	{
		pid = fork();
		if(pid < 0)
		{
			perror("fork hatasi 1");
			return ;
		}
		if(pid == 0) //bu child processtir demek
		{
			if (prev_fd != -1) //bu tek komutlu değil bir çoklu pipe ın son komutu demek
            {
                dup2(prev_fd, STDIN_FILENO); //burda wcnin inputunu değiştiriyoruz
                close(prev_fd);
            }
            if (exec_redir(cmd))
                exit(1);
            if ((init_builtin_ex(shell, cmd)) == -1)
                execute_basic(shell, cmd);
            exit(1);
		}
		if (prev_fd != -1) //bu koşul parenttaki fd kapansin die
            close(prev_fd);

        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) //normal şekilde mi bitti
            shell->last_exit_status = WEXITSTATUS(status);
        else //signal ile bitmesi
            shell->last_exit_status = 1;

        return ;
	}
	else //eğer pipe varsa
	{
		if(pipe(fd) == -1) //yeni bir pipe oluştur
		{
			perror("pipe fd hatasi");
			return ;
		}
		pid = fork();
		if(pid < 0)
		{
			perror("fork hatasi 2");
			return ;
		}
		if(pid == 0) //bu child processtir demek
		{
			if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            if ((init_builtin_ex(shell, cmd)) == -1)
                execute_basic(shell, cmd);
            exit(1); //child process bitiyo
		}

		if (prev_fd != -1)
            close(prev_fd);
        close(fd[1]);
        execute_pipe(shell, cmd->next, fd[0]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            shell->last_exit_status = WEXITSTATUS(status);
        else
            shell->last_exit_status = 1;
	}

}

