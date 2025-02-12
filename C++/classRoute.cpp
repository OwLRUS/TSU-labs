#include <iostream>
#include <time.h>
using namespace std;

class route {
	int* r, n;	// r – массив, представляющий маршрут; n – количество городов
public:
	route(int num = 0);	// создает начальную перестановку по возрастанию
	route(const route&);	// конструктор копирования
	~route() { if (r) delete[]r; r = NULL; } // деструктор

	route& operator = (const route&);   // операция присваивания
	int route_price(int**);  // вычисляет стоимость маршрута по матрице стоимости
	bool next_route();   // вычисляет следующий маршрут, используя алгоритм Дейкстры
	friend ostream& operator << (ostream&, const route&); // вывод маршрута
};
/* // конструктор типа: 0123...(n-1)
route::route(int num)
{
	if (num > 0)
	{
    	n = num;
    	r = new int[num];

    	int i = 0;
    	while (i < num)
    	{
        	r[i] = i;
        	i++;
    	}
	}
	else
	{
    	r = NULL;
    	n = 0;
	}
}
*/
route::route(int num) // конструктор типа: (num)012...(num - 1)(num + 1).
{
	if (num > 0)
	{
    	n = num + 2;
    	r = new int[n];

    	int i = 1;
    	r[0] = num;
    	while (i < n)
    	{
        	if (i - 1 < num) r[i] = i - 1;
            	else r[i] = i;    	 
        	i++;
    	}
	}
	else
	{
    	n = 5;
    	r = new int[n];

    	int i = 1;
    	r[0] = num;
    	while (i < n)
    	{
        	if (i - 1 < num) r[i] = i - 1;
        	else r[i] = i;    	 
        	i++;
    	}
	}
}

route::route(const route& R)
{
	n = R.n;
	if (n > 0)
	{
    	r = new int[n];

    	int i = 0;
    	while (i < n)
    	{
        	r[i] = R.r[i];
        	i++;
    	}
	}
	else r = NULL;
}

route& route::operator = (const route& R)
{
	if (this == &R) return *this;

	n = R.n;
	if(r) delete[]r;
	r = NULL;

	if (n > 0)
	{
    	r = new int[n];
    	int i = 0;
    	while (i < n)
    	{
        	r[i] = R.r[i];
        	i++;
    	}
	}

	return *this;
}

int route::route_price(int** MX)
{
	if (!r || !MX) return 0;

	int cart = 0;
	int i = 0;
	while (i < n - 1)
	{
    	cart += MX[r[i]][r[i + 1]];
    	i++;
	}
	cart += MX[r[i]][0];
	return cart;
}

bool route::next_route()
{
	if (!r) return false;

	size_t max_i = n, max_j = 0, tmp = 0, tmp_N = n;

	for (int i = n - 1; i > 1 && max_i == n; i--)
	{
    	if (r[i - 1] < r[i])
    	{
        	max_i = i - 1;
    	}
	}

	if (max_i == n) return false;

	for (int j = n - 1; j > max_i && max_j == 0; j--)
	{
    	if (r[max_i] < r[j])
    	{
        	max_j = j;
    	}
	}

	tmp = r[max_i];
	r[max_i] = r[max_j];
	r[max_j] = tmp;

	for (int i = max_i + 1; i < tmp_N; i++)
	{
    	tmp = r[i];
    	r[i] = r[tmp_N - 1];
    	r[tmp_N - 1] = tmp;
    	tmp_N--;
	}

	return true;
}

ostream& operator << (ostream& F, const route& R)
{
	if (!R.r) F << "empty";
 	else
 	{
    	int i = 0;
    	while (i < R.n)
    	{
        	F << R.r[i] << ",";
        	i++;
    	}
 	}
	return F;
}

int main()
{
	srand(time(NULL));
	route a, b(2), c(b);
	cout << a << "\n" << b << "\n" << c << "\n";
	a = b;
	cout << a << "\n";
    
	size_t nn = 4, mm = 4;
	int **MX = new int*[nn];
	int k = 0;
	while (k < nn)
	{
    	MX[k] = new int[mm];
    	k++;
	}
	int i = 0, j = 0;
	cout << "\n";
	while (i < nn)
	{
    	j = 0;
    	while (j < mm)
    	{
        	if (i == j)
        	{
            	MX[i][j] = 0;
            	cout << MX[i][j] << " ";
        	}
        	else
        	{
            	MX[i][j] = 1 + rand() % 9;
            	cout << MX[i][j] << " ";
        	}
        	j++;
    	}
    	cout << "\n";
    	i++;
	}

	bool flag = true;
	int min_price = b.route_price(MX);
	int tmp_price;
	route Min_route(b);
	cout << "\n" << b << "| price = " << min_price << "\n";

	while (flag)
	{
    	flag = b.next_route();
    	tmp_price = b.route_price(MX);
    	if (flag) cout << b << "| price = " << tmp_price << "\n";
    	if (min_price > tmp_price)
    	{
        	min_price = tmp_price;
        	Min_route = b;
    	}
	}

	cout << "\nMinimal price route is " << Min_route << " with price = " << min_price << "\n";
	k = 0;
	while (k < nn)
	{
    	delete[]MX[k];
    	k++;
	}
	delete[] MX;
	MX = NULL;
	return 0;
}
