#include "calc.h"
#include "variable_scope.h"

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
    char cBeforeOpen[] = "(,", cAfterOpen[]="(-", cAfterClose[] = ",)";
    for (int i=0; cRawString[i];i++ )
    {
        switch (cRawString[i])
        {
            case '(':
                iOpened += 1;
                if (i!=0 && !(isInArray(cRawString[i-1],cBeforeOpen)) && !(isChar(cRawString[i-1])) && !(isInArray(cRawString[i-1],cSymbols))) return 1;
				if (cRawString[i + 1] && !(isChar(cRawString[i+1])) && !(isInArray(cRawString[i + 1], cDigits)) && !(isInArray(cRawString[i + 1], cAfterOpen))) return 1;
                break;
            case ')':
                iClosed += 1;
                if (i!=0 && cRawString[i-1]!=')' && !(isInArray(cRawString[i-1],cDigits))){ cout <<"G3"; return 1;};
				if (cRawString[i + 1] && !(isInArray(cRawString[i + 1], cAfterClose)) && !(isInArray(cRawString[i + 1], cSymbols))) return 1;
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
	// Looks that some symbols, should not present in expression
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
		if (cRawString[i] == '(')
		{
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
		}
		else if (cRawString[i] == ')')
		{
            pSubExp[k].exp[iIndex] = '\0';
            k = (getParent(pSubExp, k) != -1) ? getParent(pSubExp, k) : 0;
            // We increase iDelta on 2 because we need to think NEXT iteration and not count '\0' symbol
            iDelta = i + 2 - strFunc.getStrLen(pSubExp[k].exp);
            iCurLevel--;
		}
		else if (cRawString[i] == '=')
		{
			// We suggest that in this case 'k' is always equal to 0 (it is very important in this 'if' statment )
			iCurLevel++;
			iDelta = i + 1;
			iLastPr++;
			k++;
			pSubExp[k].level = iCurLevel;
			floatToChar((float)k, cCharDigit);
			strFunc.concatStr(pSubExp[k - 1].exp, "=$");
			strFunc.concatStr(pSubExp[k - 1].exp, cCharDigit);
		}
		else
		{
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
int raw_materials::getTokens(char *p, subexp *pSubExp, variable_scope &varScope )
{
    int iStartNum = -1, iEndNum = 0, iOperandsPos = 0, iOperationsPos = 0;
    char cSymbols[] = "*/+-", cDigits[] = "0123456789", cSpecialSymbols[] = ".$,";
    parser parParser;
	string_func stringFunc;
	statment_type statmentType = Equation;

    for (int i = 0; p[i]; i++) {

        // Parse digits block
        if (isDigit(p[i])) {
            fOperands[iOperandsPos] = parParser.parseDigit(p,i,true);
            iCountDigits++;
			iOperandsPos++;
        }
        else if(i == 0 && p[i] == '-') {
            i++;
            fOperands[iOperandsPos] = parParser.parseDigit(p,i,true,true);
            iCountDigits++;
            iOperandsPos++;
        }
		// Parse sub expression
		else if (p[i] == '$') {
			fOperands[iOperandsPos] = pSubExp[parParser.parseSubExp(p, i, pSubExp, varScope, true)].result;
			iCountDigits++;
			iOperandsPos++;
		}
        // Parse symbols block
        else if (isInArray(p[i], cSymbols)) {

			// Validate that no dublicated symbols
			if (i > 0 && isInArray(p[i - 1], cSymbols)) return 1;
            opOperations[iOperationsPos].symbol = p[i];
            opOperations[iOperationsPos].left_op = iOperationsPos;
            opOperations[iOperationsPos].right_op = iOperationsPos + 1;
            iOperationsPos++;
            iCountSymbols++;
        }
        // Parse build in function
		else if (isChar(p[i]) && isFunc(p,i)) {
			fOperands[iOperandsPos] = pSubExp[parParser.parseBuildInFunc(p, i, pSubExp, varScope, true)].result;
			iCountDigits++;
			iOperandsPos++;
		}
		// Parse variable 
		else if (isChar(p[i]))
		{
			// !Here we become aware, whether it variable set or variable use
			int iCurVarIndex = parParser.parseVariable(p, i, varScope);
			if (iOperandsPos == 0 && p[i] && p[i] == '=' && stringFunc.isSameStr(p, pSubExp[0].exp))
			{
				i++;
				statmentType = Setter;
				varScope.vScope[iCurVarIndex].setValue(pSubExp[parParser.parseSubExp(p, i, pSubExp, varScope, true)].result);
				cout << varScope.vScope[iCurVarIndex].getValue() << "\n";
				cout << varScope.vScope[iCurVarIndex].getName() << "\n";
			}
				
		}
        else {
            return 1;
        }
    }
    if (iCountDigits - iCountSymbols != 1 && statmentType == Equation) return 1;
	// Check if there any errors during function work
	if (LAST_ERROR != 0) return 1;
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
float parser::parseDigit(char *p, int &iStartParse, bool bReturnParseIndex, bool bIsNegative)
{
    // Max len of float digit
    const int iFloatRange = 8;
    char cDigitStrPart[iFloatRange];
    int i = iStartParse, k = 0;
    int iSingModifier=bIsNegative ? -1 :1;

	for (; p[i] && (isDigit(p[i]) || p[i] == '.'); i++, k++)
        cDigitStrPart[k] = p[i];
	cDigitStrPart[k] = '\0';

    if (bReturnParseIndex) iStartParse = i-1;

    return charToFloat(cDigitStrPart,0,k-1) * iSingModifier;
}

int parser::parseSubExp(char *p, int &iStartParse, subexp *pSubExp, variable_scope &varScope, bool bReturnParseIndex)
{
	int iCurSub;
	raw_materials rawMat;

	iStartParse++;
	iCurSub = floatToInt(parseDigit(p, iStartParse, bReturnParseIndex));
	rawMat.getTokens(pSubExp[iCurSub].exp, pSubExp, varScope);
	pSubExp[iCurSub].result = rawMat.getResult();

	// we return index of SubExpression which was used 
	return iCurSub;
}

int parser::parseBuildInFunc(char *p, int &iStartParse, subexp *pSubExp, variable_scope &varScope ,bool bReturnParseIndex)
{
	// in Build in fucn class we will do next signature (*p, StartFunc, EndFunc, FuncArg)
	const int iMaxFuncSize = 20;
	const int iMaxArgsNum = 6;
	int k = 0, iArgsNum = 0, iSubIndex;
	float fArgs[iMaxArgsNum];
	char cFuncName[iMaxFuncSize];
	build_in_func buildInFunc;

	parseFuncName(p, cFuncName, iStartParse, true);
	iSubIndex = parseFuncArgs(p, iStartParse, pSubExp, fArgs, iArgsNum, varScope);
	pSubExp[iSubIndex].result = buildInFunc.doFunction(cFuncName, fArgs, iArgsNum);
	return iSubIndex;
}

int parser::parseFuncArgs(char *p, int &iStartParse, subexp *pSubExp, float *pArgs, int &iArgsNum, variable_scope &varScope)
{
	iStartParse++;
	int iSubIndex = floatToInt(parseDigit(p,iStartParse,true));
    // Implement unary minus here
	for (int i = 0; pSubExp[iSubIndex].exp[i]; i++)
	{
		// Parse digit
		if (isDigit(pSubExp[iSubIndex].exp[i]))
        {
			pArgs[iArgsNum] = parseDigit(pSubExp[iSubIndex].exp, i, true);
		}
		// Parse unary minus
        else if (pSubExp[iSubIndex].exp[i]=='-' && pSubExp[iSubIndex].exp[i+1] && isDigit(pSubExp[iSubIndex].exp[i+1]))
        {
            i++;
            pArgs[iArgsNum] = parseDigit(pSubExp[iSubIndex].exp, i, true, true);
        }
		// Parse next arg
		else if (pSubExp[iSubIndex].exp[i]==',')
		{
			iArgsNum += 1;
			continue;
		}
		// Parse build in func
        else if (isChar(pSubExp[iSubIndex].exp[i]) && isFunc(pSubExp[iSubIndex].exp, i))
        {
            pArgs[iArgsNum] = pSubExp[parseBuildInFunc(pSubExp[iSubIndex].exp, i, pSubExp, varScope, true)].result;
        }
		// Parse variable
		else if (isChar(pSubExp[iSubIndex].exp[i]))
		{
			pArgs[iArgsNum] = varScope.vScope[parseVariable(pSubExp[iSubIndex].exp, i, varScope)].getValue();
		}
		// Parse sub string
		else if (pSubExp[iSubIndex].exp[i]=='$')
		{
			pArgs[iArgsNum] = pSubExp[parseSubExp(pSubExp[iSubIndex].exp, i, pSubExp, varScope, true)].result;
		}
	}
	iArgsNum += 1;

	return iSubIndex;
}

int parser::parseVariable(char *p, int &iStartParse, variable_scope &varScope)
{
	const int iMaxVarNameSize = 36;
	int k = 0;
	char cVarName[iMaxVarNameSize];
	int iCurVarIndex;
	for (; p[iStartParse] && isChar(p[iStartParse]);k++, iStartParse++)
	{
		cVarName[k] = p[iStartParse];
	}
	cVarName[k] = '\0';
	iCurVarIndex = varScope.isExistedVar(cVarName);
	if ( iCurVarIndex != -1 )
	{
		return iCurVarIndex;
	}
	else
	{
		varScope.vScope[varScope.iVarNum].setName(cVarName);
		varScope.iVarNum++;
	}
	// Return "iVarNum-1" becouse index is always less than len on 1 
	return varScope.iVarNum-1;
}

void parser::parseFuncName(char *p, char *cFuncName, int &iStartParse, bool bReturnParseIndex)
{
	int i, k;
	for (i = iStartParse, k = 0; p[i] && isChar(p[i]); i++, k++)
	{
		cFuncName[k] = p[i];
	}
	cFuncName[k] = '\0';
	if (bReturnParseIndex) iStartParse = i;
}


// BUILD_IN_FUNC
float build_in_func::abs(float fNumber)
{
	if (fNumber >= 0) return fNumber;
	else return fNumber * -1;
}

float build_in_func::power(float fNumber, int iPowerNum)
{
	int iPowerBase = floatToInt(fNumber);

	if (iPowerNum == 0) fNumber=1;
	else if (iPowerNum > 0)
	{
		for (int i = 0; i < iPowerNum-1; i++) fNumber = iPowerBase * fNumber;
	}
	else if (iPowerNum < 0)
	{
		for (int i = 0; i < abs((float)iPowerNum)-1; i++) fNumber = iPowerBase * fNumber;
		fNumber = 1 / fNumber;
	}
	return fNumber;
}

int build_in_func::chooseFunc(char *p)
{
	string_func strFunc;
	for (int i = 0; cFuncNames[i][0]; i++) {
		if (strFunc.isSameStr(cFuncNames[i], p)) return i;
	}

	return -1;
}

float build_in_func::doFunction(char *pFuncName, float *pArgs, int iArgsNum)
{
	int iChosenIndex = chooseFunc(pFuncName);

	switch (iChosenIndex)
	{
	    case 0:
			if (iFuncArgsNum[iChosenIndex] != iArgsNum) {
				LAST_ERROR = 1;
				return 0;
			}
		    return abs(pArgs[0]);
	    case 1:
			if (iFuncArgsNum[iChosenIndex] != iArgsNum) {
				LAST_ERROR = 1;
				return 0;
			}
		    return power(pArgs[0], floatToInt(pArgs[1]));
		case 2:
			if (iFuncArgsNum[iChosenIndex] != iArgsNum){
				LAST_ERROR = 1;
				return 0;
			}
			show(pArgs[0]);
			return 0;
        default:
            // Case when we not found any build in function with such name
            LAST_ERROR = 1;
		    break;
	}

	return 0;
}

void build_in_func::show(float var)
{
	cout << var <<"\n";
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
	if (getStrLen(p1) == getStrLen(p2)) {

		for (int i = 0; p1[i] && p2[i]; i++)
		{
			if (p1[i] != p2[i]) return false;
		}
		
		return true;
	}
	else
		return false;
}


void string_func::copyStr(char *pCopyTo, char *pCopyFrom)
{
	int i;

	for (i = 0; pCopyFrom[i]; i++)
	{
		pCopyTo[i] = pCopyFrom[i];
	}

	pCopyTo[i] = '\0';
}

// VARIABLE
variable::variable(char *pName)
{
	string_func strFunc;
	name = new char[strFunc.getStrLen(pName)];
	strFunc.copyStr(name, pName);
	value = new float;
	initialized = new bool(false);
}
variable::variable(char *pName, float fValue)
{
	string_func strFunc;
	
	name = new char[strFunc.getStrLen(pName)];
	value = new float(fValue);
	strFunc.copyStr(name, pName);
	initialized = new bool(true);
}

variable::variable(const variable &obj)
{
	string_func strFunc;
	name = new char[strFunc.getStrLen(obj.name)];
	value = new float(*obj.value);
	strFunc.copyStr(name, obj.name);
	initialized = new bool(*obj.initialized);
}


// VARIABLE_SCOPE
int variable_scope::isExistedVar(char *pVarName)
{
	string_func strFunc;
	
	for (int i=0; i < iVarNum; i++ )
	{
		if (strFunc.isSameStr(pVarName, vScope[i].getName())) return i;
	};

	return -1;
}


// SUBEXP
statment_type subexp::getStatmentType()
{
	build_in_func buildInFunc;
	char *pFuncName;
	parser parParser;
	int iFuncNameLen;

	for (int i = 0; exp[i]; i++)
	{
		if (exp[i] == '=') return Setter;
		else if (isChar(exp[i]))
		{
			iFuncNameLen = isFunc(exp, i);
			if (iFuncNameLen)
			{
				pFuncName = new char[iFuncNameLen];
				parParser.parseFuncName(exp, pFuncName, i);
				if (buildInFunc.chooseFunc(pFuncName) == buildInFunc.chooseFunc("show"))
				{
					delete pFuncName;
					return VoidFunc;
				}
				delete pFuncName;
			}
		}
	}

	return Equation;
}