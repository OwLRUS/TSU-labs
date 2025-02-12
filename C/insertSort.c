#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
/*
Функция CreateArr: заполняет динамический массив случайными элементами в заданном диапазоне
*/
void CreateArr(int **Arr, const int N, int minRange, int maxRange)
{
   *Arr = (int*)malloc( 10 * sizeof *Arr);
   if (Arr)
   {
        for (int i = 0; i < N; i++) 
        {
            (*Arr)[i]= minRange + (rand() % (maxRange - minRange + 1));
            printf("%d ", (*Arr)[i]);
        }
   }
}
/*
Функция SortingArr: сортирует массив методом прямой вставки
*/
void SortingArr (int *Arr, int N)
{
    int j, x;
    for (int i = 1; i < N; i++) // Цикл будет двигаться слева направо, начиная со второго элемента
    {
        j = i - 1;
        x = Arr[i]; // Запоминаем элемент, находящийся в каждом проходе цикла
        for ( j = i - 1; Arr[j] > x && j >= 0; j--) // Цикл будет двигаться налево, пока не проверит первый элемент в массиве
            Arr[j + 1] = Arr[j]; // Если оказалось так, что предыдущий элемент оказался больше последующего, то в последующий записываем предыдущий
        Arr[j + 1] = x;
    }
}
/*
Функция CheckArr:
1 . Проверяет упорядоченность массива ( упорядочен по возрастанию, элементы друг другу равны, не упорядочен) 
2 . Выводит соответствующую строку на экран
*/
int CheckArr( int *Arr, int N)
{
    bool AscendingOrder = true;
    bool Equal = true;
    for ( int i = 0; i < (N - 1) && AscendingOrder; i++)
        if ( Arr[i] >= Arr[i + 1]) AscendingOrder = false;
    for ( int i = 0; i < (N - 1) && Equal; i++)    
        if ( Arr[i] != Arr[i + 1]) Equal = false;
    if (AscendingOrder) return 1;
        else if (Equal) return 2;
            else return 0;
}
/*
Основная функция main:
1 . Выделяет память на динамический массив
2 . Вводит через консоль минимальное и максимальное значения элементов в массиве
3 . Проверяет не ошибся ли в пользователь при вводе значений так, что минимальное больше максимального
4 . Проверяет была ли вызвана ошибка и при надобности выводит соответствующее сообщение на экран и заканчивает программу
5 . Вызывает функцию CreateArr
6 . Выводит полученный массив на экран
7 . Вызывает функцию SortingArr
8 . Ведет отсчет времени, затраченное на сортировку массива
9 . Выводит отсортированный массив на экран
10. Выводит количество секунд, затраченных на сортировку
11. Вызывает функцию CheckArr
12. Освобождает память выделенную на создание динамического массива
*/
int main()
{
   bool Error = false;
   const int N = 10;
   int *Arr = NULL;
   int minRange;
   int maxRange;
   printf("Enter the minimum value of the elements in the array: ");
   scanf("%d", &minRange);
   printf("Now enter the maximum: ");
   scanf("%d", &maxRange);
   if (minRange > maxRange)
   {
       Error = true;
   }
   if (Error)
   {
       printf("Error: bad digits imput");
       return 0;
   }
   srand(time(NULL));
   printf("Origin array is: ");
   CreateArr(&Arr, N, minRange, maxRange);
   clock_t start = clock();
   SortingArr(Arr, N);
   printf("\nSorted array is: ");
    for ( int i = 0; i < N; i++) printf("%d ", Arr[i]);
   clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nThe time of sorting is: %f seconds \n", seconds);
   int flag = CheckArr(Arr, N);
   if (flag == 1) printf("The array is ordered in ascending order");
        else if(flag == 2) printf("Array elements are equal to each other");
            else printf("The array is not ordered");
   free(Arr);
   return 0;
}
