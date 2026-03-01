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
    return (-1);
}

int built_echo(t_command *cmd)
{
    int i;

    i = 1;
    while(cmd->argv[i]) //echodan sonra gelen yazdırmak istediklerimiz
    {
        printf("%s", cmd->argv[i]);
        if(cmd->argv[i + 1])
            printf(" "); //yazdırır ve araya bir boşluk ekler
        i++;
    }
    printf("\n");
    return (0);    
}

static char *get_env_values(t_env *env, char *key)
{
    while(env)
    {
        if(is_cmd(env->key, key)) //key'i "X" olaninin kalanini döndürüyor
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
	
    if (!cmd->argv[1]) //arguman yoksa home'a gitmek zorunda
       path_target = get_env_values(shell->env, "HOME");
    else
        path_target = cmd->argv[1]; //arguman varsa yazilan pathe gider
    if (!path_target)
    {
        perror("cd home bulma hatasiii");
        return (1);
    }
    if(chdir(path_target) == -1) //change directory basarisiz olursa -1 döner
    {
        perror("cd target bulma hatasiii");
        return (1);
    }
    old_pwd = get_env_values(shell->env, "PWD"); //env listesinde pwd olarak kayıtlı mevcut dizin
    new_pwd = getcwd(NULL, 0); //chdr yaptıktan sonra shellin şu anki dizisi
    if (!old_pwd || !new_pwd)
    {
        perror("cd  hatasiii");
        return (1);
    }
    update_env(shell->env, "OLDPWD", old_pwd);
    update_env(shell->env, "PWD", new_pwd);

    free(new_pwd); //old_pwd free etmedik cünkü env lsitesinde zaten kullaniliyor
    return (0);
    
}

int built_pwd(t_command *cmd)
{
	(void)cmd;
    char *path; //calisma dizinini tutacak
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
int built_export(t_shell *shell, t_command *cmd) //env listesine ekler veya gunceller boylece child processler gorebilir
{
	int		i;
	char	*equal;

	i = 1;
	if(!cmd->argv[1]) //eğer kullanıcı bir argüman girmediyse tüm envleri göstermek lazım
		return (built_env(shell));

	while(cmd->argv[i] != NULL)
	{
		equal = ft_strchr(cmd->argv[i], '='); //myvar = hello içinde eşittir var mı diye geziyoruz
		if(equal == NULL)
		{
			char *key = ft_strdup(cmd->argv[i]); //MYVAR'ı kopyalar
			if(get_env_values(shell->env, key) == NULL) //env listesinde "MYVAR" var mi diye bakar
				add_env_node(shell, key, NULL); //yoksa env listesine myvarı ekle
			free(key);
			i++;
			continue;
		}

		else //önce eşittirden önesini key olarak bulmamız lazımm
		{
			char *key = ft_substr(cmd->argv[i], 0, equal - cmd->argv[i]);
			char *value = ft_strdup(equal + 1); // hello kısmını alır

			if(get_env_values(shell->env, key)) //key daha önceden var mı diye bakarız
				update_env(shell->env, key, value); //var olan key', düzenle
			else
				add_env_node(shell, key, value); //eğer yoksa da bu değişkeni env listesine ekle
			free(key);
			free(value);
			i++;
		}
	}
	return (0);
}

int built_unset(t_shell *shell, t_command *cmd) //değişkeni shell env lişstesinden kaldırır artik child goremez
{
	int i;
	t_env	*tmp;
	t_env	*prev;

	i = 1;
	while(cmd->argv[i]) //env listesi icinde gez
	{
		tmp = shell->env;
		prev = NULL;

		while(tmp)
		{
			if (is_cmd(tmp->key, cmd->argv[i])) //env listesi içinde MYVAR ' ı buldu
			{
				if(prev == NULL)
					shell->env = tmp->next; //başı kaldır. bu eğer myvar baştaysa çalışır
				else
					prev->next = tmp->next; //sonra bagla
				free(tmp->key);
				free(tmp->value);
				free(tmp);
				break;
			}
			prev = tmp; //listede ilerlemeden şu anki node u hep önceki olarak kayıt altında tutuyoruz ki kod MYVAR'dan öncesinin ne olduğunu bilebilsin
			tmp = tmp->next;
	
		}
		i++;
	}
	return (0);
}

int built_env(t_shell *shell) //bu fonks env listesindeki KEY=VALUE ikililerini bastırmalı
{
    t_env *tmp;

    tmp = shell->env;
    while(tmp) //env listesi icinde geziyoruz
    {
        if(tmp->value) //eger value su varsa
            printf("%s=%s\n", tmp->key, tmp->value); // key=value yazdir
        tmp = tmp->next; //listede ilerle
    }
    return (0);
}