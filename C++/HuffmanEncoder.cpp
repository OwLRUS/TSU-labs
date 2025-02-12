#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>

using namespace std;

struct Node 
{
    char letter;
    unsigned int frequency;
    Node* left;
    Node* right;
};

Node* AddNode(char key, unsigned int freq, Node* left, Node* right) {
    Node* node = new Node();
    node->letter = key;
    node->frequency = freq;
    node->left = left;
    node->right = right;
    return node;
};

struct check 
{
    bool operator()(Node* l, Node* r) 
    {
        return l->frequency > r->frequency;
    }
};

void BCode(Node* root, string bincode, unordered_map<char, string>& HuffCode) {
    if (root == NULL) return;

    if (!root->left && !root->right) {
        HuffCode[root->letter] = bincode;
    }

    BCode(root->left, bincode + "0", HuffCode);
    BCode(root->right, bincode + "1", HuffCode);
}

void HuffmanCoder(const char* input_text = "text.txt", const char* output_text = "chipher text.txt") {
    FILE* input;
    fopen_s(&input, input_text, "r");
    if (!input) exit(1);

    unsigned char lett;
    map<char, int> freq;
    while (!feof(input)) {
        lett = fgetc(input);
        if (!feof(input)) {
            freq[lett]++;
        }
    }

    char count_letters = freq.size();

    fclose(input);

    FILE* output;
    fopen_s(&output, output_text, "wb +");
    fopen_s(&input, input_text, "rb");
    if (!input || !output) exit(1);

    fputc(count_letters, output);
    for (auto pair : freq) {
        fputc(pair.first, output);
        fputc(pair.second, output);
    }

    priority_queue<Node*, vector<Node*>, check> pq;

    for (auto pair : freq) {
        pq.push(AddNode(pair.first, pair.second, nullptr, nullptr));
    }

    Node* temp;

    while (pq.size() != 1)
    {

        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        if (left->frequency == right->frequency && left->letter < right->letter) {
            temp = left;
            left = right;
            right = temp;
        }

        int sum = left->frequency + right->frequency;
        pq.push(AddNode('\0', sum, left, right));
    }

    Node* root = pq.top();

    unordered_map<char, string> HuffCode;
    BCode(root, "", HuffCode);

    cout << "Huffman Codes are :\n" << '\n';
    for (auto pair : HuffCode) {
        cout << pair.first << " " << pair.second << '\n';
    }

    char fkey, symbol = 0;
    int countOFbits = 0;

    while (!feof(input))
    {
        fkey = fgetc(input);
        if (!feof(input))
        {
            if (countOFbits + HuffCode[fkey].length() <= 8)
            {
                for (int i = 0; i < HuffCode[fkey].length(); i++)
                {
                    symbol = (symbol << 1) | (HuffCode[fkey][i] - '0');
                }
                countOFbits += HuffCode[fkey].length();
            }
            else
            {
                if (HuffCode[fkey].length() - countOFbits + 8 <= 8)
                {
                    for (int j = 0; j < 8 - countOFbits; j++)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][j] - '0');
                    }
                    fputc(symbol, output);
                    symbol = 0;
                    for (int j = 8 - countOFbits; j < HuffCode[fkey].length(); j++)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][j] - '0');
                    }
                    countOFbits = HuffCode[fkey].length() - 8 + countOFbits;
                }
                else
                {
                    for (int j = 0; j < 8 - countOFbits; j++)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][j] - '0');
                    }
                    
                    fputc(symbol, output);
                    int i = 8 - countOFbits;
                    countOFbits = 0;
                    symbol = 0;
                    
                    for (i; i < HuffCode[fkey].length(); i++)
                    {
                        symbol = (symbol << 1) | (HuffCode[fkey][i] - '0');
                        countOFbits++;
                        if (countOFbits == 8)
                        {
                            fputc(symbol, output);
                            countOFbits = 0;
                            symbol = 0;
                        }
                    }
                }
            }
        }
        else
        {
            symbol = symbol << (8 - countOFbits);
            fputc(symbol, output);
        }
    }

    fclose(input);
    fclose(output);
}

void compressValue(const char* input_text = "text.txt", const char* output_text = "chipher text.txt")
{
    long long file_size = 0;
    long long compress_size = 0;

    struct stat s1 {};
    struct stat s2 {};
    
    if (!stat(input_text, &s1)) {
        file_size = s1.st_size;
    }
    else {
        perror("compressValue ERROR ");
    }
    if (!stat(output_text, &s2)) {
        compress_size = s2.st_size;
    }
    else {
        perror("compressValue ERROR ");
    }

    cout << "\nCompress value is: " << (compress_size + 0.0) / file_size << "\n";
}

int main() 
{
    HuffmanCoder(); 
    compressValue();
}
