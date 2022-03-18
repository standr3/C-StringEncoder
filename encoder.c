#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strrev(char *str)
{
	char *p1, *p2;

	if (! str || ! *str)
		return str;

	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}

char **read(int *nr_term, int *n)
{
	char **v;
	int nr_aloc = 1;
	char term[55];

	v = (char **)malloc(3 * nr_aloc * sizeof(char *));

	int i;
	fgets(term, 80, stdin);
	term[strlen(term) - 1] = '\0';//adaugam terminatorul de sir pe care fgets nu il adauga singur
	v[0] = strdup(term);
	i = 1;
	while (strcmp(v[i - 1], "END"))
	{
		if (i == 3 * nr_aloc - 1)
			{
			nr_aloc++;
			v = (char **)realloc(v, 3 * nr_aloc * sizeof(char *));
			}//daca ramane fara spatiu realocam memorie
			fgets(term, 80, stdin);
			term[strlen(term) - 1] = '\0';
		v[i] = strdup(term);
		i++;
	}

	*nr_term = i - 2;
	sscanf(v[i-2],"%d",n);
	return v;
}
int type(char t[])
{
	int i,nr,ch;
	nr=1;ch=0;//presupunem ca initial intrarea este numar si nu este caracter
	if(strlen(t)==1 && (t[0]>'9' || t[0]<'0'))
	{
		ch=1;//daca sirul are un singur caracter si este diferit de cifre atunci acesta este caracter
	}
	if(ch)
		return 0;//caracter
	if(t[0]=='-')//verificam daca e numar negativ
	{
		if(strlen(t)==1 || t[1]=='0')
			nr=0;//daca este de forma "-0*" atunci nu e numar
		if(!(t[1]>='1' && t[1]<='9'))
			nr=0;//daca dupa '-' nu apare macar o cifra intre 1 si 9 atunci e numar
		for(i=2;i<strlen(t);i++)
			if(!(t[i]>='0' && t[i]<='9'))
				nr=0;//daca in restul sirului testat apare pe o pozitie altceva decat o cifra atunci nu e numar
	}
	else//daca e pozitiv
	{
		if(!(t[0]>='1' && t[0]<='9'))
				nr=0;//daca pe prima pozitie gasim altceva decat o cifra intre 1 si 9 atunci nu e numar
		for(i=1;i<strlen(t);i++)
			if(!(t[i]>='0' && t[i]<='9'))
				nr=0;//daca in restul sirului testat apare pe o pozitie altceva decat o cifra atunci nu e numar
	}
	for(i=1;i<strlen(t);i++)
		if(!(t[i]>='0' && t[i]<='9'))
			nr=nr*0;
	if(nr)
		return 1;//numar
	return 2; //daca a trecut de cele doua if-uri anterioare (nr == 0 && ch ==0) atunci t este cuvant
}
void type_nr(char **v, int nrterm)
{
	int ch,w,nr,i;
	ch=0;w=0;nr=0;
	for (i = 0;i<nrterm; i++) //numaram fiecare tip de termen 
	{
		if (type(v[i]) == 0)
			ch++;//crestem numarul de caractere
		if (type(v[i]) == 1)
			nr++;//crestem numarul de numere
		if (type(v[i]) == 2)
			w++;//cresetem numarul de cuvinte
	}
	printf("%d %d %d",w,ch,nr);
}

char* process_word (char* s, char* t)
{
	int n,i,l,d;
	char* newstr_word;
	l=strlen(t);
	newstr_word=(char*)malloc((l+2)*sizeof(char));
	n=1;//pp ca termenul dat nu are cifre
	for(i=0;i<l;i++)
		if((t[i]>='0' && t[i]<='9')&&n==1)
			n=0;//n primeste 0 daca cuvantul are macar o cifra
	for(i=1;i<=l/2;i++)
		if(l%i==0)
			d=i;
	if(n)//in cazul in care cuvantul nu are cifre
	{
		strncpy(newstr_word,t+d,l-d);//copiem restul in newstr_word
	newstr_word[l-d] = '\0';
	}
	else//in cazul in care cuvantul are cifre
	{
		strcpy(newstr_word,t+d);
		strrev(newstr_word);
	}
	strncpy(newstr_word+(l-d),t,d);//mutam inceputul cuvantului la sfarsitul acestuia
	newstr_word[l]='\0';
	s = (char *)realloc(s,(strlen(s) + strlen(newstr_word) + 3) * sizeof(char));//realocam
	strcat(s, newstr_word);
	free(newstr_word);
	return s;
}

char getmin(char *s)
{
	char ch, searched_ch;
	int occurrence, min_occurrence = -1;
	int i, j;

	for (i = 0; i < strlen(s); i++)//cautam fiecare caracter din sir si vedem de cate ori apare
	{
		ch = s[i];
		occurrence = 0;
		for (j = 0; j < strlen(s); j++)
			if (s[j] == s[i])
				occurrence++;
		if ((occurrence < min_occurrence )|| (min_occurrence == -1)) 
		{
			searched_ch = ch;
			min_occurrence = occurrence;
		}
	}
	return searched_ch;//se returneaza primul caracter din sir care apare de cel mai putine ori in acesta
}
char getmax(char *s)
{
	char ch, searched_ch;
	int max_occurrence , occurrence, i, j;
	max_occurrence = 0;
	for (i = 0; i < strlen(s); i++)//cautam fiecare caracter din sir si vedem de cate ori apare
	{
		ch = s[i];
		occurrence = 0;
		for (j = 0; j < strlen(s); j++)
			if (s[j] == s[i])
				occurrence++;
		if (occurrence > max_occurrence)
		{
			searched_ch = ch;
			max_occurrence = occurrence;
		}
	}
	return searched_ch;//se returneaza primul caracter din sir care apare de cel mai multe ori in acesta
}

char* process_char (char* s,char* t)
{
	if (strlen(s))//daca termenul dat nu este primul introdus de la tastatura
	{
		char chmin, chmax;
		chmin = getmin(s);
		chmax = getmax(s);
		char newstr_ch[3];
		newstr_ch[0] = chmax;
		newstr_ch[1] = t[0];
		newstr_ch[2] = chmin;
		newstr_ch[3] = '\0';
		s =(char *)realloc(s, (strlen(s) + 4) * sizeof(char));
		strcat(s, newstr_ch);
	}
	else//daca termenul dat este primul introdus de la tastatura
	{
		s =(char *)realloc(s, (strlen(s) + 2) * sizeof(char));
		strcat(s, t);
	}
	return s;
}

char *process_nr (char* s, char *t)
{
	char* newstr_nr;
	char* aux;
	int i, l;												
	l=strlen(t);
	int factor = -1;//daca numarul este pozitiv
	aux=(char*)malloc((l+5)*sizeof(char));//alocam spatiu pt aux
	strcpy(aux,t);//prelucram aux si lasam termenul neschimbat
	if(aux[0]=='-')//daca numarul este negativ
	{
		strcpy(aux,aux+1);
		factor = 1;
	}
	int aux_l=strlen(aux);
	int min;
	sscanf(aux,"%d",&min);
	i=0;
	newstr_nr=(char*)malloc((l+5)*sizeof(char));
	strcpy(newstr_nr,aux);
	while(i<aux_l-1)
	{
		char c;
		c=aux[0];
		strcpy(aux,aux+1);
		aux[aux_l-1]=c;
		aux[aux_l]='\0';
		int a;
		sscanf(aux,"%d",&a);
		a=a*factor; //pentru numere pozitive calculam minimul numerelor opuse
		if(a<min)
		{
			min=a;
			strcpy(newstr_nr,aux);
		}
		i++;
	}


	s = (char *)realloc(s,(strlen(s) + strlen(newstr_nr)+5) * sizeof(char));//realocam
	strcat(s, newstr_nr);
	free(newstr_nr);//eliberam memorie
	free(aux);//eliberam memorie
	return s;
}

char *process(char **v, int nr_termeni, char *s)
{
	int i;
	for (i = 0; i < nr_termeni; i++)
	{
		if (type(v[i]) == 0)//daca termenul e caracter si adauga varianta lui prelucrata la s
		{
			s = process_char(s, v[i]);
		}
		if (type(v[i]) == 1)//daca termenul e numar si adauga varianta lui prelucrata la s
		{
			s = process_nr(s, v[i]);
		}
		if (type(v[i]) == 2)//daca termenul e cuvant si adauga varianta lui prelucrata la s
		{
		s = process_word(s, v[i]);
		}
	}
	return s;//s este varianta prelucrata a termenilor introdusi de la tastatura
}

typedef struct 
{
	char* content;
	float complexity;
}component;

void str_to_components (char* p_msg, int n, component* d_msg)
{
	//p_msg este mesajul procesat iar d_msg este mesajul descompus
	int l, i, l_c;
	//l este lungimea mesajului procesat iar l_c este lungimea mesajului descompus
	l = strlen(p_msg);
	l_c = l / n;
	for (i = 0; i < (n - 1); i++)//copiem in vector continutul componentelor pline
		d_msg[i].content = strndup(p_msg + (i * l_c), l_c);
	d_msg[n-1].content = strdup(p_msg + (n-1)*l_c);//separat copiem continutul ultimei componente care poate sa fie incompleta
}

void calculate_complexity (int n, component* d_msg)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		//parcurgem vectorul de structuri din n in n elemente
		float sum;
		for (j = 0; j < strlen(d_msg[i].content); j++)
		{
			int aux;
			aux = (int)d_msg[i].content[j];
			sum = sum + aux;
		}
		d_msg[i].complexity = sum / strlen(d_msg[i].content);
	}
}

void sort(component d_pr_str[], int n)
{	//folosesc algoritmul clasic de sortare prin interclasare
	component aux;
	int i, j;
	for (i = 0; i < n - 1; i++)
		for (j = i + 1; j < n; j++)
			if (d_pr_str[i].complexity < d_pr_str[j].complexity) 
			{
				aux = d_pr_str[i];
				d_pr_str[i] = d_pr_str[j];
				d_pr_str[j] = aux;
			}
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) 
		{
			if (d_pr_str[j].complexity != d_pr_str[i].complexity)
				break;
			if (strcmp(d_pr_str[i].content, d_pr_str[j].content) < 0) //se decide ordinea lexicografica
			{
				aux = d_pr_str[i];
				d_pr_str[i] = d_pr_str[j];
				d_pr_str[j] = aux;
			}
		}
}

void resort(component d_pr_str[], int n, int l_r)
{

	char *new_msg;
	new_msg = (char *)malloc(l_r * sizeof(char));
	new_msg[0] = '\0';

	int st, dr;
	st = 0;
	dr = n - 1;

	while ((dr - st) > 0)
	{
		new_msg = strcat(new_msg, d_pr_str[st].content);
		new_msg = strcat(new_msg, d_pr_str[dr].content);
		st++;
		dr--;
	}
	if(dr == st)
		new_msg = strcat(new_msg, d_pr_str[st].content);
	puts(new_msg);

	free(new_msg);
	int i;
	for (i = 0; i < n; i++)
		free(d_pr_str[i].content);
}

int main()
{
	char** input;
	int n, str_nr, i;
	component d_pr_str[50];
	input=read(&str_nr,&n);
	type_nr(input,str_nr);
	char *pr_str;
	pr_str = (char *)malloc(sizeof(char));
	pr_str[0] = '\0';
	//urmeaza afisarea textului codificat
	pr_str = process(input, str_nr, pr_str);
	printf("\n");
	puts(pr_str);
	for(i=0;i<(str_nr+2);i++)
		free(input[i]);
	//eliberam memorie
	str_to_components(pr_str, n, d_pr_str);//transformam din string in vector de struct
	calculate_complexity(n, d_pr_str);//calculam complexitatea

	sort(d_pr_str, n);//sortam
	int l_r = strlen(pr_str) + 1;
	resort(d_pr_str, n, l_r);//rearanjam
	//eliberarea memoriei
	free(input);
	free(pr_str);

	return 0;
}
