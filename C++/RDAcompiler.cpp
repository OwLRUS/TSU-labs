#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

bool varTree(string* expr, map<char, string> prods, string T, string* tree)
{
    bool correct = false;
    while (tree->size())
    {
        if (T.find(tree[0][0]) != string::npos)
        {
            if (expr[0][0] == tree[0][0])
            {
                expr->erase(0, 1);
                tree->erase(0, 1);
            }
            else return false;
        }
        else
        {
            char base = tree[0][0];
            bool once = false;
            while (tree->size())
            {
                size_t pos = prods[base].find(expr[0][0]);

                if (pos == string::npos && correct) return true;

                if (pos == string::npos || pos < prods[base].find('{'))
                {
                    if (once) return false;
                        else once = true;
                }

                if (pos != string::npos)
                {
                    string prod = prods[base].substr(pos);
                    pos = prod.find('|');
                    prod = prod.substr(0, pos);
                    if (pos == string::npos && prods[base].find('}') != string::npos) prod.erase(2, 1);

                    if (base == tree[0][0]) prod.append(tree->substr(1));
                        else prod.append(*tree);
                    
                    *tree = prod;
                    correct = varTree(expr, prods, T, tree);
                }
                else
                {
                    string prod; prod += prods[base][0];

                    if (T.find(prod) != string::npos) return false;

                    pos = prod.find('|');
                    prod = prod.substr(0, pos - 1);

                    prod.append(tree->substr(1));
                    *tree = prod;
                    correct = varTree(expr, prods, T, tree);
                }
            }
            if (!correct) return false;
        }
    }
    return true;
}

bool areCorrect(vector<string> prods, string T, string N)
{
    int prods_size = prods.size();
    int T_size = T.size();
    int N_size = N.size();
    if (prods_size == 0 || T_size == 0 || N_size == 0) return false;

    T.append("{}|");
    for (int i = 0; i < prods_size; i++)
    {
        bool sign = false, brackets = false;
        int str_size = prods[i].size();
        for (int j = 0; j < str_size; j++)
        {
            if (T.find(prods[i][j]) == string::npos && N.find(prods[i][j]) == string::npos)
            {
                if (j == 1 && prods[i].substr(1, 3) == "::=") j = 4;
                    else return false;
            }

            if (prods[i][j] == '{' && !brackets) brackets = true;
                else if (prods[i][j] == '{') return false;
            if (prods[i][j] == '}' && brackets) brackets = false;
                else if (prods[i][j] == '}') return false;
            if (prods[i][j] == '|' && j == str_size - 1) return false;
        }
    }

    return true;
}

string delSpaces(string str)
{
    string result;

    int size_str = str.size();
    for (int i = 0; i < size_str; i++)
    {
        if (str[i] != ' ') result += str[i];
    }

    return result;
}

map<char, string> convert(vector<string> prods_str, string T, string N)
{
    int size_prods = prods_str.size();
    for (int i = 0; i < size_prods; i++)
    {
        prods_str[i] = delSpaces(prods_str[i]);
    }


    if (!areCorrect(prods_str, T, N))
    {
        cout << "Error {convert}: bad productions";
        exit(-1);
    }

    map<char, string> result;

    for (int i = 0; i < size_prods; i++)
    {
        string prod;

        result[prods_str[i][0]] = prods_str[i].substr(4);
    }

    return result;
}

bool RDA(string expr, vector<string> prods_str, string T, string N)
{
    if (expr.empty())
    {
        cout << "Error {RDA}: example is empty";
        exit(-1);
    }

    map<char, string> prods = convert(prods_str, T, N);
    string tree = "A";

    return varTree(&expr, prods, T, &tree);
}

int main()
{
    cout << "Run by default? (Y/N)\n";
    char ans; cin >> ans;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "How many times?\n";
    int times; cin >> times;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (ans == 'N')
    {
        cout << "\nEnter grammas:\nT = ";
        string T;
        cin >> T;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        T = delSpaces(T);

        cout << "N = ";
        string N;
        cin >> N;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        N = delSpaces(N);

        cout << "Enter the number of productions: ";
        int num;
        cin >> num;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter productions one by one:\n";
        vector<string> prods;
        for (int i = 0; i < num; i++)
        {
            string tmp;
            cout << i + 1 << ") ";
            cin >> tmp;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            prods.push_back(tmp);
        }

        while (times)
        {
            cout << "Enter the expression: ";
            string example;
            cin >> example;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool result = RDA(example, prods, T, N);
            if (result) cout << "Done...";
                else cout << "Error: bad expression\n";

            times--;
        }
    }
    else
    {
        string T = "! + - * / ( ) a b c d x";
        cout << "\nGrammas are:\nT = " << T << endl;
        T = delSpaces(T);

        string N = "A B T M";
        cout << "N = " << N << endl;
        N = delSpaces(N);

        cout << "Productions:\n";
        vector<string> prods;

        string prod = "A ::= !B!";
        cout << "1) " << prod << endl;
        prods.push_back(delSpaces(prod));

        prod = "B ::= T {+T | -T}";
        cout << "2) " << prod << endl;
        prods.push_back(delSpaces(prod));

        prod = "T ::= M {*M | /M}";
        cout << "3) " << prod << endl;
        prods.push_back(delSpaces(prod));

        prod = "M ::= a | b | c | d | x | (B)";
        cout << "4) " << prod << endl;
        prods.push_back(delSpaces(prod));

        while (times)
        {
            cout << "\nEnter the expression: ";
            string example;
            cin >> example;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool result = RDA(example, prods, T, N);
            if (result) cout << "Done...\n";
            else cout << "Error: bad expression\n";

            times--;
        }
    }
    return 0;
}
