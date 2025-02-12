#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <stack>

using namespace std;

class Function {
private:
    vector <string> expression;
    bool Letter(char);
    bool Digit(char);
    bool Operation(char);

public:
    Function(string);//заполнение полиза из строки

    float GetY(); //медот для вычисления координаты Y по координате X
    void print() {
        for (string substr : expression) {
            cout << substr << " ";
        }
    }
};

bool Function::Letter(char c) {//Функция проверки символа выражения (является ли символ буквой)
    if ('a' <= c && c <= 'z') return true;
    return false;
}
bool Function::Digit(char c) {//Функция проверки символа выражения (является ли символ числом)
    if ('0' <= c && c <= '9') return true;
    return false;
}
bool Function::Operation(char c) {//Функция проверки символа выражения (является ли символ операцией)
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') return true;
    return false;
}
/*Cоздание ПОЛИЗа*/
Function::Function(string s) {
    s = "(" + s + ")";//заключаю данное выражение в еще одни скобочки, на случай, если выражение имеет вид (а+б)-в, 
    //в данном случае минус не будет учтен при построение полиза в моей реализации,
    //поэтому я выбрала такую форму устранения проблемы

    stack <string> Stek;//создаем стек
    map <string, int> operationPriority = { { "(", 0 },//	Список и приоритет операторов
                                { "+", 1 },
                                { "-", 1 },
                                { "*", 2 },
                                { "/", 2 },
                                { "^", 3 },
                                { "sin()", 4 },
                                { "asin()", 4 },
                                { "sinh()", 4 },
                                { "asinh()", 4 },
                                { "cos()", 4 },
                                { "acos()", 4 },
                                { "cosh()", 4 },
                                { "acosh()", 4 },
                                { "tan()", 4 },
                                { "atan()", 4 },
                                { "tanh()", 4 },
                                { "atanh()", 4 },
                                { "cotan()", 4 },
                                { "acotan()", 4 },
                                { "cotanh()", 4 },
                                { "acotanh()", 4 },
                                { "exp()", 4 },
                                { "log()", 4},
                                { "log10()", 4 },
                                { "loga()", 4 },
                                { "~", 5 } };//	Унарный минус
    int len = (int)s.length();
    bool check = true;//переменная для проверки
    string symbols;
    string deletestek;
    int bracket = 0;//счетчик скобок

    for (int i = 0; (i < len) && check; i++) {
        if (Digit(s[i])) {//если число, записываем его в строку
            symbols = s[i];//как только встретили символ-не число, добавляем получившееся число в полиз
            while (Digit(s[i + 1])) {
                symbols += s[i + 1];
                i++;
            }
            expression.push_back(symbols);
        }
        else if (Letter(s[i])) {//если встретили букву, это может бытьх, операция или константа
            switch (s[i]) {
            case 'x':
                symbols = "x";//переменную х просто добавляем в полиз
                expression.push_back(symbols);
                break;
            case 's'://рассматриваем sin, sinh
                if ((s[i + 1] == 'i') && (s[i + 2] == 'n')) {
                    if (s[i + 3] == 'h') {
                        symbols = "sinh()";
                        i++;
                    }
                    else symbols = "sin()";
                    while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["sin()"])) {
                        deletestek = Stek.top();//выталкиваем из стека все операции, чей приоритет выше
                        Stek.pop();
                        expression.push_back(deletestek);
                    }
                    Stek.push(symbols);//Заносим в стек
                    i = i + 2;
                }
                else check = false;
                break;
            case 'c'://аналогично case 's'
                if ((s[i + 1] == 'o') && (s[i + 2] == 's')) {
                    if (s[i + 3] == 'h') {
                        symbols = "cosh()";
                        i++;
                    }
                    else symbols = "cos()";
                    i = i + 2;
                }
                else if ((s[i + 1] == 'o') && (s[i + 2] == 't') && (s[i + 3] == 'a') && (s[i + 4] == 'n')) {
                    if (s[i + 5] == 'h') {
                        symbols = "cotanh()";
                        i++;
                    }
                    else symbols = "cotan()";
                    i = i + 4;
                }
                else if ((s[i + 1] == 't') && (s[i + 2] == 'g')) {
                    if (s[i + 3] == 'h') {
                        symbols = "cotanh()";
                        i++;
                    }
                    else symbols = "cotan()";
                    i = i + 2;
                }
                else check = false;
                while (check && (!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["cos()"])) {
                    deletestek = Stek.top();
                    Stek.pop();
                    expression.push_back(deletestek);
                }
                Stek.push(symbols);//	Заносим в стек
                break;
            case 't'://аналогично case 's'
                if ((s[i + 1] == 'a') && (s[i + 2] == 'n')) {
                    if (s[i + 3] == 'h') {
                        symbols = "tanh()";
                        i++;
                    }
                    else symbols = "tan()";
                    i = i + 2;
                }
                else if (s[i + 1] == 'g') {
                    if (s[i + 2] == 'h') {
                        symbols = "tanh()";
                        i++;
                    }
                    else symbols = "tan()";
                    i++;
                }
                else check = false;
                while (check && (!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["tan()"])) {
                    deletestek = Stek.top();
                    Stek.pop();
                    expression.push_back(deletestek);
                }
                Stek.push(symbols);//	Заносим в стек
                break;
            case 'a'://аналогично case 's'
                if ((s[i + 1] == 's') && (s[i + 2] == 'i') && (s[i + 3] == 'n')) {
                    if (s[i + 4] == 'h') {
                        symbols = "asinh()";
                        i++;
                    }
                    else symbols = "asin()";
                    i = i + 3;
                }
                else if ((s[i + 1] == 'c') && (s[i + 2] == 'o') && (s[i + 3] == 's')) {
                    if (s[i + 4] == 'h') {
                        symbols = "acosh()";
                        i++;
                    }
                    else symbols = "acos()";
                    i = i + 3;
                }
                else if ((s[i + 1] == 't') && (s[i + 2] == 'a') && (s[i + 3] == 'n')) {
                    if (s[i + 4] == 'h') {
                        symbols = "atanh()";
                        i++;
                    }
                    else symbols = "atan()";
                    i = i + 3;
                }
                else if ((s[i + 1] == 't') && (s[i + 2] == 'g')) {
                    if (s[i + 3] == 'h') {
                        symbols = "atanh()";
                        i++;
                    }
                    else symbols = "atan()";
                    i = i + 2;
                }
                else if ((s[i + 1] == 'c') && (s[i + 2] == 'o') && (s[i + 3] == 't') && (s[i + 4] == 'a') && (s[i + 5] == 'n')) {
                    if (s[i + 6] == 'h') {
                        symbols = "acotanh()";
                        i++;
                    }
                    else symbols = "acotan()";
                    i = i + 5;
                }
                else if ((s[i + 1] == 'c') && (s[i + 2] == 't') && (s[i + 3] == 'g')) {
                    if (s[i + 4] == 'h') {
                        symbols = "acotanh()";
                        i++;
                    }
                    else symbols = "acotan()";
                    i = i + 3;
                }
                else check = false;
                while (check && (!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["tan()"])) {
                    deletestek = Stek.top();
                    Stek.pop();
                    expression.push_back(deletestek);
                }
                Stek.push(symbols);//	Заносим в стек
                break;
            case 'e'://экспонента может встретится в 2х видах exp(...) или e^(...)
                if (s[i + 1] == '^') {
                    symbols = "exp()";
                    while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["exp()"])) {
                        deletestek = Stek.top();
                        Stek.pop();
                        expression.push_back(deletestek);
                    }
                    Stek.push(symbols);//	Заносим в стек
                    i++;
                }
                else if ((s[i + 1] == 'x') && (s[i + 2] == 'p')) {
                    symbols = "exp()";
                    while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["exp()"])) {
                        deletestek = Stek.top();
                        Stek.pop();
                        expression.push_back(deletestek);
                    }
                    Stek.push(symbols);//	Заносим в стек
                    i = i + 2;
                }
                else check = false;
                break;
            case 'p'://число пи, просто добавляем в полиз
                if (s[i + 1] == 'i') {
                    symbols = "pi";
                    expression.push_back(symbols);
                    i++;
                }
                else check = false;
                break;
            case 'l':
                if ((s[i + 1] == 'n')) {//натуральный логарифм
                    symbols = "log()";//в с++ ln() это log()
                    while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["log()"])) {
                        deletestek = Stek.top();
                        Stek.pop();
                        expression.push_back(deletestek);
                    }
                    i++;
                    Stek.push(symbols);//	Заносим в стек
                }
                else if ((s[i + 1] == 'o') && (s[i + 2] == 'g') && (s[i + 3] == '1') && (s[i + 4] == '0') && (s[i + 5] == '(')) {
                    symbols = "log10()";
                    while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["log10()"])) {
                        deletestek = Stek.top();
                        Stek.pop();
                        expression.push_back(deletestek);
                    }
                    Stek.push(symbols);//	Заносим в стек
                    i = i + 4;//скобочку последнюю не учитываем
                }
                else if ((s[i + 1] == 'o') && (s[i + 2] == 'g')) {
                    while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["log()"])) {
                        deletestek = Stek.top();
                        Stek.pop();
                        expression.push_back(deletestek);
                    }//здесь мы должны показать разницу между 'log a (...)', 'log (...)(...)' и 'log (...)'
                    if (s[i + 3] == '(') {
                        int k = i;
                        while (s[k] != ')') k++;
                        if (s[k + 1] == '(') {
                            symbols = "loga()";
                        }
                        else symbols = "log()";
                        Stek.push(symbols);
                        i = i + 2;
                    }
                    else if (Digit(s[i + 3]) || Letter(s[i + 3])) {
                        symbols = "loga()";
                        Stek.push(symbols);
                        i = i + 2;
                    }
                    else check = false;
                    break;
                }
            }
        }
        else {
            switch (s[i]) {
            case '(':
                Stek.push("(");//просто добавляем в стек
                bracket++;//счетчик скобок
                break;
            case ')':
                //	Заносим в выходную строку из стека всё вплоть до открывающей скобки
                while ((!Stek.empty()) && (Stek.top() != "(")) {
                    symbols = Stek.top();
                    Stek.pop();
                    expression.push_back(symbols);
                }
                if (Stek.top() == "(") Stek.pop();//	Удаляем открывающуюся скобку из стека
                bracket--;//счетчик скобок, при правильном количесвте он должен выйти в 0
                break;
            case '+':
                //	Заносим в выходную строку все операторы из стека, имеющие более высокий приоритет
                while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["+"])) {
                    symbols = Stek.top();
                    Stek.pop();
                    expression.push_back(symbols);
                }
                Stek.push("+");//	Заносим в стек
                break;
            case '-':
                if (s[i - 1] == '(') {//рассматриваем случай унарного минуса
                    Stek.push("~");//	Заносим в стек
                }
                else {
                    while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["-"])) {
                        symbols = Stek.top();
                        Stek.pop();
                        expression.push_back(symbols);
                    }
                    Stek.push("-");//	Заносим в стек
                }
                break;
            case '*':
                while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["*"])) {
                    symbols = Stek.top();
                    Stek.pop();
                    expression.push_back(symbols);
                }
                Stek.push("*");//	Заносим в стек
                break;
            case '/':
                while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["/"])) {
                    symbols = Stek.top();
                    Stek.pop();
                    expression.push_back(symbols);
                }
                Stek.push("/");//	Заносим в стек
                break;
            case '^':
                while ((!Stek.empty()) && (operationPriority[Stek.top()] >= operationPriority["^"])) {
                    symbols = Stek.top();
                    Stek.pop();
                    expression.push_back(symbols);
                }
                Stek.push("^");//	Заносим в стек
                break;
            }
        }
    }
    if (check == false) cout << "invalid character entered";
    if (bracket != 0) cout << "the number of brackets is incorrect";
}

float Function::GetY() {
    stack <float> Stek1;//создаем стек
    float result;
    float first;
    float second;
    int i = 0;

    for (string s : expression) {
        if (Digit(s[0])) {
            float number = 0;
            for (int j = 0; j < s.length(); j++)
                number = number * 10 + s[j];
            Stek1.push(number);
        }
        else if (Operation(s[0])) {
            second = Stek1.top();
            Stek1.pop();
            first = Stek1.top();
            Stek1.pop();
            switch (s[0]) {
            case '+':
                Stek1.push(first + second);
                break;
            case '-':
                Stek1.push(first - second);
                break;
            case '*':
                Stek1.push(first * second);
                break;
            case '/':
                Stek1.push(first / second);
                break;
            case '^':
                float z = pow(first, second);
                Stek1.push(z);
                break;
            }
        }
        else if (s[0] == '~') {
            first = Stek1.top();
            Stek1.pop();
            Stek1.push(0 - first);
        }
        else if (Letter(s[0])) {
            if (s == "loga()") {
                second = Stek1.top();
                Stek1.pop();
                first = Stek1.top();
                Stek1.pop();
                float z = (log10(second)) / (log10(first));
                Stek1.push(z);
            }
            else {
                switch (s[0]) {}
            }

        }
        else {
            cout << "Error when finding Y";
            exit;
        }
    }
    result = Stek1.top();
    Stek1.pop();
    return result;
}


int main() {
    Function f("exp(e^2)");
    f.print();
    float y = f.GetY();
    cout << endl << y;
}
