#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Объявляем новый тип данных, который описывает узел стека 
struct nodeList
{
    int inf;
    struct nodeList* next;
};

//Объявляем новый тип данных, который описывет стек
struct StekList
{
    struct nodeList* Top; //Вершина стека(голова на начало списка)
    int size; // количество элементов в стеке
};

// Для удобства, даем новые имена, для соданных новых типов, с помощью typedef
typedef struct nodeList Node;
typedef struct StekList Stek;

/*
Функция добавления элемента в стек
Входные аргументы:
Stek*st - указатель на стек - куда добавляем элемент со значением value
int value - добавляемое значение
Выходящие аргументы:
true - если значение добавленно в стек
false - если произошла ошибка
*/

bool Push(Stek* st, int value)
{
    Node* el = (Node*)malloc(sizeof(Node));

    if (el) {
        //Если элеиент создан, то инициализируем поля структуры, 
        //затем добавляем элемент в ерхушку стека(т.е. в голову списка).
        el->inf = value;
        el->next = NULL;
        if (st->Top == NULL)
        {
            st->Top = el;
        }
        else
        {
            el->next = st->Top;
            st->Top = el;
        }
        st->size++;
        return true;
    }
    return false;
}

/*
Функция возвращает значение хранящееся в верхушке стека, при этом сам элемент удаляется
Входные аргументы:
Stek* st - указатель на стек, откуда берем значение верхушки
Выходящие аргументы:
int value - значение верхушки, если она не пуста
0 - если верхушка или указатель пуста/пуст
*/
int Pop(Stek* st)
{
    if(st) // Проверяем не пуст ли сам стек
    {
        if (st->Top) // Преверяем не пуста ли верхушка
        // Сначала берем значение из верхушки, затем запоминаем адрес на верхушку, потом сдвигаем верхушку на слудующий элемент,
        // уменьшаем размер стека на один и удалям старую верхушку
        {
            int value = st->Top->inf;
            Node* PtrX = NULL;
            PtrX = st->Top;
            st->Top = st->Top->next;
            st->size--;
            free(PtrX);
            return value;
        }
        return 0;
    }
    return 0;
}

/*
Функция очистки стека.
Здесь удаляем полностью весь стек, путем циклического удаления верхушки до полного очищения.
Входные аргументы:
Stek* st - указатель на стек, который очищаем
*/
void Clear(Stek* st)
{
    Node* PtrX;
    while (st->Top)
    {
        PtrX = st->Top;
        st->Top = st->Top->next;
        free(PtrX);
    }
    st->size = 0;
}
/*
Функция создания ПОЛИЗа с помощью алгоритма Дейкстры
Входные аргументы:
char* FileName - указатель на название файла, где хранится выражение
Выходящие аргументы:
char RPN - массив, хранящий польскую инверсную запись
NULL - если название файла имеет значение NULL/сам файл пуст/произошла ошибка при вызове функции Push/ошибка при выделении памяти для 
динамического массива
*/
char* CreateRPN(char* FileName)
{
    if (FileName) // Проверка входного значения
    {
        FILE* PtrFile = NULL;
        PtrFile = fopen(FileName, "r");
        if (PtrFile) // Проверка на успешность отрытия файла
        {
            fseek(PtrFile, 0, SEEK_END); // Перемещаю внутренний указатель на конец файла
            long pos = ftell(PtrFile); // Возвращаю текущее положение внутреннего указателя
            if (pos == 0) return NULL; // Проверяю не пуст ли файл
            fseek(PtrFile, 0, SEEK_SET); // Возвращаю внутренний указатель на начало файла 
            char *Expression = (char*)malloc((pos + 1) * sizeof(char));
            if(Expression)
            {
                char el;
                bool flag = true; // Флаг, который в случае технической ошибки остановит работу функции и всех циклов
                for(int i = 0; fscanf(PtrFile, "%c", &el) != -1; i++)
                {
                    Expression[i] = el;
                }
                Expression[pos] = '\0';
                bool BadExpression = false;
                for(int i = 0; Expression[i] != '\0'; i++) // Проверка корректности выражения
                {
                    if((Expression[i] > 96 && Expression[i] < 123) && ((Expression[i + 1] > 96 && Expression[i + 1] < 123) || (Expression[i + 1] > 47 && Expression[i + 1] < 58)))
                        BadExpression = true;
                }
                if(!BadExpression)
                {
                    fseek(PtrFile, 0, SEEK_SET);
                    Stek st;
                    st.size = 0;
                    st.Top = NULL;
                    while (fscanf(PtrFile, "%c", &el) != -1 && flag) // Цикл, который заполнит стек операторами и выражениями, чтобы подсчитать их количество
                    {
                        if ((el > 39 && el < 44) || el == 45 || (el > 46 && el < 58) || (el > 96 && el < 123))
                        {
                            flag = Push(&st, el);
                        }
                    }
                    char* RPN = (char*)malloc((st.size + 1) * sizeof(char)); // Создаем массив для ПОЛИЗа
                    if (RPN && flag)
                    {
                        Clear(&st); // Очищаю стек после подсчета
                        fseek(PtrFile, 0, SEEK_SET); // Возвращаю внутренний указатель на начало файла
                        int ix = 0; char del;
                        while (fscanf(PtrFile, "%c", &el) != -1 && flag) // Цикл осуществит алгоритм Дейкстры
                        {
                            switch (el) 
                            {
                            case '(':
                                flag = Push(&st, el);
                                break;
        
                            case ')':
                                while (st.Top->inf != '(')
                                {
                                    RPN[ix] = Pop(&st);
                                    ix++;
                                }
                                del = Pop(&st); // Удаляю открывающуюся скобку
                                break;
        
                            case '+':
                                if (st.Top && (st.Top->inf == 42 || st.Top->inf == 43 || st.Top->inf == 45 || st.Top->inf == 47))
                                {
                                    RPN[ix] = Pop(&st);
                                    ix++;
                                    flag = Push(&st, el);
                                }
                                else flag = Push(&st, el);
                                break;
        
                            case '-':
                                if (st.Top && (st.Top->inf == 42 || st.Top->inf == 43 || st.Top->inf == 45 || st.Top->inf == 47))
                                {
                                    RPN[ix] = Pop(&st);
                                    ix++;
                                    flag = Push(&st, el);
                                }
                                else flag = Push(&st, el);
                                break;
        
                            case '*':
                                if (st.Top && (st.Top->inf == 42 || st.Top->inf == 47))
                                {
                                    RPN[ix] = Pop(&st);
                                    ix++;
                                    flag = Push(&st, el);
                                }
                                else flag = Push(&st, el);
                                break;
        
                            case '/':
                                if (st.Top && (st.Top->inf == 42 || st.Top->inf == 47))
                                {
                                    RPN[ix] = Pop(&st);
                                    ix++;
                                    flag = Push(&st, el);
                                }
                                else flag = Push(&st, el);
                                break;
        
                            default: // Алгорим для выражений
                                if ((el > 47 && el < 58) || (el > 96 && el < 123))
                                {
                                    RPN[ix] = el;
                                    ix++;
                                }
                                break;
                            }
                        }
                        // После выполнения цикла в стеке остаются ничем не вытолкнутые операторы
                        if(flag) // Если алгоритм Дейкстры проработал без ошибок
                            {
                            while (st.Top != NULL) // Заношу в ПОЛИЗ оставшиеся операторы
                            {
                                RPN[ix] = Pop(&st);
                                ix++;
                            }
                            RPN[ix] = '\0'; // Зачищаю конец массива
                            fclose(PtrFile);
                            return RPN;
                        }
                        return NULL;
                    }
                    return NULL;
                }
                return NULL;
            }
            return NULL;
        }
        return NULL;
    }
    return NULL;
}
/*
Функция записи выражения из файла в массив
Входные аргументы:
char* FileName - указатель на название файла, где хранится выражение
Выходящие аргументы:
char Expression - массив, хранящий выражение из файла
NULL - если название файла имеет значение NULL/сам файл пуст/произошла ошибка при вызове функции Push/ошибка при выделении памяти для 
динамического массива
*/
char* FindExpression(char* FileName)
{
    if (FileName)// Проверка входного значения
    {
        FILE* PtrFile = NULL;
        PtrFile = fopen(FileName, "r");
        if (PtrFile)// Проверка на успешность отрытия файла
        {
            fseek(PtrFile, 0, SEEK_END); // Перемещаю внутренний указатель на конец файла
            long pos = ftell(PtrFile); // Возвращаю текущее положение внутреннего указателя
            if (pos == 0) return NULL;// Проверяю не пуст ли файл
            fseek(PtrFile, 0, SEEK_SET);// Возвращаю внутренний указатель на начало файла
            Stek st;
            st.size = 0;
            st.Top = NULL;
            char el;
            bool flag = true;// Флаг, который в случае технической ошибки остановит работу функции и всех циклов
            while (flag && fscanf(PtrFile, "%c", &el) != -1)// Цикл, который заполнит стек операторами и выражениями, чтобы подсчитать их количество
            {
                if ((el > 39 && el < 44) || el == 45 || (el > 46 && el < 58) || (el > 96 && el < 123))
                {
                    flag = Push(&st, el);
                }
            }
            if(flag)
            {
                char* Expression = (char*)malloc((st.size + 1) * sizeof(char));// Создаем массив для выражения
                if (Expression)
                {
                    Clear(&st);// Очищаю стек после подсчета
                    fseek(PtrFile, 0, SEEK_SET);// Возвращаю внутренний указатель на начало файла
                    int ix = 0;
                    while (fscanf(PtrFile, "%c", &el) != -1)// Теперь переписываю само выражения (с пропуском не используемых элементов)
                    {
                        if ((el > 39 && el < 44) || el == 45 || (el > 46 && el < 58) || (el > 96 && el < 123))
                        {
                            Expression[ix] = el;
                            ix++;
                        }
                    }
                    Expression[ix] = '\0'; // Зачищаю конец массива
                    fclose(PtrFile);
                    bool BadExpression = false;
                    for(int i = 0; Expression[i] != '\0'; i++) // Проверка корректности выражения
                    {
                        if((Expression[i] > 96 && Expression[i] < 123) && ((Expression[i + 1] > 96 && Expression[i + 1] < 123) || (Expression[i + 1] > 47 && Expression[i + 1] < 58)))
                            BadExpression = true;
                    }
                    if(BadExpression) return NULL;
                    return Expression;
                }
                return NULL;
            }
            return NULL;
        }
        return NULL;
    }
    return NULL;
}
/*
Функция создает файл, где записывает результат подсчета, также подсчитывает сам результат если выражение корректно, а если не корректно, то
пишет в файле соответствующее сообщение
Входные аргументы:
char* FileName - указатель на название файла, котором отобразится результат
char* RPN - указатель на массив ПОЛИЗа
char* Expression -  указатель на массив выражения
Выходящие аргументы:
true - если функция проработала без ошибок и получила результат
false - если хотя бы один из входящих аргументов имеет значение NULL/при ошибке открытия файла/произошла ошибка при вызове функции Push/
ошибка при выделении памяти для динамического массива/функция не получила результат
*/
bool CalculationAtFile(char* FileName, char* RPN, char* Expression)
{
    if (FileName && RPN && Expression) // Проверка входящих значений
    {
        FILE* PtrFile = NULL;
        PtrFile = fopen(FileName, "w");
        if (PtrFile) // Проверка на успешность открытия файла
        {
            fprintf(PtrFile, "Expression: ");
            for (int i = 0; Expression[i] != '\0'; i++) // Запись выражения в файл
            {
                fprintf(PtrFile, "%c", Expression[i]);
            }
            fprintf(PtrFile, "\nReverse Polish notation (RPN): ");
            for (int i = 0; RPN[i] != '\0'; i++) // Запись ПОЛИЗа в файл
            {
                fprintf(PtrFile, "%c ", RPN[i]);
            }
            fprintf(PtrFile, "\n\nCalculation of the Expression:\n\n");
            int CountOfLetters = 0;
            for (int i = 0; Expression[i] != '\0'; i++) // Подсчет количества переменных в выражении
            {
                if (Expression[i] > 96 && Expression[i] < 123)
                    CountOfLetters++;
            }
            int* ValuesOfLetters = (int*)malloc((CountOfLetters + 1) * sizeof(int)); // Массив для значений переменных
            char* Letters = (char*)malloc((CountOfLetters + 1) * sizeof(char)); // Массив самих переменных
            if (ValuesOfLetters && Letters) // Проверка на успешное выделение памяти
            {
                int j = 0;
                for (int i = 0; Expression[i] != '\0'; i++) // Заполнение массива переменными
                {
                    if (Expression[i] > 96 && Expression[i] < 123)
                    {
                        Letters[j] = Expression[i];
                        j++;
                    }
                }
                for (int i = 0; i < CountOfLetters; i++) // Заполнение массива значениями переменных и запись их в файл
                {
                    printf("Enter value of %c: ", Letters[i]);
                    scanf("%d", &ValuesOfLetters[i]);
                    fprintf(PtrFile, "%c - %d\n", Letters[i], ValuesOfLetters[i]);
                }
                ValuesOfLetters[CountOfLetters] = '\0'; // Зачищаю конец массива
                Letters[CountOfLetters] = '\0';
                Stek st;
                st.size = 0;
                st.Top = NULL;
                j = CountOfLetters - 1;
                bool flag = true;
                for (int i = strlen(RPN) - 1; i >= 0 && flag; i--) // Заполняю стек ПОЛИЗом, начиная с конца для удобства подсчета
                {
                    if (RPN[i] > 96 && RPN[i] < 123) // Если встретилась переменная, то сразу заношу ее значение
                    {
                        flag = Push(&st, ValuesOfLetters[j]);
                        j--;
                    }
                    else
                    {
                        if (RPN[i] > 47 && RPN[i] < 58) flag = Push(&st, RPN[i] - 48); // Если встретилось конкретное значение, то заношу само значение, 
                                                                                       // а не его код
                        else flag = Push(&st, RPN[i]); // Оператор просто заношу в стек
                    }
                }
                if(flag) // Если не было ошибок
                {
                    Node* PtrSt = NULL, * PtrIx = NULL, * PtrJx = NULL;
                    PtrSt = st.Top;
                    while (PtrSt->next->next->next != NULL) // Цикл, который произведет арфмитические вычисления в стеке, пока
                    // в стеке не останется 3 элемента
                    /*
                    Подсчет идет таким образом:
                    Берется первые три элемента стека, к примеру a b -, проверяется третий элемент: является ли он оператором.
                    Если является, то выполняем следующие действия:
                    В голове стека сохраняем результат соответстующего вычисления перового и второго элемента, a = a - b.
                    Далее удаляем второй и третий элемент так, чтобы голова начала смотреть на четвертый элемент, и уменьшаем размер массива на 2 
                    Если не является, к примеру abc, то переходим на следующий элемент стека и повторяем действия.
                    */
                    {
                        switch (PtrSt->next->next->inf) // Смотрим на значение третьего элемента
                        {
                        case(42):
                            PtrSt->inf = PtrSt->inf * PtrSt->next->inf;
                            PtrIx = PtrSt->next;
                            PtrIx = PtrSt->next->next;
                            PtrSt->next = PtrSt->next->next->next;
                            st.size = st.size - 2;
                            free(PtrIx);
                            free(PtrJx);
                            break;
    
                        case(43):
                            PtrSt->inf = PtrSt->inf + PtrSt->next->inf;
                            PtrIx = PtrSt->next;
                            PtrIx = PtrSt->next->next;
                            PtrSt->next = PtrSt->next->next->next;
                            st.size = st.size - 2;
                            free(PtrIx);
                            free(PtrJx);
                            break;
    
                        case(45):
                            PtrSt->inf = PtrSt->inf - PtrSt->next->inf;
                            PtrIx = PtrSt->next;
                            PtrIx = PtrSt->next->next;
                            PtrSt->next = PtrSt->next->next->next;
                            st.size = st.size - 2;
                            free(PtrIx);
                            free(PtrJx);
                            break;
    
                        case(47):
                            PtrSt->inf = PtrSt->inf / PtrSt->next->inf;
                            PtrIx = PtrSt->next;
                            PtrIx = PtrSt->next->next;
                            PtrSt->next = PtrSt->next->next->next;
                            st.size = st.size - 2;
                            free(PtrIx);
                            free(PtrJx);
                            break;
    
                        default:
                            PtrSt = PtrSt->next;
                            break;
                        }
                        if (PtrSt->next && PtrSt->next->inf < 48 && PtrSt->next->inf > 41) PtrSt = st.Top; //Если произошла такая ситуация, что первые четыре элемента стека
                        // выглядит, к примеру ab*c, а мы находимся на элементе b и видим только b*c, то возврящаемся на первый элемент
                    }
                    bool BadExpression = false;
                    switch (PtrSt->next->next->inf) // Проводим подсчет для оставшихся трех элементов в стеке
                    {
                    case(42):
                        PtrSt->inf = PtrSt->inf * PtrSt->next->inf;
                        PtrIx = PtrSt->next;
                        PtrIx = PtrSt->next->next;
                        PtrSt->next = PtrSt->next->next->next;
                        st.size = st.size - 2;
                        free(PtrIx);
                        free(PtrJx);
                        break;
    
                    case(43):
                        PtrSt->inf = PtrSt->inf + PtrSt->next->inf;
                        PtrIx = PtrSt->next;
                        PtrIx = PtrSt->next->next;
                        PtrSt->next = PtrSt->next->next->next;
                        st.size = st.size - 2;
                        free(PtrIx);
                        free(PtrJx);
                        break;
    
                    case(45):
                        PtrSt->inf = PtrSt->inf - PtrSt->next->inf;
                        PtrIx = PtrSt->next;
                        PtrIx = PtrSt->next->next;
                        PtrSt->next = PtrSt->next->next->next;
                        st.size = st.size - 2;
                        free(PtrIx);
                        free(PtrJx);
                        break;
    
                    case(47):
                        PtrSt->inf = PtrSt->inf / PtrSt->next->inf;
                        PtrIx = PtrSt->next;
                        PtrIx = PtrSt->next->next;
                        PtrSt->next = PtrSt->next->next->next;
                        st.size = st.size - 2;
                        free(PtrIx);
                        free(PtrJx);
                        break;
    
                    default:
                        BadExpression = true; // Если последний элемент не оператор, то выражение с ошибкой
                        break;
                    }
                    if (!BadExpression) // Условие, которое занесет в файл соответстующий результат
                    {
                        fprintf(PtrFile, "Expression is correct: %d", PtrSt->inf);
                        fclose(PtrFile);
                        free(ValuesOfLetters); // Освобождаю память, выделенную на динамический массив
                        free(Letters);
                        return true;
                    }
                    else
                    {
                        fprintf(PtrFile, "Error: Expression is uncorrect");
                        fclose(PtrFile);
                        free(ValuesOfLetters); // Освобождаю память, выделенную на динамический массив
                        free(Letters);
                        return false;
                    }
                }
                return false;
            }
            return false;
        }
        return false;
    }
    return false;
}

int main()
{
    char ExpressionName[15] = "Expression.txt";
    char* RPN = CreateRPN(ExpressionName);
    char* Expression = FindExpression(ExpressionName);
    if(RPN && Expression)
    {
    char Result[11] = "Result.txt";
    bool flag = CalculationAtFile(Result, RPN, Expression);
    if(flag) printf("\nSuccess!\n");
        else printf("\nFailure...\n");
    }
    else printf("\nError\n");
    return 0;
}
