//#include <stdio.h>
//#include <ctype.h>
//#include <string.h>
//#define _CRT_SECURE_NO_WARNINGS
//
//char* glas(char* str1, char* str2);
//
//int main(void)
//{
//	char s1[100], s2[100], * stroka;
//	puts("vvedite stroku 1");
//	gets(s1);
//	puts("vvedite stroku 2");
//	gets(s2);
//
//	stroka = glas(s1, s2);
//
//	int q = 3;
//	int p = 4;
//
//	putchar('\n');
//	printf("ITOGOVAYA stroka = %s \n\n\n", stroka);
//
//	return 0;
//}
//
//char* glas(char* str1, char* str2)
//{
//	int i, end;
//	putchar('\n');
//	printf("Per: %s\n", str1);
//	printf("Vtor: %s\n", str2);
//	end = strlen(str1);
//	for (i = 0; i < end; i++)
//		str1[i] = toupper(str1[i]);
//	return str1;
//}



//#include < stdio.h >  // Для printf
//#include < string.h > // Для strtok
//int main(void)
//{
//	// Массив со строкой для поиска
//	char str[26] = "test1/test2///test3/test4";
//	// Набор символов, которые должны входить в искомый сегмент
//	char sep[10] = "/";
//	// Переменная, в которую будут заноситься начальные адреса частей
//	// строки str
//	char* istr;
//
//	printf("Исходная строка: %s\n", str);
//	printf("Результат разбиения :\n");
//	// Выделение первой части строки
//	istr = strtok(str, sep);
//
//	// Выделение последующих частей
//	while (istr != NULL)
//	{
//		// Вывод очередной выделенной части
//		printf("%s\n", istr);
//		// Выделение очередной части строки
//		istr = strtok(NULL, sep);
//	}
//
//	return 0;
//}