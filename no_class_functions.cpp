#include <math.h>
#include "calc.h"

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





int charToInt(char *p, int iStartNum, int iEndNum)
{
	int iCurNum, iFinalNum = 0;

	for (int i = iStartNum; i <= iEndNum; i++)
	{
		iCurNum = (int)p[i];
		iCurNum = iCurNum - 48;
		iFinalNum = iFinalNum * 10 + iCurNum;
	}

	return iFinalNum;
}


bool isChar(char cSymbol)
{
	int iCharCode = (int)cSymbol;
	if (iCharCode < 97 || iCharCode > 122) return false;

	return true;
}


bool isDigit(char cSymbol)
{
    int iCharCode = (int)cSymbol;
    if (iCharCode < 48 || iCharCode > 57) return false;

    return true;
}


int floatToInt(float fNumber)
{
	const int iIntLimits[] = { -2147483647, 2147483647};
	if (fNumber < iIntLimits[0] || fNumber > iIntLimits[1])
		cout << "Warning : you try to convert " << fNumber <<" to int, it is out of int limits \n";

	fNumber = trunc(fNumber);
	return (int)fNumber;
}

int isFunc(char *p, int &iStartParse)
{
	int i;
	for (i = iStartParse; p[i] && isChar(p[i]); i++);
	// Add 1 becouse we must think about zero symbol in the end.
	if (p[i] && p[i] == '$') return i+1;
	return 0;
}