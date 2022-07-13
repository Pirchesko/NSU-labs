#include <iostream>
#include "TritSet.h";

using namespace std;

int main()
{
	TritSet a(5);
	a[0] = Trit::True;
	a[1] = Trit::False | a[0];
	TritSet b(5);
	TritSet c;
	c = a | b;
}