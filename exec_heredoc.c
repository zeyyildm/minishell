/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:43:08 by hakalkan          #+#    #+#             */
/*   Updated: 2026/02/16 13:47:18 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

// Heredoc mini bir pipe gibi davranıyor
// Shell heredoc için geçici bir pipe açar
// fd[1] ucu = heredoc yazma ucu
// fd[0] ucu = komut okuma ucu

void exec_heredoc(t_shell *shell, t_command *cmd)
{
	t_command	*tmp;
	t_redir		*redir;
	int			fd[2];
	pid_t		pid;
	char		*line;
	char		*delimiter;

	tmp = cmd;
	while (tmp)
	{
		redir = tmp->redirs;
		while (redir)
		{
			if (redir->type == T_HEREDOC)
			{
				if (pipe(fd) == -1)
				{
					perror("pipe");
					return;
				}

				// Delimiter'ı quote'lardan temizle
				delimiter = for_quotes(redir->file);

				// Parent process user'dan heredoc satırlarını okur
				while (1)
				{
					line = readline("> ");
					if (!line)
						break;

					// Delimiter ile eşit mi kontrol et
					if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
						&& ft_strlen(line) == ft_strlen(delimiter))
					{
						free(line);
						break;
					}

					// Satırı pipe'a yaz
					write(fd[1], line, ft_strlen(line));
					write(fd[1], "\n", 1);
					free(line);
				}
				close(fd[1]);	// Yazma ucunu kapat, child EOF alsin
				free(delimiter);

				// Child process oluştur
				pid = fork();
				if (pid < 0)
				{
					perror("fork");
					return;
				}
				if (pid == 0)
				{
					// Child: STDIN'i heredoc pipe'ından oku
					dup2(fd[0], STDIN_FILENO);
					close(fd[0]);

					// Diğer redirectionları yap (heredoc hariç)
					if (exec_redir(tmp) != 0)
						exit(1);

					// Komut çalıştır
					if (init_builtin_ex(shell, tmp) == -1)
						execute_basic(shell, tmp);
					exit(0);
				}

				// Parent: okuma ucunu kapat ve child'i bekle
				close(fd[0]);
				waitpid(pid, NULL, 0);
			}
			redir = redir->next;
		}
		tmp = tmp->next;
	}
}
