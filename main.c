#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

//diyelim ki girdi ls

static int is_only_spaces(const char *s) //girilen satır boş mu veya sadece space mi
{
    int i = 0;

    if(!s)
        return 1;
    while(s[i])
    {
        if(s[i] != ' ' && s[i] != '\t')
            return 0;
        i++;
    }

    return 1;
}

static int is_same(const char *a, const char *b) //iki string birebir aynı mı?
{
    int i = 0;

    if (!a || !b)
        return 0;
    while (a[i] && b[i] && a[i] == b[i])
        i++;
    return (a[i] == '\0' && b[i] == '\0');
}

int main(int ac, char **av, char **envp)
{
    char *line;

    (void)ac;
    (void)av;
    (void)envp;

    while(1)
    {
        line = readline("minishell$ "); // =ls. readline bellek kullanır o yüzden free edilmeli
        if(line == NULL)
        {
            printf("exit\n");
            break;
        }

        if(is_only_spaces(line))
        {
            free(line);
            continue;
        }

        add_history(line);

        if(is_same(line, "exit"))
        {
            free(line); //eğer direkt exit yapıldıysa break ile çıkış yaparı
            break;
        }

        printf("Girdi: %s\n", line);
        free(line);
    }

    return (0);

}


//TOKENIZER

// -BOŞLUK GÖRÜRSEN GEÇ
// - | GÖRÜRSEN -> PIPE
// - < GÖRÜRSEN SONRAKİ < İSE HEREDOC DEĞİLSE REDIR
// - > GÖRÜRSEN SONRAKİ > İSE APPEND DEĞİLSE REDIR OUT
// - BUNLARIN HİÇBİRİNE UYMUYORSA WORD(DOSYA BİLE OLSA WORD KABUL EDERİZ)