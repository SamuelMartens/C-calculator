/*Create simple calculator*/

#include <cstdio>
#include "calc.h"

int LAST_ERROR = 0;


/* sizeof(int k) return 16 
1) Do validation of Build in Fucntion (its must ends with "()" )
(5+6)*(11-2/4*(11+1)+12)+(7+2) == 196
*/

int main()
{
	variable_scope varScope;

	for (;;){

		raw_string rawString;
		raw_materials rawMat;
		string_func strFunc;
		float fResult;
		subexp suSubExp[SZ_GLOBAL_SIZE];


		cout << "<< ";
		fgets(rawString.cRawString, SZ_RAW_STRING, stdin);
	
		rawString.cRawString[strFunc.getStrLen(rawString.cRawString)-2] = '\0';

		if (strFunc.isSameStr(rawString.cRawString, "exit()")) break;

		rawString.removeWhitesp();
		// LAST_ERROR = rawString.validateAll();
		if (LAST_ERROR != 0) {
			cout << "Syntax error \n";
		//	fgets(rawString.cRawString, SZ_RAW_STRING, stdin);
			return 1;
		}
		rawString.splitOnSubExp(suSubExp);
		fgets(rawString.cRawString, SZ_RAW_STRING, stdin);
		return 0;
		LAST_ERROR = rawMat.getTokens(suSubExp[0].exp, suSubExp, varScope);
		if (LAST_ERROR != 0) {
			cout << "Syntax error in token \n";
	//		fgets(rawString.cRawString, SZ_RAW_STRING, stdin);
			return 1;
		}
		fResult = rawMat.getResult();
		cout << "<< " << fResult << "\n";
	//	fgets(rawString.cRawString, SZ_RAW_STRING, stdin);
	}

	return 0;
}


