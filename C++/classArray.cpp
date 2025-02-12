#include <iostream>
#include <ctime>
using namespace std;

class Array
{
	int* a, n;

public:
	// конструктор 1
	// len – число элементов в массиве
	// t = 1 – неупорядоченный массив
	// t = 2 – массив, упорядоченный по неубыванию
	// t = 3 – массив, упорядоченный по невозрастанию
	// d – диапазон псевдослучайных чисел для неупорядоченного массива (при t = 1)
	Array(int len = 1, int t = 1, int d = 10);

	Array(int*, int);   // конструктор 2: по массиву
	Array(Array&);
	~Array() { if (a) delete[] a; a = NULL; }

	Array& operator = (Array&);
	int& operator [](int);

	bool Test();   // проверка на упорядоченность по неубыванию
	bool operator == (Array);   // равенство элементов массивов (но не порядка)

	void Shell_sort();
	void Heapsort();
	void Hoar_sort(int l = -1, int r = 0);
	void Bit_sort();

	friend istream& operator >> (istream&, Array&);
	friend ostream& operator << (ostream&, Array&);
};

Array::Array(int len, int t, int d)
{
	if (len <= 0 || t > 3 || t < 1 || d < 1)
	{
    	a = NULL; n = 0;
	}
	else
	{
    	srand(time(NULL));
    	if (t == 1)
    	{
        	a = new int[len];
        	n = len;
        	for (int i = 0; i < len; i++)
        	{
            	a[i] = rand() % d;
        	}
    	}
    	if (t == 2)
    	{
        	a = new int[len];
        	n = len;
        	a[0] = rand() % d;
        	for (int i = 1; i < len; i++)
        	{
            	a[i] = a[i - 1] + rand() % d;
        	}
    	}
    	if (t == 3)
    	{
        	a = new int[len];
        	n = len;
        	a[len - 1] = rand() % d;
        	for (int i = len - 2; i >= 0; i--)
        	{
            	a[i] = a[i + 1] + rand() % d;
        	}
    	}
	}
}

Array::Array(Array& Arr)
{
	if (!Arr.a || Arr.n == 0)
	{
    	a = NULL;
    	n = 0;
	}
	else
	{
    	n = Arr.n;
    	a = new int[n];
    	for (int i = 0; i < n; i++)
    	{
        	a[i] = Arr.a[i];
    	}
	}
}

Array::Array(int* Arr, int Len)
{
	if (!Arr || Len == 0)
	{
    	a = NULL;
    	n = 0;
	}
	else
	{
    	a = new int[Len];
    	n = Len;
    	for (int i = 0; i < Len; i++)
    	{
        	a[i] = Arr[i];
    	}
	}
}

Array& Array::operator = (Array& Arr)
{
	if (this == &Arr) return *this;
	if (!Arr.a || Arr.n == 0)
	{
    	a = NULL;
    	n = 0;
    	return *this;
	}

	delete[] a;
	a = NULL;

	n = Arr.n;
	a = new int[n];

	for (int i = 0; i < n; i++)
	{
    	a[i] = Arr.a[i];
	}
	return *this;
}

int& Array::operator [](int Idx)
{
	if (Idx < 0 || Idx >= n)
	{
    	puts("ERROR: Index outside the row");
    	exit(0);
	}

	return a[Idx];
}

bool Array::Test()
{
	if (!a || n == 0) return false;

	int i = 0;
	while (a[i] <= a[i + 1] && i < n - 1)
	{
    	i++;
	}

	if (i == n - 1) return true;
	return false;
}

bool Array::operator == (Array Arr)
{
	if (!a || !Arr.a || n == 0 || Arr.n == 0 || n != Arr.n) return false;

	int j; bool flag;
	for (int i = 0; i < n; i++)
	{
    	j = 0;
    	flag = false;
    	while (!flag && j < Arr.n)
    	{
        	if (a[i] == Arr.a[j])
        	{
            	flag = true;
            	Arr.a[j] = -1;
        	}
        	j++;
    	}
	}
    
	flag = true;
	for (int i = 0; i < Arr.n && flag; i++)
	{
    	if (Arr.a[i] != -1) flag = false;
	}

	return flag;
}

ostream& operator << (ostream& r, Array& Arr)
{
	if (!Arr.a || Arr.n == 0)
	{
    	r << "Array is empty";
    	return r;
	}

	for (int i = 0; i < Arr.n - 1; i++)
	{
    	r << Arr.a[i] << ", ";
	}
	r << Arr.a[Arr.n - 1];
    
	return r;
}

istream& operator >> (istream& r, Array& Arr)
{
	cout << "Enter the count of numbers: ";
	cin >> Arr.n;

	int* buf = new int[Arr.n];

	for (int i = 0; i < Arr.n; i++)
	{
    	cout << "Enter a[" << i << "] = ";
    	cin >> buf[i];
	}

	if (Arr.a) delete[] Arr.a;
	Arr.a = buf;

	return r;
}

void Sift(int* Arr, int n, int i)
{
	if (Arr && n > 0 && i >= 0)
	{
    	int j = 2 * i + 1, x = Arr[i], f = 1;

    	while (j < n && f)
    	{
        	if (j + 1 < n && Arr[j + 1] > Arr[j])
        	{
            	j++;
        	}
        	if (Arr[j] > x)
        	{
            	Arr[i] = Arr[j];
            	i = j;
            	j = 2 * i + 1;
        	}
        	else f = 0;
    	}
    	Arr[i] = x;
	}
}

void Array::Heapsort()
{
	if (a && n > 0)
	{
    	int i = n / 2 - 1;
    	while (i >= 0)
    	{
        	Sift(a, n, i);
        	i--;
    	}
    	size_t row = n; int tmp;
    	while (row > 1)
    	{
        	tmp = a[row - 1];
        	a[row - 1] = a[0];
        	a[0] = tmp;
        	row--;

        	Sift(a, row, 0);
    	}
	}
}

void Array::Shell_sort()
{
	if (a && n != 0)
	{
    	int tmp, i, j, k;
    	for (i = n / 2; i > 0; i = i / 2)
    	{
        	for (j = i; j < n; j++)
        	{
            	tmp = a[j];
            	for (k = j; k >= i && tmp < a[k - i]; k = k - i)
            	{
                	a[k] = a[k - i];
            	}
            	a[k] = tmp;
        	}
    	}
	}
}

void Array::Hoar_sort(int l, int r)
{
	if (a && n != 0 && l < r)
	{
    	if (l == -1)
    	{
        	l = 0; r = n - 1;
    	}

    	int i = l, j = r, x = a[(l + r) / 2], tmp; //a[l + rand(r - l + 1)]
   	 
    	while (i <= j)
    	{
        	while (a[i] < x) i++;
        	while (a[j] > x) j--;
       	 
        	if (i <= j)
        	{
            	tmp = a[i];
            	a[i] = a[j];
            	a[j] = tmp;
            	i++; j--;
        	}
    	}
    	//cout << "Etalon = " << x << ", j = " << j << ", i = " << i << "\n";
    	//cout << *this << "\n";
    	if (l < j) Hoar_sort(l, j);
    	if (i < r) Hoar_sort(i, r);
	}
}

void Array::Bit_sort()
{
	int l = 0, r = n - 1, max_el = 0, k = 0, i = 0, j, mask = 0, tmp;

	while (i < n)
	{
    	if (max_el < a[i]) max_el = a[i];
	}
	while (max_el / 2 != 0)
	{
    	k++;
    	max_el /= 2;
	}

	i = l; j = r;
	while (k >= 0 && l < r)
	{
    	while (i <= j)
    	{
        	while (i <= j && mask == 0)
        	{
            	mask = 1 << k;
            	mask = mask & a[i];
            	if (!mask) i++;
        	}
       	 
        	while (i <= j && mask != 0)
        	{
            	mask = 1 << k;
            	mask = mask & a[i];
            	if (mask) j--;
        	}

        	if (i < j)
        	{
            	tmp = a[i];
            	a[i] = a[j];
            	a[j] = tmp;
            	i++; j--;
        	}
    	}
    	k--;
	}
}

int main()
{
	/*int* Arr = new int[4];
	int *Brr = new int[4];
	Arr[0] = 1; Arr[1] = 2; Arr[2] = 3; Arr[3] = 4;
	Brr[0] = 3; Brr[1] = 2; Brr[2] = 4; Brr[3] = 1;
	Array a(Arr, 4); Array b(Brr, 4);*/
	Array c(10000000, 2);
	Array a(c), b(c);
	//cin >> c;
	//cout << c << "\n";
	time_t start, end;
	double elapsed;
    
	time(&start);
	c.Hoar_sort();
	time(&end);
	elapsed = difftime(end, start);
	cout << "Hoar sort time: " << elapsed << " sec. \n";
    
	time(&start);
	a.Shell_sort();
	time(&end);
	elapsed = difftime(end, start);
	cout << "Shell sort time: " << elapsed << " sec. \n";
    
	time(&start);
	b.Heapsort();
	time(&end);
	elapsed = difftime(end, start);
	cout << "Heapsort time: " << elapsed << " sec. \n";
    
	//c.Hoar_sort();
	//cout << c << "\n";
	//if (a.Test() && b.Test() && c.Test() && a == b && a == c) cout << "Arrays are sorted and equal";
	return 0;
	/*
    
	*/
}
