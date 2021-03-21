#include <iostream>
#include <string>

class Complex
{
	double realpart;
	double imaginary;
public:
	Complex(double real = 0.0, double imag = 0.0) { realpart = real; imaginary = imag; }
	void showResult(double, double);
};

class Convert
{
private:
	double real1, real2, imag1, imag2; //final realpart, final imaginary part
public:
	Convert(double realpart1 = 0.0, double imagpart1 = 0.0, double realpart2 = 0.0, double imagpart2 = 0.0)
	{
		real1 = realpart1;
		imag1 = imagpart1;
		real2 = realpart2;
		imag2 = imagpart2;
	}
	void calculate();
};
