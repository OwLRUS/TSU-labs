#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

class BoolV
{
	unsigned int* v;
	int nbit, m; // nbit – количество бит вектора, m – количество элементов массива v
public:
	BoolV(int nn = 1); // формируется булев вектор из nn бит; все биты со значениями 0
	BoolV(const char* strV); // формируется булев вектор по строке из нулей и единиц
	BoolV(const BoolV& Vec);
	~BoolV() { if (v) delete[] v; v = NULL;};
	void Set1(int bit); // устанавливает указанный бит в 1
	void Set0(int bit); // устанавливает указанный бит в 0
	int operator[](int bit);
	BoolV operator = (const BoolV& Vec);
	bool operator == (const BoolV& Vec); // равенство векторов
	BoolV operator | (const BoolV& Vec);
	BoolV operator & (const BoolV& Vec);
	BoolV operator ~ ();
	friend ostream& operator << (ostream& r, const BoolV& Vec);
	friend istream& operator >> (istream& r, BoolV& Vec);
};

BoolV::BoolV(int nn)
{
	if (nn < 1) nbit = 1;
	else nbit = nn;

	m = (nbit - 1) / 32 + 1;
	v = new unsigned int[m];

	for (int i = 0; i < m; i++) v[i] = 0;
}

BoolV::BoolV(const char* strV)
{
	if (strV)
	{
    	int i = 0, count = 0, j = 0, k = 0, sizeSTR = strlen(strV), tail;
    	unsigned int mask = 1;
    	char* strVec = new char[sizeSTR + 1];

    	while (strV[i] != '\0')
    	{
        	if (strV[i] == '1' || strV[i] == '0')
        	{
            	strVec[count] = strV[i];
            	count++;
        	}
        	i++;
    	}
    	strVec[count] = '\0';

    	if (count == 0) nbit = 1;
    	else nbit = count;

    	m = (nbit - 1) / 32 + 1;
    	v = new unsigned int[m];

    	k = count - 1;
    	for (i = 0; i < m; i++)
    	{
        	v[i] = 0;
        	j = 0;
        	mask = 1 << 31;
        	while (j < 32 && k >= 0)
        	{
            	if (strVec[k] == '1')
            	{
                	v[i] = v[i] | mask;
            	}
            	mask = mask >> 1;
            	k--; j++;
        	}
    	}
	}
}

BoolV::BoolV(const BoolV& Vec)
{
	nbit = Vec.nbit;
	m = Vec.m;
    
	v = new unsigned int[m];

	for (int i = 0; i < m; i++)
	{
    	v[i] = Vec.v[i];
	}
}

void BoolV::Set1(int bit) //
{
	if (bit <= nbit && bit >= 0)
	{
    	size_t i = bit / 32;
   	 
    	unsigned int mask = 1 << (31 - bit % 32);

    	v[i] = v[i] | mask;
	}
}

void BoolV::Set0(int bit) //
{
	if (bit <= nbit && bit >= 0)
	{
    	int i = bit / 32, j = 0, setbit = bit % 32;

    	unsigned int mask = 1 << (31 - setbit);
    	mask = ~mask;

    	v[i] = v[i] & mask;
	}
}

int BoolV::operator[](int bit) //
{
	if (bit > nbit || bit < 0) return -1;

	size_t mbit = bit / 32, nnbit = bit % 32;

	unsigned int mask = 1 << (31 - nnbit);
    
	if ((v[mbit] & mask) != 0) return 1;
	return 0;
}

BoolV BoolV::operator = (const BoolV& Vec)
{
	if (&Vec == this) return *this;

	nbit = Vec.nbit;
	m = Vec.m;

	if(v) delete[]v;
	v = NULL;

	v = new unsigned int[m];
	for (size_t i = 0; i < m; i++)
	{
    	v[i] = Vec.v[i];
	}

	return *this;
}

bool BoolV::operator == (const BoolV& Vec)
{
	if (nbit != Vec.nbit) return false;

	size_t i = 0;
	while (i < m - 1)
	{
    	if (Vec.v[i] != v[i]) return false;
    	i++;
	}

	return true;
}

BoolV BoolV::operator | (const BoolV& Vec)
{
	if (m > Vec.m)
	{
    	BoolV result(*this);

    	int i = 0;
    	while (i < Vec.m)
    	{
        	result.v[i] = result.v[i] | Vec.v[i];
        	i++;
    	}

    	return result;
	}
	else if (m > 0)
     	{
        	BoolV result(Vec);
       	 
        	int i = 0;
        	while (i < m)
        	{
            	result.v[i] = result.v[i] | v[i];
            	i++;
        	}

        	return result;
     	}
}

BoolV BoolV::operator & (const BoolV& Vec)
{
	if (m > Vec.m)
	{
    	BoolV result(*this);

    	int i = 0;
    	while (i < Vec.m)
    	{
        	result.v[i] = result.v[i] & Vec.v[i];
        	i++;
    	}
    	return result;
	}
	else if (m > 0)
	{
    	BoolV result(Vec);

    	size_t i = 0;
    	while (i < m)
    	{
        	result.v[i] = result.v[i] & v[i];
        	i++;
    	}
    	return result;
	}
}

BoolV BoolV::operator ~ ()
{
	int i = 0;
	BoolV result(*this);

	while (i < m)
	{
    	result.v[i] = ~v[i];
    	i++;
	}

	int tail = m * 32 - nbit;
	unsigned int mask;
	if (tail) // tail = 3; mask = 11111000;
	{
    	mask = 1;
    	for (i = 0; i < tail - 1; i++)
    	{
        	mask = mask << 1;
        	mask++;
    	}

    	mask = ~mask;
    	result.v[m - 1] = result.v[m - 1] & mask;
	}

	return result;
}

ostream& operator << (ostream& r, const BoolV& Vec)
{
	size_t j = 0, cart = 0;
	unsigned int mask = 1;

	for (int i = Vec.m - 1; i >= 0; i--)
	{
    	j = 0;

    	while (j < 32)
    	{
        	mask = 1 << j;
        	if ((Vec.v[i] & mask) != 0) r << "1";
            	else r << "0";
        	j++;

        	cart++;
        	if (cart == 8)
        	{
            	r << " ";
            	cart = 0;
        	}
    	}
	}

	return r;
}

istream& operator >> (istream& r, BoolV& Vec)
{
	char buf[128];
	gets_s(buf);

	BoolV BUF(buf);
	Vec = BUF;

	return r;
}

class BoolM
{
	BoolV* bm;
	int m, n; // m – количество строк, n – количество столбцов
public:
	BoolM(int k = 1, int l = 1); // формируется нулевая булева матрица размера kl
	BoolM(const BoolM& M);
	BoolV& operator[] (int l);
	BoolM operator = (const BoolM& M);
	friend ostream& operator << (ostream& r, BoolM& M);
};

BoolM::BoolM(int k, int l)
{
	if (k < 1) k = 1;
	if (l < 1) l = 1;

	m = l; n = k;
	bm = new BoolV[l];
	BoolV vec(k);

	for (int i = 0; i < l; i++)
	{
    	bm[i] = vec;
	}
}

BoolM::BoolM(const BoolM& M)
{
	m = M.m;
	n = M.n;
	bm = new BoolV[m];

	for (int i = 0; i < m; i++)
	{
    	bm[i] = M.bm[i];
	}
}

BoolV& BoolM::operator[] (int l)
{
	if (l < 0 || l > m)
	{
    	cout << "Index not in a row\n";
    	exit(0);
	}

	return bm[l];
}

BoolM BoolM::operator = (const BoolM& M)
{
	if (&M == this) return *this;

	m = M.m;
	n = M.n;
    
	if (bm) delete[] bm;
	bm = NULL;
	bm = new BoolV[m];

	for (int i = 0; i < m; i++)
	{
    	bm[i] = M.bm[i];
	}

	return *this;
}

ostream& operator << (ostream& r, BoolM& M)
{
	for (int i = 0; i < M.m; i++)
	{
    	r << "( " << M.bm[i] << " )\n";
	}
	r << "\n";

	return r;
}

void topsort(BoolM& M, int m, int* a)
{
	if(m <= 0 && !a) exit(0);

	BoolV v0(m), v1(m), v2(m), null(m);

	int cart = 0; BoolM tmp(M); bool flag = true;
	while (cart != m && flag)
	{
    	v1 = null;
    	for (int i = 0; i < m; i++)
    	{
        	v1 = v1 | tmp[i];
    	}

    	v2 = (~v1) & (~v0); // 0011 1101 & 1110 1110 = 0010 1100

    	flag = false;
    	for (int i = 0; i < m; i++)
    	{
        	if (v2[i] != 0)
        	{
            	flag = true;
            	a[cart] = i + 1;
            	cart++;
            	tmp[i] = null;
        	}
    	}
    	v0 = v0 | v2;
    	if (!flag) puts("Matrix have cycle");
	}
}

int main()
{
	/**char sample[38] = "1 01101111 11100000 00000000 00000000";
	char sample2[18] = "00000000 01000000";
    
	BoolV a(33), b(sample), c(sample2);
	cout << a << "\n" << b << "\n" << c << "\n";

	b.Set1(3);
	b.Set0(32);
	cout << b << "\n";

	a = b;
	cout << a << "\n";

	if (a == b) cout << "Equal\n";
    	else cout << "Not equal\n";

	int bit = 30;
	cout << "The digit of bit = " << bit << " is equal " << b[bit] << "\n";

	a = ~a;
	cout << a << "\n";

	c = c | a;
	cout << c << "\n";

	c = c & a;
	cout << c << "\n";

	cin >> a;
	cout << a << "\n";

	BoolM Mx(2, 2);
	cout << Mx;/**/

	FILE* graphInFile = fopen("C:\\Users\\elena\\source\\repos\\class bool\\graph.txt", "r");
	int vertexA = 0, vertexB = 0;
	int k = 0, size;
	if (graphInFile)
	{
    	k = fscanf(graphInFile, "%d", &size);
    	BoolM Mx(size, size);
    	int* a = new int[size];

    	while (k = fscanf(graphInFile, "%d%d", &vertexA, &vertexB) > 0) {
        	printf("(%d,%d)\n", vertexA, vertexB);
        	Mx[vertexA - 1].Set1(vertexB - 1);
    	}

    	fclose(graphInFile);
   	 
    	cout << Mx;

    	topsort(Mx, size, a);

    	for (int i = 0; i < size; i++)
    	{
        	cout << a[i] << ", ";
    	}
	}
	else
    	puts("File is not open");

	return 0;
}
