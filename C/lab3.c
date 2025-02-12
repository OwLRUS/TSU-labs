#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
/*
Функция SortingArrChar сортирует входящий массив слов по алфавиту
char **Arr - Матрица, строки которой являются словами 
size_t N - Количество строк(слов) в матрице
*/
void SortingArrChar (char **Arr, size_t N)
{
  if(Arr && N) // Проверка входных значений 
  {
    size_t LeftBorder = 0;
    size_t RightBorder = N - 1; // Левая и правая границы сортируемой области массива
    int i;
    char* trade;
    bool flag = true;  // Флаг наличия перемещений
    // Цикл будет выполняться пока левая граница не сомкнётся с правой и пока в массиве имеются перемещения
    while ((LeftBorder < RightBorder) && flag)
    {
        flag = false; // За каждый проход цикла влаг должен менять значение
        for (i = LeftBorder; i<RightBorder; i++)  // Цикл осуществит движение слева направо
        {
            if (strcmp(Arr[i], Arr[i + 1]) > 0) // Если следующий элемент меньше текущего, меняем их местами
            {             
                /*strcpy(trade, Arr[i]);
                strcpy(Arr[i], Arr[i + 1]);
                strcpy(Arr[i + 1], trade);*/
                trade = Arr[i];
                Arr[i] = Arr[i + 1];
                Arr[i + 1] = trade;
                flag = true;      // Знак того, что перемещения были
            }
        }
        RightBorder--; // Сдвигаем правую границу на один элемент влево
        for (i = RightBorder; i>LeftBorder; i--)  // Цикл уже будет двигаться справа налево
        {
            if (strcmp(Arr[i - 1], Arr[i]) > 0) // Если предыдущий элемент больше текущего, меняем их местами
            {             
            /*strcpy(trade, Arr[i]);
            strcpy(Arr[i], Arr[i - 1]);
            strcpy(Arr[i - 1], trade);*/
            trade = Arr[i];
            Arr[i] = Arr[i - 1];
            Arr[i - 1] = trade;
            flag = true;    // Знак того, что перемещения были
            }
        }
        LeftBorder++; // Сдвигаем левую границу на один элемент вправо
    }
  }
  else if(!Arr) printf("Array is empty. Error\n"); // Если входные значения не устраивают решение, то выведем соотвестствующее сообщение
        else printf("Error: Bad digits imput\n");
}
/*
Функция CreateMx создает динамическую матрицу по входящим значениям
char ***ptr - Тройной указатель, в котором будет создаваться динамический массив 
size_t sizeC - количество(размер) колонок в массиве
size_t sizeS - количество строк в массиве 
*/
void CreateMx (char ***ptr, size_t sizeC, size_t sizeS)
{
    srand(time(NULL));
    if(ptr) 
    {
        if(sizeS && sizeC) // Проверка входных значений 
        {
            *ptr = (char**)calloc((sizeS + 1), sizeof(char*)); // Создаем строки динамической матрицы
            if(ptr) // Проверка на правильное выделение памяти на динамический массив
            {
                for( int i = 0; i < sizeS; i++)
                {
                    (*ptr)[i] = (char*)calloc((sizeC + 1), sizeof(char)); // Создаем столбцы динамической матрицы
                    if(ptr) // Проверка на правильное выделение памяти на динамический массив
                    {
                        for( int j = 0; j < sizeC; j++) // С помощью цикла заполняем матрицу случайными буквами латинского алфавита 
                        {
                            (*ptr)[i][j] = 'a' + rand()%26;
                        }
                        (*ptr)[i][sizeC] = '\0';
                    }
                    else printf("Exeption: Bad allocation\n");
                }
            }
            else printf("Exeption: Bad allocation\n");
        }
        else printf("Error: Bad digits imput\n");
    }
    else printf("Array is empty. Error\n");
}
/*
Функция PrintStrChar поочередно выводит на экран строки входящего массива
char **arr - двойной указатель на входящую матрицу
size_t N - количество строк в матрице
*/
void PrintStrChar( char **arr, size_t N)
{
    if(arr)
    {
        if(N) // Проверка входных значений
        {
            for( int i = 0; i < N; i++) // Цикл выведет на экран по очереди строки матрицы
            {
                printf("%s, ", arr[i]);
            }
            printf("\n");
        }
        else printf("Error: Bad digits imput\n");
    }
    else printf("Array is empty. Error\n");
}
/*
Основная функция main задает размеры матрицы, создает двойной указатель для нее и вызывает исполняющие функции
*/
void main()
{
    size_t count = 5; // Количество слов в матрице (строки)
    size_t symbols = 8; // Количество букв в строке (колонки)
    char** dMx= NULL; // Задаем двойной указатель для матрицы
    CreateMx(&dMx, symbols, count);
    PrintStrChar(dMx, count);
    SortingArrChar(dMx, count);
    PrintStrChar(dMx, count);
}
