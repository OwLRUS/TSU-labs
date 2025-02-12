#include <iostream>

using namespace std;

class Key
{
    int key;
public:
    friend class SPN;
    Key()
    {
        //srand(time(NULL));

        key = rand() % 512;
        /*
        int mask = 1;
        for (int i = 0; i < 9; i++, mask <<= 1)
        {
            bool flag = rand() % 2;

            if (flag)
            {
                key |= mask;
            }
        }
        */
    }
    Key(int in)
    {
        key = in;
    }
    Key(string str)
    {
        if (str.size() > 9)
        {
            cout << "Error{Key(string)}: size > 9";
            exit(1);
        }
        
        int mask = 1;
        for (int i = 8; i >= 0; i--, mask <<= 1)
        {
            if (str[i] == '1')
            {
                key |= mask;
            }
        }
    }
    void print()
    {
        cout << " Key:        ";
        int mask = 256;
        for (int i = 0; i < 9; i++, mask >>= 1)
        {
            if (key & mask)
            {
                cout << '1';
            }
            else
            {
                cout << '0';
            }
        }
        cout << " (" << key << ")" << endl;
    }
};

class SPN
{
    int plain;
    int cipher;

    int S[8] = { 5, 0, 1, 4, 7, 2, 6, 3 };
    int invS[8] = { 1, 2, 5, 7, 3, 0, 6, 4 };
    int P[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
public:
    SPN()
    {
        //srand(time(NULL));

        plain = rand() % 512;
    }
    SPN(int in)
    {
        plain = in;
    }
    SPN(string str)
    {
        if (str.size() > 9)
        {
            cout << "Error{SPN(string)}: size > 9";
            exit(1);
        }

        int mask = 1;
        for (int i = 8; i >= 0; i--, mask <<= 1)
        {
            if (str[i] == '1')
            {
                plain |= mask;
            }
        }
    }
    void Encrypt(Key K)
    {
        int XOR = plain;
        for (int round = 0; round < 3; round++)
        {
            // XOR
            cipher = XOR ^ K.key;
            //S-BOX
            int blocks[3] = {};
            for (int i = 0; i < 3; i++)
            {
                blocks[i] = cipher % 8;

                blocks[i] = S[blocks[i]];

                cipher >>= 3;
            }
            //cout << "Blocks: " << blocks[0] << ", " << blocks[1] << ", " << blocks[2] << endl;
            //Permutation
            if (round != 2)
            {
                int mask = 1;
                for (int i = 0; i < 9; i++, mask <<= 1)
                {
                    if (mask == 8) mask = 1;
                    if (blocks[i / 3] & mask)
                    {
                        cipher |= 1 << P[i];
                    }
                }
            }
            else
            {
                for (int i = 2; i > 0; i--)
                {
                    cipher |= blocks[i];
                    cipher <<= 3;
                }
                cipher |= blocks[0];

                cipher = cipher ^ K.key;
                return;
            }

            XOR = cipher;
        }
    }
    void Encrypt(Key K, int in)
    {
        plain = in;

        Encrypt(K);
    }
    void Decrypt(Key K)
    {
        int XOR = cipher ^ K.key; int blocks[3] = {};
        for (int round = 0; round < 3; round++)
        {
            blocks[0] = 0; blocks[1] = 0; blocks[2] = 0;
            //Permutation
            if (round)
            {
                for (int i = 0; i < 9; i++)
                {
                    if (XOR & (1 << i))
                    {
                        blocks[P[i] / 3] |= 1 << (P[i] % 3);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    blocks[i] = XOR % 8;

                    XOR >>= 3;
                }
            }
            //cout << "Blocks: " << blocks[0] << ", " << blocks[1] << ", " << blocks[2] << endl;
            //S-BOX
            XOR = 0;
            for (int i = 2; i > 0; i--)
            {
                blocks[i] = invS[blocks[i]];

                XOR |= blocks[i];
                XOR <<= 3;
            }
            blocks[0] = invS[blocks[0]];
            //cout << "Blocks: " << blocks[0] << ", " << blocks[1] << ", " << blocks[2] << endl;
            XOR |= blocks[0];
            
            // XOR
            XOR = XOR ^ K.key;
            //cout << "XOR: " << XOR << endl;
        }

        plain = XOR;
    }
    void Decrypt(Key K, int in)
    {
        cipher = in;

        Decrypt(K);
    }

    void printConf(Key K)
    {
        cout << "\n---------------------------------------------------------------------------------\n";
        cout << "                                  CONFIGURATION\n\n";
        cout << " Plaintext:  ";
        int mask = 256;
        for (int i = 0; i < 9; i++, mask >>= 1)
        {
            if (plain & mask)
            {
                cout << '1';
            }
            else
            {
                cout << '0';
            }
        }
        cout << " (" << plain << ")\n";
        cout << " Ciphertext: ";
        mask = 256;
        for (int i = 0; i < 9; i++, mask >>= 1)
        {
            if (cipher & mask)
            {
                cout << '1';
            }
            else
            {
                cout << '0';
            }
        }
        cout << " (" << cipher << ")\n";
        K.print();
        cout << "\n---------------------------------------------------------------------------------" << endl;
    }

    int getPlain()
    {
        return plain;
    }

    int getCipher()
    {
        return cipher;
    }
};

void SPN_test()
{
    bool equal = true; int mismatch = 0;
    for (int i = 0; i < 512; i++)
    {
        Key K(i);
        for (int j = 0; j < 512; j++)
        {
            SPN tst(j);
            tst.Encrypt(K);
            tst.Decrypt(K);
            if (tst.getPlain() != j)
            {
                equal = false;
                mismatch++;
            }
        }
    }

    if (equal) cout << "OK\n";
        else cout << "There are " << mismatch << " mismatches" << endl;
}

bool isTgreater(Key K, int n = 100000)
{
    string X, Y;
    cout << "Enter X coefs: ";
    cin >> X;
    cout << "Now enter Y coefs: ";
    cin >> Y;

    size_t X_size = X.size();
    size_t Y_size = Y.size();

    int T = 0;
    for (int i = 0; i < n; i++)
    {
        SPN test;
        test.Encrypt(K);
        bool bT = false;
        int tmp = test.getPlain();
        for (int x = 0; x < X_size; x++)
        {
            if (tmp & (1 << (X[x] - 49))) bT ^= 1;
            else bT ^= 0;
        }
        tmp = test.getCipher();
        for (int y = 0; y < Y_size; y++)
        {
            if (tmp & (1 << (Y[y] - 49))) bT ^= 1;
            else bT ^= 0;
        }

        if (!bT) T++;
    }

    cout << "T:" << T << endl;

    return T > n / 2;
}

int main()
{
    srand(time(NULL));

    Key K(213); // 011010101
                // 987654321
    bool result = isTgreater(K);

    cout << "Result is " << result;

    return 0;
}
