
#include <iostream>


#ifndef C_CALCULATOR_CALC_H
#define C_CALCULATOR_CALC_H


const int SZ_RAW_STRING = 80;
const int SZ_GLOBAL_SIZE = 20;
extern int LAST_ERROR;


class opr {
public:
    char symbol;
    char processed;
    int left_op;
    int right_op;

    opr() {processed = 0;};
    float doOperation(float fOperandL, float fOperandR);
};

class subexp {
public:
    // Use contstructor instead default value
    char exp[SZ_RAW_STRING]="\0";
    int level = 0;
    float result;
};

class raw_string{
public:
    char cRawString[SZ_RAW_STRING];

    void removeWhitesp();
    void splitOnSubExp(subexp *pSubExp);
	int validateAll();
    int validateParentheses();
	int validateSpecialSymbols();
};

class raw_materials{
public:
    int iCountDigits;
    int iCountSymbols;
    float fOperands[SZ_GLOBAL_SIZE];
    opr opOperations[SZ_GLOBAL_SIZE];

    raw_materials() {iCountDigits = 0; iCountSymbols = 0;};
    int getTokens(char *p, subexp *pSubExp);
    void doOperationGroup(char *pOperationGroup, const int *pOperationGroupLim, int &iProcDigits);
    float getResult();
};

class parser{
public:
	float parseDigit(char *p, int &iStartParse, bool bReturnParseIndex = false, bool bIsNegative=false);
	int parseBuildInFunc(char *p, int &iStartParse, subexp *pSubExp, bool bReturnParseIndex = false);
	// Think about change of the bReturnParseIndex ( maybe remove it and just always true?)
	int parseSubExp(char *p, int &iStartParse, subexp *pSubExp ,bool bReturnParseIndex = false);
	int parseFuncArgs(char *p, int &iStartParse, subexp *pSubExp, float *pArgs, int &iArgsNum);
};

class build_in_func {
	// Dont forget about zero symbol when correct size
	char cFuncNames[3][6] = { "abs", "power" };
	// It is number of function args. Must go in the same order as names
	const int iFuncArgsNum[2] = { 1, 2 };
public:
	// Staff functions
	int chooseFunc(char *p);
	float doFunction(char *pFuncName, float *pArgs, int iArgsNum);
	// Allowed functions
	float abs(float fNumber);
	float power(float fNumber, int iPowerNum);
};

class string_func {
public:
	int concatStr(char *pOriginS, char *pAdditStr);
	int getStrLen(char *p);
	bool isSameStr(char *p1, char *p2);
};


int getParent(subexp *pSubExp, int iChildInd);
int floatToChar(float fDigit, char *pCharDigit, const int iDigitSize=8);
int charToInt(char *p, int iStartNum, int iEndNum);
int floatToInt(float fNumber);
float charToFloat(char *p, int iStartNum, int iEndNum);
char isInArray(char cSymbol,char *pContainer, int iStart = -1, int iEnd = -1);
bool isChar(char cSymbol);
bool isDigit(char cSymbol);

using namespace std;

#endif //C_CALCULATOR_CALC_H
