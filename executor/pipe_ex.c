#include "../minishell.h"

void execute_pipe(t_shell *shell, t_command *cmd)
{
    int		fd[2]; //okuma ve yazma ucu
    pid_t	pid1;
    pid_t	pid2;
	int		status;

	if(pipe(fd) == -1)
	{
		perror("pipe fd hatasi");
		return ;
	}

	pid1 = fork();
	if(pid1 < 0)
	{
		perror("fork hatasi 1");
		return ;
	}
	if(pid1 == 0) //bu child processtir demek
	{
		close(fd[0]); //okuma ucu kapatılır çünkü buna ihtiyacımız yok
		dup2(fd[1], STDOUT_FILENO); //çıktıyı pipe ın yazma ucuna bağla
		if((init_builtin_ex(shell, cmd)) == -1) //demek ki builtin değil
			execute_basic(shell, cmd);
		close(fd[1]);
		exit(1); //execve başarılı olursa buraya gelmeyecek
	}

	pid2 = fork();
	if(pid2 < 0)
	{
		perror("fork hatasi 2");
		return ;
	}
	if(pid2 == 0) //bu child processtir demek
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO); //girdiyi pipe ın okuma ucundan al
		if((init_builtin_ex(shell, cmd->next)) == -1) //demek ki builtin değil
			execute_basic(shell, cmd->next);
		close(fd[0]);
		exit(1); //execve başarılı olursa buraya gelmeyecek
	}

	close(fd[0]);
	close(fd[1]);

	waitpid(pid1, &status, 0); //(hangisini bekleyeceğim, çocuğun exit kodunu sakla, davranış ne)
	if(WIFEXITED(status)) //bu exit kod normal bir çıkış mı
		shell->last_exit_status = WEXITSTATUS(status); //exit kodunu çıkar
	else
		shell->last_exit_status = 1;

	waitpid(pid2, &status, 0);
	if(WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else
		shell->last_exit_status = 1;
}