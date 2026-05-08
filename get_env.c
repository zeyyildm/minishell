#include "minishell.h"

static void	set_env_helper(t_env *new_env, char *env)
{
	char	*key;
	int		i;

	key = ft_strchr(env, '=');
	if (key)
	{
		i = key - env;
		new_env->key = ft_substr(env, 0, i);
		new_env->value = ft_substr(env, i + 1, ft_strlen(env) - i - 1);
	}
	else
	{
		new_env->key = ft_strdup(env);
		new_env->value = NULL;
	}
}

void	get_env(t_shell *shell)
{
	t_env	*new_env;
	char	**envp;

	envp = shell->envp;
	while (*envp)
	{
		new_env = malloc(sizeof(t_env));
		if (!new_env)
			exit(1);
		set_env_helper(new_env, *envp);
		new_env->next = shell->env;
		shell->env = new_env;
		envp++;
	}
}
