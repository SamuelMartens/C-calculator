/*Create simple calculator*/

#include <iostream>
#include <cstdio>
using namespace std;

// ivan: it would be better to move all function declarations to separated header file
void removeWhitesp(char *p);
int getTokens(char *p, float *pOperands, char *pOperations, int *pCountDigits, int *pCountSymbols);
int charToInt(char *p, int iStartNum, int iEndNum);
float getResult(char *pOperations, float *pOperands, int *pCountDigits);
float doOperation(float iOperandL, float iOperandR, char cOperation);
bool isInArray(char cSymbol,char *pContainer);
bool isFloat(char *p, int iStartNum, int iEndNum);
float charToFloat(char *p, int iStartNum, int iEndNum);

int main()
{
	const int szRawString = 80 , szGlobalSize = 20;
	char cRawString[szRawString], operations[szGlobalSize];
	float operands[szGlobalSize], iResult;
	int iCountDigits=0, iCountSymbols=0;
	int iError = 0;

	cout << "Print your equation: ";
	gets_s(cRawString, szRawString);

	removeWhitesp(cRawString);
	iError = getTokens(cRawString, operands, operations, &iCountDigits, &iCountSymbols);
	if (iError != 0) {
		cout << "Error in your equation";
		return 1;
	}
	// Turn tprefix "f"
	iResult = getResult(operations, operands, &iCountDigits);
	cout << "Result " << iResult << "\n";

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
	char cSymbols[] = "+-*/", cDigits[] = "0123456789", cSpecialSymbols[] = ".()";

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
			*(pOperations + iOperationsPos) = *(p + i);
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


int charToInt(char *p, int iStartNum, int iEndNum) 
{
	
	int iCurNum;
	int iFinalNum = 0;

	for (int i = iStartNum; i <= iEndNum; i++ ) {
		iCurNum = (float) *(p + i);
		iCurNum = iCurNum - 48;
		cout << "Icur " << iCurNum << "\n";
		iFinalNum = iFinalNum * 10 + iCurNum;
	}

	return iFinalNum;
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
			cout << "Cur Num " << fCurNum << "\n";
			cout << "Final Num " << fFinalNum << "\n";
			cout << "TensNum " << iTensNum << "\n";
			cout << "\n \n";
			break;
		}

	}
	
	cout << "charToFloat " << fFinalNum << "\n";
	return fFinalNum;
}


float getResult(char *pOperations, float *pOperands, int *pCountDigits) 
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


float doOperation(float iOperandL, float iOperandR, char cOperation)
{
	//Change on "f" prefix 
	cout << "L " << iOperandL << " " << iOperandR << "\n";
	switch (cOperation)
	{
	case '+':
		return iOperandL + iOperandR;
	case '-':
		return iOperandL - iOperandR;
	}

	return 0;
}

bool isFloat(char *p, int iStartNum, int iEndNum) 
{
	for (int i = iStartNum; i <= iEndNum; i++) {
		if (*(p + i) == '.') return true;
	}

	return false;
}

