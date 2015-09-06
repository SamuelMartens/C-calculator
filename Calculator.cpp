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
void doOperationGroup(opr *pOperations, float *pOperands, char *pOperationGroup, const int *pOperationGroupLim, int *pCountDigits, int &iProcDigits);
int getTokens(char *p, float *pOperands, opr *pOperations, int *pCountDigits, int *pCountSymbols);
float getResult(opr *pOperations, float *pOperands, int *pCountDigits);
float doOperation(float fOperandL, float fOperandR, char cOperation);
float charToFloat(char *p, int iStartNum, int iEndNum);
char isInArray(char cSymbol,char *pContainer, int iStart = 0, int iEnd = 0);




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
			cout << "OP " << (pOperations + iOperationsPos)->symbol << " " << (pOperations + iOperationsPos)->left_op << " " << (pOperations + iOperationsPos)->right_op << "\n";
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
	if (iStart == 0 && iEnd == 0)
	{
		for (int i = 0; *(pContainer + i); i++) {
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
	cout << "is done? ";
    // Do plus and minus
    doOperationGroup(pOperations, pOperands, cOperationSymb, cPriority2Sz, pCountDigits, iProcDigits);
	return *(pOperands);
}


float doOperation(float fOperandL, float fOperandR, char cOperation)
{

	cout << "left " << fOperandL << " right " << fOperandR << "\n";
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
	// pCountDigits - переписать на ссылку
	//Wrong 2+5*2+4+3/3+100 expected 117
    for (int i = 0; i < *pCountDigits-1; i++){
		cCurOperation = isInArray((pOperations + i)->symbol, pOperationGroup, *(pOperationGroupLim), *(pOperationGroupLim + 1));
		cout << "OP " << (pOperations + i)->symbol << " " << (pOperations + i)->left_op << " " << (pOperations + i)->right_op << "\n";
        if (cCurOperation) {
			*(pOperands + (pOperations + i)->left_op) = doOperation(*(pOperands + (pOperations + i)->left_op),
                                                                    *(pOperands + (pOperations + i) ->right_op),cCurOperation);

			cout << "Res " << *(pOperands + (pOperations + i)->left_op) << "\n";
			iProcDigits += 1;
		//	if (iProcDigits != *(pCountDigits) - 1 ) (pOperations + i + 1)->left_op = (pOperations + i)->left_op;
			if (iProcDigits != *(pCountDigits)-1)
			{ 
				int z = i + 1;
				while (!(isInArray((pOperations + z)->symbol, pOperationGroup, *(pOperationGroupLim), *(pOperationGroupLim + 1)))) z++;

				(pOperations + z)->left_op = (pOperations + i)->left_op;
			};
        }
    }
}