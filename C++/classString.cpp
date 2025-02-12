#include <iostream>
#include <cstdio>
using namespace std;

class String
{
	char* str;
	int len;

public:

	String(int l = 0);
	String(const char*);
	String(const String&);
	~String() { if (str) delete[]str; str = NULL; }

	int Len() { return len; }

	String& operator = (const String&);
	char& operator [](int);

	bool operator == (String&);
	bool operator != (String&);

	String operator + (const String&);
	String& operator += (const String&);

	friend istream& operator >> (istream&, String&);
	friend ostream& operator << (ostream& r, String& s) { r << s.str; return r; }

	int BMH_search(String&);
};

String::String(int l)
{
	str = new char[l + 1];
	len = l;
	str[l] = '\0';
}

String::String(const char* s)
{
	len = strlen(s);
	str = new char[len + 1];
    
	for (int i = 0; s[i] != '\0'; str[i] = s[i], i++);
	str[len] = '\0';
}

String::String(const String& s)
{
	len = s.len;
	str = new char[len + 1];

	for (int i = 0; s.str[i] != '\0'; str[i] = s.str[i], i++);
	str[len] = '\0';
}

String& String::operator = (const String& s)
{
	len = s.len;
	char* buf = new char[len + 1];

	for (int i = 0; s.str[i] != '\0'; buf[i] = s.str[i], i++);
	buf[len] = '\0';

	delete[]str;
	str = buf;

	return *this;
}

char& String::operator [](int index)
{
	if (index >= len || index < 0) { puts("Index outside the row"); exit(0); }
    
	return str[index];
}

bool String::operator == (String& s)
{
	if (len != s.len) return false;
    
	int i = 0;
	while (str[i] == s.str[i] && i < len) { i++; }
    
	if (i == len) return true;
    
	return false;
}

bool String::operator != (String& s)
{
	if (len != s.len) return true;
    
	int i = 0;
	while (str[i] == s.str[i] && i < len) { i++; }
    
	if (i == len) return false;
    
	return true;
}

String String::operator + (const String& s)
{
	int _len = len + s.len;
	String buf(_len);
    
	strcpy_s(buf.str, buf.len + 1, str);
	strcat_s(buf.str, buf.len + 1, s.str);

	return buf;
}

String& String::operator += (const String& s)
{
	char* buf = new char[len + s.len + 1];

	strcpy_s(buf, len + s.len + 1, str);
	strcat_s(buf, len + s.len + 1, s.str);

	delete[]str;

	len += s.len;
	str = buf;

	return *this;
}

istream& operator >> (istream& r, String& s)
{
	char buf[128];
	gets_s(buf);

	String BUF(buf);
	s = BUF;

	return r;
}

int String::BMH_search(String& s) // str - строка, s.str - образец
{
	if (len == 0 || s.len == 0) return -2;
	if (!str && !s.str) return -3;

	int i, j, k;
	size_t Tab[256];

	for (i = 0; i < 256; i++) { Tab[i] = s.len; }
	for (i = 0; i < s.len - 1; i++) { Tab[s.str[i]] = s.len - 1 - i; } // этап предтрансляции

	i = s.len - 1; j = s.len - 1;
	while (i < len && j >= 0)
	{
    	k = i; j = s.len - 1;
    	while(j >= 0 && str[k] == s.str[j])
    	{
        	j--; k--;
    	}
    	if (j >= 0)
    	{
        	i = i + Tab[str[i]];
        	j = s.len - 1;
    	}
	}

	if (j >= 0) return -1;
	return i + 1 - s.len;
}

int main()
{
	/*String a, b("Example"), c(" is simple"), d(b);
	cout << a << "\n\n" << b << "\n\n" << c << "\n\n" << d << "\n\n";
    
	a = c;
	cout << a << "\n\n";

	char symbol = b[3];
	cout << symbol << "\n\n";

	if (b == d) cout << "Equal\n\n"; else cout << "Not Equal\n\n";
	if (b != c) cout << "Not Equal\n\n"; else cout << "Equal\n\n";

	a = b + c;
	cout << a << "\n\n";

	b += c;
	cout << b << "\n\n";

	cin >> a;
	cout << a << "\n";*/
	/*
	String a, b("Example"), c(" is simple");
	a = b + c;
	cout << a << "\n" << b << "\n";
	*/
    
	String str("mot to To be or not to be"), obr("not to");
	int flag = str.BMH_search(obr);
    
	if (flag > 0) cout << "Position is " << flag << "\n";
	if (flag == -1) cout << "Position not found\n";
	if (flag < -1) cout << "Error: bad digits\n";
    
	return 0; //Position is 16
}
