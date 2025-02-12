#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
/*
Функция CreateArray создает динамический массив для строк матрицы и заполняет его случайными числами в заданном диапазоне
int **ptr - двойной указатель на первый элемент строки входящего динамического массива 
size_t size - число, количество создаваемых столбцов динамической матрицы
int rangeA и int rangeB - границы значений случайных чисел
*/
void CreateArray(int **ptr, size_t size, int rangeA, int rangeB)
{
    if (ptr) // Проверка на то, не пуст ли массив 
	{
        *ptr = (int*)calloc(size, sizeof(int)); // Выделяем память на size элементов размером int
        if(ptr)
        {
		    if ( size && rangeA && rangeB ) // Проверка входящих значений для определения последующих действий
		    {
	    	    for (int i = 0; i < size; i++) // Цикл заполнит зозданный массив случайными числами в заданном диапазоне
		        {
		            *ptr[i] = rangeA + rand() % rangeB;
		        }
		    }
		    else printf("Error: bad digits imput\n"); // Если входящие значения не удовлетворяют решение, то выведем ошибку
        }
	}
}
/*
Функция displayArrayQ выводит входящую матрицу на экран
int** arr - двойной указатель на первый элемент входящей динамической матрицы
size_t sizeStr и size_t sizeCol - размеры матрицы; количество строк и колонок соответсвенно
*/
void displayArrayQ(int** arr, size_t sizeStr, size_t sizeCol)
{
if (arr) 
{
    if(sizeStr && sizeCol)
    {
        for ( int i = 0; i < sizeStr; i++) // Последующие два цикла пройдут по всей матрице и выведут ее элементы на экран
        {
            if(arr[i])
            {
                for (int j = 0; j < sizeCol; j++)
                {
                    printf("%d, ", arr[i][j]);
                }
            }
            else printf("NULL, ");
        }
    }
    else printf("Error: bad digits imput\n");
}
else printf("Array is empty\n");
}
/*
Функция FindCopyNumberCol проверяет есть ли во входящей колонке матрицы копия заданного элемента
int **arr - двойной указатель на первый элемент входящей динамической матрицы
int i и int h - индексы проверяемых строки и столбца
Выходящее значение - значение true/false в зависимости от того, нашлась ли копия элемента
*/
bool FindCopyNumberCol(int **arr, int i, int h)
{
    if (i >= 0 && h >= 0)
    {
        for( int j = i - 1; j >= 0; j--) // Цикл пройдет по столбцу начиная от i-1 элемента до самого первого и проверит колонку на копии
            {
                if (arr[i][h] == arr[j][h])
                    return true;
            }
        return false;
    }
    else printf("Error: bad digits imput\n");
}
/*
Основная функция main создает динамеческую матрицу, задает ее пределы значений и удаляет повторяющиеся элементы в ней
*/
void main()
{
  bool flag = false;
  int rangeA, rangeB;
  size_t sizeStr = 10;
  size_t sizeCol = 1;
  //Задаем пределы значений в динамической матрице
  printf("Enter minRange: ");
  scanf("%d", &rangeA);
  printf("Enter maxRange: ");
  scanf("%d", &rangeB);
  if (rangeB < rangeA) // Если оказалось так, что минимальное значение оказалось больше максимального, то за пределы берем 1 и 9
  {
      printf("Error: bad ranges. Return to sample digits [1;9]\n");
      rangeA = 1;
      rangeB = 9;
  }
  
  srand(time(NULL));
  
  int** dArray = (int**)calloc(sizeStr, sizeof(int*));
  if(dArray) 
    for ( int i = 0; i < sizeStr; i++) // Создаем строки динамической матрицы
        CreateArray(&dArray[i], sizeCol, rangeA, rangeB);
    else printf("Array is Empty, ERROR\n");
    
  printf("Origin array: ");
  if(dArray) displayArrayQ(dArray, sizeStr, sizeCol); // Выводим получившуюся матрицу на экран
        else printf("Array is Empty, ERROR\n");

  for ( int i = sizeStr - 1; i >= 0; i--) // Далее ищем копии элементов в столбце
  {
    for( int j = 0; j < sizeCol; j++) 
    {
        flag = FindCopyNumberCol( dArray, i, j);
        if(flag) // Если копия найдена, то освобождаем память, выделенную на этот элемент, и записываем в него значение NULL
        {
            free(dArray[i]);
	        dArray[i] = NULL;
        }
    }
  }
  
  printf("\nArray without copies: ");
  if(dArray) displayArrayQ(dArray, sizeStr, sizeCol); // Выводим матрицу без копий на экран
        else printf("Array is Empty, ERROR\n");
}
