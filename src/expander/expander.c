#include "minishell.h"

char	*remove_env_from_token(char *token, int dollar_index, int key_len)
{
	char	*before;
	char	*after;
	char	*new_token;

	before = ft_substr(token, 0, dollar_index); // '$' öncesi
	after = ft_strdup(token + dollar_index + key_len + 1); // '$' + key sonrası
	new_token = ft_strjoin(before, after);

	free(before);
	free(after);
	free(token);
	return (new_token);
}


int special_character(char *token, t_expander *expander)
{
	if (token[expander->i] == '$')
	{
		expander->i++;
		return (1);
	}
	else if (special_ch_check(token[expander->i]))
	{
		expander->i++;
		while (special_ch_check(token[expander->i]))
			expander->i++;
		while(token[expander->i] != '\0' && (is_valid_ch(token, expander->i))) 
			expander->i++;
		return(1);
	}
	return (0);
}
int	question_mark(t_lexer_list *temp, int i, t_expander *expander)
{
	if (temp->token[i] == '?')
	{
		//execde işlem sonucunda dönen sayıyıyı yazdırır şimdlilik full 0 koyucam 
		expander->start = i;
		while (temp->token[i] != '\0' && is_valid_ch(temp->token, i))
			i++;
		expander->env_key = "?";
		expander->env_val = ft_itoa(expander->exit_value);
		temp->token = ft_strjoin_free(temp->token, expander);
		expander->i = 0;
		return (1);
	}
	return (0);
}
int	change_to_env(t_lexer_list *temp, int i, t_expander *expander, t_env *env_list) //return değeri expanderda continue'ya girmek için kullanılıyor
{
	expander->start = i;//$EMPTY
	while (temp->token[i] != '\0' && (is_valid_ch(temp->token ,i)))
	{
		i++;
	}
	expander->env_key = ft_substr(temp->token, expander->start, i - expander->start); // $ işaretinden sonraki karakterden başlayarak geçerli karaktere kadar olan kısmı alır
	expander->env_val = env_value(env_list, expander->env_key);
	if (expander->env_val != NULL)
	{
		temp->token = ft_strjoin_free(temp->token, expander);
		expander->i = 0;
		return (1);
	}//burayı < > >> << göre güncelle null olma durumunu
	else
	{
		temp->token = remove_env_from_token(temp->token, expander->start - 1, i - expander->start);
		expander->i = expander->start - 2; // <-- kritik değişiklik
	}
	return (0);
}

int quote(char *token, t_expander *expander)
{
	if (token[expander->i] == '\'')
	{
		expander->i++;
		while (token[expander->i] != '\'' && token[expander->i] != '\0')
			expander->i++;
		return (1);
	}
	return (0);
}

void expander(t_lexer_list *temp, t_env *env_list, t_expander *expander)
{
	if (temp == NULL || temp->token == NULL)
	    return;
	expander->token_len = ft_strlen(temp->token);
	while (temp != NULL)
	{
		expander->i = 0; //$$$$USER$$$USER$$USER$USE
		while (temp->token[expander->i] != '\0')
		{
			/* printf("i : %zu\n",expander->i); */
			if (quote(temp->token, expander))
				continue;
			if(temp->type == HEREDOC && temp->next !=NULL)
			{
				temp = temp->next;
				expander->i = ft_strlen(temp->token);
			}
			else if((temp->type >= 2 && temp->type <= 4) && temp->next != NULL)
			{
				if(temp->next->token[0] == '$')
				{
					temp = temp->next; //< $USER 
					if(!env_value(env_list,temp->token+1))
						expander->i = ft_strlen(temp->token);
					else
						continue;
				}
			}
			else if (temp->token[expander->i] == '$')
			{ //"adad$"
				expander->dollar_index = expander->i;
				expander->i++;
				if(temp->token[expander->i] == '\0' || temp->token[expander->i] == '\"' || temp->token[expander->i] == ' ')
					continue;
				if (special_character(temp->token, expander))
					continue;
				if (question_mark(temp, expander->i, expander))
					continue;
				if (change_to_env(temp, expander->i, expander, env_list))
					continue;
			}

			expander->i++;
			/* printf("i : %zu\n",expander->i);
			printf("token : %s\n",temp->token); */
		}
		temp = temp->next;
	}
}
