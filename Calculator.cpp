/*Create simple calculator*/

#include <iostream>
#include <cstdio>
#include <math.h>
#include "calc.h"
using namespace std;

/*
 What class should i create?
 1) Raw string class
 2) Operation class
 3) Token keeper class

 Also:
 1) Rewrite *(p+i) to p[i] (Done)
 2) Do error validator ( do it after OOP refactoring)
 */

int main()
{
	raw_string rawString;
	raw_materials rawMat;
	float fResult;
	int iError = 0;
	subexp suSubExp[SZ_GLOBAL_SIZE];

	cout << "Print your equation: ";
	fgets(rawString.cRawString, SZ_RAW_STRING, stdin);
	rawString.cRawString[getStrLen(rawString.cRawString)-2] = '\0';

	rawString.removeWhitesp();
	rawString.splitOnSubExp(suSubExp);
	iError = rawMat.getTokens(suSubExp[0].exp, suSubExp);
	if (iError != 0) {
		cout << "Error in your equation";
		return 1;
	}

	fResult = rawMat.getResult();
	cout << "Result " << fResult << "\n";

	return 0;
}


void raw_string::removeWhitesp()
{
	char cNext;
	bool bWtLeft = true;

	while (bWtLeft){
		bWtLeft = false;
		for (int i = 0;  cRawString[i]; i++) {
			if ( cRawString[i] == ' ') {
				bWtLeft = true;
				for (int k = i;  cRawString[k]; k++) {
					cRawString[k] =  cRawString[k+1];
				}
			}
		}
	}
}


int raw_materials::getTokens(char *p, subexp *pSubExp )
{
	int iStartNum = -1, iEndNum = 0, iOperandsPos = 0, iOperationsPos = 0;
	char cSymbols[] = "*/+-", cDigits[] = "0123456789", cSpecialSymbols[] = ".$";
	bool bIsSubExp = false;

	for (int i = 0; p[i]; i++) {

		if (isInArray(p[i], cDigits)) {

			if (iStartNum == -1) iStartNum = i;
			else iEndNum = i;

			if (isInArray(p[i+1], cSymbols) || !(p[i+1])) {
				if (iEndNum < iStartNum) iEndNum = iStartNum;
				if (!bIsSubExp) fOperands[iOperandsPos] = charToFloat(p, iStartNum, iEndNum);
				else
				{
					int iCurSub;
					{
						// Initialize separate parameters in this block
						raw_materials rawMat;
						
						// Here i will catch last error
						iCurSub = charToFloat(p, iStartNum, iEndNum);
						rawMat.getTokens(pSubExp[iCurSub].exp, pSubExp);
						bIsSubExp = false;
						pSubExp[iCurSub].result = rawMat.getResult(rawMat.opOperations, rawMat.fOperands, &(rawMat.iCountDigits));
					}
					fOperands[iOperandsPos] = pSubExp[iCurSub].result;
				}
				iCountDigits++;
			} 

		}
		else if (isInArray(p[i], cSymbols)) {

			if (iStartNum != -1) { 
				iStartNum = -1;
				iOperandsPos++;
			}
			opOperations[iOperationsPos].symbol = p[i];
			opOperations[iOperationsPos].left_op = iOperationsPos;
			opOperations[iOperationsPos].right_op = iOperationsPos + 1;
			iOperationsPos++;
			iCountSymbols++;
		}
		else if (isInArray(p[i], cSpecialSymbols)) {
			switch (p[i])
			{
				case '.':
					continue;
					break;
				case '$':
					bIsSubExp = true;
					break;
			}
			
		}
		else {
			cout << "Error " << p[i] << "\n";
			return 1;
		}
	}

	if (iCountDigits - iCountSymbols != 1 || iCountDigits < 2) return 1;

	return 0;
}


char isInArray(char cSymbol, char *pContainer, int iStart, int iEnd )
{
	if (iStart == -1 && iEnd == -1)
	{
		for (int i = 0; pContainer[i]; i++) {
			if (cSymbol == pContainer[i]) return cSymbol;
		}
	}
	else if (iStart != -1 && iEnd == -1)
	{
		for (int i = iStart; pContainer[i]; i++) {
			if (cSymbol == pContainer[i]) return cSymbol;
		}
	}
	else if (iStart == -1 && iEnd != -1)
	{
		for (int i = 0; i < iEnd && pContainer[i]; i++) {
			if (cSymbol == pContainer[i]) return cSymbol;
		}
	}
	else
	{
		for (int i = iStart; i < iEnd && pContainer[i]; i++) {
			if (cSymbol == pContainer[i]) return cSymbol;
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
		if (p[i] == '.') {
			bAfterPoint = true;
			continue;
		}
		fCurNum = (float)p[i];
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


float raw_materials::getResult()
{
	const int iOperSz = 5, cPriority1Sz[] = {0, 2}, cPriority2Sz[] = {2, 4};
    char cOperationSymb[iOperSz] = "*/+-";
	int iProcDigits = 0;
    char cPartString[iOperSz];

    // Do multiple and division
    doOperationGroup(cOperationSymb, cPriority1Sz, iProcDigits);
    if (iProcDigits == *pCountDigits - 1) return *(pOperands);
    // Do plus and minus
    doOperationGroup(cOperationSymb, cPriority2Sz,iProcDigits);
	// The result is always on first member of pOperands, so we just need to return pointer value of pOperands pointer
	return pOperands[0];
}


float opr::doOperation(float fOperandL, float fOperandR)
{
	switch (symbol)
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


void raw_materials::doOperationGroup(char *pOperationGroup,const int *pOperationGroupLim, int &iProcDigits)
{
    char cCurOperation = '\0';
    for (int i = 0; i < iCountDigits-1; i++){
		cCurOperation = isInArray(opOperations[i].symbol, pOperationGroup, *(pOperationGroupLim), *(pOperationGroupLim + 1));
        if (cCurOperation) {
			fOperands[opOperations[i].left_op] = doOperation(fOperands[opOperations[i].left_op],
                                                                    fOperands[opOperations[i].right_op],cCurOperation);
			opOperations[i].processed = 1;
			iProcDigits += 1;
			if (iProcDigits != iCountDigits-1)
			{
				int z = i + 1;
				// Check left operand index, which is not changed, if it is last operation
				while (opOperations[z].processed && (opOperations[z].right_op + 1 != iCountDigits)) z++;
				opOperations[z].left_op = opOperations[i].left_op;
			};
        }
    }
}


void raw_string::splitOnSubExp(subexp *pSubExp)
{
	const int iDigitSize = 8;
	char cCharDigit[iDigitSize];
	int iCurLevel = 0, iLastPr = 0, iPrevK;

	for (int i = 0, k = 0, iDelta = 0, iIndex = 0; cRawString[i]; i++){
		iIndex = i - iDelta;
		switch (cRawString[i])
		{
			case '(':
				pSubExp[k].exp[iIndex] = '$';
				iCurLevel++;
				iDelta = i + 1;
				iPrevK = k;
				k = iLastPr;
				iLastPr++;
				k++;
				pSubExp[k].level = iCurLevel;
				floatToChar((float)k, cCharDigit);
				concatStr(pSubExp[iPrevK].exp, cCharDigit);
				break;
			case ')':
				pSubExp[k].exp[iIndex] = '\0';
				k = (getParent(pSubExp, k) != -1) ? getParent(pSubExp, k) : 0;
				// We increase iDelta on 2 because we need to think NEXT iteration and not count '\0' symbol
				iDelta = i + 2 - getStrLen(pSubExp[k].exp);
				iCurLevel--;
				break;
			default:
				pSubExp[k].exp[iIndex] = cRawString[i];
		}
	}
}


int getStrLen(char *p)
{
	int i = 0;
	while (p[i]) i++;
	return i + 1;
}


int getParent(subexp *pSubExp, int iChildInd)
{
	// Return index of parent element. If no parent element return "-1"
	for (int i = iChildInd - 1; i > 0; i--)
	{
		if (pSubExp[i].level+1 == pSubExp[iChildInd].level) return i;
	}

	return -1;
}


int floatToChar(float fDigit, char *pCharDigit, const int iDigitSize)
{
	//const int iDigitSize = 8;
	int i = 1, k = 0;

	while (fDigit / (10 * i) >= 10) i++;
	if (i > 8) return 1;

	while (i > 0)
	{
		pCharDigit[k] = (char)(fmod(fDigit,10*i) + 48);
		i--;
		fDigit /= 10;
	}
	pCharDigit[k + 1] = '\0';

	return 0;
}


int concatStr(char *pOriginS, char *pAdditStr)
{
	int iAdditLen = getStrLen(pAdditStr);

	for (int i = getStrLen(pOriginS)-1, k=0;k < iAdditLen;k++,i++) pOriginS[i]=pAdditStr[k];

	return 0;
}
