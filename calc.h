

#ifndef C_CALCULATOR_CALC_H
#define C_CALCULATOR_CALC_H

#include <iostream>
#include "variable_scope.h"

const int SZ_RAW_STRING = 80;
const int SZ_GLOBAL_SIZE = 20;
extern int LAST_ERROR;

enum statment_type { Equation, Setter, VoidFunc };
enum error_type 
{
	NotAllowedSybolError,
	ParentlessError,
	SyntaxError,
	VariableInitError,
	BuildInFuncExistanceError,
	UndefinedError,
	// Use it if the stament is without err and need to return something
	StatmentIsOk
};

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

	statment_type getStatmentType();
};

class raw_string{
public:
    char cRawString[SZ_RAW_STRING];

	raw_string() { cRawString[0] = '\0'; };
    void removeWhitesp();
    void splitOnSubExp(subexp *pSubExp);
	int validateAll();
    int validateParentheses();
	int validateSpecialSymbols();
	void showError(error_type Err);
};

class raw_materials{
public:
    int iCountDigits;
    int iCountSymbols;
    float fOperands[SZ_GLOBAL_SIZE];
    opr opOperations[SZ_GLOBAL_SIZE];

    raw_materials() {iCountDigits = 0; iCountSymbols = 0;};
    int getTokens(char *p, subexp *pSubExp, variable_scope &varScope);
    void doOperationGroup(char *pOperationGroup, const int *pOperationGroupLim, int &iProcDigits);
    float getResult();
};

class parser{
public:
	float parseDigit(char *p, int &iStartParse, bool bReturnParseIndex = false, bool bIsNegative=false);
	int parseBuildInFunc(char *p, int &iStartParse, subexp *pSubExp, variable_scope &varScope, bool bReturnParseIndex = false);
	// Think about change of the bReturnParseIndex ( maybe remove it and just always true?)
	int parseSubExp(char *p, int &iStartParse, subexp *pSubExp , variable_scope &varScope, bool bReturnParseIndex = false);
	int parseFuncArgs(char *p, int &iStartParse, subexp *pSubExp, float *pArgs, int &iArgsNum ,variable_scope &varScope);
	int parseVariable(char *p, int &iStartParse, variable_scope &varScope);
	void parseFuncName(char *p, char *cFuncName, int &iStartParse, bool bReturnParseIndex = false);
};

class build_in_func {
	// !Dont forget about zero symbol when correct size
	char cFuncNames[3][6] = { "abs", "power","show" };
	// It is number of function args. Must go in the same order as names
	const int iFuncArgsNum[3] = { 1, 2, 1 };
public:
	// Staff functions
	int chooseFunc(char *p);
	float doFunction(char *pFuncName, float *pArgs, int iArgsNum);
	// Allowed functions
	float abs(float fNumber);
	float power(float fNumber, int iPowerNum);
	void show(float var);
};


int getParent(subexp *pSubExp, int iChildInd);
int floatToChar(float fDigit, char *pCharDigit, const int iDigitSize=8);
int charToInt(char *p, int iStartNum, int iEndNum);
int floatToInt(float fNumber);
float charToFloat(char *p, int iStartNum, int iEndNum);
char isInArray(char cSymbol,char *pContainer, int iStart = -1, int iEnd = -1);
bool isChar(char cSymbol);
bool isDigit(char cSymbol);
int isFunc(char *p, int &iStartParse);

using namespace std;

#endif //C_CALCULATOR_CALC_H
