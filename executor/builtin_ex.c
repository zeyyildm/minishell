#include "minishell.h"

int init_builtin_ex(t_shell *shell, t_command *cmd)
{
    char *name = cmd->argv[0];

    if(!name)
        return (0);
    if(ft_strcmp(name, "echo") == 0)
        return (built_echo(cmd));
    else if(ft_strcmp(name, "cd") == 0)
        return (built_cd(cmd));
    else if(ft_strcmp(name, "pwd") == 0)
        return (built_pwd(cmd));
    else if(ft_strcmp(name, "export") == 0)
        return (built_export(cmd));
    else if(ft_strcmp(name, "unset") == 0)
        return (built_unset(cmd));
    else if(ft_strcmp(name, "env") == 0)
        return (built_env(cmd));
    else if(ft_strcmp(name, "exit") == 0)
        return (built_exit(cmd));
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
        if(ft_strcmp(env->key, key) == 0) //key'i "X" olaninin kalanini döndürüyor
            return env->value;
        env = env->next;
    }

    return (NULL);
}
static void update_env(t_env *env, char *key, char *new_value)
{
    while(env)
    {
        if(ft_strcmp(env->key, key) == 0)
        {
            free(env->value);
            env->value = ft_strdup(new_value);
            return;
        }
        env = env->next;
    }
}

int built_cd(t_shell *shell,t_command *cmd)
{
    char *path_target;
    if(!cmd->argv[1]) //arguman yoksa home'a gitmek zorunda
       path_target = get_env_values(shell->env, "HOME");
    else
        path_target = cmd->argv[1]; //arguman varsa yazilan pathe gider
    if(!path_target)
    {
        perror("cd home bulma hatasiii");
        return (1);
    }
    if(chdir(path_target) == -1) //change directory basarisiz olursa -1 döner
    {
        perror("cd target bulma hatasiii");
        return (1);
    }
    //simdi sourun yoksa dizini degistirecegiz
    //once eski pwd degiskenini sonra yenisini ogrenmemiz lazim
    char *old_pwd = get_env_values(shell->env, "PWD"); //env listesinde pwd olarak kayıtlı mevcut dizin
    char *new_pwd = getcwd(NULL, 0); //chdr yaptıktan sonra shellin şu anki dizisi
    if(!old_pwd || !new_pwd)
    {
        perror("cd  hatasiii");
        return (1);
    }
    //simdi node'larda path değiştirme yapacagiz
    update_env(shell->env, "OLDPWD", old_pwd);
    //eskiyi guncelledik
    update_env(shell->env, "PWD", new_pwd);

    free(new_pwd); //old_pwd free etmedik cünkü env lsitesinde zaten kullaniliyor
    return (0);
    
}

int built_pwd(t_command *cmd)
{
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

int built_export(t_command *cmd) //env listesine ekler veya gunceller boylece child processler gorebilir
{
    //varsa degistir (update_env kullanicaz)
    //yoksa ekle(add_env_list gibi bi fonks)
}

int built_unset(t_command *cmd) //değişkeni shell env lişstesinden kaldırır artik child goremez
{
 //unset sadece shell ortamini degistirir kalici degisiklik yapamaz
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