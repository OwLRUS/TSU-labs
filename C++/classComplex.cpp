#include <iostream>
using std::ostream;
using std::istream;

class complex
{
	double re, im;

public:

	complex(double _re = 0, double _im = 0)
	{
    	re = _re;
    	im = _im;
	}

	double getRe()
	{
    	return re;
	}
	void setRe(double _re)
	{
    	re = _re;
	}
	double getIm()
	{
    	return im;
	}
	void setIm(double _im)
	{
    	im = _im;
	}

	bool operator == (complex c);
	bool operator != (complex c);

	double mod()
	{
    	return(sqrt(re*re + im*im));
	}
	complex operator +(const complex& c);
	complex operator -(const complex& c);
	complex operator *(const complex& c);
	complex operator /(const complex& c);

	void Input();
	void Output();

	friend ostream& operator << (ostream& r, const complex& c);
	friend istream& operator >> (istream& r, complex& c);
    
};

bool complex::operator == (complex c)
{
	if (re == c.re && im == c.im) return true;
	return false;
}

bool complex::operator != (complex c)
{
	if (re != c.re || im != c.im) return true;
	return false;
}

complex complex::operator +(const complex& c)
{
	complex res;

	res.re = re + c.re;
	res.im = im + c.im;

	return res;
}

complex complex::operator -(const complex& c)
{
	complex res;

	res.re = re - c.re;
	res.im = im - c.im;

	return res;
}

complex complex::operator *(const complex& c)
{
	complex res;

	res.re = re * c.re - im * c.im;
	res.im = re * c.im + im * c.re;

	return res;
}

complex complex::operator /(const complex& c)
{
	complex res;

	res.re = (re * c.re + im * c.im) / ((c.im) * (c.im) + (c.re) * (c.re));
	res.im = (im * c.re - re * c.im) / ((c.im) * (c.im) + (c.re) * (c.re));

	return res;
}

void complex::Input()
{
	std::cout << "Enter value of re:";
	std::cin >> re;
	std::cout << "Enter value of im:";
	std::cin >> im;
}

void complex::Output()
{
	std::cout << "Number is " << re << " + " << im << "i\n";
}

ostream& operator << (ostream& r, const complex& c)
{
	r << "Number is " << c.re << " + " << c.im << "i\n";

	return r;
}

istream& operator >> (istream& r, complex& c)
{
	std::cout << "Enter value of re:";
	std::cin >> c.re;

	std::cout << "Enter value of im:";
	std::cin >> c.im;

	return r;
}

int main() // переделать вывод на х + yi и вынести определения член-функций из класса
{
	complex a(2, 5);
	complex b(3);
	complex c;
	complex d(2, 5);
	double mod, re = 9, im = 9;
	//c.setRe(re);
	//c.setIm(im);
	c = a + b;
	//c.Input();
	std::cout << c;
	//std::cin >> c;
	//c.Output();
	//re = c.getRe();
	//im = c.getIm();
	//std::cout << "Number is re = " << re << ", im = " << im;
	//mod = a.mod();
	//std::cout << mod;
	//if (a == d) std::cout << "Numbers are equal\n";
	//if (a != b) std::cout << "Numbers aren't equal \n";
	return 0;
}
