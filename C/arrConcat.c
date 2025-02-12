#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main ()
{
int minRange; // Переменная, которая задаст минимальное значение случайного числа 
  scanf ("%d", &minRange);
  int maxRange; // Переменная, которая задаст максимальное значение случайного числа
  scanf("%d", &maxRange); 
 const int n = 5;
  const int m = 5;
  int Array_A[5] ;
srand(time(NULL));
  int i = 0;
   for ( i = 0; i < n; i++) // Цикл заполнит массив случайными числами в заданном диапазоне
        Array_A[i]= minRange + (rand() % (maxRange - minRange + 1));
  int Array_B[5] ;
for ( i = 0; i < m; i++) // Цикл заполнит массив случайными числами в заданном диапазоне
        Array_B[i]= minRange + (rand() % (maxRange - minRange + 1));
  int Array_C[10] = { 0 };
  int Count = 0; // Счетчик, который является индексом массива Array_C
  int i = 0;
  int j = 0;
  int t = 0; // Переменная понадобится для перестановки элементов в массиве
  for (i = 0; i < n; i++) // Два следующих цикла упорядочат массив Array_A по возрастанию методом выбора
    {
        for(j = i + 1; j < n; j++)
        if (Array_A[i] > Array_A[j])
        {
            t = Array_A[i];
            Array_A[i] = Array_A[j];
            Array_A[j] = t;
        }
    }
  i = 0;
  for (i = 0; i < m; i++) // Два следующих цикла упорядочат массив Array_B по возрастанию методом выбора
    {
        for(j = i + 1; j < m; j++)
        if (Array_B[i] > Array_B[j])
        {
            t = Array_B[i];
            Array_B[i] = Array_B[j];
            Array_B[j] = t;
        }
    }
  for ( i = 0; i < n; i++) // Цикл выведет элементы массива на экран
    printf("%d ", Array_A[i]);
    printf(" \n"); // Использую эту строчку для разделения выводимых данных
  for ( i = 0; i < n; i++) // Цикл выведет элементы массива на экран
    printf("%d ", Array_B[i]);
    printf(" \n");  // Использую эту строчку для разделения выводимых данных
 i = 0;
  j = 0;
  while (i != n && j != m) // Цикл составит упорядоченный массив Array_C, пока не закончится хотя бы один из массивов
    {
      if (Array_A[i] < Array_B[j]) // Условие выбирает элементы из двух массивов и ставит наименьший из элементов в ячейку массива Array_C
	{
	  Array_C[Count] = Array_A[i];
	  i++;
	  printf ("%d, ", Array_C[Count]);
	}
      else
	{
	  Array_C[Count] = Array_B[j];
	  j++;
	  printf ("%d, ", Array_C[Count]);
	}
      Count++; // Меняем индекс ячейки, чтобы вставить следующий элемент
    }
  if (j != m) // Условие проверяет закончился ли массив Array_B, если нет, то дополняет массив Array_C оставшимися элементами
    {
      while (j != m)
	{
	  Array_C[Count] = Array_B[j];
	  j++;
	  printf ("%d, ", Array_C[Count]);
	  Count++;
	}
    }
  if (i != n) // Условие проверяет закончился ли массив Array_A, если нет, то дополняет массив Array_C оставшимися элементами

    {
      while (i != n)
	{
	  Array_C[Count] = Array_A[i];
	  i++;
	  printf ("%d, ", Array_C[Count]);
	  Count++;
	}
    }
}
