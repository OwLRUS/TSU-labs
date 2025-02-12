#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <ctime>
#include <string.h>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

typedef unsigned char BASE;
typedef unsigned long long int DBASE;
#define BASE_SIZE (sizeof(BASE)*8)

class BN
{
    int len, maxlen;
    BASE* coef;
public:
    BN();
    BN(int size, bool flag = false);
    BN(const BN&);

    ~BN()
    {
        if (coef) delete[] coef;
        coef = NULL;
    }

    BN& operator = (const BN&);
    friend ostream& operator <<(ostream&, BN&);
    friend istream& operator >>(istream&, BN&);

    bool operator ==(const BN&) const;
    bool operator !=(const BN&) const;
    bool operator <(const BN&) const;
    bool operator <=(const BN&) const;
    bool operator >(const BN&) const;
    bool operator >=(const BN&) const;

    bool operator >(const BASE) const;
    bool operator <(const BASE) const;

    BN operator +(const BN&);
    BN& operator +=(const BN&);
    BN operator +(BASE);
    BN& operator +=(BASE);

    BN operator -(const BN&);
    BN& operator -=(const BN&);
    BN operator -(BASE);
    BN& operator -=(BASE);

    BN operator *(BASE);
    BN operator *(const BN&);
    BN& operator *=(const BN&);

    BN operator /(BASE);
    BASE operator %(BASE);
    void output10();
    void input10();

    BN operator /(BN);
    BN operator %(BN);

    BN pow2();
    BN powY(BASE);
    BN powY(BN);
    BN powYmod(BN, BN);
    BN mod(BN, BN);
    int getLen() { return len; }

    bool isPrime_Fermat(int);
    bool isPrime_MR(int);
    BN generatePrime();

    double getCoef(BN, int t = 5);

    BN sqrt();
    BN qbrt();
    vector<BN> trialDiv();
    BN Always_TD();
    vector<BN> FermasAlg();
    BN GCD(BN);
    BN LCM(BN);
    BN Ro_Pollard();
    BN log(BN);
    BN Ro1_Pollard();
    BN logGelfond(BN, BN);
    BN logPollard(BN, BN);

private:
    BN f_Pollard(BN);
    BN inv(BN);
};

BN::BN()
{
    maxlen = 1, len = 1;

    coef = new BASE[maxlen];
    coef[0] = 0;
}

BN::BN(int size, bool flag)
{
    if (size < 1) exit(EXIT_FAILURE);

    maxlen = size, len = size;
    coef = new BASE[maxlen];

    if (flag)
    {
        //srand(time(NULL));

        for (int i = 0; i < maxlen; i++)
        {
            coef[i] = (rand() << 16) + rand();
            //coef[i] = rand();
        }

        for (int i = maxlen - 1; i > 0 && coef[i] == 0; i--)
        {
            len--;
        }
    }
    else
    {
        len = 1;

        for (int i = 0; i < maxlen; i++)
        {
            coef[i] = 0;
        }
    }
}

BN::BN(const BN& BigN)
{
    if (!BigN.coef || BigN.maxlen < 1 || BigN.len < 1) exit(0);

    maxlen = BigN.maxlen;
    len = BigN.len;

    coef = new BASE[maxlen];
    for (int i = 0; i < maxlen; i++)
    {
        coef[i] = BigN.coef[i];
    }
}

BN& BN::operator=(const BN& BigN)
{
    if (this == &BigN) return *this;
    if (BigN.maxlen < 1 || BigN.len < 1 || !BigN.coef) exit(0);

    delete[] coef;

    maxlen = BigN.maxlen;
    len = BigN.len;

    coef = new BASE[maxlen];
    for (int i = 0; i < maxlen; i++)
    {
        coef[i] = BigN.coef[i];
    }

    return *this;
}

ostream& operator <<(ostream& r, BN& BN)
{
    if (!BN.coef) return r;

    int i = 0, k = BASE_SIZE - 4;
    char* str = new char[BN.len * BASE_SIZE / 4 + 1];
    unsigned int tmp;

    for (int j = BN.len - 1; j >= 0; i++)
    {
        tmp = (BN.coef[j] >> k) & (0xf);
        if (0 <= tmp && tmp <= 9) str[i] = (char)(tmp + '0');
        if (10 <= tmp && tmp <= 15) str[i] = (char)(tmp - 10 + 'A');

        k -= 4;
        if (k < 0)
        {
            k = BASE_SIZE - 4;
            j--;
        }
    }
    /*
    int g = 0, l = 0;
    while(str[g] == '0') g++;
    while(str[l] != '\0')
    {
        str[l] = str[g];
        l++; g++;
    }
    */
    str[BN.len * BASE_SIZE / 4] = '\0';
    r << str << "\n";

    return r;
}

istream& operator >>(istream& r, BN& BN2)
{
    char* str = new char[100];
    gets_s(str, 100);

    int size = (strlen(str) - 1) / (BASE_SIZE / 4) + 1;
    BN BN1(size);
    BN1.len = size;

    unsigned int tmp; int k = 0, j = 0;
    bool flag = true;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        flag = false;
        if ('0' <= str[i] && str[i] <= '9')
        {
            tmp = str[i] - '0';
            flag = true;
        }
        if ('a' <= str[i] && str[i] <= 'f')
        {
            tmp = str[i] - 'a' + 10;
            flag = true;
        }
        if ('A' <= str[i] && str[i] <= 'F')
        {
            tmp = str[i] - 'A' + 10;
            flag = true;
        }
        if (!flag)
        {
            puts("ERROR: Wrong numbers imputed");
            exit(EXIT_FAILURE);
        }

        BN1.coef[j] |= tmp << (k * 4);
        k++;

        if (k >= BASE_SIZE / 4)
        {
            k = 0; j++;
        }
    }

    for (int i = BN1.maxlen - 1; i > 0 && BN1.coef[i] == 0; i--)
    {
        BN1.len--;
    }

    BN2 = BN1;

    delete[] str;
    return r;
}

bool BN::operator ==(const BN& BN2) const
{
    if (len != BN2.len) return false;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (i == -1) return true;
    else return false;
}

bool BN::operator !=(const BN& BN2) const
{
    if (len != BN2.len) return true;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (i == -1) return false;
    else return true;
}

bool BN::operator <(const BN& BN2) const
{
    if (len < BN2.len) return true;
    if (len > BN2.len) return false;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (i >= 0 && coef[i] < BN2.coef[i]) return true;
    else return false;
}

bool BN::operator <=(const BN& BN2) const
{
    if (len < BN2.len) return true;
    if (len > BN2.len) return false;

    int i = len - 1;
    while (i > 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (coef[i] <= BN2.coef[i]) return true;
    else return false;
}

bool BN::operator >(const BN& BN2) const
{
    if (len > BN2.len) return true;
    if (len < BN2.len) return false;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (i >= 0 && coef[i] > BN2.coef[i]) return true;
    else return false;
}

bool BN::operator >=(const BN& BN2) const
{
    if (len > BN2.len) return true;
    if (len < BN2.len) return false;

    int i = len - 1;
    while (i > 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (coef[i] >= BN2.coef[i]) return true;
    else return false;
}
/*
if(*this == BN2 || *this > BN2) return true;
return false;
*/
bool BN::operator >(const BASE x) const
{
    if (len > 1) return true;
    if (coef[0] > x) return true;
    return false;
}

bool BN::operator <(const BASE x) const
{
    if (len > 1) return false;
    if (coef[0] > x) return false;
    return true;
}

BN BN::operator +(const BN& BN2)
{
    if (!BN2.coef) exit(EXIT_FAILURE);

    DBASE tmp; bool k = false; int min_len, max_len;
    BASE* MaxCoef;

    if (len > BN2.len)
    {
        max_len = len + 1;
        min_len = BN2.len;
        MaxCoef = coef;
    }
    else
    {
        max_len = BN2.len + 1;
        min_len = len;
        MaxCoef = BN2.coef;
    }

    BN result(max_len, 0);

    for (int i = 0; i < min_len; i++)
    {
        tmp = (DBASE)coef[i] + (DBASE)BN2.coef[i] + k;
        result.coef[i] = (BASE)tmp;
        k = (bool)(tmp >> BASE_SIZE);
    }

    for (int i = min_len; i < max_len - 1; i++)
    {
        tmp = (DBASE)MaxCoef[i] + k;
        result.coef[i] = (BASE)tmp;
        k = (bool)(tmp >> BASE_SIZE);
    }

    result.coef[max_len - 1] = k;

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator +=(const BN& BN2)
{
    *this = *this + BN2;

    return *this;
}

BN BN::operator+(BASE Num)
{
    BN result(len + 1);
    bool k = false;

    DBASE tmp = (DBASE)(coef[0]) + (DBASE)Num;
    k = (bool)(tmp >> BASE_SIZE);
    result.coef[0] = (BASE)tmp;

    for (int i = 1; i < len; i++)
    {
        tmp = (DBASE)(coef[i]) + k;
        k = (bool)(tmp >> BASE_SIZE);
        result.coef[i] = (BASE)tmp;
    }
    result.coef[len] = k;

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator+=(BASE Num)
{
    *this = *this + Num;
    return *this;
}

BN BN::operator -(const BN& BN2)
{
    if (!BN2.coef) exit(EXIT_FAILURE);
    if (*this < BN2) exit(EXIT_FAILURE);

    //if (len == BN2.len && coef[len-1] < BN2.coef[len-1]) exit(EXIT_FAILURE);

    DBASE tmp; bool k = false;

    BN result(maxlen, 0);

    for (int i = 0; i < BN2.len; i++)
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coef[i];
        tmp = tmp - BN2.coef[i] - k;
        result.coef[i] = (BASE)tmp;
        k = !(bool)(tmp >> BASE_SIZE);
    }

    for (int i = BN2.len; i < maxlen; i++)
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coef[i];
        tmp -= k;
        result.coef[i] = (BASE)tmp;
        k = !(bool)(tmp >> BASE_SIZE);
    }

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator -=(const BN& BN2)
{
    *this = *this - BN2;

    return *this;
}

BN BN::operator -(BASE num)
{
    BN tmp, result;
    tmp += num;
    result = *this - tmp;

    return result;
}

BN& BN::operator -=(BASE num)
{
    *this = *this - num;

    return *this;
}

BN BN::operator *(BASE Num)
{
    DBASE tmp; BASE k = 0;

    //bool extra = (bool)((((DBASE)coef[len - 1]*(DBASE)Num))>>BASE_SIZE);
    //DBASE gg = (DBASE)coef[len - 1] * (DBASE)Num + ((DBASE)1 << BASE_SIZE) - 1;
    //gg = gg >> BASE_SIZE;
    //bool extra = (bool)gg;

    //int t = len;
    //if (extra) t++;
    //BN result(t, 0);

    BN result(maxlen + 1, 0);

    for (int i = 0; i < len; i++)
    {
        tmp = (DBASE)coef[i] * Num + k;
        k = (BASE)(tmp >> BASE_SIZE);
        result.coef[i] = (BASE)tmp;
    }
    result.coef[len] = k;

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN BN::operator*(const BN& BN2)
{
    if (!BN2.coef) exit(EXIT_FAILURE);

    DBASE tmp; BASE k;
    BN result(maxlen + BN2.maxlen, 0);

    for (int i = 0; i < len; i++)
    {
        if (coef[i] != 0)
        {
            k = 0;
            for (int j = 0; j < BN2.len; j++)
            {
                tmp = (DBASE)coef[i] * (DBASE)BN2.coef[j] + (DBASE)result.coef[i + j] + (DBASE)k;
                result.coef[i + j] = (BASE)tmp;
                k = (BASE)(tmp >> BASE_SIZE);
            }
            result.coef[i + BN2.len] = k;
        }
    }


    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator *=(const BN& BN2)
{
    *this = *this * BN2;
    return *this;
}

BN BN::operator /(BASE Num)
{
    DBASE tmp; DBASE r = 0;
    BN result(len, 0);

    for (int j = len - 1; j >= 0; j--)
    {
        tmp = (r << BASE_SIZE) + coef[j];
        result.coef[j] = tmp / Num;
        r = tmp % Num;
    }

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BASE BN::operator %(BASE Num)
{
    DBASE tmp; DBASE r = 0;

    for (int j = len - 1; j >= 0; j--)
    {
        tmp = (r << BASE_SIZE) | coef[j];
        r = tmp % Num;
    }

    return r;
}

void BN::input10()
{
    char* str = new char[1000];
    gets_s(str, 1000);

    BN result((strlen(str) - 1) / (BASE_SIZE / 4) + 1);
    bool flag;

    for (int i = 0; str[i] != '\0'; i++)
    {
        flag = false;
        if (str[i] >= '0' && str[i] <= '9')
        {
            result = result * (BASE)10;
            //result.output10();
            BASE k = (BASE)(str[i] - '0'); // i = 40
            result += k; // 8450486 16720
            // result.coef[0] += k;
            //result.output10(); // -252 вместо +4
             // 64569874894479645641343798798345132449794984311246879641432465
            flag = true;
        }
        if (!flag)
        {
            puts("ERROR: Wrong numbers imputed");
            exit(EXIT_FAILURE);
        }
    }

    *this = result;
}

void BN::output10()
{
    char* str = new char[10000];
    BN tmp(*this), null(maxlen);

    if (tmp == null)
    {
        cout << 0;
    }

    int j;
    for (j = 0; tmp != null; j++)
    {
        str[j] = (char)(tmp % 10) + '0';
        tmp = tmp / 10;
    }

    for (int i = j - 1; i >= 0; i--)
    {
        cout << str[i];
    }
    cout << '\n';

    delete[] str;
}

BN BN::operator /(BN Big)
{
    if (Big.len == 1 && Big.coef[0] == 0) {
        cout << "Error";
        exit(0);
    }

    if (*this < Big) {
        BN q(1);
        return q;
    }

    if (Big.len == 1) return *this / Big.coef[0];

    if (*this == Big) {
        BN q(1);
        q.coef[0] = 1;
        return q;
    }

    int m = len - Big.len;
    DBASE b = ((DBASE)1 << BASE_SIZE);
    DBASE d = b / (DBASE)(Big.coef[Big.len - 1] + (BASE)1); // coef
    BN q(m + 1); // chastnoe
    q.len = m + 1;

    BN U = *this; // delimoe
    BN V = Big; // delitel

    // D1
    // narmalization
    U = U * d;
    V = V * d;
    if (U.len == len) {
        if (U.maxlen == U.len) {
            U.len = U.maxlen;
            delete[]U.coef;
            U.coef = new BASE[U.maxlen];
            for (int i = 0; i < len; i++) {
                U.coef[i] = coef[i];
            }
            U = U * d;
            U.coef[U.len - 1] = 0; // add null to begin
        }
        U.coef[len] = 0;// add null to begin
        U.len = len + 1;
    }

    // D2
    for (int j = m; j >= 0; j--) {
        // D3
        DBASE qq = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) / V.coef[Big.len - 1];
        DBASE rr = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) % V.coef[Big.len - 1];

        // correction
        if (qq == b || ((qq * V.coef[Big.len - 2]) > (b * rr + U.coef[j + Big.len - 2]))) {
            qq--;
            rr = rr + V.coef[Big.len - 1];
            if ((rr < b) && ((qq == b) || (qq * V.coef[Big.len - 2] > b * rr + U.coef[j + Big.len - 2]))) {
                qq--;
                rr = rr + V.coef[Big.len - 1];
            }
        }

        BN u(V.len + 1);
        u.len = V.len + 1;
        for (int i = 0; i < V.len + 1; i++) {
            u.coef[i] = U.coef[j + i];
        }
        // D6
        if (u < V * qq) qq--;
        // D4
        u = u - V * qq;
        // D5
        q.coef[j] = qq;
        // D6
        for (int i = 0; i < V.len + 1; i++) {
            U.coef[j + i] = u.coef[i];
        }
        // D7
    }
    // D8

    q.len = q.maxlen;
    for (int i = q.maxlen - 1; i > 0 && q.coef[i] == 0; i--)
    {
        q.len--;
    }

    return q;
}


BN BN::operator %(BN Big) {
    if (Big.len == 1 && Big.coef[0] == 0) {
        cout << "Error";
        exit(0);
    }

    if (*this < Big) return *this;

    if (Big.len == 1)
    {
        BN result;
        result += *this % Big.coef[0];
        return result;
    }

    if (*this == Big) {
        BN Rem(1);
        return Rem;
    }

    int m = len - Big.len; // length subtraction
    DBASE b = ((DBASE)1 << BASE_SIZE);
    DBASE d = b / (DBASE)(Big.coef[Big.len - 1] + (BASE)1); // normalization coef

    int k = 0;

    BN U = *this;
    BN V = Big;

    U = U * d; // normalization
    V = V * d;

    if (U.len == len) {
        if (U.maxlen == U.len) {
            U.maxlen++;
            U.len = U.maxlen;
            delete[]U.coef;
            U.coef = new BASE[U.maxlen];
            for (int i = 0; i < len; i++) {
                U.coef[i] = coef[i];
            }
            U = U * d;
            U.coef[U.len - 1] = 0;
        }
        U.coef[len] = 0;
        U.len = len + 1;
    }

    for (int j = m; j >= 0; j--) {

        DBASE qq = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) / V.coef[Big.len - 1];
        DBASE rr = (((DBASE)U.coef[j + Big.len] << BASE_SIZE) + U.coef[j + Big.len - 1]) % V.coef[Big.len - 1];

        if (qq == b || ((qq * V.coef[Big.len - 2]) > (b * rr + U.coef[j + Big.len - 2]))) {
            qq--;
            rr = rr + V.coef[Big.len - 1];
            if ((rr < b) && ((qq == b) || (qq * V.coef[Big.len - 2] > b * rr + U.coef[j + Big.len - 2]))) {
                qq--;
                rr = rr + V.coef[Big.len - 1];
            }
        }

        BN u(V.len + 1);
        u.len = V.len + 1;
        for (int i = 0; i < V.len + 1; i++) {
            u.coef[i] = U.coef[j + i];
        }

        if (u < V * qq) qq--;

        u = u - V * qq;

        for (int i = 0; i < V.len + 1; i++) {
            U.coef[j + i] = u.coef[i];
        }

    }

    U = U / d;
    return U;
}

/*BN BN::operator %(BN BN2)
{
    if (BN2.len == 1 && BN2.coef[0] == 0)
    {
        puts("ERROR: it is impossible to divide by zero\n");
        exit(1);
    }
    if (BN2.len == 1 && BN2.coef[0] != 0)
    {
        BN result;
        result += *this % BN2.coef[0];
        return result;
    }
    if (*this == BN2)
    {
        BN result;
        return result;
    }
    if (*this < BN2)
    {
        return *this;
    }

    DBASE d = (1ll << BASE_SIZE) / ((DBASE)BN2.coef[BN2.len - 1] + 1);

    BN BN_u(*this);
    //cout << BN_u.coef[2] << "\n" << this->coef[2] << "\n";
    BN_u = BN_u * (BASE)d;
    //cout << BN_u.coef[2] << "\n" << d << "\n";
    //if (d == 1)
    {
        BN BN_tmp(len + 1);
        BN_tmp += *this;
        BN_u = BN_tmp;
    }
    else
    {
        BN_u = *this * (BASE)d;
    }

    if (len == BN_u.len)
    {
        if (BN_u.len == BN_u.maxlen)
        {
            BN BN_tmp(len + 1);
            for (int i = 0; i < len; i++)
            {
                BN_tmp.coef[i] = BN_u.coef[i];
            }
            BN_u = BN_tmp;
        }
        BN_u.coef[len] = 0;
        BN_u.len = len + 1;
    }

    BN BN_v = BN2 * (BASE)d;

    int m = len - BN2.len; // t = n + m
    DBASE q, r;
    DBASE b = 1ll << BASE_SIZE;// 15136855105158 1908081172
    bool k = false;
    for (int j = m; j >= 0; j--)
    {
        int i = j + BN2.len;
        q = (((DBASE)BN_u.coef[i] << BASE_SIZE) + (DBASE)BN_u.coef[i - 1]) / (DBASE)BN_v.coef[BN_v.len - 1]; // << BASE_SIZE
        r = (((DBASE)BN_u.coef[i] << BASE_SIZE) + (DBASE)BN_u.coef[i - 1]) % (DBASE)BN_v.coef[BN_v.len - 1];

        //cout << "(DBASE)BN_v.coef[BN2.len - 2] * q = " << (DBASE)BN_v.coef[BN2.len - 2] * q << "\n";
        //cout << "b * r + BN_u.coef[i - 2] =" << b * r + BN_u.coef[i - 2] << "\n";

        if ((q == b) || (((DBASE)BN_v.coef[BN2.len - 2] * q) > (b * r + (DBASE)BN_u.coef[i - 2])))
        {
            q--;
            r = r + (DBASE)BN_v.coef[BN2.len - 1];
            if ((r < b) && ((q == b) || (((DBASE)BN_v.coef[BN2.len - 2] * q) > (b * r + (DBASE)BN_u.coef[i - 2]))))
            {
                q--;
                r = r + (DBASE)BN_v.coef[BN2.len - 1];
            }
        }

        //if (BN_u < (BN_v * (BASE)q)) k = true;

        //BN_u -= BN_v * (BASE)q;
        //result.coef[j] = (BASE)q;
        DBASE DBASE_tmp;
        BN BN_tmp = BN_v * (BASE)q;
        bool e = 0;
        for (int i = 0; i < BN_tmp.len; i++) {
            //cout<<i+j<<" "<<len<<endl;
            DBASE_tmp = (DBASE)BN_u.coef[i + j] + b - (DBASE)BN_tmp.coef[i] - e;
            BN_u.coef[i + j] = (BASE)DBASE_tmp;
            e = !((DBASE_tmp) >> BASE_SIZE);
        }
        for (int i = BN_tmp.len; e && (i + j) < len; i++) {
            //cout<<i+j<<" "<<len<<endl;
            DBASE_tmp = (DBASE)BN_u.coef[i + j] + b - e;
            BN_u.coef[i + j] = (BASE)DBASE_tmp;
            e = !((DBASE_tmp) >> BASE_SIZE);
        }

        if (e)
        {
            //result.coef[j]--;
            //BN_u += BN_v;
            //k = false;
            e = 0;
            for (int i = 0; i < BN_v.len; i++) {
                DBASE_tmp = (DBASE)BN_u.coef[i + j] + (DBASE)BN_v.coef[i] + e;
                e = (bool)(DBASE_tmp >> BASE_SIZE);
                BN_u.coef[i + j] = (BASE)DBASE_tmp;
            }
            for (int i = BN_v.len; e && (i + j) < len; i++) {
                DBASE_tmp = (DBASE)BN_u.coef[i + j] + e;
                e = (bool)(DBASE_tmp >> BASE_SIZE);
                BN_u.coef[i + j] = (BASE)DBASE_tmp;
            }
        }
    }

    BN_u = BN_u / (BASE)d;
    return BN_u;
}/**/

BN BN::pow2()
{
    BN result(len * 2);
    result.len = 2 * len;
    DBASE cuv;

    for (int i = 0; i < len; i++)
    {
        cuv = 0;
        cuv = (DBASE)result.coef[2 * i] + (DBASE)coef[i] * (DBASE)coef[i]; 
        result.coef[2 * i] = (BASE)cuv;
        for (int j = i + 1; j < len; j++)
        {
            cuv = (DBASE)result.coef[i + j] + 2 * (DBASE)coef[i] * (DBASE)coef[j] + (cuv >> BASE_SIZE);
            result.coef[i + j] = (BASE)cuv;
        }
        result.coef[i + len + 1] += (BASE)(cuv >> 2 * BASE_SIZE);
        result.coef[i + len] += (BASE)(cuv >> BASE_SIZE);
    }

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN BN::powY(BASE y)
{
    if (y == 0)
    {
        BN result;
        result += 1;
        return result;
    }
    
    BN result(*this);
    if (y == 1) return result;
    if (y == 2) return result.pow2();

    BASE mask = y;
    int n = 0;

    while (mask)
    {
        mask = mask >> 1;
        n++;
    }

    for (int i = n - 2; i >= 0; i--)
    {
        result = result.pow2();
        if ((1 << i) & y) result *= *this;
    }

    return result;
}

BN BN::powY(BN y)
{
    if (y.len == 1 && y.coef[0] == 0)
    {
        BN result;
        result += 1;
        return result;
    }

    BN result(*this);
    if (y.len == 1 && y.coef[0] == 1) return result;
    if (y.len == 1 && y.coef[0] == 2) return result.pow2();

    BN mask = y;
    int n = 0;

    while (mask.len > 1 || mask.coef[0] != 0)
    {
        mask = mask / 2;
        n++;
    }

    for (int i = n - 2; i >= 0; i--)
    {
        result = result.pow2();
        if ((1 << (i % BASE_SIZE)) & y.coef[i / BASE_SIZE]) result = (result * *this);
    }

    return result;
}

BN BN::powYmod(BN y, BN mod)
{
    if (mod.len == 1 && mod.coef[0] < 2)
    {
        BN result;
        return result;
    }

    if (y.len == 1 && y.coef[0] == 0)
    {
        BN result;
        result += 1;
        return result;
    }

    BN result(*this);
    if (y.len == 1 && y.coef[0] == 1) return result % mod;
    if (y.len == 1 && y.coef[0] == 2) return result.pow2() % mod;

    BN mask = y;
    int n = 0;

    while (mask.len > 1 || mask.coef[0] != 0)
    {
        mask = mask / 2;
        n++;
    }

    for (int i = n - 2; i >= 0; i--)
    {
        result = result.pow2() % mod;
        if ((1 << (i % BASE_SIZE)) & y.coef[i / BASE_SIZE]) result = (result * *this) % mod;
    }

    return result;
}

BN BN::mod(BN m, BN z)
{
    BN _q, r1, r2, result, zero;
    if (m == zero) exit(EXIT_FAILURE);
    if (BASE_SIZE <= 3) exit(EXIT_FAILURE);
    if (len > 2 * m.len) exit(EXIT_FAILURE);

    int k = m.len;

    _q = *this;
    for (int i = k - 1; i < len; i++) _q.coef[i - (k - 1)] = _q.coef[i];
    _q.len = _q.len - (k - 1);
    _q *= z;
    for (int i = k + 1; i < _q.len; i++) _q.coef[i - (k + 1)] = _q.coef[i];
    _q.len = _q.len - (k + 1);

    r1 = *this;
    for (int i = k + 1; i < r1.len; i++) r1.coef[i] = 0;
    r1.len = k + 1;

    r2 = _q * m;
    for (int i = k + 1; i < r2.len; i++) r2.coef[i] = 0;
    r2.len = k + 1;

    //r1 = *this % _BASE.powY(k + 1);
    //r2 = (_q * m) % _BASE.powY(k + 1);

    if (r1 > r2) result = r1 - r2;
    else
    {
        BN _BASE;
        _BASE += (1 << BASE_SIZE) - 1;
        _BASE += 1;
        result = _BASE.powY(k + 1) + r1 - r2;
    }

    while (result >= m)
    {
        result -= m;
    }

    return result;
}

BN phi(BN n) 
{
    BN result = n;

    BN i, zero, one; one += 1;

    i += 2;
    for (; i.pow2() <= n; i += 1)
    {
        if (n % i == zero) 
        {
            while (n % i == zero) n = n / i;

            result -= result / i;
        }
    }

    if (n > one) result -= result / n;

    return result;
}

bool BN::isPrime_Fermat(int t)
{
    if (this->len == 1 && this->coef[0] <= 3) return true;
    if (*this % 2 == 0) return false;
    if (t < 1)
    {
        cout << "ERROR {isPrime_Fermat}: wrong input\n";
        exit(1);
    }    

    while (t > 0)
    {
        BN a(this->len, true), r;
        if (a.len == 1 && a.coef[0] < 2) a.coef[0] = 2;
        if (a > *this) a = *this - 2;
        
        r = a.powYmod(*this - 1, *this);
        if (r.len != 1 || r.coef[0] != 1) return false;
        
        t--;
    }

    return true;
}

bool BN::isPrime_MR(int t)
{
    if (this->len == 1 && this->coef[0] <= 3) return true;
    if (*this % 2 == 0) return false;
    if (t < 1)
    {
        cout << "ERROR {isPrime_MR}: wrong input\n";
        exit(1);
    }

    BN r, one, _one;
    one += 1;
    _one += *this - 1;

    int S = 0;
    r = *this - 1;
    while (r % 2 == 0)
    {
        r = r / 2;
        S++;
    }

    while (t > 0)
    {
        BN b(this->len, true), y;
        if (b.len == 1 && b.coef[0] < 2) b.coef[0] = 2;
        if (b > *this) b = *this - 2;

        y = b.powYmod(r, *this);

        if (y != one && y != _one)
        {
            for (int j = 1; j < S && y % *this != _one; j++)
            {
                y = y.pow2() % *this;

                if (y == one) return false;
            }

            if (y != _one) return false;
        }
        t--;
    }
    return true;
}

BN BN::generatePrime()
{
    BN s(5, true), t(5, true);

    while (!s.isPrime_MR(10))
    {
        BN regen(5, true);
        s = regen;
    }
    while (!t.isPrime_MR(10))
    {
        BN regen(5, true);
        t = regen;
    }

    BN i0(2, true), j0(2, true), r, p0, result, zero;

    do
    {
        r = i0 * 2 * t + 1;
        i0 += 1;
    } while (!r.isPrime_MR(10));

    p0 = s.powYmod(r - 2, r) * 2 * s - 1;

    do
    {
        result = j0 * 2 * r * s + p0;
        j0 += 1;
    } while (!result.isPrime_MR(10));

    if ((result - 1) % r == zero && (result + 1) % s == zero && (r - 1) % t == zero) cout << "Strong prime number: ";

    return result;
}

double BN::getCoef(BN BN2, int t)
{
    if (*this > BN2)
    {
        cout << "ERROR {getCoef}: the divisor must be greater than the divisible\n";
        exit(1);
    }
    if (t < 1)
    {
        cout << "ERROR {getCoef}: coefficient t must be greater than 0\n";
        exit(1);
    }

    double result = 0.0;

    BN tmp(*this), BN_res;

    for(int i = t; i > 0; i--) tmp = tmp * 10;
    BN_res = tmp / BN2;

    long long _tmp = 0;
    for (int i = BN_res.len - 1; i >= 0; i--)
    {
        _tmp += BN_res.coef[i] * pow((1ll << BASE_SIZE), i);
    }

    result = (double)_tmp / pow(10, t);

    return result;
}

BN BN::sqrt()
{
    BN x(*this), x0, a, b, c;

    do
    {
        x0 = x;
        x = (*this / x + x) / 2;
    } while (x < x0);

    return x0;
}

BN BN::qbrt() 
{
    BN x(*this), x0;

    do
    {
        x0 = x;
        x = (*this / x.pow2() + x * 2) / 3;
    } while (x < x0);

    return x0;
}

vector<BN> BN::trialDiv()
{
    BN one, zero; one += 1;
    if (*this <= one) 
    {
        cout << "ERROR {trialDiv}: the number must be greater than 1\n";
        exit(1);
    }

    //int t = 0;
    vector<BN> result; vector<BN> d; long long k = 0;
    BN q, r;

    while (*this % 2 == 0)
    {
        BN two; two += 2;
        *this = *this / 2;
        result.push_back(two);
    }

    for (int i = 0; i < 3; i++)
    {
        BN tmp; tmp += 3 + 2 * i;
        d.push_back(tmp);
    }

    while (*this != one)
    {
        if (k >= d.size())
        {
            d.push_back(d[d.size() - 2] + 6);
        }
        
        r = *this % d[k];
        q = *this / d[k];

        if (r == zero)
        {
            //t++;
            result.push_back(d[k]);
            *this = q;
        }
        else
        {
            if (q > d[k])
            {
                k++;
            }
            else
            {
                //t++;
                result.push_back(*this);
                return result;
            }
        }
    }
    return result;
}

BN BN::Always_TD()
{
    BN zero;
    if (*this % 2 == 0)
    {
        cout << "Error {Always_TD}: The input number must be odd\n";
        exit(1);
    }

    BN _div, _sqrt, r1, r2, q1, q2, r, q;

    _div = (*this).qbrt() * 2 + 1;
    if (_div % 2 == 0) _div += 1;
    cout << "_div: "; _div.output10();
    
    r1 = *this % _div;
    r2 = *this % (_div - 2);

    q1 = *this / _div;
    q2 = *this / (_div - 2);

    q = (q2 - q1) * 4;
    _sqrt = (*this).sqrt();
    cout << "_sqrt: "; _sqrt.output10();
    
    _div = _div + 2;
    while (_div <= _sqrt)
    {
        if (q + r1 * 2 < r2)
        {
            r = _div + q + r1 * 2 - r2;
            r2 = r1;
            r1 = r;

            q += 4;
        }
        else
        {
            r = q + r1 * 2 - r2;
            r2 = r1;
            r1 = r;
        }

        while (r1 >= _div)
        {
            r1 -= _div;
            q -= 4;
        }
        
        if (r1 == zero)
        {
            return _div;
        }

        _div = _div + 2;
    }

    cout << "Error {Always_TD}: There are no divisors\n";
    exit(1);
}

vector<BN> BN::FermasAlg()
{
    if (*this % 2 == 0)
    {
        cout << "Error {FermasAlg}: The input number must be odd\n";
        exit(1);
    }

    vector<BN> result;
    BN _sqrt = (*this).sqrt(), y, z;
    bool flag = false;

    if (_sqrt.pow2() == *this)
    {
        result.push_back(_sqrt);
        return result;
    }

    do
    {
        _sqrt += 1; // x++

        if (_sqrt == (*this + 1) / 2) return result; // prime, vector size is equal to 0

        if(flag) z += _sqrt * 2 - 1;
        else
        {
            z = _sqrt.pow2() - *this;
            flag = true;
        }
        //z = _sqrt.pow2() - *this;
        y = z.sqrt();

    } while (y.pow2() != z);

    result.push_back(_sqrt + y);
    result.push_back(_sqrt - y);
    return result;
}

BN BN::GCD(BN num)
{
    BN zero;
    if (*this % num == zero) return num;
    if (num % *this == zero) return *this;

    if (*this > num) return (*this % num).GCD(num);
    return (*this).GCD(num % *this);
}

BN BN::LCM(BN num)
{
    return (*this * num) / (*this).GCD(num);
}

BN BN::f_Pollard(BN n)
{
    return ((*this).pow2() + 1) % n;
}

BN BN::Ro_Pollard()
{
    if ((*this).isPrime_MR(100))
    {
        cout << "Error {Ro_Pollard}: the imput number must be composite\n";
        exit(1);
    }

    BN a, b, d, one; a += 2; b += 2; one += 1;

    do
    {
        a = a.f_Pollard(*this); // f(a) 
        b = (b.f_Pollard(*this)).f_Pollard(*this); // f(f(b))

        if (a == b)
        {
            cout << "Output {RoPollard}: The end of the period has been reached\n";
            exit(1);
        }

        if (a > b) d = (a - b).GCD(*this);
            else d = (b - a).GCD(*this);
    } while (d == one);

    return d;
}

BN BN::log(BN num)
{
    BN result, one, zero, copy(num); one += 1;
    if (num <= one)
    {
        cout << "Error {log}: The base of the logarithm must be greater then 1\n";
        exit(1);
    }
    if (*this == zero)
    {
        cout << "Error {log}: The number of the logarithm must be greater then 0\n";
        exit(1);
    }
    
    if (*this == one) return result;
    
    while (true)
    {
        if (num > *this) return result;

        num *= copy;
        result += 1;
    }
}

BN BN::Ro1_Pollard()
{
    if ((*this).isPrime_MR(100))
    {
        cout << "Error {Ro1_Pollard}: the imput number must be composite\n";
        exit(1);
    }

    BN B, d, a((*this).len, true), q, e, one, two; B += 50; one += 1; two += 2; q += 2;
    a = a % (*this - 2);
    if (a < two) a = two;

    d = a.GCD(*this);
    if (d > one) return d;

    while (q < B)
    {
        e = (*this).log(q);
        //cout << "e: "; e.output10(); cout << "q: "; q.output10(); cout << "a: "; a.output10();
        a = a.powYmod(q.powY(e), *this);
        //cout << "new a: "; a.output10();
        
        if (a == one)
        {
            BN tmp((*this).len, true);
            tmp = tmp % (*this - 2);
            if (tmp < two) tmp = two;
            a = tmp;

            d = a.GCD(*this);
            if (d > one)
            {
                //cout << "q = "; q.output10();
                return d;
            }
        }
        else d = (a - 1).GCD(*this);

        if (d == one)
        {
            B += 50;
            //cout << "B = "; B.output10();
        }
        else return d;

        do
        {
            if (q == two) q += 1;
                else q += 2;
        } while (!q.isPrime_MR(100));
    }
}

bool operator >(const BASE x, const BN& BN1)
{
    BN tmp; tmp += x;

    return tmp > BN1;
}

bool operator <(const BASE x, const BN& BN1)
{
    BN tmp; tmp += x;

    return tmp < BN1;
}

BN BN::logGelfond(BN a, BN g)
{
    BN h, b, result; // result = x
    h = (*this).sqrt() + (BASE)1;
    b = g.powYmod(h, *this);

    map<BN, BASE> u; map<BN, BASE>::iterator iter;
    vector<BN> v;

    for (BASE i = (BASE)1; h + (BASE)1 > i; i++)
    {
        BN tmp; tmp += i;
        u.insert(make_pair(b.powYmod(tmp, *this), i));
    }

    bool _final = false;
    for (BASE i = (BASE)1; h > i && !_final; i++)
    {
        v.push_back((a * g.powY(i)) % (*this));

        iter = u.find(v[i - (BASE)1]);

        if (iter != u.end())
        {
            result = (h * iter->second - i) % (*this);
            _final = true;
        }
    }

    return result;
}

BN BN::inv(BN m) 
{
    BN u1(m);
    BN u2;
    BN one; one += 1;
    BN v1(*this);
    BN v2(one);
    BN zero;
    while (v1 != zero)
    {
        BN q = u1 / v1;
        BN t1 = u1 % v1;
        BN str = (q * v2) % m;
        if (u2 < str)
        {
            u2 += m;
        }
        BN t2 = u2 - str;
        u1 = v1;
        v1 = t1;
        u2 = v2;
        v2 = t2;
    }
    return (u2 % m);
}

BN BN::logPollard(BN g, BN a) 
{
    BN x1, x2, u1, u2, v1, v2, zero;
    x1 += (BASE)1;
    x2 += (BASE)1;

    BN n(*this);
    BN m = n - (BASE)1;

    auto func = [&](BN& x, BN& u, BN& v)
    {
        if (x % (BASE)3 == (BASE)1) // S1
        {
            x = x * a % n;
            v = (v + (BASE)1) % m;
        }
        else if (x % (BASE)3 == (BASE)2) // S2
        {
            x = x * x % n;
            u = u * (BASE)2 % m;
            v = v * (BASE)2 % m;
        }
        else if (x % (BASE)3 == (BASE)0) // S3
        {
            x = x * g % n;
            u = (u + (BASE)1) % m;
        }
    };

    BN r;
    while (r == zero)
    {
        do
        {
            func(x1, u1, v1);
            func(x2, u2, v2);
            func(x2, u2, v2);

        } while (x1 != x2);

        //x1.output10();
        //x2.output10();

        if (v1 >= v2)
            r = (v1 - v2) % m;
        else
            r = (v1 + m - v2) % m;

        if (r == zero)
        {
            BN u_tmp((*this).len, true), v_tmp((*this).len, true);
            u1 = u_tmp % (*this - 1);
            v1 = v_tmp % (*this - 1);

            x1 = g.powY(u1) * a.powY(v1);

            x2 = x1;
            u2 = u1;
        }
    }
    //r.output10();
    //m.output10();

    BN d = r.GCD(m);
    //d.output10();

    BN U;
    if (u2 >= u1)
        U = (u2 - u1) % m;
    else
        U = (u2 + m - u1) % m;

    //U.output10();

    r = r / d;
    m = m / d;
    U = U / d;

    BN r_inv = r.inv(m);
    //r_inv.output10();

    BN x = (r_inv * U) % m;
    //x.output10();

    for (; d > (BASE)0 && g.powYmod(x, n) != a; d -= (BASE)1)
    {
        x += m;
    }

    //x.output10();

    return x;
}

int main()
{   
    srand(time(NULL));

    BN p, g, a, result;

    for (int l = 0; l < 12; l++)
    {
        cout << "Insert p = ";
        p.input10();
        cout << "Insert g = ";
        g.input10();
        cout << "Insert a = ";
        a.input10();

        result = p.logPollard(g, a);
        cout << "Result is ";
        result.output10();
        cout << "\n";
    }
    
    return 0;

    // Кармайкла: 232250619601 (5+-) , 9746347772161 (5-), 5394826801 (5+) {MR (-, -, -)}
    // Простые: 67001640321444207149 [20], 
    // 87393520364697804444076797303045106471721359079521 [50],
    // 8647488593211501438897410262632290050227416372263495298011364163992447808435440478665323558107436131 [100]
    // 29, 809, 9941, 26437, 173629, 6982009, 61787437, 189703133, 8379705787, 95025363427
}
