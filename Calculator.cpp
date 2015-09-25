/*Create simple calculator*/

#include <iostream>
#include <cstdio>
#include <math.h>
#include "calc.h"
using namespace std;

/*

Validation do:
 1) Check dublicated operations
 2) Check if the operation presents between operands and parentheses

 */

int main()
{
	raw_string rawString;
	raw_materials rawMat;
	float fResult;
	subexp suSubExp[SZ_GLOBAL_SIZE];

	cout << "Print your equation: ";
	fgets(rawString.cRawString, SZ_RAW_STRING, stdin);
	rawString.cRawString[getStrLen(rawString.cRawString)-2] = '\0';

	rawString.removeWhitesp();
	LAST_ERROR = rawString.checkParentheses();
	if (LAST_ERROR != 0) {
		cout << "Parentheses error \n";
		return 1;
	}
	rawString.splitOnSubExp(suSubExp);
	LAST_ERROR = rawMat.getTokens(suSubExp[0].exp, suSubExp);
	if (LAST_ERROR != 0) {
		cout << "Syntax error \n";
		return 1;
	}

	fResult = rawMat.getResult();
	cout << "Result " << fResult << "\n";

	return 0;
}


