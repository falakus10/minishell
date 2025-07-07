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
char *ft_strjoin_free(char *env_val, char *token, const char *key)
{
    size_t i;
	size_t j;
	size_t k;
    size_t key_len;
    size_t val_len;
    size_t new_len;

	i = 0;
	j = 0;
	k = 0;
	key_len = ft_strlen(key);
	val_len = ft_strlen(env_val);

    if (!token || !env_val)
        return (NULL);

    new_len = ft_strlen(token) - (key_len + 1) + val_len; /* Yeni uzunluk = eski – ($+KEY) + VALUE */
    char *new_token = malloc(new_len + 1); /* +1 ⇒ '\0' */

    if (!new_token)
    {
        free(token);
        free(env_val);
        return (NULL);
    }

	while (token[i] && token[i] != '$')  /* '$' işaretine kadar kopyala */
		new_token[k++] = token[i++];
	if (token[i] == '$') /* '$' + KEY yerine VALUE kopyala */
    {
        i++; /* '$' atla */
		while (j < val_len) /* VALUE kopyası */
			new_token[k++] = env_val[j++];
        i += key_len; /* KEY'i atla */
    }

	while (token[i]) /* Geri kalan SUFFIX'i kopyala */
		new_token[k++] = token[i++];

    new_token[k] = '\0';
    free(token); /* eski token */
    free(env_val); /* env_val malloc'lı idi → serbest */
    return (new_token);
}

void expander(t_lexer_list *list, char **env)
{
    t_lexer_list	*temp;
    size_t i;
	size_t start;
	char *env_key;
	char *env_val;
	char ch;

    temp = list;
    i = 0;
	start = 0;
    while (temp != NULL) // her bir node 'u geziyoruz, tokenlarını kontrol ediyoruz
    {	
		i = 0;
        while(temp->token[i] != '\0') //burda eski tokenla başlıyorum aşağıda aynı token ı freeliyorum sorun olabilir !! //tırnak görünce tırnağı atlasın. Çift tırnak içindeki expand durumunu ayrı bir yerde işleyeceğim.
		{
			if (temp->token[i] == '$') //$1SER kısmını da ayrı kontrol edicem
			{
				i++;
				start = i; // $ işaretinden sonraki karakter başlangıç indexi olacak
				while(temp->token[i] != '\0' && (is_valid_ch(temp,i)))  //inputun sonuna gelmediğimiz sürece ve geçerli bir karakter olduğu sürece devam
					i++;  //Variable'ın key'inin uzunluğunu alıyoruz
				env_key = ft_substr(temp->token, start, i - start); // $ işaretinden sonraki karakterden başlayarak geçerli karaktere kadar olan kısmı alır
				env_val = env_value(env, env_key); //env'de bu key var mı diye bakar ve varsa değerini alır
				temp->token = ft_strjoin_free(env_val, temp->token, env_key);//env_value'den dönen değeri token'in bulunduğu yere koyar
				free(env_key);
				i = 0;
				continue; // $ işaretinden sonra gelen karakteri genişlettik, token'a yazdık devam ediyoruz
			}
			if (temp->token[i] == '\'' || temp->token[i] == '\"') //eğer tırnak varsa tırnağı atla, tırnak içini ayrıca kontrol edicez
			{
				ch = temp->token[i];
				i++;
				while(temp->token[i] != '\0' && temp->token[i] != ch) //tırnak kapatılana kadar devam et
					i++;
				if (temp->token[i] != '\0') //eğer tırnak kapatıldıysa tırnağı atla // '\"' \" durumlarını kontrol etmedim
					i++;
				continue;
			}
			if(temp->token[i] != '$' && temp->token[i] != '\'' && temp->token[i] != '\"') //eğer $ işareti, tırnak veya çift tırnak değilse
				i++;
		}
		temp = temp->next; //sonraki token'e geç
    }
}