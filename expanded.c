#include "minishell.h"

char *search_env()


void expand_arg(t_shell *shell, char *arg)
{
    char    *new_arg;
    int     i;
    int     j;
    
    i = 0;
    j = 0;
    while (arg[i])
    {
        if(arg[i] !='"' || arg[i] != '\'')
            j++;
        if(arg[i] == '"')
        {
            
        }
        if (j != 0)

        i++;
    }
    
}

void expanded(t_shell *shell)
{
    int i;

    while (shell->command)
    {
        i = 0;
        while (shell->command->args[i])
        {
            shell->command->args[i] = expand_arg(shell, shell->command->args[i]);
            i++;
        }
        shell->command = shell->command->next;
    }
    
}