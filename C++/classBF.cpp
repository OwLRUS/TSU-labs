#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <chrono>

using namespace std;

class BF
{
    unsigned int n; // число переменных
    unsigned int nw; // длина массива
    unsigned int* f = NULL;
public:
    BF();
    BF(int count, int value);
    BF(BF& copy);
    BF(string s);

    friend ostream& operator<<(ostream& r, BF& out);
    BF operator=(const BF& copy);
    bool operator==(BF& sec);

    ~BF()
    {
        delete[] f;
        f = NULL;
    }

    int weight();
    BF MobiusF();
    void ANF();
    int deg();
    vector<int> WHtransLong();
    vector<int> WHtransShort();
    int* WHtransInt();
    unsigned int corIm();
    unsigned int ZakrevAlg(unsigned int);
    int nonLin();
    void BAA();
};

BF::BF()
{
    n = 1;
    nw = 1;
    f = new unsigned int[1];
    f[0] = 0;
}

BF::BF(int count, int value)
{
    n = count;
    nw = ((unsigned int)(1 << n) + 31) >> 5;

    f = new unsigned int[nw];
    for (int i = 0; i < nw; i++)
    {
        f[i] = 0;
    }

    if (value == 1)
    {
        if (n < 5)
        {
            f[0] = (unsigned int)(1 << (1 << n)) - 1;
        }
        else
        {
            for (int i = 0; i < nw; i++)
            {
                f[i] = ~0;
            }
        }
    }
    else if (value == 2)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> dis(0, UINT_MAX);

        if (n < 5)
        {
            f[0] = dis(gen);
            f[0] = f[0] >> (32 - (1 << n));
        }
        else
        {
            for (int i = 0; i < nw; i++)
            {
                f[i] = dis(gen);
            }
        }
    }
    else if (value != 0)
    {
        cout << "Error {BF(int, int)}: Wrong type input\n";
        delete[] f;
        exit(-1);
    }
}

BF::BF(BF& copy)
{
    n = copy.n;
    nw = copy.nw;

    f = new unsigned int[nw];
    for (int i = 0; i < nw; i++)
    {
        f[i] = copy.f[i];
    }
}

BF::BF(string s)
{
    int size = s.size();
    if (size != 0 && (size & (size - 1)) == 0)
    {
        for (int i = 0; i < size; i++)
        {
            if (s[i] != '0' && s[i] != '1')
            {
                cout << "Error {BF(string)}: Wrong signs input\n";
                exit(-1);
            }
        }

        nw = ((size - 1) >> 5) + 1;
        f = new unsigned int[nw];
        for (int i = 0; i < nw; i++)
        {
            f[i] = 0;
        }

        unsigned int mask = 1;
        int index = nw - 1;

        for (int i = size - 1; i >= 0; i--, mask = mask << 1)
        {
            if (!mask)
            {
                mask = 1;
                index--;
            }
            if (s[i] == '1')
            {
                f[index] |= mask;
            }
        }

        int _n = 0;
        while (size)
        {
            size = size >> 1;
            _n++;
        }
        n = _n - 1;
    }
    else
    {
        cout << "Error {BF(string)}: Wrong string length\n";
        exit(-1);
    }
}

ostream& operator<<(ostream& r, BF& out)
{
    string s; bool flag = false;

    for (int i = 0; i < out.nw; i++)
    {
        unsigned int mask = 1 << 31;
        for (int j = 0; j < 32; j++)
        {
            if (out.f[i] & mask)
            {
                s += '1';
                flag = true;
            }
            else if (flag) s += '0';

            mask = mask >> 1;
        }
    }

    if (s.empty()) s += '0';

    if (s.size() != (1 << out.n))
    {
        int diff = (1 << out.n) - s.size();
        string s_diff;
        for (int i = 0; i < diff; i++)
        {
            s_diff += '0';
        }
        s_diff += s;
        s = s_diff;
    }

    r << s;
    return r;
}

BF BF::operator=(const BF& copy)
{
    if (this == &copy) return *this;

    n = copy.n;
    nw = copy.nw;

    delete[] f;
    f = new unsigned int[nw];
    for (int i = 0; i < nw; i++)
    {
        f[i] = copy.f[i];
    }

    return *this;
}

bool BF::operator==(BF& sec)
{
    if (nw != sec.nw) return false;

    for (int i = 0; i < nw; i++) if (f[i] != sec.f[i]) return false;

    return true;
}

int BF::weight()
{
    int cart = 0;

    for (int i = 0; i < nw; i++)
    {
        unsigned int tmp = f[i];
        while (tmp)
        {
            tmp &= tmp - 1;
            cart++;
        }
    }

    return cart;
}

unsigned int weightNum(unsigned int num)
{
    unsigned int cart = 0;

    while (num)
    {
        num &= num - 1;
        cart++;
    }

    return cart;
}

BF BF::MobiusF()
{
    BF result(*this);

    for (int i = 0; i < nw; i++)
    {
        result.f[i] = ((result.f[i] << 1) & 0xaaaaaaaa) ^ result.f[i];
        result.f[i] = ((result.f[i] << 2) & 0xcccccccc) ^ result.f[i];
        result.f[i] = ((result.f[i] << 4) & 0xf0f0f0f0) ^ result.f[i];
        result.f[i] = ((result.f[i] << 8) & 0xff00ff00) ^ result.f[i];
        result.f[i] = ((result.f[i] << 16) & 0xffff0000) ^ result.f[i];
    }
    if (n < 5)
        result.f[0] &= (1 << (1 << n)) - 1;

    for (int k = 1; k < nw; k <<= 1)
        for (int j = 0; j < nw; j += (k << 1))
            for (int s = j; s < k + j; s++)
                result.f[s + k] ^= result.f[s];

    return result;
}

void BF::ANF()
{
    BF Mob; Mob = MobiusF();

    cout << "ANF: ";
    bool first = true;
    for (int i = 0; i < Mob.nw; i++)
    {
        for (int j = 0; j < (1 << Mob.n); j++)
        {
            int mask = 1 << j;
            if (Mob.f[i] & mask)
            {
                if (!first) cout << "+ ";
                for (int k = 0; k < Mob.n; k++)
                {
                    int mask_ = 1 << k;
                    if (j & mask_)
                    {
                        cout << "x" << k << " ";
                        first = false;
                    }
                }
            }
        }
    }
}

int BF::deg()
{
    int result = 0, current;

    BF Mob; Mob = MobiusF();

    for (int i = 0; i < Mob.nw; i++)
    {
        for (int j = 0; j < (1 << Mob.n); j++)
        {
            int mask = 1 << j;
            if (Mob.f[i] & mask)
            {
                current = 0;
                for (int k = 0; k < Mob.n; k++)
                {
                    int mask_ = 1 << k;
                    if (j & mask_)
                    {
                        current++;
                    }
                }
                if (current > result) result = current;
            }
        }
    }

    return result;
}

vector<int> BF::WHtransLong()
{
    unsigned int size = 1 << n;
    vector<int> result;

    for (unsigned int i = 0; i < size; i++)
    {
        if ((f[i >> 5] & (1 << (i % 32))) != 0)
            result.push_back(-1);
        else
            result.push_back(1);
    }

    for (int k = 1; k < size; k = k << 1)
        for (int j = 0; j < size; j += (k << 1))
            for (int s = j; s < k + j; s++)
            {
                int copy = result[s];
                result[s] = result[s] + result[s + k];
                result[s + k] = copy - result[s + k];
            }

    return result;
}

vector<int> BF::WHtransShort()
{
    unsigned int size = 1 << n;
    vector<int> result;
    result.resize(size);

    for (unsigned int i = 0; i < size; i++)
    {
        if ((f[i >> 5] & (1 << (i % 32))) != 0)
            result[i] = -1;
        else
            result[i] = 1;
    }

    for (int k = 1; k < size; k = k << 1)
        for (int j = 0; j < size; j += (k << 1))
            for (int s = j; s < k + j; s++)
            {
                int copy = result[s];
                result[s] += result[s + k];
                result[s + k] = copy - result[s + k];
            }

    return result;
}

int* BF::WHtransInt()
{
    int size = 1 << n;
    int* result = new int[size];

    for (unsigned int i = 0; i < size; i++)
    {
        if ((f[i >> 5] & (1 << (i % 32))) != 0)
            result[i] = -1;
        else
            result[i] = 1;
    }

    for (int k = 1; k < size; k = k << 1)
        for (int j = 0; j < size; j += (k << 1))
            for (int s = j; s < k + j; s++)
            {
                int copy = result[s];
                result[s] += result[s + k];
                result[s + k] = copy - result[s + k];
            }

    return result;
}

unsigned int BF::corIm()
{
    vector<int> WH = WHtransShort();

    for (int k = 1; k <= n; k++)
    {
        unsigned int a = ((1 << k) - 1) << (n - k);
        unsigned int end = (1 << k) - 1;

        while (a != end)
        {
            if (WH[a] == 0)
                a = ZakrevAlg(a);
            else
                return k - 1;
        }

        if (WH[a] != 0) return k - 1;
    }
    return n;
}

unsigned int BF::ZakrevAlg(unsigned int a)
{
    unsigned int b = (a + 1) & a;
    unsigned int c = weightNum((b - 1) ^ a) - 2;
    a = (((((a + 1) ^ a) << 1) + 1) << c) ^ b;

    return a;
}

int BF::nonLin() {
    int* _f = WHtransInt();
    int len = 1 << n;
    int max = 0;
    int maxValue = abs(_f[max]);
    for (int i = 0; i < len; i++) {
        if (abs(_f[i]) > maxValue) {
            max = i;
            maxValue = abs(_f[i]);
        }
    }
    cout << "\nmax(_f):" << max;
    int nonLine = (1 << (n - 1)) - (max >> 1);
    return nonLine;
}

void printBA(int i, unsigned int n) {
    if (i == 0) {
        cout << "0";
        return;
    }
    unsigned int mask = 1;
    for (int j = 0; j < n; j++, mask <<= 1) {
        if ((i & mask) != 0) {
            cout << "x" << j << " + ";
        }
    }
    cout << "\n";
}

void BF::BAA() {
    int* _f = WHtransInt();
    int len = 1 << n;
    unsigned int mask = 1;
    cout << "\nBest Affine Approx:\n";

    int max = 0;
    int maxValue = abs(_f[max]);
    for (int i = 0; i < len; i++) {
        if (abs(_f[i]) > maxValue) {
            max = i;
            maxValue = abs(_f[i]);
        }
    }

    if (_f[max] < 0)
        cout << "1 + ";

    printBA(max, n);
}

int main()
{
    srand(time(NULL));

    //string str;
    //cin >> str;
    BF test(30, 1);

    auto begin = std::chrono::steady_clock::now();
    vector<int> WH = test.WHtransLong(); //cout << "WHtrans: ";
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The long function time: " << elapsed_ms.count() << " ms\n"; //  23.477 ms (26) | 431.166 ms (30) | x7

    begin = std::chrono::steady_clock::now();
    vector<int> WH2 = test.WHtransShort(); //cout << "WHtrans: ";
    end = std::chrono::steady_clock::now();
    elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The short function time: " << elapsed_ms.count() << " ms\n"; // 16.379 ms (26) | 306.292 ms (30) | x5

    unsigned int size;
    begin = std::chrono::steady_clock::now();
    int* WH3 = test.WHtransInt(); //cout << "WHtrans: ";
    end = std::chrono::steady_clock::now();
    elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The int function time: " << elapsed_ms.count() << " ms\n"; //  3.305 ms (26)  | 65.691 ms (30) | x1

    //int WH_size = WH.size();
    /**for (int i = 0; i < WH_size; i++)
    {
        cout << WH[i] << " ";
    }*/

    //unsigned int cor = test.corIm(); cout << "\ncorIm: " << cor << endl;

    return 0;
}
