#include "../minishell.h"
#include <errno.h>

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

static int  env_size(t_env *env)
{
    int count;

    count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return (count);
}

static void free_tmp_envp(char **envp)
{
    int i;

    i = 0;
    if (!envp)
        return ;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}

static char **build_envp(t_env *env)
{
    char    **envp;
    char    *tmp;
    int     i;

    envp = malloc(sizeof(char *) * (env_size(env) + 1));
    if (!envp)
        return (NULL);
    i = 0;
    while (env)
    {
        if (env->value)
        {
            tmp = ft_strjoin(env->key, "=");
            envp[i] = ft_strjoin(tmp, env->value);
            free(tmp);
        }
        else
            envp[i] = ft_strdup(env->key);
        if (!envp[i])
            return (free_tmp_envp(envp), NULL);
        i++;
        env = env->next;
    }
    envp[i] = NULL;
    return (envp);
}

void execute_basic(t_shell *shell, t_command *cmd)
{
    pid_t pid;
    int status; //childın exit kodunu almak icin
    char *full_path;

    status = 0;

    full_path = find_ex_path(shell, cmd->argv[0]);
    if(!full_path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        shell->last_exit_status = 127;
        return;
    }

    pid = fork(); //yeni bir process yarattik
    if(pid < 0) //-1 hata
    {
        perror("fork");
        free(full_path);
        shell->last_exit_status = 1;
        return ;
    }
    if(pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        char **child_envp;

        if(exec_redir(cmd))
            exit(1);
        child_envp = build_envp(shell->env);
        if (!child_envp)
            exit(1);
        execve(full_path, cmd->argv, child_envp);
        int err = errno;
        struct stat path_stat;
        
        stat(cmd->argv[0], &path_stat);
        if (err == EACCES && S_ISDIR(path_stat.st_mode))
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->argv[0], 2);
            ft_putstr_fd(": Is a directory\n", 2);
            free_tmp_envp(child_envp);
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
        free_tmp_envp(child_envp);
        free(full_path);
        free_lists(shell);
        free_env(shell->env);
        if (err == ENOENT)
            exit(127);
        exit(126);
    }
    else
    {
        while (waitpid(pid, &status, 0) == -1)
        {
            if (errno != EINTR)
                break;
        }

        if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == SIGINT)
                shell->last_exit_status = 130;
            else if (WTERMSIG(status) == SIGQUIT)
            {
                write(1, "Quit (core dumped)\n", 20);
                shell->last_exit_status = 131;
            }
        }
        if(WIFEXITED(status)) //child normal exit yaptı mı
            shell->last_exit_status = WEXITSTATUS(status); //childın exit kodunu al
        else
            shell->last_exit_status = 1;
    }

    free(full_path);
}