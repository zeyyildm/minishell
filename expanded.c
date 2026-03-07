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
    int    i;

    i = 0;
    while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
    {
        i++;
    }
    key_name = ft_substr(arg, 0, i);
    if(key_name)
    {
        char *value = get_env_value(shell, key_name);
        if (value)
        {
            *result = ft_strjoin(*result, value);
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
    line = malloc(ft_strlen(s));
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

    t_command *tmp;
    tmp = shell->commands;
    while (tmp)
    {
        i = 0;
        while (tmp->argv[i])
        {
            tmp->argv[i] = expand_arg(shell, tmp->argv[i]);
            i++;
        }
        tmp = tmp->next;
    }
}