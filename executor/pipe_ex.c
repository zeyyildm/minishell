#include "../minishell.h"
#include <errno.h>


void exec_external_no_fork(t_shell *shell, t_command *cmd)
{
    char *full_path;

    full_path = find_ex_path(shell, cmd->argv[0]);
    if (!full_path)
    {
        printf("command not found\n");
        free_lists(shell);
        free_env(shell->env);
        exit(127);
    }

    if (exec_redir(cmd))
    {
        free(full_path);
        free_lists(shell);
        free_env(shell->env);
        exit(1);
    }

    execve(full_path, cmd->argv, shell->envp);
    int err = errno;
    struct stat path_stat;

    stat(cmd->argv[0], &path_stat);
    if (err == EACCES && S_ISDIR(path_stat.st_mode))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": Is a directory\n", 2);
        free(full_path);
        free_lists(shell);
        free_env(shell->env);
        exit(126);
    }
    else
    {
        errno = err; // Restore errno for perror
        perror(cmd->argv[0]);
    }
    free(full_path);
    free_lists(shell);
    free_env(shell->env);
    if (err == ENOENT)
        exit(127);
    if (err == EACCES)
        exit(126);
    exit(127);
}
// ls | wc 
void execute_pipe(t_shell *shell, t_command *cmd, int prev_fd) //bu if else olarak iki fonska bölünecek
{
    int		fd[2]; //okuma ve yazma ucu
    pid_t	pid;
	int		status;
    int     ret;

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
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
			if (prev_fd != -1) //bu tek komutlu değil bir çoklu pipe ın son komutu demek
            {
                dup2(prev_fd, STDIN_FILENO); //burda wcnin inputunu değiştiriyoruz
                close(prev_fd);
            }            // Eğer redirect fail ederse, çalışma duracak ama exit code 1            
            if (exec_redir(cmd) != 0)
            {
                free_lists(shell);
                free_env(shell->env);
                exit(1);
            }
            ret = init_builtin_ex(shell, cmd);
            if (ret == -1)
                exec_external_no_fork(shell, cmd);
            free_lists(shell);
            free_env(shell->env);
            exit(ret);
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
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
			if (prev_fd != -1) //bu tek komutlu değil bir çoklu pipe ın son komutu demek
            {
                dup2(prev_fd, STDIN_FILENO); //burda wcnin inputunu değiştiriyoruz
                close(prev_fd);
            }
            close(fd[0]); // Read end kapatılmalı (sadece sol yazacak)
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            // Eğer redirect fail ederse, çalışma duracak ama exit code 1
            if (exec_redir(cmd) != 0)
            {
                free_lists(shell);
                free_env(shell->env);
                exit(1);
            }
            ret = init_builtin_ex(shell, cmd);
            if (ret == -1)
                exec_external_no_fork(shell, cmd);
            free_lists(shell);
            free_env(shell->env);
            exit(ret);
		}

		if (prev_fd != -1)
            close(prev_fd);
        close(fd[1]);
        execute_pipe(shell, cmd->next, fd[0]);
        close(fd[0]);
        while(waitpid(pid, &status, 0) == -1)
        {
            if(errno != EINTR)
                break;
        }
        // Don't override exit status - it's already set by the last command in the pipe
	}

}

