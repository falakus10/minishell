#include "minishell.h"

void	print_export(t_env *env)
{
	t_env	*tmp;
	char	*line;

	tmp = env;
	while (tmp)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		line = format_export_line(tmp);
		write(STDOUT_FILENO, line, ft_strlen(line));
		write(STDOUT_FILENO, "\n", 1);
		free(line);
		tmp = tmp->next;
	}
}


char	*add_quotes(char *str)
{
	int		len;
	int		i;
	char	*new_line;
	char	*equal_sign;

	equal_sign = ft_strchr(str, '=');
	if (!equal_sign)
		return (ft_strdup(str)); // '=' yoksa direkt kopyala
	len = ft_strlen(equal_sign + 1);
	new_line = malloc(sizeof(char) * (len + 3)); // "value" + \0
	if (!new_line)
		ft_error();
	new_line[0] = '\"';
	i = 0;
	while (i < len)
	{
		new_line[i + 1] = equal_sign[1 + i];
		i++;
	}
	new_line[i + 1] = '\"';
	new_line[i + 2] = '\0';
	return (new_line);
}

void	fill_env(t_env *env, int index, char *str)
{
	t_env	*tmp;
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		ft_error();
	tmp = env;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = node;
	node->line = ft_strdup(str);
	if (index != -1)
	{
		node->value = ft_strdup(str + index);
		node->flag = 1;
	}
	else
	{
		node->value = NULL;
		node->flag = 0;
	}
	node->next = NULL;
}

int	check_valid(char *str)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(str[i])) && (str[i] != '_'))
		return (1);
	while(str[i] != '\0' && str[i] != '=')
	{
		if (!(ft_isalpha(str[i])) && (str[i] != '_') && !(ft_isdigit(str[i])))
			return (1);
		i++;
	}
	return (0);
}

int	ft_export(t_command_block *cmd, t_env  *env)
{
	int		i;
	char	*equal_point;
	int		index;
	int		flag;

	index = -1;
	flag = 0;
	if (cmd->args[1] == NULL)
		print_export(env);
	i = 0;
	while (cmd->args[++i] != NULL)
	{
		if (check_valid(cmd->args[i]))
		{	
			flag = 1;
			printf("minishell: export : '%s': not a valid identifier\n", cmd->args[i]);
			continue;
		}
		equal_point = ft_strchr(cmd->args[i], '=');
		if (equal_point)
			index = equal_point - cmd->args[i];
		fill_env(env, index, cmd->args[i]);
	}
	return (flag);
}
