#include "minishell.h"

int is_valid_ch(t_lexer_list *list, int i) //$ işaretinden sonraki karakter geçerli bir karakter mi kontrol eder
{
	t_lexer_list *temp;
	temp = list;
	if (ft_isalnum(temp->token[i]) || temp->token[i] == '_')
		return (1);
	return (0);
}

char *env_value(char **env, const char *key)
{
	size_t	key_len;
	int		i;
	char	*env_value;

	if (env == NULL || key == NULL)
		return (0);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			env_value = ft_substr(env[i], key_len + 1, ft_strlen(env[i]) - (key_len + 1));                         
			return (env_value);
		}
		i++;
	}
	return (ft_strdup(""));                                
}

/* token içinde İLK görülen $KEY‘i env_val ile değiştirir
 * – key  : sadece KEY  ( '$' yok )
 * – env_val, token  : malloc’lı  (fonksiyon sonunda serbest bırakılır)
 * Sonuç : malloc’lı yeni string  (çağıran free edecektir)
 */
int find_index(const char *haystack, const char *needle)
{
	char *ptr;

	ptr = ft_strnstr(haystack, needle, ft_strlen(haystack));
	if (ptr == NULL)
		return (-1);
	return (ptr - haystack);  // pointer farkı → index
}

char *ft_strjoin_free(char *env_val, char *token, const char *key)
{
	int	i;
	int	j;
	int k;
	int	new_len;
	char *new_token;

	i = 0;
	j = 0;
	k = 0;
	if (!token || !env_val)
		return (NULL);
	new_len = ft_strlen(token) - (ft_strlen(key) + 1) + ft_strlen(env_val);
	new_token = malloc(new_len + 1);
	if (!new_token)
	{
		free(token);
		free(env_val);
		return (NULL);
	}
	i = find_index(token, key);
	if (i == (-1))
		return (NULL);
	if (token[i - 1] == '$')
	{
		while ((i - 1) > j)
		{
			new_token[j] = token[j];
			j++;
		}
		while(env_val[k])
			new_token[j++] = env_val[k++];
		i = i + ft_strlen(key);
		while (token[i])
			new_token[j++] = token[i++];
		new_token [j++] = '\0';
	}
	else
	{
		i = find_index(token + i, key);
		if (i == (-1))
			return (NULL);
		while ((i - 1) > j)
		{
			new_token[j] = token[j];
			j++;
		}
		while(env_val[k])
			new_token[j++] = env_val[k++];
		i = i + ft_strlen(key);
		while (token[i])
			new_token[j++] = token[i++];
		new_token [j++] = '\0';
	}
	return (new_token);
}

int special_ch_check(char c)
{
	if (c >= '1' && c <= '9')
		return (1);
	else if (c == '#')
		return (1);
	else if (c == '*')
		return (1);
	else if (c == '@')
		return (1);
	else if (c == '-')
		return (1);
	else if (c == '!')
		return (1);
	else
		return (0);
}

void expander(t_lexer_list *list, char **env)
{
	t_lexer_list	*temp;
	size_t	i;
	size_t	start;
	char	*env_key;
	char	*env_val;
	char	ch;

	temp = list;
	i = 0;
	start = 0;
	while (temp != NULL) // her bir node 'u geziyoruz, tokenlarını kontrol ediyoruz
	{	
		i = 0;
		while(temp->token[i] != '\0') //burda eski tokenla başlıyorum aşağıda aynı token ı freeliyorum sorun olabilir !! //tırnak görünce tırnağı atlasın. Çift tırnak içindeki expand durumunu ayrı bir yerde işleyeceğim.
		{
			if (temp->token[i] == '$') //Literalde $1SER kısmını da kontrol ediyoruz
			{
				i++; //$?
				if (special_ch_check(temp->token[i]) == 1)
				{
					while (special_ch_check(temp->token[i]))
						i++;
					while(temp->token[i] != '\0' && (is_valid_ch(temp,i))) 
						i++;
					continue;
				}			//U
				start = i; // $ işaretinden sonraki karakter başlangıç indexi olacak
				while(temp->token[i] != '\0' && (is_valid_ch(temp,i)))  //inputun sonuna gelmediğimiz sürece ve geçerli bir karakter olduğu sürece devam
					i++;  //Variable'ın key'inin uzunluğunu alıyoruz
				env_key = ft_substr(temp->token, start, i - start); // $ işaretinden sonraki karakterden başlayarak geçerli karaktere kadar olan kısmı alır
				env_val = env_value(env, env_key); //env'de bu key var mı diye bakar ve varsa değerini alır
				if (temp->token[0] == '$' && temp->token[1] == '\0')
				{
					temp->token = malloc(sizeof(char) * 2); // Eğer sadece $ varsa, token'ı sadece $ olarak ayarla
					temp->token[0] = '$';
					temp->token[1] = '\0';
					free(env_key);
					free(env_val);
					if(temp->next != NULL)
						temp = temp->next; // Eğer sadece $ varsa, sonraki token'a geç
					else
						return; // Eğer sadece $ varsa, son token ise fonksiyondan çık //listenin son elemanı demek , return demek doğru mu ?? 
					continue; // Eğer sadece $ varsa, token'ı sadece $ olarak ayarla ve sonraki token'a geç
				}
				temp->token = ft_strjoin_free(env_val, temp->token, env_key);//env_value'den dönen değeri token'in bulunduğu yere koyar
				free(env_key);
				i = 0;
				continue; // $ işaretinden sonra gelen karakteri genişlettik, token'a yazdık devam ediyoruz
			}
			if (temp->token[i] == '\'') //eğer tırnak varsa tırnağı atla, tırnak içini ayrıca kontrol edicez //else if mi 
			{
				ch = temp->token[i];
				i++;
				while(temp->token[i] != '\0' && temp->token[i] != ch) //tırnak kapatılana kadar devam et
					i++;
				if (temp->token[i] != '\0') //eğer tırnak kapatıldıysa tırnağı atla // '\"' \" durumlarını kontrol etmedim
					i++;
				continue;
			}
			if (temp->token[i] == '\"')    // Çift tırnak içi genişletiliyor. //Çift tırnak içinde $1SER kısmını da kontrol edicez
			{
				i++;
				while (temp->token[i] != '\0' && temp->token[i] != '\"')
				{
					if(temp->token[i] == '$')
					{
						i++;
						start = i; 
						while(temp->token[i] != '\0' && (is_valid_ch(temp,i)))
							i++;
						env_key = ft_substr(temp->token, start, i - start);
						env_val = env_value(env, env_key);
						temp->token = ft_strjoin_free(env_val, temp->token, env_key);
						free(env_key);
						i = 1;
					}
					i++;
				}
			}
			if(temp->token[i] != '$' && temp->token[i] != '\'' && temp->token[i] != '\"') //eğer $ işareti, tırnak veya çift tırnak değilse
				i++;
		}
		temp = temp->next; //sonraki token'e geç
	}
}
