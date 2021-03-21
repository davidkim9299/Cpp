#include "Complex.h"
#include <iostream>
#include <string>

using namespace std;

void Complex::showResult(double real, double imag)
{
	char sign = '+';
	if (imag < 0) sign = '-';
	if (real == 0 && imag == 0)
	{
		cout << "\nResult : " << 0 << endl;
	}
	else if (real == 0)
	{
		cout << "\nResult : " << imag << "i\n" << endl;
	}
	else if (imag == 0)
	{
		cout << "\nResult : " << real << endl;
	}
	else
	{
		cout << "\nResult : " << real << ' ' << sign << ' ' << abs(imag) << "i\n" << endl;
	}
}


void Convert::calculate()
{
	double fr, fi;
	string choice;
	int code;

	do {
		cout << "\nEnter the code for conversion";
		cout << "\nEnter 1 for addition";
		cout << "\nEnter 2 for subtraction";
		cout << "\nEnter 3 for multiply";
		cout << "\nEnter 4 for division";
		cout << "\nCode : ";

		cin >> code;
		try
		{
			if (cin.fail()) throw runtime_error("Input is not on the list\n");
			else
				if (code == 1)
				{
					fr = real1 + real2;
					fi = imag1 + imag2;
					Complex a;
					a.showResult(fr, fi);

				}
				else if (code == 2) {
					fr = real1 - real2;
					fi = imag1 - imag2;
					Complex b;
					b.showResult(fr, fi);
				}
				else if (code == 3)
				{
					fr = real1 * real2 + (imag1 * imag2 * -1);
					fi = real1 * imag2 + real2 * imag1;
					Complex c;
					c.showResult(fr, fi);

				}
				else if (code == 4)
				{
					fr = ((real1 * real2) - (imag1 * imag2 * -1)) / ((real2 * real2) + (imag2 * imag2));
					fi = ((-1 * real1 * imag2) + (imag1 * real2)) / ((real2 * real2) + (imag2 * imag2));
					Complex d;
					d.showResult(fr, fi);
				}
				else
				{
					cout << "Enter the right code" << endl;
				}
		}
		catch (const runtime_error& e)
		{
			cout << "\nERROR : " << e.what();
			system("pause");
		}

		cout << "Another result ? (y/n) :";
		cin >> choice;
	} while (choice == "y");

}
