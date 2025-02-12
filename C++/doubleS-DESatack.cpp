#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

class Keys
{
    int P10[10] = { 3,5,2,7,4,10,1,9,8,6 };
    int P8[8] = { 6,3,7,4,8,5,10,9 };
    vector<bool> left;
    vector<bool> right;

    vector<bool> MasterKey;
    vector<bool> K0;
    vector<bool> K1;
protected:
    vector<bool> Permutation(vector<bool> key, size_t size, bool Knum = false)
    {
        int* P_rule;
        bool isMaster = size - 8;
        if (isMaster) P_rule = P10;
        else P_rule = P8;

        vector<bool> result; result.resize(size);

        for (int i = 0; i < size; i++)
        {
            result[i] = key[P_rule[i] - 1];
        }

        return result;
    }

    void Split(vector<bool> key)
    {
        size_t size = 5;

        left.resize(size);
        right.resize(size);

        for (int i = 0; i < size; i++)
        {
            left[i] = key[i];
            right[i] = key[size + i];
        }
    }

    // side == false (left) | side == true (right)
    void leftRotate(int bits, bool side)
    {
        vector<bool> save; save.resize(bits);
        vector<bool> *it;

        if (side) it = &right;
        else it = &left;

        for (int i = 0; i < bits; i++)
        {
            save[i] = (*it)[i];
        }

        int size = 5 - bits;
        for (int i = 0; i < size; i++)
        {
            (*it)[i] = (*it)[i + bits];
        }

        for (int i = 0; i < bits; i++)
        {
            (*it)[size + i] = save[i];
        }
    }

    vector<bool> combine()
    {
        size_t size = 10;
        int mid = 5;

        vector<bool> result; result.resize(size);

        for (int i = 0; i < mid; i++)
        {
            result[i] = left[i];
            result[mid + i] = right[i];
        }

        return result;
    }

    void rKeyGen()
    {
        vector<bool> temp = Permutation(MasterKey, 10);

        Split(temp);

        leftRotate(1, false);
        leftRotate(1, true);

        temp.clear();
        temp = combine();
        K0 = Permutation(temp, 8);

        leftRotate(2, false);
        leftRotate(2, true);

        temp.clear();
        temp = combine();
        K1 = Permutation(temp, 8);
    }

public:
    Keys()
    {
        MasterKey.resize(10);

        for (int i = 0; i < 10; i++)
        {
            MasterKey[i] = rand() % 2;
        }

        rKeyGen();
    }

    Keys(string str)
    {
        MasterKey.resize(10);

        for (int i = 0; i < 10; i++)
        {
            MasterKey[i] = str[i] - 48;
        }

        rKeyGen();
    }

    Keys(int key)
    {
        MasterKey.resize(10);
        int mask = 512;

        for (int i = 0; i < 10; i++, mask >>= 1)
        {
            if(key & mask) MasterKey[i] = true;
                else MasterKey[i] = false;
        }

        rKeyGen();
    }

    void printKeys()
    {
        cout << "---------------------------------------------------------------------------------" << endl << "Master key: ";
        for (int i = 0; i < 10; i++)
            cout << MasterKey[i];

        cout << endl << "Round key K1: ";
        for (int i = 0; i < 8; i++)
            cout << K0[i];

        cout << endl << "Round key K2: ";
        for (int i = 0; i < 8; i++)
            cout << K1[i];

        cout << endl << "---------------------------------------------------------------------------------" << endl << endl;
    }

    vector<bool> getMaster()
    {
        return MasterKey;
    }

    vector<bool> getK1()
    {
        return K0;
    }

    vector<bool> getK2()
    {
        return K1;
    }
};

class S_DES
{
    vector<bool> plaintext;
    vector<bool> ciphertext;
    vector<bool> left;
    vector<bool> right;

    int IP[8] = { 2,6,3,1,4,8,5,7 };
    int IP1[8] = { 4,1,3,5,7,2,8,6 };
    int S0[4][4] = { {1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,3,2} };
    int S1[4][4] = { {0,1,2,3},{2,0,1,3},{3,0,1,0},{2,1,0,3} };
    int Expanrule[8] = { 4,1,2,3,2,3,4,1 };
    int P4[4] = { 2,4,3,1 };

protected:
    vector<bool> Permutation(vector<bool> text, size_t size, int* P_rule)
    {
        vector<bool> result; result.resize(size);

        for (int i = 0; i < size; i++)
        {
            result[i] = text[P_rule[i] - 1];
        }

        return result;
    }

    void Split8(vector<bool> text)
    {
        for (int i = 0; i < 4; i++)
        {
            left[i] = text[i];
            right[i] = text[4 + i];
        }
    }

    vector<bool> XOR(vector<bool> l, vector<bool> r, size_t size)
    {
        vector<bool> result; result.resize(size);

        for (int i = 0; i < size; i++)
        {
            result[i] = l[i] ^ r[i];
        }

        return result;
    }

    vector<bool> combine(vector<bool> l, vector<bool> r, size_t size)
    {
        vector<bool> result; result.resize(size);
        int mid = size / 2;

        for (int i = 0; i < mid; i++)
        {
            result[i] = l[i];
            result[mid + i] = r[i];
        }

        return result;
    }

    vector<bool> int2bool(int num)
    {
        vector<bool> result; result.resize(2);

        for (int i = 0; i < 2; i++)
        {
            result[i] = num & (2 >> i);
        }

        return result;
    }

    vector<bool> S_box(vector<bool> text)
    {
        vector<bool> l; l.resize(4);
        vector<bool> r; r.resize(4);
        
        for (int i = 0; i < 4; i++)
        {
            l[i] = text[i];
            r[i] = text[4 + i];
        }

        int S0_row = l[0] & 1; 
        S0_row <<= 1; 
        S0_row += l[3] & 1;

        int S0_col = l[1] & 1;
        S0_col <<= 1; 
        S0_col += l[2] & 1;

        int S1_row = r[0] & 1;
        S1_row <<= 1;
        S1_row += r[3] & 1;

        int S1_col = r[1] & 1;
        S1_col <<= 1; 
        S1_col += r[2] & 1;

        vector<bool> S0_res = int2bool(S0[S0_row][S0_col]);
        vector<bool> S1_res = int2bool(S1[S1_row][S1_col]);

        return combine(S0_res, S1_res, 4);
    }

    void Swap()
    {
        bool tmp;
        for (int i = 0; i < 4; i++)
        {
            tmp = left[i];
            left[i] = right[i];
            right[i] = tmp;
        }
    }
public:
    S_DES()
    {
        size_t size = 8;

        plaintext.resize(size);
        left.resize(4);
        right.resize(4);

        for (int i = 0; i < size; i++)
        {
            plaintext[i] = rand() % 2;
        }
    }

    S_DES(string str)
    {
        size_t size = 8;

        plaintext.resize(size);
        left.resize(4);
        right.resize(4);

        for (int i = 0; i < size; i++)
        {
            plaintext[i] = str[i] - 48;
        }
    }

    S_DES(const S_DES &copy)
    {
        plaintext.resize(8);
        ciphertext.resize(8);
        left.resize(4);
        right.resize(4);

        if (!copy.ciphertext.empty())
        {
            for (int i = 0; i < 8; i++)
            {
                plaintext[i] = copy.plaintext[i];
                ciphertext[i] = copy.ciphertext[i];
            }
        }
        else
        {
            for (int i = 0; i < 8; i++)
            {
                plaintext[i] = copy.plaintext[i];
            }
        }
    }

    S_DES(int num)
    {
        size_t size = 8;

        plaintext.resize(size);
        left.resize(4);
        right.resize(4);

        int mask = 128;
        for (int i = 0; i < size; i++, mask >>= 1)
        {
            if (num & mask) plaintext[i] = true;
                else plaintext[i] = false;
        }
    }

    void printArr(vector<bool> arr, size_t size)
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i]) cout << '1';
            else cout << '0';
        }
    }

    vector<bool> Encrypt(Keys k, vector<bool> *text = NULL)
    {
        if (text) plaintext = *text;

        vector<bool> tmp = Permutation(plaintext, 8, IP);
        //cout << "After IP: "; printArr(tmp, 8); cout << endl;


        Split8(tmp);
        //cout << "After Split8: "; printArr(left, 4); cout << " "; printArr(right, 4); cout << endl;
        //Round 1
        tmp.clear();
        tmp = Permutation(right, 8, Expanrule);
        //cout << "After Expan: "; printArr(tmp, 8); cout << endl;
        
        vector<bool> K1 = k.getK1();
        tmp = XOR(tmp, K1, 8);
        //cout << "After XOR: "; printArr(tmp, 8); cout << endl;

        tmp = S_box(tmp);
        //cout << "After SBOX: "; printArr(tmp, 4); cout << endl;

        tmp = Permutation(tmp, 4, P4);
        //cout << "After P4: "; printArr(tmp, 4); cout << endl;

        left = XOR(left, tmp, 4);
        //cout << "After XOR: "; printArr(left, 4); cout << endl;

        Swap();
        //cout << "After Swap: "; printArr(left, 4); cout << " "; printArr(right, 4); cout << endl << "Round 2" << endl;
        
        //Round 2
        tmp.clear();
        tmp = Permutation(right, 8, Expanrule);
        //cout << "After Expan: "; printArr(tmp, 8); cout << endl;

        vector<bool> K2 = k.getK2();
        tmp = XOR(tmp, K2, 8);
        //cout << "After XOR: "; printArr(tmp, 8); cout << endl;

        tmp = S_box(tmp);
        //cout << "After SBOX: "; printArr(tmp, 4); cout << endl;

        tmp = Permutation(tmp, 4, P4);
        //cout << "After P4: "; printArr(tmp, 4); cout << endl;

        left = XOR(left, tmp, 4);
        //cout << "After XOR: "; printArr(left, 4); cout << endl;

        tmp.clear();
        tmp = combine(left, right, 8);
        //cout << "After combine: "; printArr(tmp, 8); cout << endl;

        if (!ciphertext.empty()) ciphertext.clear();
        ciphertext = Permutation(tmp, 8, IP1);

        return ciphertext;
    }

    vector<bool> Decrypt(Keys k, vector<bool> *text = NULL)
    {
        if (text) ciphertext = *text;

        vector<bool> tmp = Permutation(ciphertext, 8, IP);

        Split8(tmp);

        //Round 1
        tmp.clear();
        tmp = Permutation(right, 8, Expanrule);

        vector<bool> K2 = k.getK2();
        tmp = XOR(tmp, K2, 8);

        tmp = S_box(tmp);

        tmp = Permutation(tmp, 4, P4);

        left = XOR(left, tmp, 4);

        Swap();

        //Round 2
        tmp.clear();
        tmp = Permutation(right, 8, Expanrule);

        vector<bool> K1 = k.getK1();
        tmp = XOR(tmp, K1, 8);

        tmp = S_box(tmp);

        tmp = Permutation(tmp, 4, P4);

        left = XOR(left, tmp, 4);

        //Swap();

        tmp.clear();
        tmp = combine(left, right, 8);

        if (!plaintext.empty()) plaintext.clear();
        plaintext = Permutation(tmp, 8, IP1);

        return plaintext;
    }

    void printResult()
    {
        cout << "---------------------------------------------------------------------------------" << endl << "Plaintext:  ";
        for (int i = 0; i < 8; i++)
        {
            if (plaintext[i]) cout << '1';
                else cout << '0';
        }

        if (!ciphertext.empty())
        {
            cout << endl << "Ciphertext: ";
            for (int i = 0; i < 8; i++)
            {
                if (ciphertext[i]) cout << '1';
                else cout << '0';
            }
        }

        cout << endl << "---------------------------------------------------------------------------------" << endl << endl;
    }

    vector<bool> DoubleEnc(Keys k1, Keys k2)
    {
        Encrypt(k1);

        vector<bool> save = plaintext;
        plaintext = ciphertext;

        Encrypt(k2);

        plaintext = save;

        return ciphertext;
    }

    vector<bool> DoubleDec(Keys k1, Keys k2)
    {
        Decrypt(k2);

        vector<bool> save = ciphertext;
        ciphertext = plaintext;

        Decrypt(k1);

        ciphertext = save;

        return plaintext;
    }

    vector<bool> GetPlain()
    {
        return plaintext;
    }
};

struct key_pair
{
    Keys k1;
    Keys k2;
};

Keys* MakeSet()
{
    Keys* result = new Keys[1024];

    for (int i = 0; i < 1024; i++)
    {
        Keys tmp(i);
        result[i] = tmp;
    }

    return result;
}

void MITM()
{
    Keys k1("0010011111"), k2("1000000001");
    key_pair keys; keys.k1 = k1; keys.k2 = k2;
    vector<key_pair> vec;
    //vec.push_back(keys);
    
    S_DES text("11010001");
    vector<bool> cipher = text.DoubleEnc(keys.k1, keys.k2);
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    cout << "                              DEFAULT CONFIGURATION\n\n" << "First Master key:" << endl;
    keys.k1.printKeys();
    cout << "Second Master key:" << endl;
    keys.k2.printKeys();
    cout << "Intercepted plain-/ciphertext:" << endl;
    text.printResult();
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;

    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;
    cout << "                                FINDING KEY PAIRS\n\n";

    Keys* keysSet = MakeSet();

    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 1024; i++)
    {
        for (int j = 0; j < 1024; j++)
        {
            S_DES copy(text);
            
            vector<bool> T1 = copy.Encrypt(keysSet[i]);
            vector<bool> T2 = copy.Decrypt(keysSet[j], &cipher);
            bool equal = true;

            /**if (i == 762 && j == 266)
            {
                cout << "=================================================================================\n" << "Debug:\n";
                keysSet[i].printKeys();
                keysSet[j].printKeys();

                cout << "T1: ";
                for (int o = 0; o < 8; o++)
                {
                    cout << T1[o];
                }

                cout << endl << "T2: ";
                for (int o = 0; o < 8; o++)
                {
                    cout << T2[o];
                }
                cout << "\n\n=================================================================================\n";
            }/**/

            for(int k = 0; k < 8 && equal; k++)
            {
                if (T1[k] + T2[k] == 1) equal = false;
            }

            if (equal)
            {
                key_pair variant;
                variant.k1 = keysSet[i];
                variant.k2 = keysSet[j];

                //if (i == 762 && j == 266) cout << "OK" << endl;

                vec.push_back(variant);
            }
        }
    }
    auto end = std::chrono::steady_clock::now();

    size_t size = vec.size();
    cout << "There are " << size << " potentially key pairs" << endl;
    auto elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    std::cout << "Finding takes: " << elapsed_sec.count() << " seconds\n\n";

    vector<key_pair> result;

    cout << "                               FINDING ACTUAL KEYS\n\n";

    int count = 0;
    begin = std::chrono::steady_clock::now();
    while (result.size() != 1 && count < 256)
    {
        S_DES validate(count); count++;
        vector<bool> saveCipher = validate.DoubleEnc(keys.k1, keys.k2);

        cout << "Current plain-/ciphertext is: " << endl;
        validate.printResult();

        for (int i = 0; i < size; i++)
        {
            S_DES copy(validate);

            vector<bool> T1 = copy.Encrypt(vec[i].k1);
            vector<bool> T2 = copy.Decrypt(vec[i].k2, &saveCipher);

            bool equal = true;
            for (int k = 0; k < 8 && equal; k++)
            {
                if (T1[k] != T2[k])
                {
                    equal = false;
                    if (!((T1[k] == 1 && T2[k] == 0) || (T1[k] == 0 && T2[k] == 1)))
                    {
                        cout << T1[k] << " =! " << T2[k] << endl;
                    }
                }  
            }

            if (equal)
            {
                result.push_back(vec[i]);
            }
        }

        if (result.size() == 0)
        {
            cout << "ERROR: result size is 0";
            exit(1);
        }

        if (result.size() != 1 && result.size() != 0)
        {
            vec = result;
            size = result.size();
            result.clear();
            cout << "Current size is: " << size << endl << endl;
        }
        //cout << "Current size is: " << size << endl;
    }
    end = std::chrono::steady_clock::now();

    cout << count << " texts were used" << endl;
    elapsed_sec = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    std::cout << "Finding takes: " << elapsed_sec.count() << " seconds\n\n";

    cout << "The result is:" << endl;
    if (result.size() == 1)
    {
        result[0].k1.printKeys();
        result[0].k2.printKeys();
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            vec[i].k1.printKeys();
            vec[i].k2.printKeys();
        }
    }

    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
}

void Double_SDES_test()
{
    int mismatch = 0;
    cout << "Interations:" << endl;
    for (int i = 0; i < 256; i++)
    {
        S_DES test(i);
        for (int j = 0; j < 1024; j++)
        {
            Keys key(j);

            vector<bool> orig = test.GetPlain();
            test.Encrypt(key);
            vector<bool> pl = test.Decrypt(key);

            bool equal = true;
            for (int h = 0; h < 8 && equal; h++)
            {
                if (orig[h] != pl[h])
                {
                    equal = false;
                    mismatch++;

                    cout << orig[h] << " != " << pl[h] << endl;
                }
            }
        }
        cout << i << ") " << mismatch << endl;
    }
    cout << "There are " << mismatch << " mismatches" << endl;
}

int main()
{
    srand(time(NULL));
    
    MITM();

    return 0;
}
