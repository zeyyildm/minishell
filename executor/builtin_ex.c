#include "../minishell.h"


static int is_cmd(char *name, char *cmd)
{
    if (!name || !cmd)
        return (0);
    if (ft_strlen(name) != ft_strlen(cmd))
        return (0);
    return (ft_strncmp(name, cmd, ft_strlen(cmd)) == 0);
}
int init_builtin_ex(t_shell *shell, t_command *cmd)
{
    char *name;

    name = cmd->argv[0];
    if (!name)
        return (0);
    if (is_cmd(name, "echo"))
        return (built_echo(cmd));
    else if (is_cmd(name, "cd"))
        return (built_cd(shell, cmd));
    else if (is_cmd(name, "pwd"))
        return (built_pwd(cmd));
    else if (is_cmd(name, "export"))
        return (built_export(shell, cmd));
    else if (is_cmd(name, "unset"))
        return (built_unset(shell, cmd));
    else if (is_cmd(name, "env"))
        return (built_env(shell));
    else if (is_cmd(name, "exit"))
        return (built_exit(cmd));
    return (-1);
}

int built_echo(t_command *cmd)
{
    int i;

    i = 1;
    while(cmd->argv[i])
    {
        printf("%s", cmd->argv[i]);
        if(cmd->argv[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
    return (0);    
}

static char *get_env_values(t_env *env, char *key)
{
    while(env)
    {
        if(is_cmd(env->key, key))
            return env->value;
        env = env->next;
    }

    return (NULL);
}
static void update_env(t_env *env, char *key, char *new_value)
{
    while(env)
    {
        if(is_cmd(env->key, key))
        {
            free(env->value);
            env->value = ft_strdup(new_value);
            return;
        }
        env = env->next;
    }
}

int	built_cd(t_shell *shell,t_command *cmd)
{
    char	*path_target;
    char	*old_pwd;
    char	*new_pwd;
	
    if (!cmd->argv[1])
       path_target = get_env_values(shell->env, "HOME");
    else
        path_target = cmd->argv[1];
    if (!path_target)
    {
        perror("cd home bulma hatasiii");
        return (1);
    }
    if(chdir(path_target) == -1)
    {
        perror("cd target bulma hatasiii");
        return (1);
    }
    old_pwd = get_env_values(shell->env, "PWD");
    new_pwd = getcwd(NULL, 0);
    if (!old_pwd || !new_pwd)
    {
        perror("cd  hatasiii");
        return (1);
    }
    update_env(shell->env, "OLDPWD", old_pwd);
    update_env(shell->env, "PWD", new_pwd);

    free(new_pwd);
    return (0);
    
}

int built_pwd(t_command *cmd)
{
	(void)cmd;
    char *path;
    path = getcwd(NULL, 0);
    if(!path)
    {
        perror("pwd error");
        return (1);
    }
    printf("%s\n", path);
    free(path);
    return (0);
}
void add_env_node(t_shell *shell, char *key, char *value)
{
    t_env *new;
    t_env *tmp;

    new=malloc(sizeof(t_env));
    new->key = ft_strdup(key);
    if(value)
        new->value = ft_strdup(value);
    else
        new->value = NULL;
    new->next = NULL;

    if(!shell->env)
        shell->env = new;
    else
    {
        tmp = shell->env;
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }

}
int built_export(t_shell *shell, t_command *cmd)
{
	int		i;
	char	*equal;

	i = 1;
	if(!cmd->argv[1])
		return (built_env(shell));

	while(cmd->argv[i] != NULL)
	{
		equal = ft_strchr(cmd->argv[i], '=');
		if(equal == NULL)
		{
			char *key = ft_strdup(cmd->argv[i]);
			if(get_env_values(shell->env, key) == NULL)
				add_env_node(shell, key, NULL);
			free(key);
			i++;
			continue;
		}

		else
		{
			char *key = ft_substr(cmd->argv[i], 0, equal - cmd->argv[i]);
			char *value = ft_strdup(equal + 1);

			if(get_env_values(shell->env, key))
				update_env(shell->env, key, value);
			else
				add_env_node(shell, key, value);
			free(key);
			free(value);
			i++;
		}
	}
	return (0);
}

int built_unset(t_shell *shell, t_command *cmd)
{
	int i;
	t_env	*tmp;
	t_env	*prev;

	i = 1;
	while(cmd->argv[i])
	{
		tmp = shell->env;
		prev = NULL;

		while(tmp)
		{
			if (is_cmd(tmp->key, cmd->argv[i]))
			{
				if(prev == NULL)
					shell->env = tmp->next;
				else
					prev->next = tmp->next;
				free(tmp->key);
				free(tmp->value);
				free(tmp);
				break;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (0);
}

int built_env(t_shell *shell)
{
    t_env *tmp;

    tmp = shell->env;
    while(tmp)
    {
        if(tmp->value)
            printf("%s=%s\n", tmp->key, tmp->value);
        tmp = tmp->next;
    }
    return (0);
}

int built_exit(t_command *cmd)
{
    //exit-code: programın kapattığında os e döndürdüğü 0-255 arasındaki sayı
    //0 = başarılı diğerleri hata
    //shellde her bir komut exit code bırakır $? ile kodu öğrenebiliriz
    int exit_code;

    exit_code = 0;
    if(cmd->argv[1]) //exit 42
        exit_code = ft_atoi(cmd->argv[1]);
    printf("exit\n");
    exit(exit_code); //burada process zorlanır, tüm kaynaklar temizlenir, fdler kapanır, parenta sinyal gider
}