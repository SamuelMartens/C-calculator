/*Create simple calculator*/

#include <iostream>
#include <cstdio>
using namespace std;

struct opr {
	char symbol;
	int left_op;
	int right_op;
};

// ivan: it would be better to move all function declarations to separated header file
void removeWhitesp(char *p);
int getTokens(char *p, float *pOperands, opr *pOperations, int *pCountDigits, int *pCountSymbols);
float getResult_old(opr *pOperations, float *pOperands, int *pCountDigits);
float getResult(opr *pOperations, float *pOperands, int *pCountDigits);
float doOperation(float fOperandL, float fOperandR, opr cOperation);
bool isInArray(char cSymbol,char *pContainer);
float charToFloat(char *p, int iStartNum, int iEndNum);


int main()
{
	const int szRawString = 80 , szGlobalSize = 20;
	char cRawString[szRawString];
	float fOperands[szGlobalSize], fResult;
	int iCountDigits=0, iCountSymbols=0;
	int iError = 0;
	opr opOperations[szGlobalSize];

	cout << "Print your equation: ";
	gets_s(cRawString, szRawString);

	removeWhitesp(cRawString);
	iError = getTokens(cRawString, fOperands, opOperations, &iCountDigits, &iCountSymbols);
	if (iError != 0) {
		cout << "Error in your equation";
		return 1;
	}
	// Turn tprefix "f"
	fResult = getResult(opOperations, fOperands, &iCountDigits);
	cout << "Result " << fResult << "\n";

	return 0;
}


void removeWhitesp(char *p)
{	
	char cNext;
	bool bWtLeft = true;
	 
	while (bWtLeft){
		bWtLeft = false;
		for (int i = 0; *(p + i); i++) {
			if (*(p + i) == ' ') {
				bWtLeft = true;
				for (int k = i; *(p + k); k++) {
					*(p + k) = *(p + k + 1);
				}
			}
		}
	}
}


int getTokens(char *p, float *pOperands, char *pOperations, int *pCountDigits, int *pCountSymbols )
{
	int iStartNum = -1, iEndNum = 0, iOperandsPos = 0, iOperationsPos = 0;
	const char cSymbols[] = "*/+-", cDigits[] = "0123456789", cSpecialSymbols[] = ".()";

	for (int i = 0; *(p + i); i++) {
		
		if (isInArray(*(p + i), cDigits)) {

			if (iStartNum == -1) iStartNum = i;
			else iEndNum = i;

			if (isInArray(*(p + i + 1), cSymbols) || !(*(p + i + 1))) {
				if (iEndNum < iStartNum) iEndNum = iStartNum;
				*(pOperands + iOperandsPos) = charToFloat(p, iStartNum, iEndNum);
				(*pCountDigits)++;
			} 

		}
		else if (isInArray(*(p + i), cSymbols)) {

			if (iStartNum != -1) { 
				iStartNum = -1;
				iOperandsPos++;
			}
			*(pOperations + iOperationsPos).symbol = *(p + i);
			*(pOperations + iOperationsPos).left_op = i - 1;
			*(pOperations + iOperationsPos).right_op = i + 1;
			iOperationsPos++;
			(*pCountSymbols)++;
		}
		else if (isInArray(*(p + i), cSpecialSymbols)) {
			continue;
		}
		else {
			return 1;
		}
	}

	if (*pCountDigits - *pCountSymbols != 1 || *pCountDigits < 2) return 1;

	return 0;
}


bool isInArray(char cSymbol, char *pContainer) 
{
	for (int i = 0; *(pContainer + i); i++) {
		if (cSymbol == *(pContainer + i)) return true;
	}

	return false;
}


float charToFloat(char *p, int iStartNum, int iEndNum)
{
	int iTensNum = 1;
	float fCurNum, fFinalNum = 0;
	bool bAfterPoint = false;

	for (int i = iStartNum; i <= iEndNum; i++) {
		if (*(p + i) == '.') {
			bAfterPoint = true;
			continue;
		}
		fCurNum = (float)*(p + i);
		fCurNum = fCurNum - 48;
		switch (bAfterPoint)
		{
		case false:
			fFinalNum = fFinalNum * 10 + fCurNum;
			break;
		case true:
			fFinalNum = fFinalNum + fCurNum / (10 * iTensNum);
			iTensNum++;
			break;
		}

	}
	
	cout << "charToFloat " << fFinalNum << "\n";
	return fFinalNum;
}


float getResult_old(char *pOperations, float *pOperands, int *pCountDigits) 
{
	
	int k=1;
    float iResult;

	iResult = doOperation(*(pOperands), *(pOperands + 1), *pOperations);

	for (int i = 2; i < *pCountDigits; i++) {
		iResult = doOperation(iResult, *(pOperands + i), *(pOperations + k));
		k++;
		cout << "Result1 " << iResult << "\n";
	}

	return iResult;
}

float getResult(opr *pOperations, float *pOperands, int *pCountDigits)
{
	const char cOperationSymb[] = "*/+-";
	float fResult;

	for (int i = 0; i < *pCountDigits, i++) {
		if (isInArray(*(pOperations + i).symbol, ))
		
	}
	
	return fResult;
}


float doOperation(float fOperandL, float fOperandR, opr cOperation)
{
	switch (cOperation)
	{
	case '+':
		return fOperandL + fOperandR;
	case '-':
		return fOperandL - fOperandR;
	}

	return 0;
}
