#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct Drob
{
    int chislitel;
    int znamenatel;
};
/*
Функция CreateDmasDrob создает динамический массив дробей, со случайными значениями в заданном диапозоне
int count - количество дробей
int minRange - минимальное значение в числителе/знаменателе
int maxRange - максимальное значение в числителе/знаменателе
Выходящее значение - динамический массив дробей
*/
struct Drob *CreateDmasDrob (int count, int minRange, int maxRange)
{
    // Три условия для проверки входящих значений
    if(minRange > maxRange)  // Если минимальное значение оказалось больше максимального, то меняем их местами
    {
        printf("Bad ranges. Swapping values\n");
        int trade = minRange;
        minRange = maxRange;
        maxRange = trade;
        printf("Min range = %d, max range = %d\n", minRange, maxRange);
    }
    if(count < 0) // Если количество дробей отрицательное, то инвертируем его
    {
        printf("Bad count. Invert digit\n");
        count = -count;
        printf("Count = %d\n", count);
    }
    if(count == 0) // Если количество дробей равно нулю, то запишем тестовое значение равное пяти
    {
        printf("Bad count digit. Return to default (5)\n");
        count = 5;
    }
    srand(time(NULL));
    struct Drob *frC = NULL; // Вводим указатель для динамического массива
    frC = (struct Drob*)malloc(count * sizeof(struct Drob)); // Создаем динамический массив
    if(frC) // Проверка на правильное выделение памяти на массив
    {
        for(int i = 0; i < count; i++) // Заполняем массив значениями
        {
            frC[i].chislitel = minRange + (rand() % (maxRange - minRange + 1));
            frC[i].znamenatel = minRange + (rand() % (maxRange - minRange + 1));
        }
        return frC; // Возвращяем получившийся массив
    }
    else printf("Error: bad allocation");
}
/*
Основная функция main вводит требуемые знаения(проверяет их), выводит динамический массив на экран, ищет количество одинаковых дробей 
Выходящее знаение - 0
*/
int main()
{
    int minRange, maxRange, count;
    printf("Enter min range of values: ");
    scanf("%d", &minRange);
    printf("Enter max range of values: ");
    scanf("%d", &maxRange);
    printf("Enter count of Drob: ");
    scanf("%d", &count);
    if(minRange > maxRange) // Три условия для проверки введенных значений, аналогичные условиям в функции CreateDmasDrob
    {
        printf("Bad ranges. Swapping values\n");
        int trade = minRange;
        minRange = maxRange;
        maxRange = trade;
        printf("Min range = %d, max range = %d\n", minRange, maxRange);
    }
    if(count < 0) 
    {
        printf("Bad count. Invert digit\n");
        count = -count;
        printf("Count = %d\n", count);
    }
    if(count == 0)
    {
        printf("Bad count digit. Return to default (5)\n");
        count = 5;
    }
    struct Drob *frC = CreateDmasDrob(count, minRange, maxRange);
    if(frC) // Проверка значений получившегося массива
    {
        for( int i = 0; i < count; i++) // Выводим массив на экран
        {
        printf("%d/%d, ", frC[i].chislitel, frC[i].znamenatel);
        }
    }
    else printf("\nError: Array is empty\n");
    if(count != 1) // Если количество дробей равно единице, то нет смысла искать равные
    {
        int cart = 0; // Счетчик любых копий
        bool tab[100]; // Массив для отметок при нахождении копии, чтобы при повторном проходе не засчитывать дробь снова
        for( int i = 0; i < 100; i++) tab[i] = false;
        for( int i = 0; i < (count - 1); i++) // Циклы для поиска копий 
        {
            for( int j = i + 1; j < count; j++)
            {
                if( frC[i].chislitel == frC[j].chislitel && frC[i].znamenatel == frC[j].znamenatel && !tab[j])
                {
                    cart++;
                    tab[j] = true;
                }
            }
        }
        if(cart) printf("\nCount of equal Drobs is %d", cart); // Вывод работы циклов
            else printf("\nEqual Drobs not found");
    }
    else printf("\nOnly one Drob. Have not equal\n");
    free(frC); // Освобождаем память, выделенную на динамический массив
    return 0;
}
