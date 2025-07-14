#include "minishell.h"

// strdup kullanmak bellek güvenliği için daha doğrudur.
t_command_block *parser(t_lexer_list *list)
{
    t_command_block *command_block = NULL;
    t_command_block *temp_block = NULL;
    t_lexer_list *temp = list;
    int pipe_count = 0;
	int last_token_type = -1;

    while (temp != NULL)
    {
        int is_cmd_pointed = 0;
        int first_token_flg = 0;

        // Yeni bir komut bloğu oluştur
        t_command_block *new_block = malloc(sizeof(t_command_block));
        if (!new_block)
            return NULL; // malloc başarısızsa

        new_block->next = NULL;
        new_block->args = NULL;
        new_block->fd = NULL;
        new_block->files = NULL;
        new_block->operators = NULL;
        new_block->heredoc_delimiters = NULL;
        new_block->heredoc_count = 0;
        new_block->operator_count = 0;
        new_block->argument_count = 0;
        new_block->command = NULL;

        if (command_block == NULL)
        {
            command_block = new_block;
            temp_block = command_block;
        }
        else
        {
            temp_block->next = new_block;
            temp_block = temp_block->next;
        }

        if (temp->type == PIPE)
        {
            printf("First token cannot be a pipe\n");
            ft_error();
        }

        while (temp != NULL && temp->type != PIPE)
        {
            if (first_token_flg == 0 && temp->next == NULL &&
                (temp->type == REDIR_IN || temp->type == REDIR_OUT || temp->type == APPEND || temp->type == HEREDOC))
            {
                printf("Operator cannot be single\n");
                ft_error();
            }
            first_token_flg = 1;

            if (temp->next != NULL &&
                (temp->type == REDIR_IN || temp->type == REDIR_OUT || temp->type == APPEND || temp->type == HEREDOC))
            {
                if (temp->next->type != WORD && temp->next->type != S_QUOTE && temp->next->type != D_QUOTE)
                {
                    printf("Operator must be followed by a word\n");
                    ft_error();
                }

                if (temp->type == HEREDOC)
                {
                    temp_block->heredoc_delimiters = realloc(temp_block->heredoc_delimiters,
                        sizeof(char *) * (temp_block->heredoc_count + 1));
                    temp_block->heredoc_delimiters[temp_block->heredoc_count++] = ft_strdup(temp->next->token);
                    temp = temp->next;
                }
                else // REDIR_IN, REDIR_OUT, APPEND
                {
                    temp_block->files = realloc(temp_block->files,
                        sizeof(char *) * (temp_block->operator_count + 1));
                    temp_block->files[temp_block->operator_count++] = ft_strdup(temp->next->token);
                    temp = temp->next;
                }
            }
            else if (!is_cmd_pointed && (temp->type == WORD || temp->type == S_QUOTE || temp->type == D_QUOTE))
            {
                temp_block->command = ft_strdup(temp->token);
                is_cmd_pointed = 1;
            }
            else if (is_cmd_pointed && (temp->type == WORD || temp->type == S_QUOTE || temp->type == D_QUOTE))
            {
                temp_block->args = realloc(temp_block->args,
                    sizeof(char *) * (temp_block->argument_count + 1));
                temp_block->args[temp_block->argument_count++] = ft_strdup(temp->token);
            }
			last_token_type = temp->type; // işlenmiş en son token tipi // mkmk mlmlmll | 
            temp = temp->next;
        }
		if (temp == NULL && (
			last_token_type == PIPE ||
			last_token_type == REDIR_IN ||
			last_token_type == REDIR_OUT ||
			last_token_type == APPEND ||
			last_token_type == HEREDOC))
		{
		printf("Syntax error: input cannot end with an operator like '|', '<', '>', '>>', or '<<'\n");
		ft_error();
		}
		
        if (temp != NULL) //temp NULL sa zaten döngü bitecek, komut kümesi bitti demektir
            temp = temp->next; // PIPE varsa sonrasına geç
        pipe_count++;
    }
    return (command_block);
}
/*
realloc değişecek
değişken listem doğru mu ? yani mesela heredoc_delimiters , operator_count gerekli mi ? 
heredoc_delimiters için direk realloc ypılıyor NULL olduğu durum için de olmadığı durum için de , redir in, redir out, append , args için de aynı şekilde 
fd'leri kayıt etmeliyim ?  
fonksiyon bölünecek
son token | ise hata vermiyor ?
girdi çok girince segmentation fault veriyor
*/