#include "../minishell.h"

void	add_env_node(t_shell *shell, char *key, char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	if (!shell->env)
		shell->env = new;
	else
	{
		tmp = shell->env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static void	swap(t_env *i, t_env *j)
{
	char	*tmp;

	tmp = i->key;
	i->key = j->key;
	j->key = tmp;
	tmp = i->value;
	i->value = j->value;
	j->value = tmp;
}

t_env	*env_sort(t_env *env)
{
	t_env	*i;
	t_env	*j;

	i = env;
	while (i)
	{
		j = i->next;
		while (j)
		{
			if (ft_strncmp(i->key, j->key,
					ft_strlen(i->key) + ft_strlen(j->key) + 1) > 0)
				swap(i, j);
			j = j->next;
		}
		i = i->next;
	}
	return (env);
}

void	env_helper(t_shell *shell, t_command *cmd)
{
	t_env	*tmp;

	if (exec_redir(cmd) != 0)
	{
		free_lists(shell);
		free_env(shell->env);
		exit(1);
	}
	tmp = env_sort(shell->env);
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	free_lists(shell);
	free_env(shell->env);
	exit(0);
}
