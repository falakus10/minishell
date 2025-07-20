#include "minishell.h"

t_lexer_list	**input_loop(void)
{
	char *input;
	char *temp_input;
	t_lexer_list **list;

	while (1)
	{
		temp_input = readline("minishell>"); //temp_input yerine input kullanamayız çünkü readline'dan dönen alanı kaybederiz, leak çıkar.
		if (temp_input == NULL)
		{
			free(temp_input); //NULL şeyi freelemek saçma
			write(1, "exit\n", 5);
			exit(0); //0 mı 1 mi
		}
		if (temp_input[0] == '\0')
    	{
        free(temp_input);
        continue;
    	}
		input = ft_strtrim(temp_input, " ");
		list = lexer_function(input);
		free(temp_input); // bununla işimiz bitti
		// input'u da işimiz bitince free'lemeliyiz
		break;
	}
	return (list);
}
	
int	main(int argc, char *argv[], char **env)
{
	(void)argc;
	(void)argv;
	(void)env;
	t_lexer_list	*temp;
	t_lexer_list	**list;
	t_expander		*exp;
	t_command_block	*command_block;
	t_joined_lexer_list **new_list;

	exp = malloc(sizeof(t_expander));
	if (!exp)
		return (0);
	signal_handler();
	list = input_loop(); //bu listede lexer'da ayrılmış olan token'ları tutuyoruz 
	temp = *list;
	expander(temp, env, exp);
	remove_quotes(*list);
	new_list = token_join(temp);
	command_block = parser(*new_list);


int i = 0;

while (command_block != NULL)
{
    printf("command = %s\n", command_block->command);

    if (command_block->files != NULL)
    {
        i = 0;
        while (command_block->files[i] != NULL)
        {
            printf("files[%d] = %s\n", i, command_block->files[i]);
            i++;
        }
    }
	printf("input fd :%d\n", command_block->input_fd);
	printf("output fd :%d\n", command_block->output_fd);
	if (command_block->heredoc_delimiters != NULL)
	{
		i = 0;
		while (command_block->heredoc_delimiters[i] != NULL)
		{
			printf("heredoc_delimiters[%d] = %s\n", i, command_block->heredoc_delimiters[i]);
			i++;
		}
	}

    if (command_block->args != NULL)
    {
        i = 0;
        while (command_block->args[i] != NULL)
        {
            printf("args[%d] = %s\n", i, command_block->args[i]);
            i++;
        }
    }

    command_block = command_block->next;
}
	return (0);
}