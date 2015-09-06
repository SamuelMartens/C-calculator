/*Create simple calculator*/

#include <iostream>
#include <cstdio>
#include <stdio.h>
using namespace std;

struct opr {
	char symbol;
	int left_op;
	int right_op;
};


// ivan: it would be better to move all function declarations to separated header file
void removeWhitesp(char *p);
void getStringPart(char *pFullString, char *pPartString, int &iStart, int &iEnd);
void doOperationGroup(opr *pOperations, float *pOperands, char *pOperationGroup, int *pOperationGroupLim, int *pCountDigits, int &iProcDigits);
int getTokens(char *p, float *pOperands, opr *pOperations, int *pCountDigits, int *pCountSymbols);
float getResult(opr *pOperations, float *pOperands, int *pCountDigits);
float doOperation(float fOperandL, float fOperandR, char cOperation);
float charToFloat(char *p, int iStartNum, int iEndNum);
char isInArray(char cSymbol,char *pContainer);




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
	char cSymbols[] = "*/+-", cDigits[] = "0123456789", cSpecialSymbols[] = ".()";

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
			*(pOperations + iOperationsPos)->symbol = *(p + i);
			*(pOperations + iOperationsPos)->left_op = i - 1;
			*(pOperations + iOperationsPos)->right_op = i + 1;
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


char isInArray(char cSymbol, char *pContainer)
{
	for (int i = 0; *(pContainer + i); i++) {
		if (cSymbol == *(pContainer + i)) return cSymbol;
	}

	return '\0';
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


float getResult(opr *pOperations, float *pOperands, int *pCountDigits)
{
	const int iOperSz = 5, cPriority1Sz[] = {0, 2}, cPriority2Sz[] = {2, 4};
    const char cOperationSymb[iOperSz] = "*/+-";
	int iProcDigits = 0;
    char cPartString[iOperSz];

    // Do multiple and division
    doOperationGroup(pOperations, pOperands, cOperationSymb, &cPriority1Sz, pCountDigits, iProcDigits);
    if (iProcDigits == *(pCountDigits) -1) return *(pOperands);
    // Do plus and minus
    doOperationGroup(pOperations, pOperands, cOperationSymb, &cPriority2Sz, pCountDigits, iProcDigits);
	return *(pOperands);
}


float doOperation(float fOperandL, float fOperandR, char cOperation)
{
	switch (cOperation)
	{
	case '+':
		return fOperandL + fOperandR;
	case '-':
		return fOperandL - fOperandR;
    case '*':
        return fOperandL * fOperandR;
    case '/':
        return  fOperandL / fOperandR;
	}

	return 0;
}


void getStringPart(char *pFullString, char *pPartString , int &iStart, int &iEnd)
{
    int k;
    for (int i = iStart, k = 0; i < iEnd; i++, k++) *(pPartString + k) = *(pFullString + i);
    if (*(pFullString + k + 1)) *(pFullString  + k + 1) = '\0';
}

void doOperationGroup(opr *pOperations, float *pOperands, char *pOperationGroup, int *pOperationGroupLim, int *pCountDigits, int &iProcDigits)
{
    char cCurOperation = '\0';

    for (int i= 0; i < *pCountDigits; i++){
        for (int k = *(pOperationGroupLim); k < *(pOperationGroupLim + 1); k++) {
            cCurOperation = (isInArray(*(pOperations + k)->symbol,
                                       getStringPart(cOperationSymb, cPartString, cPriority1Sz[0], cPriority1Sz[1])));
            if (cCurOperation) {
                *(pOperands + *(pOperations + k)->left_op) = doOperation(*(pOperands + *(pOperations +k)->left_op),
                                                                         *(pOperands + *(pOperations + k) ->right_op),cCurOperation);
                if (*(pOperations + k + 1)) *(pOperations + k +1)->right_op = *(pOperations + k) ->right_op;
                iProcDigits+=1;
            }
        }
    }
}