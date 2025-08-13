#include "minishell.h"

char *ft_strcpy(char *dest, const char *src)
{
	int i = 0;

	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return dest;
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return dest;
}

char *ft_strcat(char *dest, const char *src)
{
	int i = 0;
	int j = 0;

	// dest'in sonuna git
	while (dest[i])
		i++;

	// src'yi dest'in sonuna kopyala
	while (src[j])
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return dest;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

t_env	*add_new_node3(t_env **env_list)
{
	t_env	*node;
	t_env	*temp;
	node = malloc(sizeof(t_env));
	if (!node)
		ft_error();
	node->next = NULL;
	node->value = NULL;
	node->flag = 0;
	if (*env_list == NULL)
		*env_list = node;
	else
	{
		temp = *env_list;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = node;
	}
	return (node);
}

t_env	**take_env(t_env **env_list, char **env)
{
	int	i;
	int	j;
	t_env *current;

	*env_list = NULL; //mainde de yaptım, sorun olur mu ? sanmıyorum
	i = -1;
	while (env[++i] != NULL)
	{
		j = -1;
		current = add_new_node3(env_list);
		current->line = ft_strdup(env[i]);
		while (env[i][++j] != '\0')
		{
			if (env[i][j] == '=')
			{
				current->flag = 1;
				current->value = ft_strdup(&env[i][j + 1]);
				break;
			}
		}
	}
	return (env_list);
}

void	free_arr(char **arr)
{
	int i;

	i = 0;
	if (!arr)
		return;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}
void	fill_int_array(int *arr, int value, int count)
{
    int i;

	i = 0;
    while (i < count)
    {
        arr[i] = value;
        i++;
    }
}


int	is_space_or_tab(char c)
{
	return (c == ' ' || c == '\t');
}

char	*trim_whitespace(const char *input)
{
	size_t	start;
	size_t	end;
	size_t	i;
	char	*trimmed;

	start = 0;
	while (input[start] && is_space_or_tab(input[start]))
		start++;
	end = 0;
	while (input[end])
		end++;
	if (end == 0)
		return (strdup(""));  // input boşsa
	end--;  // son geçerli karakterin index'i
	while (end > start && is_space_or_tab(input[end]))
		end--;
	trimmed = (char *)malloc(end - start + 2);
	if (!trimmed)
		return (NULL);
	i = 0;
	while (start <= end)
		trimmed[i++] = input[start++];
	trimmed[i] = '\0';
	return (trimmed);
}

void	write_message(char *msg1, char *msg2, char *msg3, int fd)
{
	if (msg1)
		write(fd, msg1, ft_strlen(msg1));
	if (msg2)
		write(fd, msg2, ft_strlen(msg2));
	if (msg3)
		write(fd, msg3, ft_strlen(msg3));
	write(fd, "\n", 1);
}