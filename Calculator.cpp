/*Create simple calculator*/

#include <cstdio>
#include "calc.h"


/* sizeof(int k) return 16 
1) Do validation of Build in Fucntion (its must ends with "()" )
(5+6)*(11-2/4*(11+1)+12)+(7+2) == 196
*/

int main()
{
	variable_scope varScope;

	for (;;)
	{

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
		try
		{
			rawString.validateAll();
			rawString.splitOnSubExp(suSubExp);
			rawMat.getTokens(suSubExp[0].exp, suSubExp, varScope);
			if (suSubExp[0].getStatmentType() == Equation)
			{
				fResult = rawMat.getResult();
				cout << "<< " << fResult << "\n";
			}
		}
		catch (error_type Err) 
		{
			rawString.showError(Err);
		}
	}

	return 0;
}


