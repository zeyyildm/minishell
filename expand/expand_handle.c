#include "../minishell.h"

int	handle_named_var(t_shell *shell, char *arg, char **result)
{
	char	*key_name;
	int		i;
	char	*value;
	char	*tmp;

	i = 0;
	while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
		i++;
	key_name = ft_substr(arg, 0, i);
	if (key_name)
	{
		value = get_env_value(shell, key_name);
		if (value)
		{
			tmp = ft_strjoin(*result, value);
			free(*result);
			*result = tmp;
		}
		free(key_name);
		return (i + 1);
	}
	return (0);
}

int	handle_dollar(t_shell *shell, char *arg, int i, char **new_arg)
{
	int	step;

	step = expand_variable(shell, arg + i + 1, new_arg);
	return (step);
}

void	expand_argv(t_shell *shell, char **argv)
{
	int		i;
	char	*old;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		old = argv[i];
		argv[i] = expand_arg(shell, argv[i]);
		free(old);
		i++;
	}
}
