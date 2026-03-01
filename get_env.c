#include "minishell.h"

//bu fonks shell başladığı gibi otomatik çalışacak, ortam değişkenlerini yükleyecek
//shellin env listesi dolu olacak
void get_env(t_shell *shell)
{
    t_env *new_env;
    char **envp;
    char *key;    
    int i;
    
    i = 0;
    envp = shell->envp;
    while (*envp)
    {
        new_env = malloc(sizeof(t_env));
        if(!new_env)
            exit(1);
        key = ft_strchr(*envp, '=');
        if (key)
		{
			i = key - *envp;
			new_env->key = ft_substr(*envp, 0, i);
			new_env->value = ft_substr(*envp, i + 1, ft_strlen(*envp) - i - 1);
		}
        else
		{
			new_env->key = ft_strdup(*envp);
			new_env->value = NULL;
		}
        new_env->next = shell->env; // yeni eklenen env listenin başına ekle
        shell->env = new_env; // shell'in env pointerını yeni env güncele    
        envp++;
    }
    
}
