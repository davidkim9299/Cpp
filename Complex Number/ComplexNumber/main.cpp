#include "Complex.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
	string complex1, complex2;
	string choice;

	do
	{
		cout << "Enter first complex number in 'a+bi' form : ";
		getline(cin, complex1);
		complex1.erase(remove_if(complex1.begin(), complex1.end(), isspace), complex1.end());
		int index1 = complex1.find_last_of("+-");
		int r1 = atoi(complex1.substr(0, index1).c_str());
		int i1 = atoi(complex1.substr(index1++).c_str());

		cout << "First real part : " << r1 << endl;
		cout << "First imag part : " << i1 << 'i' << endl;

		cout << "Enter second complex number in 'a+bi' form : ";
		getline(cin, complex2);
		complex2.erase(remove_if(complex2.begin(), complex2.end(), isspace), complex2.end());
		int index2 = complex2.find_last_of("+-");
		int r2 = atoi(complex2.substr(0, index2).c_str());
		int i2 = atoi(complex2.substr(index2++).c_str());

		cout << "Second real part : " << r2 << endl;
		cout << "Second imag part : " << i2 << 'i' << endl;

		Convert a(r1, i1, r2, i2);
		a.calculate();

		cout << "Continue ? (y/n) : ";
		cin >> choice;

	} while (choice == "y");


	system("pause");
	return 0;
}
