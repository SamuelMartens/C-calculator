#include "calc.h"

// RAW_STRING
void raw_string::removeWhitesp()
{
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


int raw_string::validateAll()
{
	// Run all validators of this class
	if (!(validateParentheses()) && !(validateSpecialSymbols())) return 0;
	return 1;
}


int raw_string::validateParentheses()
{
    // Check parentless and also check operation symbol before open parentheses
    // and digits before closed parentheses

    int iOpened = 0, iClosed = 0;
    char cSymbols[] = "*/+-", cDigits[] = "0123456789";
    for (int i=0; cRawString[i];i++ )
    {
        switch (cRawString[i])
        {
            case '(':
                iOpened += 1;
                if (i!=0 && cRawString[i-1]!='(' && !(isInArray(cRawString[i-1],cSymbols))) return 1;
				if (cRawString[i + 1] && !(isInArray(cRawString[i + 1], cDigits))) return 1;
                break;
            case ')':
                iClosed += 1;
                if (i!=0 && cRawString[i-1]!=')' && !(isInArray(cRawString[i-1],cDigits))) return 1;
				if (cRawString[i + 1] && cRawString[i + 1] != ')' && !(isInArray(cRawString[i + 1], cSymbols))) return 1;
                if (iClosed>iOpened) return 1;
                break;
            default:
                continue;

        }
    }

    if (iClosed != iOpened) return 1;
    return 0;
}

int raw_string::validateSpecialSymbols()
{
	// Looks that some symbols, should not present is expression
	char cSpecialSymbols[] = "$";

	for (int i = 0; cRawString[i]; i++)
	{
		if (isInArray(cRawString[i], cSpecialSymbols)) return 1;
	};

	return 0;
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

// OPR
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

// RAW_MATERIALS

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
                        iCurSub = charToInt(p, iStartNum, iEndNum);
                        rawMat.getTokens(pSubExp[iCurSub].exp, pSubExp);
                        bIsSubExp = false;
                        pSubExp[iCurSub].result = rawMat.getResult();
                    }
                    fOperands[iOperandsPos] = pSubExp[iCurSub].result;
                }
                iCountDigits++;
            }

        }
        else if (isInArray(p[i], cSymbols)) {

			// Validate that no dublicate symbols
			if (i > 0 && isInArray(p[i - 1], cSymbols)) return 1; 

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
            return 1;
        }
    }
    if (iCountDigits - iCountSymbols != 1 || iCountDigits < 2) return 1;
    return 0;
}

float raw_materials::getResult()
{
    const int iOperSz = 5, cPriority1Sz[] = {0, 2}, cPriority2Sz[] = {2, 4};
    char cOperationSymb[iOperSz] = "*/+-";
    int iProcDigits = 0;
 //   char cPartString[iOperSz];

    // Do multiple and division
    doOperationGroup(cOperationSymb, cPriority1Sz, iProcDigits);
    if (iProcDigits == iCountDigits - 1) return fOperands[0];
    // Do plus and minus
    doOperationGroup(cOperationSymb, cPriority2Sz,iProcDigits);
    // The result is always on first member of pOperands, so we just need to return pointer value of pOperands pointer
    return fOperands[0];
}

void raw_materials::doOperationGroup(char *pOperationGroup,const int *pOperationGroupLim, int &iProcDigits)
{
    char cCurOperation = '\0';
    for (int i = 0; i < iCountDigits-1; i++){
        cCurOperation = isInArray(opOperations[i].symbol, pOperationGroup, *(pOperationGroupLim), *(pOperationGroupLim + 1));
        if (cCurOperation) {
            fOperands[opOperations[i].left_op] = opOperations[i].doOperation(fOperands[opOperations[i].left_op],
                                                                             fOperands[opOperations[i].right_op]);
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