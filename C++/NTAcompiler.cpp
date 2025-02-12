#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct ProdNode
{
    bool simple;
    char key;
    string prod;
};

bool varTree(string expr, string tree, vector<ProdNode> prods, string T, vector<int>* result)
{
    int pros_size = prods.size();
    bool correct = false;
    while(!correct && !tree.empty())
    {
        if (T.find(tree[0]) != string::npos)
        {
            if (expr[0] == tree[0])
            {
                expr.erase(0, 1);
                tree.erase(0, 1);
            }
            else
            {
                return false;
            }
        }
        else
        {
            for (int k = 0; k < pros_size && !correct; k++)
            {
                if (tree[0] == prods[k].key)
                {
                    string tmp;
                    tmp.append(prods[k].prod + tree);
                    if (prods[k].simple)
                    {
                        tmp.erase(1, 1);
                    }
                    else
                    {
                        tmp.erase(3, 1);
                    }

                    result->push_back(k + 1);
                    correct = varTree(expr, tmp, prods, T, result);
                    if (!correct) result->pop_back();
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

    string arrow = "->";
    T.append(arrow);

    for (int i = 0; i < prods_size; i++)
    {
        int str_size = prods[i].size();
        for (int j = 0; j < str_size; j++)
            if (T.find(prods[i][j]) == string::npos && N.find(prods[i][j]) == string::npos) return false;
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

vector<ProdNode> convert(vector<string> prods_str, string T, string N)
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

    vector<ProdNode> result;

    for (int i = 0; i < size_prods; i++)
    {
        ProdNode node;
        node.key = prods_str[i][0];

        if (prods_str[i].size() == 4) node.simple = true;
        else node.simple = false;

        node.prod.append(prods_str[i].erase(0, 3));

        result.push_back(node);
    }

    return result;
}

vector<int> NTA(string str, vector<string> prods_str, string T, string N)
{
    if (str.empty())
    {
        cout << "Error {NTA}: example is empty";
        exit(-1);
    }

    vector<int> result;
    vector<ProdNode> prods = convert(prods_str, T, N);
    string tree;
    tree += 'A';

    if(varTree(str, tree, prods, T, &result)) return result;
    else
    {
        cout << "Error {NTA}: bad example";
        exit(-1);
    }
}

void printResult(vector<int> result, bool doTree = false)
{
    int size = result.size();
    for (int i = 0; i < size - 1; i++)
    {
        cout << result[i] << " ";
    }
    cout << result[size - 1] << endl;
}

vector<int> enterResult()
{
    string result_str;
    vector<int> result;
    getline(cin, result_str);

    int size = result_str.size(), num = 0;
    for (int i = 0; i < size; i++)
    {
        if(result_str[i] >= '0' && result_str[i] <= '9') num = num * 10 + (result_str[i] - '0');
        if (result_str[i] == ' ')
        {
            result.push_back(num);
            num = 0;
        }
    }

    return result;
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
            cout << "Enter the example: ";
            string example;
            cin >> example;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            vector<int> test = NTA(example, prods, T, N);
            cout << "The result is:    ";
            printResult(test);

            cout << "Enter the answer: ";
            vector<int> answer;
            answer = enterResult();

            int answer_size = answer.size(); bool correct = true;
            for (int i = 0; i < answer_size && correct; i++)
            {
                if (test[i] != answer[i]) correct = false;
            }

            if (correct) cout << "The result is correct\n\n";
            else cout << "The result is incorrect\n\n";

            times--;
        }
    }
    else
    {
        string T = "! + * ( ) a b";
        cout << "\nGrammas are:\nT = " << T << endl;
        T = delSpaces(T);

        string N = "A B T M";
        cout << "N = " << N << endl;
        N = delSpaces(N);

        int num = 8;
        cout << "The number of productions: " << num << endl;

        cout << "Productions:\n";
        vector<string> prods;
        
        string prod = "A->!B!";
        cout << "1) " << prod << endl;
        prods.push_back(prod);

        prod = "B->T";
        cout << "2) " << prod << endl;
        prods.push_back(prod);
        
        prod = "B->T+B";
        cout << "3) " << prod << endl;
        prods.push_back(prod);
        
        prod = "T->M";
        cout << "4) " << prod << endl;
        prods.push_back(prod);

        prod = "T->M*T";
        cout << "5) " << prod << endl;
        prods.push_back(prod);
        
        prod = "M->a";
        cout << "6) " << prod << endl;
        prods.push_back(prod);
        
        prod = "M->b";
        cout << "7) " << prod << endl;
        prods.push_back(prod);

        prod = "M->(B)";
        cout << "8) " << prod << endl;
        prods.push_back(prod);

        while (times)
        {
            cout << "\nEnter the example: ";
            string example;
            cin >> example;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            vector<int> test = NTA(example, prods, T, N);
            cout << "The result is:    ";
            printResult(test);

            cout << "Enter the answer: ";
            vector<int> answer;
            answer = enterResult();

            int answer_size = answer.size(); bool correct = true;
            for (int i = 0; i < answer_size && correct; i++)
            {
                if (test[i] != answer[i]) correct = false;
            }

            if (correct) cout << "The result is correct\n";
            else cout << "The result is incorrect\n";

            times--;
        }
    }
    return 0;
}
