#include<stdio.h>

int main() {
    int N;
    scanf("%d", &N);
    int oN = 0;
    int V = 0;
    int digit = 0;
    oN = N; // Сохранил исходное чисто для вывода 
// Данный цикл перевернет число и будет хранить его в переменной V
while (N)
    {
        V = V * 10;
        digit = N % 10;
        V = V + digit;
        N = N / 10;
    }
    printf("%d => %d", oN, V);
}
