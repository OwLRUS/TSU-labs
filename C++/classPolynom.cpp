#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>

using namespace std;

class Poly
{
private:
    vector<int> coef;
    int degree;
    int mod;

public:
    Poly(const vector<int>& coefficients, int m = 2) : coef(coefficients), mod(m) {
        degree = coef.size() - 1;
        normalPoly();
    }

    Poly(int m = 2, int d = 0) {
        degree = d;
        mod = m;
        for (int i = 0; i <= degree; i++)
            coef.push_back(0);
    }

    Poly(const Poly& poly) {
        coef = poly.coef;
        degree = poly.degree;
        mod = poly.mod;
    }

    Poly& operator= (const Poly& poly) {
        if (this != &poly) {
            coef = poly.coef;
            degree = poly.degree;
            mod = poly.mod;
        }
        return *this;
    }

    Poly& operator= (const int m) {
        degree = 0;
        coef.clear();
        coef.push_back(m);
        return *this;
    }

    bool operator== (const Poly& poly) const {
        return coef == poly.coef;
    }

    bool operator!= (const Poly& poly) const {
        return coef != poly.coef;
    }

    bool operator== (const int m) const {
        return coef.size() == 1 && coef[0] == m;
    }

    bool operator!= (const int m) const {
        return coef.size() != 1 || coef[0] != m;
    }

    Poly operator+ (const Poly& poly) {
        Poly res;
        if (degree > poly.degree)
        {
            res.coef = coef;
            res.degree = degree;
        }
        else
        {
            res.coef = poly.coef;
            res.degree = poly.degree;
        }
        for (int i = 0; i < min(coef.size(), poly.coef.size()); i++)
        {
            res.coef[i] = coef[i] + poly.coef[i];
            while (res.coef[i] < 0)
                res.coef[i] += mod;
            res.coef[i] %= mod;
        }
        return res;
    }

    Poly operator- (const Poly& poly) {
        Poly res(poly.mod, max(coef.size(), poly.coef.size()));
        for (int i = 0; i < coef.size(); i++)
            res.coef[i] = coef[i];
        for (int i = 0; i < poly.coef.size(); i++) {
            res.coef[i] = res.coef[i] - poly.coef[i];
            while (res.coef[i] < 0)
                res.coef[i] += mod;
            res.coef[i] %= mod;
        }
        return res;
    }

    Poly operator* (const Poly& poly) {
        Poly res(poly.mod, degree + poly.degree);
        for (int i = 0; i < coef.size(); i++) {
            for (int j = 0; j < poly.coef.size(); j++) {
                res.coef[i + j] = (coef[i] * poly.coef[j] + res.coef[i + j]) % res.mod;
            }
        }
        return res;
    }

    Poly operator/ (const Poly& poly) 
    {
        Poly temp(*this), res(temp.mod, temp.degree - poly.degree);
        int m = poly.coef[poly.coef.size() - 1];
        for (int i = 1; i < res.mod - 2; i++)
            m *= poly.coef[poly.coef.size() - 1];

        m %= res.mod;
        for (int i = 0; i < res.coef.size(); i++) 
        {
            res.coef[res.coef.size() - i - 1] = (temp.coef[temp.coef.size() - i - 1] * m) % res.mod;
            for (int j = 0; j < poly.coef.size(); j++) 
            {
                temp.coef[temp.coef.size() - j - i - 1] -= poly.coef[poly.coef.size() - j - 1] * res.coef[res.coef.size() - i - 1];
                while (temp.coef[temp.coef.size() - j - i - 1] < 0)
                    temp.coef[temp.coef.size() - j - i - 1] += temp.mod;
                temp.coef[temp.coef.size() - j - i - 1] %= temp.mod;
            }
        }
        return res;
    }

    Poly operator% (const Poly& poly) 
    {
        Poly temp(*this), res(temp.mod, temp.degree - poly.degree);
        int m = poly.coef[poly.coef.size() - 1];
        for (int i = 1; i < res.mod - 2; i++) m *= poly.coef[poly.coef.size() - 1];

        m %= res.mod;
        for (int i = 0; i < res.coef.size(); i++) 
        {
            res.coef[res.coef.size() - i - 1] = (temp.coef[temp.coef.size() - i - 1] * m) % res.mod;
            for (int j = 0; j < poly.coef.size(); j++) 
            {
                temp.coef[temp.coef.size() - j - i - 1] -= poly.coef[poly.coef.size() - j - 1] * res.coef[res.coef.size() - i - 1];

                while (temp.coef[temp.coef.size() - j - i - 1] < 0)
                    temp.coef[temp.coef.size() - j - i - 1] += temp.mod;

                temp.coef[temp.coef.size() - j - i - 1] %= temp.mod;
            }
        }
        temp.normalPoly();
        return temp;
    }

    void printPoly() const 
    {
        for (int i = coef.size() - 1; i >= 0; --i) cout << coef[i] << " ";
        cout << endl;
    }

    void normalPoly() 
    {
        degree = coef.size() - 1;
        while (coef[degree] == 0 && degree > 0) 
        {
            coef.pop_back();
            degree--;
        }
    }

    Poly power(int m) 
    {
        Poly res(mod, m * degree);

        for (int i = 0; i < coef.size(); i++)
            res.coef[i * m] = coef[i];

        return res;
    }

    Poly gcd(Poly f, Poly g) 
    {
        Poly r, result;

        while (f % g != 0) {
           
            r = f % g;
            
            f = g;
            g = r;
        }
        int a = g.coef[g.coef.size() - 1];

        for (int i = 1; i < g.mod - 2; i++) a *= g.coef[g.coef.size() - 1];
        
        a %= g.mod;
        result = a;
        result = result * g;
        result.normalPoly();

        return result;
    }

    bool fermatTest(int num, int t) 
    {
        srand(time(0));

        if (num == 2 || num == 3)
            return true;
        if (num % 2 == 0)
            return false;

        for (t; t > 0; t--) 
        {
            int a = 2 + rand() % (num - 3), r;
            r = pow(a, num - 1);
            r %= num;

            if (r != 1)
                return false;
        }

        return true;
    }

    vector<int> factor(int n) {
        vector<int> fact;

        if (n < 1) return fact;

        if (fermatTest(n, 10)) {
            fact.push_back(n);
            return fact;
        }

        int  k = 1;
        bool flag = true, _break = false;
        int d = sqrt(n);
        int dk = 0;

        while (n % 2 == 0) {
            fact.push_back(2);
            n /= 2;
        }

        while (dk <= d && n != 1 && !_break) {
            if (flag == true) {
                if (k == 1) {
                    dk = 3;
                }
                else
                    if (k == 2) {
                        dk = 5;
                    }
                    else
                        if (k % 2 != 0) {
                            dk = dk + 2;
                        }
                        else {
                            dk = dk + 4;
                        }
            }

            int q = n / dk;
            int r = n % dk;

            if (r == 0) {
                fact.push_back(dk);
                n = q;
                flag = false;
            }
            else 
                if (q > dk) 
                {
                    flag = true;
                    k++;
                }
                else 
                { 
                    fact.push_back(n);
                    _break = true;
                }

        }
        return fact;
    }

    bool isIrreducible()
    {
        Poly x, d;

        x.coef.push_back(1);
        x.degree = 1;
        x.mod = mod;
        Poly u = x;

        for (int i = 0; i < degree / 2; i++)
        {
            // u(x) = u(x)^p mod f(x)
            u = u.power(mod) % *this;
            // d(x) = НОД(f(x), u(x) - x)
            d = Poly::gcd(*this, u - x);

            if (d != 1) return false;
        }

        return true;
    }

    bool isPrimitive() {
        if (!isIrreducible()) return false; // Приводимый => непримитивный

        int pn = pow(mod, degree) - 1;
        vector<int> p = factor(pn);
        Poly x, r;
        x.coef.push_back(1);
        x.degree = 1;
        x.mod = mod;

        for (int i = 0; i < p.size(); i++)
        {
            // r(x) = x^((p^n - 1)/q) mod f(x)
            r = x.power(pn / p[i]) % *this;

            if (r == 1) return false;
            // Пропускаем степени простого
            for (; i + 1 < p.size() && p[i] == p[i + 1]; i++);
        }

        return true;
    }
};

int main()
{
    while (true) 
    {
        vector<int> coef;
        vector<int>::const_iterator it = coef.begin();
        int n, coef_row, num, Z;

        cout << "Insert Z = ";
        cin >> Z;

        cout << "Insert n = ";
        cin >> n;

        cout << "Insert coef[n] = ";
        cin >> coef_row;

        for (int i = 0; i < n + 1; i++) 
        {
            num = coef_row % 10;
            coef_row = coef_row / 10;
            coef.insert(it, num);
            it = coef.begin();
        }

        Poly x(coef, Z); bool isIr, isPr;
        isIr = x.isIrreducible();
        isPr = x.isPrimitive();

        cout << "Polynom is ";
        if (isIr) cout << "irreducible and "; else cout << "reducible and ";
        if (isPr) cout << "primitive\n\n"; else cout << "non-primitive\n\n";
    }
}
