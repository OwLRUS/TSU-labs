#include <stdio.h>
#include <stdbool.h>
/*
Функция PrintText выводит текст входящего файла
char *FILEname - указатель на первую букву названия нужного файла
Выходящее значение - true(если текст вывелся удачно) и false(если входящий элемент имеет значение NULL или не удалось открыть файл)
*/
bool PrintText(char *FILEname)
{
    if(FILEname) // Проверка входящего значения
    {
        FILE *ptrFile = NULL;
        ptrFile = fopen(FILEname,"r");
        if(ptrFile) // Проверка успешного открытия файла
        {
            char el;
            while(fscanf(ptrFile,"%c",&el)!=-1) // Выполяем цикл вывода текста на экран, пока не дойдем до конца файла
            {
                /* Пытался исправить проблему с ключем ~
                if(el > 0 && el < 128) printf("%c", el);
                    else printf("[ERROR]");
                */
                printf("%c", el);
            }
            fclose(ptrFile);
            return true;
        }
        else 
        {
            fclose(ptrFile);
            return false;
        }
    }
    else return false;
}
/*
Функция Coding кодирует текст файла заданным ключом
char *FILEname - указатель на первую букву названия файла с оригинальным текстом 
char *CodeFILEname - указатель на первую букву названия файла, где будет храниться зашифрованный текст 
char key - ключ кодирования
Выходящее значение - если хотя бы одно из входящих значений имело значение NULL или не удалось открыть файлы, то false, иначе текст закодируется и выйдет true
*/
bool Coding(char *FILEname, char *CodeFILEname, char key)
{
    if(FILEname && CodeFILEname && key)
    {
        FILE *ptrFile = NULL;
        FILE *ptrCodeF = NULL;
        ptrFile = fopen(FILEname,"r"); // Открываем файл с оригинальным текстом на чтение для его кодирования
        ptrCodeF = fopen(CodeFILEname,"w"); // Открываем файл для записи, где будет хранится зашифрованное сообщени
        if(ptrFile && ptrCodeF) // Проверка успешного открытия файлов
        {
            char el;
            fprintf(ptrCodeF,"%c", key); // Решил спрятать ключ в начале закодированного текста
            while(fscanf(ptrFile,"%c",&el)!=-1) // Цикл кодирования текста, пока не дойдет до конца оригинального текста
            {
                fprintf(ptrCodeF,"%c", (el^key));
            }
            fclose(ptrCodeF); // Закрываем файлы
            fclose(ptrFile);
            return true;
        }
        else 
        {
            fclose(ptrCodeF); // Закрываем файлы
            fclose(ptrFile);
            return false;
        }
    }
    else return false;
}
/*
Функция DeCoding декодирует текст с помощью спрятанного ключа
char *CodeFILEname - указатель на первую букву названия файла с зашифрованным текстом
char *DeCodeFILEname - указатель на первую букву названия файла, где будет храниться дешифрованныый текст
Выходящее значение - если хотя бы одно из выходящих значений имело значение NULL или не удалость открыть файлы, то false, иначе текст декодируется и выйдет true
*/
bool DeCoding(char *CodeFILEname, char *DeCodeFILEname)
{
    if(CodeFILEname && DeCodeFILEname)
    {
        FILE *ptrCodeF = NULL;
        FILE *ptrDeCodeF = NULL;
        ptrCodeF = fopen(CodeFILEname,"r"); // Открывем для чтения и дешифровки сообщения
        ptrDeCodeF = fopen(DeCodeFILEname,"w"); // Открываем для записи, в заданном файле будет хранится декодированный текст
        if(ptrCodeF && ptrDeCodeF) // Проверка успешного открытия файлов
        {
            char el;
            fscanf(ptrCodeF,"%c",&el); // Сразу после проверки беру ключ для декодирвания 
            char key = el; 
            while(fscanf(ptrCodeF,"%c",&el)!=-1) // Цикл кодирования текста, пока не дойдет до конца закодированного текста
            {
                fprintf(ptrDeCodeF,"%c", (el^key));
            }
            fclose(ptrCodeF);
            fclose(ptrDeCodeF);// Заканчиваем работу с ними
            return true;
        }
        else 
        {
            fclose(ptrCodeF);
            fclose(ptrDeCodeF);// Заканчиваем работу с ними
            return false;
        }
    }
    else return false;
}
/*
Функция CheckForEqual проверяет эквивалентен ли оригинальный текст и декодированный текст
char *FILEname - указатель на первую букву названия файла с оригинальным текстом
char *DeCodeFILEname - указатель на первую букву названия файла с расшифрованным текстом
Выходящее значение - 0(если тексты совпадают и не вложены один в другой), 1(если они не совпадают или вложены один в другой), 
2(если входящее значение равно NULL или не удалось открыть файлы)
*/
int CheckForEqual(char *FILEname, char *DeCodeFILEname)
{
    if(FILEname && DeCodeFILEname) // Проверка входящего значения
    {
        FILE *ptrFile = NULL;
        FILE *ptrDeCodeF = NULL;
        ptrFile = fopen(FILEname,"r"); // Открываем оригинальный файл для чтения
        ptrDeCodeF = fopen(DeCodeFILEname,"r"); // Открываем декодированный текст для чтения
        if(ptrFile && ptrDeCodeF) 
        {
            char elx = 'a'; // Задаю начальные значения проверяемых элементов текстов
            char ely = 'a';
            int ix,iy; // Показатели конца текстов
            while( ix != -1 && iy != -1 && elx == ely) // Цикл проверки эквивалентности, будет выполняться пока не 
                                                       // дойдет до конца хотя бы одного из текстов или элементы не перестанут быть равными
            {
                ix = fscanf(ptrFile,"%c",&elx); 
                iy = fscanf(ptrDeCodeF,"%c",&ely);
            }
            if (ix == -1 && iy == -1) 
            {
                fclose(ptrFile);
                fclose(ptrDeCodeF); // Закрываем файлы
                return 0;
            }
            else
            {
                fclose(ptrFile);
                fclose(ptrDeCodeF); // Закрываем файлы
                return 1;
            }
        }
        else 
        {
            fclose(ptrFile);
            fclose(ptrDeCodeF); // Закрываем файлы
            return 2;
        }
    }
    else return 2;
}
/*
Функция CheckForEmpty проверяет не пуст ли файл
char *FILEname - указатель на первую букву названия файла
Выходящее значение - -1(если не удалось открыть файл или выходящее значение равно NULL), 0(если файл не пуст), 1(если пуст)
*/
int CheckForEmpty(char *FILEname)
{
    if(FILEname)
    {
      FILE *ptrFile = NULL;
      ptrFile = fopen(FILEname,"r"); // Открываем файл для чтения
      if(ptrFile)
      {
          fseek(ptrFile, 0, SEEK_END); // Перемещаю внутренний указатель на конец файла
          long pos= ftell(ptrFile); // Возвращаю текущее положение внутреннего указателя
          if(!pos) // Если положение оказалось равно нулю, то файл пуст и программе нет смысла работать дальше
          {
              fclose(ptrFile); // Закрываю файл
              return 1;
          }
          else
          {
              fclose(ptrFile); // Закрываю файл
              return 0;
          }
      }
      else
      {
          fclose(ptrFile); // Закрываю файл
          return -1;
      }
    }
    else return -1;
}
/*
Основная функция main вызывает функции и выводит соответстующие сообщения
*/
void main ()
{
  bool flag;
  int emptyflag; // Флаг для проверки пустоты файла
  char key = '+'; // Ключ для зашифровки
  int flagint; // Флаг типа int, для вывода определенного сообщения
  
  emptyflag = CheckForEmpty("File.txt");
  
  if(emptyflag == 0) // Если оригинальный текст не пуст, то работаем с его кодированием и декодиованием 
  {
    printf("Origin text is:\n");
    flag = PrintText("File.txt"); // Вывод на экран текста и сохранение выходящего значения функции
    if(!flag) printf("Error: bad digits imput or can't open the file\n"); // Если функция вывела false, то выводим соответстуающее сообщение
      
    flag = Coding("File.txt","CodingFile.txt", key); // Кодируем сообщение и сохраняем выходящее значение функции
    if(!flag) printf("Error: bad coding or bad digits imput\n"); // Если функция проработала безуспешно, то выводим ошибку
        
    printf("\nCoding text is:\n");
    flag = PrintText("CodingFile.txt");
    if(!flag) printf("Error: bad digits imput or can't open the file\n");
      
    flag = DeCoding("CodingFile.txt", "DeCodingFile.txt");
    if(!flag) printf("Error: bad decoding or bad digits imput\n");
      
    printf("\nDecoding text is:\n");
    flag = PrintText("DeCodingFile.txt");
    if(!flag) printf("Error: bad digits imput or can't open the file\n");

    flagint = CheckForEqual("File.txt", "DeCodingFile.txt"); // Проверяем равенство текстов и выводим соответсвующее сообщение
    if(flagint == 0) printf("\nOrigin text and decoded text are equal\n");
        else if(flagint == 1) printf("\nOrigin text and decoded text are not equal\n");
            else printf("\nError: bad digits imput\n");
  }
  else if(emptyflag == 1) printf("Error: file is empty\n");
            else printf("Can't open the file\n");
}
