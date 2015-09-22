

#ifndef C_CALCULATOR_CALC_H
#define C_CALCULATOR_CALC_H

const int SZ_RAW_STRING = 80;
const int SZ_GLOBAL_SIZE = 20;

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
int getTokens(char *p, float *pOperands, opr *pOperations, int *pCountDigits, int *pCountSymbols, subexp *pSubExp);
int getStrLen(char *p);
int getParent(subexp *pSubExp, int iChildInd);
int floatToChar(float fDigit, char *pCharDigit, const int iDigitSize=8);
int concatStr(char *pOriginS, char *pAdditStr);
float getResult(opr *pOperations, float *pOperands, int *pCountDigits);
float doOperation(float fOperandL, float fOperandR, char cOperation);
float charToFloat(char *p, int iStartNum, int iEndNum);

char isInArray(char cSymbol,char *pContainer, int iStart = -1, int iEnd = -1);

#endif //C_CALCULATOR_CALC_H
