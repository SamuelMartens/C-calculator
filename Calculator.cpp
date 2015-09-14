/*Create simple calculator*/

#include <iostream>
#include <cstdio>
using namespace std;

const int SZ_RAW_STRING = 80;

struct opr {
	char symbol;
	char processed = 0;
	int left_op;
	int right_op;
};

struct subexp {
	// This size must be the same as in szRawString
	char exp[SZ_RAW_STRING] = "\0";
	int level = 0;
	float result;
};


// ivan: it would be better to move all function declarations to separated header file
void removeWhitesp(char *p);
void splitOnSubExp(subexp *pSubExp, char *pRawString);
void doOperationGroup(opr *pOperations, float *pOperands, char *pOperationGroup, const int *pOperationGroupLim, int *pCountDigits, int &iProcDigits);
int getTokens(char *p, float *pOperands, opr *pOperations, int *pCountDigits, int *pCountSymbols);
int getStrLen(char *p);
int getParent(subexp *pSubExp, int iChildInd);
float getResult(opr *pOperations, float *pOperands, int *pCountDigits);
float doOperation(float fOperandL, float fOperandR, char cOperation);
float charToFloat(char *p, int iStartNum, int iEndNum);
char isInArray(char cSymbol,char *pContainer, int iStart = -1, int iEnd = -1);
char *floatToChar(float fDigit);


int main()
{
	const int szGlobalSize = 20;
	char cRawString[SZ_RAW_STRING];
	float fOperands[szGlobalSize], fResult;
	int iCountDigits=0, iCountSymbols=0;
	int iError = 0;
	opr opOperations[szGlobalSize];
	subexp suSubExp[szGlobalSize];

	cout << "Print your equation: ";
	fgets(cRawString, SZ_RAW_STRING, stdin);
	cRawString[getStrLen(cRawString)-2] = '\0';

	removeWhitesp(cRawString);
	splitOnSubExp(suSubExp, cRawString);
	return 1; //Debug
	iError = getTokens(cRawString, fOperands, opOperations, &iCountDigits, &iCountSymbols);
	if (iError != 0) {
		cout << "Error in your equation";
		return 1;
	}

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


int getTokens(char *p, float *pOperands, opr *pOperations, int *pCountDigits, int *pCountSymbols )
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
			(pOperations + iOperationsPos)->symbol = *(p + i);
			(pOperations + iOperationsPos)->left_op = iOperationsPos;
			(pOperations + iOperationsPos)->right_op = iOperationsPos + 1;
			iOperationsPos++;
			(*pCountSymbols)++;
		}
		else if (isInArray(*(p + i), cSpecialSymbols)) {
			continue;
		}
		else {
			cout << "Error " << *(p + i) << "\n";
			return 1;
		}
	}

	if (*pCountDigits - *pCountSymbols != 1 || *pCountDigits < 2) return 1;

	return 0;
}


char isInArray(char cSymbol, char *pContainer, int iStart, int iEnd )
{
	if (iStart == -1 && iEnd == -1)
	{
		for (int i = 0; *(pContainer + i); i++) {
			if (cSymbol == *(pContainer + i)) return cSymbol;
		}
	}
	else if (iStart != -1 && iEnd == -1)
	{
		for (int i = iStart; *(pContainer + i); i++) {
			if (cSymbol == *(pContainer + i)) return cSymbol;
		}
	}
	else if (iStart == -1 && iEnd != -1)
	{
		for (int i = 0; i < iEnd && *(pContainer + i); i++) {
			if (cSymbol == *(pContainer + i)) return cSymbol;
		}
	}
	else
	{
		for (int i = iStart; i < iEnd && *(pContainer + i); i++) {
			if (cSymbol == *(pContainer + i)) return cSymbol;
		}
	};
	
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
	
	return fFinalNum;
}


float getResult(opr *pOperations, float *pOperands, int *pCountDigits)
{
	const int iOperSz = 5, cPriority1Sz[] = {0, 2}, cPriority2Sz[] = {2, 4};
    char cOperationSymb[iOperSz] = "*/+-";
	int iProcDigits = 0;
    char cPartString[iOperSz];

    // Do multiple and division
    doOperationGroup(pOperations, pOperands, cOperationSymb, cPriority1Sz, pCountDigits, iProcDigits);
    if (iProcDigits == *pCountDigits - 1) return *(pOperands);
    // Do plus and minus
    doOperationGroup(pOperations, pOperands, cOperationSymb, cPriority2Sz, pCountDigits, iProcDigits);
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


void doOperationGroup(opr *pOperations, float *pOperands, char *pOperationGroup,const int *pOperationGroupLim, int *pCountDigits, int &iProcDigits)
{
    char cCurOperation = '\0';
    for (int i = 0; i < *pCountDigits-1; i++){
		cCurOperation = isInArray((pOperations + i)->symbol, pOperationGroup, *(pOperationGroupLim), *(pOperationGroupLim + 1));
        if (cCurOperation) {
			*(pOperands + (pOperations + i)->left_op) = doOperation(*(pOperands + (pOperations + i)->left_op),
                                                                    *(pOperands + (pOperations + i) ->right_op),cCurOperation);
			(pOperations + i)->processed = 1;
			iProcDigits += 1;
			if (iProcDigits != *(pCountDigits)-1)
			{
				int z = i + 1;
				// Check left operand index, which is not changed, if it is last operation
				while ((pOperations + z)->processed && ((pOperations + z)->right_op + 1 != *pCountDigits)) z++;
				(pOperations + z)->left_op = (pOperations + i)->left_op;
			};
        }
    }
}


void splitOnSubExp(subexp *pSubExp, char *pRaw)
{
	int iCurLevel = 0;
	int iLastPr = 0;

	for (int i = 0, k = 0, iDelta = 0, iIndex = 0; *(pRaw + i); i++){
		iIndex = i - iDelta;
		switch (*(pRaw + i))
		{
			case '(':
				(pSubExp + k)->exp[iIndex] = '$';
				(pSubExp + k)->exp[iIndex + 1] = '\0';
				iCurLevel++;
				iDelta = i + 1;
				// Why it is help?
				k = iLastPr;
				iLastPr++;
				k++;
				(pSubExp + k)->level = iCurLevel;
				break;
			case ')':
				(pSubExp + k)->exp[iIndex] = '\0';
				k = (getParent(pSubExp, k) != -1) ? getParent(pSubExp, k) : 0;
				// We increase iDelta on 2 becouse we need to think NEXT iteration and not count '\0' symbol
				iDelta = i + 2 - getStrLen((pSubExp + k)->exp);
				iCurLevel--;
				break;
			default:
				(pSubExp + k)->exp[iIndex] = *(pRaw + i);
		}
	}
	for (int i = 0; i < 10; i++) cout << i <<") " << (pSubExp + i)->exp << "\n";
}


int getStrLen(char *p)
{
	int i = 0;
	while (*(p+i)) i++;
	return i + 1;
}


int getParent(subexp *pSubExp, int iChildInd)
{
	// Return index of parent element. If no parent element return "-1"
	for (int i = iChildInd - 1; i > 0; i--)
	{
		if ((pSubExp + i)->level+1 == (pSubExp + iChildInd)->level) return i;
	}

	return -1;
}


char *floatToChar(float fDigit)
{
	const int iDigitSize = 8;
	char cDigitString[iDigitSize];
	int i = 1, k = 0;

	while (fDigit / (10 * i) => 10) i++;

	while (i > 0)
	{
		cDigitString[k] = (char)(fDigit % (10*i) + 48);
		i--;
		fDigit /= 10;
	}
	cDigitString[k + 1] = '\0';

	return &cDigitString;
}