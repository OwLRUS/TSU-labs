#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // Инициализация S-блока
    int S[8] = { 0,1,2,3,4,5,6,7 };
    int K[8] = { 6,9,1,6,9,1,6,9 };

    int swap, i, j, t = 0;
    vector<int> gamma;
    // Предварительная перестановка S
    for (i = j = 0; i < 8; i++)
    {
        j = (j + S[i] + K[i]) % 8;
        cout << "i = " << i << " " << "| j = " << j << " ";

        swap = S[i];
        S[i] = S[j];
        S[j] = swap;

        cout << "| S: ";
        for (int h = 0; h < 8; h++)
        {
            cout << S[h] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    // Генерация гаммы
    for (i = 1, j = 0; i < 8; i++)
    {
        i = i % 8;
        j = (j + S[i]) % 8;
        cout << "i = " << i << " " << "| j = " << j << " ";

        swap = S[i];
        S[i] = S[j];
        S[j] = swap;

        cout << "| S: ";
        for (int h = 0; h < 8; h++)
        {
            cout << S[h] << " ";
        }
        
        t = (S[i] + S[j]) % 8;
        gamma.push_back(S[t]);
        cout << "| t = " << t << " | gamma = " << gamma[i - 1] << "\n";
    }
    // Вывод результата
    cout << "\nResult is: ";
    for (int l = 0; l < gamma.size(); l++)
    {
        cout << gamma[l] << " ";
    }
}
