#include "../minishell.h"

//ls -l
//path bul
//fork ()
//child execve
//parent waitpid
//exit statusu sürekli kaydedecegiz


static char *find_path(t_shell *shell) //bunun yerine get_env_values da kulllanılır staticten çıkartılıp
{
    t_env *tmp = shell->env;

    while(tmp)
    {
        if(is_cmd(tmp->key, "PATH"))
            return tmp->value;
        tmp = tmp ->next;
    }
    return (NULL);
}

char *find_ex_path(t_shell *shell, char *cmd_name)
{
    char    *path_env;
    char    **paths;
    char    *full_path;
    int     i;

    if(ft_strchr(cmd_name, '/'))
        return (ft_strdup(cmd_name));

    path_env = find_path(shell);
    if(!path_env)
        return (NULL);
    
    paths = ft_split(path_env, ':');
    if(!paths)
        return NULL;
    i = 0;
    while(paths[i])
    {
        full_path = ft_strjoin(paths[i], "/");
        char *tmp = full_path;
        full_path = ft_strjoin(full_path, cmd_name);
        free(tmp);

        if(access(full_path, X_OK) == 0)
        {
            int j = 0;
            while(paths[j])
                free(paths[j++]);
            free(paths);

            return full_path;
        }

        free(full_path);
        i++;
    }

    i = 0;
    while(paths[i])
        free(paths[i++]);
    free(paths);

    return (NULL);
}
void execute_basic(t_shell *shell, t_command *cmd)
{
    pid_t pid;
    int status; //childın exit kodunu almak icin
    char *full_path;

    full_path = find_ex_path(shell, cmd->argv[0]);
    if(!full_path)
    {
        printf("hataaa");
        shell->last_exit_status = 127;
        return;
    }

    pid = fork(); //yeni bir process yarattik
    if(pid < 0) //-1 hata
    {
        printf("hta");
    }

    if(pid == 0)
    {
        if(exec_redir(cmd))
            exit(1);
        execve(full_path, cmd->argv, shell->envp); 
        perror("execve");
        exit(126);
    }
    else
    {
        waitpid(pid, &status, 0);

        if(WIFEXITED(status)) //child normal exit yaptı mı
            shell->last_exit_status = WEXITSTATUS(status); //childın exit kodunu al
        else
            shell->last_exit_status = 1;
    }

    free(full_path);
}