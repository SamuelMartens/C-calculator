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
                if (i!=0 && cRawString[i-1]!='(' && !(isChar(cRawString[i-1])) && !(isInArray(cRawString[i-1],cSymbols))) return 1;
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
	string_func strFunc;

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
                strFunc.concatStr(pSubExp[iPrevK].exp, cCharDigit);
                break;
            case ')':
                pSubExp[k].exp[iIndex] = '\0';
                k = (getParent(pSubExp, k) != -1) ? getParent(pSubExp, k) : 0;
                // We increase iDelta on 2 because we need to think NEXT iteration and not count '\0' symbol
                iDelta = i + 2 - strFunc.getStrLen(pSubExp[k].exp);
                iCurLevel--;
                break;
            default:
                pSubExp[k].exp[iIndex] = cRawString[i];
        }
    }

	//cout << iLastPr << " last \n";
	// for Debug
	for (int i = 0; i <= iLastPr; i++)
	{
		cout << i << ") " << pSubExp[i].exp << " " << pSubExp[i].level  <<" \n";
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
    char cSymbols[] = "*/+-", cDigits[] = "0123456789", cSpecialSymbols[] = ".$,";
    parser parParser;

    for (int i = 0; p[i]; i++) {

		// Parse digits block
        if (isDigit(p[i])) {
            fOperands[iOperandsPos] = parParser.parseDigit(p,i,true);
            iCountDigits++;
			iOperandsPos++;
        }
		// Parse sub expression
		else if (p[i] == '$') {
			fOperands[iOperandsPos] = pSubExp[parParser.parseSubExp(p, i, pSubExp, true)].result;
			iCountDigits++;
			iOperandsPos++;
		}
        // Parse symbols block
        else if (isInArray(p[i], cSymbols)) {

			// Validate that no dublicate symbols
			if (i > 0 && isInArray(p[i - 1], cSymbols)) return 1; 

            opOperations[iOperationsPos].symbol = p[i];
            opOperations[iOperationsPos].left_op = iOperationsPos;
            opOperations[iOperationsPos].right_op = iOperationsPos + 1;
            iOperationsPos++;
            iCountSymbols++;
        }
		else if (isChar(p[i])) {
			cout << "G2";
			fOperands[iOperandsPos] = pSubExp[parParser.parseBuildInFunc(p, i, pSubExp, true)].result;
			iCountDigits++;
			iOperandsPos++;
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

// PARSER
float parser::parseDigit(char *p, int &iStartParse, bool bReturnParseIndex)
{
    // Max len of float digit
    const int iFloatRange = 8;
    char cDigitStrPart[iFloatRange];
    int i = iStartParse, k = 0;


	for (; p[i] && (isDigit(p[i]) || p[i] == '.'); i++, k++)
        cDigitStrPart[k] = p[i];
	cDigitStrPart[k] = '\0';

    if (bReturnParseIndex) iStartParse = i-1;

    return charToFloat(cDigitStrPart,0,k-1);
}

int parser::parseSubExp(char *p, int &iStartParse, subexp *pSubExp, bool bReturnParseIndex)
{
	int iCurSub;
	raw_materials rawMat;

	iStartParse++;
	iCurSub = floatToInt(parseDigit(p, iStartParse, bReturnParseIndex));
	rawMat.getTokens(pSubExp[iCurSub].exp, pSubExp);
	pSubExp[iCurSub].result = rawMat.getResult();

	// we return index of SubExpression which was used 
	return iCurSub;
}

int parser::parseBuildInFunc(char *p, int &iStartParse, subexp *pSubExp, bool bReturnParseIndex)
{
	// in Build in fucn class we will do next signature (*p, StartFunc, EndFunc, FuncArg)
	// Don't forget about bReturnParseIndex

	// 1) Найти функцию
	// 2) Сделать функцию которая будет парсить аргументы функции
	//	  она же будет возвращать значения и кол-ва аргументов
	// 3) Передать результаты парсинга функции аргументов обратно парсеру функций
	// 4) Парсер выбирает какая функция будет выполнена
	// 5) Выполняет
	// 6) Возвращает цифру обратно в вызвавшую функцию
	const int iMaxFuncSize = 20;
	const int iMaxArgsNum = 6;
	int k = 0, iArgsNum = 0, iSubIndex;
	float fArgs[iMaxArgsNum];
	char cFuncName[iMaxFuncSize];
	build_in_func buildInFunc;
	cout << "G1";
	for (;p[iStartParse] && isChar(p[iStartParse]); iStartParse, k++) 
	{
		cFuncName[k] = p[iStartParse];
	};
	cFuncName[k + 1] = '\0';
	cout << "Parse func name " << cFuncName << "\n";
	iSubIndex = parseFuncArgs(p, iStartParse, pSubExp, fArgs, iArgsNum);
	pSubExp[iSubIndex].result = buildInFunc.doFunction(cFuncName, fArgs, iArgsNum);
	// Нужна функция для валидации подвыражения после функции (типа оно с запятыми и все дела)
	return iSubIndex;
}

int parser::parseFuncArgs(char *p, int &iStartParse, subexp *pSubExp, float *pArgs, int &iArgsNum)
{
	cout << "Parsed symbol " << p[iStartParse] << "\n";
	iStartParse++;
	int iSubIndex = floatToInt(parseDigit(p,iStartParse,true));
	cout << "Parsed index " << iSubIndex << "\n";

	for (int i = 0; pSubExp[iSubIndex].exp[i]; i++)
	{
		if (isDigit(pSubExp[iSubIndex].exp[i])) {
			pArgs[iArgsNum] = parseDigit(pSubExp[iSubIndex].exp, i, true);
		}
		else if (pSubExp[iSubIndex].exp[i]==',')
		{
			iArgsNum += 1;
			continue;
		}
		else if (pSubExp[iSubIndex].exp[i]=='$')
		{
			pArgs[iArgsNum] = pSubExp[parseSubExp(pSubExp[iSubIndex].exp, i, pSubExp, true)].result;
		}
	}
	cout << "Parsed args " << pArgs << "\n";
	iArgsNum += 1;

	return iSubIndex;
}


// BUILD_IN_FUNC
float build_in_func::abs(float fNumber)
{
	if (fNumber >= 0) return fNumber;
	else return fNumber * -1;
}

float build_in_func::power(float fNumber, int iPowerNum)
{
	if (iPowerNum == 0) fNumber=1;
	else if (iPowerNum > 0)
	{
		for (int i = 0; i < iPowerNum; i++) fNumber *= fNumber;
	}
	else if (iPowerNum < 0)
	{
		for (int i = 0; i < abs((float)iPowerNum); i++) fNumber *= fNumber;
		fNumber = 1 / fNumber;
	}

	return fNumber;
}

int build_in_func::chooseFunc(char *p)
{
	string_func strFunc;
	for (int i = 0; cFuncNames[i]; i++) {
		if (strFunc.isSameStr(cFuncNames[i], p)) return i;
	}

	return -1;
}

float build_in_func::doFunction(char *pFuncName, float *pArgs, int iArgsNum)
{
	// Think how to use iArgsNum for validation
	switch (chooseFunc(pFuncName))
	{
	case 0:
		return abs(pArgs[0]);
	case 1:
		return power(pArgs[0], floatToInt(pArgs[1]));
	default:
		break;
	}

	return 0;
}

// STRING_FUNC
int string_func::concatStr(char *pOriginS, char *pAdditStr)
{
	int iAdditLen = getStrLen(pAdditStr);

	for (int i = getStrLen(pOriginS) - 1, k = 0; k < iAdditLen; k++, i++) pOriginS[i] = pAdditStr[k];

	return 0;
}

int string_func::getStrLen(char *p)
{
	int i = 0;
	while (p[i]) i++;
	return i + 1;
}

bool string_func::isSameStr(char *p1, char *p2)
{
	for (int i = 0; p1[i] && p2[i]; i++)
	{
		if (p1[i] != p2[i]) return false;
	}

	return true;
}
