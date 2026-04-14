#include "minishell.h"

static char *get_env_value(t_shell *shell, char *key)
{
    t_env *tmp;

    tmp = shell->env;
    while (tmp)
    {
        if (ft_strncmp(tmp->key, key, ft_strlen(key)) == 0 && tmp->key[ft_strlen(key)] == '\0')
        {
            return tmp->value;
        }
        tmp = tmp->next;
    }
    return NULL;
}

static int expand_variable(t_shell *shell, char *arg ,char **result)
{
    char    *key_name;
    int     i;
    char    *value;
    char    *tmp;

    i = 0;
    if (arg[0] == '?')
    {
        value = ft_itoa(shell->last_exit_status);
        if (!value)
            return (0);
        tmp = ft_strjoin(*result, value);
        free(*result);
        *result = tmp;
        free(value);
        return (2);
    }
    if (!arg[0] || (!(ft_isalnum(arg[0]) || arg[0] == '_')))
    {
        tmp = ft_strjoin(*result, "$\0");
        free(*result);
        *result = tmp;
        return (1);
    }
    while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
    {
        i++;
    }
    key_name = ft_substr(arg, 0, i);
    if(key_name)
    {
        value = get_env_value(shell, key_name);
        if (value)
        {
            tmp = ft_strjoin(*result, value);
            free(*result);
            *result = tmp;
        }
        free(key_name);
        return i + 1;
    }
    return 0;
}


static char *append_char(char *str,char c)
{
    char *new_str;
    int size;
    int i;

    size = ft_strlen(str);
    new_str = malloc(size + 2);
    if (!new_str)
        return NULL;
    i = 0;
    while (str[i])
    {
        new_str[i] = str[i];
        i++;
    }
    new_str[i] = c;
    new_str[i + 1] = '\0';
    free(str);
    return new_str;
}

char *expand_arg(t_shell *shell, char *arg)
{
    char    *new_arg;
    int     i;
    char    state;
    
    i = 0;
    state = 0;
    new_arg = ft_strdup("");
    while (arg[i])
    {
        if(arg[i] == '\'' && state != '"')
        {
            if(state == '\'')
                state = 0;
            else
                state = '\'';
        }
        else if(arg[i] == '"' && state != '\'')
        {
            if(state == '"')
                state = 0;
            else
                state = '"';
        }
        else if (arg[i] == '$' && state != '\'')
        {
            i += expand_variable(shell, arg  + i + 1,&new_arg);
            continue;
        }
        else
            new_arg = append_char(new_arg, arg[i]);
        i++;
    }
    return (new_arg);
}

char *for_quotes(char *s)
{
    char    *line;
    int     i;
    int     j;
    char    k;

    k = 0;
    i = 0;
    j = 0;
    line = malloc(ft_strlen(s) + 1);
    if(!line)
        return (NULL);
    while(s[i])
    {
        if((s[i] == '"' || s[i] == '\'') && k == 0)
        {
            k = s[i];
            i++;
            while(s[i] && s[i] != k)
                line[j++] = s[i++];
            if(s[i] == k)
                i++;
            k = 0;
            continue;
        }
        line[j++] = s[i++];
    }
    line[j] = '\0';
    return (line);
}

void expanded(t_shell *shell)
{
    int i;
    int j;
    char *old;
    char **new_argv;
    int count;

    t_command *tmp;
    tmp = shell->commands;
    while (tmp)
    {
        // First pass: expand all arguments
        i = 0;
        while (tmp->argv[i])
        {
            old = tmp->argv[i];
            tmp->argv[i] = expand_arg(shell, tmp->argv[i]);
            free(old);
            i++;
        }
        
        // Second pass: remove empty strings and rebuild argv
        count = 0;
        i = 0;
        while (tmp->argv[i])
        {
            if (tmp->argv[i][0] != '\0')
                count++;
            i++;
        }
        
        // If any empty strings, rebuild argv without them
        if (count < i)
        {
            new_argv = malloc(sizeof(char *) * (count + 1));
            if (new_argv)
            {
                j = 0;
                i = 0;
                while (tmp->argv[i])
                {
                    if (tmp->argv[i][0] != '\0')
                    {
                        new_argv[j] = tmp->argv[i];
                        j++;
                    }
                    else
                        free(tmp->argv[i]);
                    i++;
                }
                new_argv[j] = NULL;
                free(tmp->argv);
                tmp->argv = new_argv;
            }
        }
        
        tmp = tmp->next;
    }
}
