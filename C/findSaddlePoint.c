#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int minRange; // Переменная, которая задаст минимальное значение случайного числа 
    scanf ("%d", &minRange);
    int maxRange; // Переменная, которая задаст максимальное значение случайного числа
    scanf("%d", &maxRange);
    printf("minRange = %d, maxRange = %d \n", minRange, maxRange); // Вывожу значения прошлых переменных для удобства
    const int n = 3;
    const int m = 5;
    int array[3][5];
    srand(time(NULL));
    int i = 0;
    int j = 0;
    printf("Array:\n");
    for( i = 0; i < n; i++) // Далее вложенный цикл заполнит массив и выведет его на экран
    {
        for(j = 0; j < m; j++)
        {
            array[i][j] = minRange + (rand() % (maxRange - minRange + 1));
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
    i = 0;
    j = 0;
    int min = 0;
    int max = 0;
    int index_i = 0;
    int index_j = 0;
    int k = 0;
    while (i != n) // Основной цикл, который пройдет по строчкам массива
    {
        j = 0;
        min = array[i][j]; 
        max = array[i][j];
        while(j != m) // Данный цикл найдет минимальное и максимальное значения в строке
        {
            if(min > array[i][j]) min = array[i][j];
            if(max < array[i][j]) max = array[i][j];
            j++;
        }
        j = 0;
        while (j != m) // Далее цикл пройдется по строке еще раз и, находя минимальные и максимальные значения, проверит являются ли они соответственно максимальными и минимальными в столбце
        {
            k = 0;
            if( min == array[i][j])
            {
                index_j = j;
                index_i = i;
                while ( k != n && min >= array[k][j]) // Цикл подсчитывает количество сделанных им оборотов, удовлетворяя условие, по которому он проверяет является ли минимум в строке максимумом в столбце
                {
                    k++;
                }
                if (k == n) printf(" Index = (%d,%d) \n", index_i, index_j); // Если количество оборотов прошлого цикла оказалось равными числу строк, то данный минимум седловая точка
            }
            k = 0;
            if( max == array[i][j]) // Далее условие аналогично прошлому, но проверяет максимум
            {
                index_j = j;
                index_i = i;
                while ( k != n && max <= array[k][j])
                {
                    k++;
                }
                if (k == n) printf(" Index = (%d,%d) \n", index_i, index_j);
            }
            j++;
        }
        i++;
    }
}
