

#ifndef C_CALCULATOR_CALC_H
#define C_CALCULATOR_CALC_H

const int SZ_RAW_STRING = 80;
const int SZ_GLOBAL_SIZE = 20;
int LAST_ERROR = 0;

class opr {
public:
    char symbol;
    char processed;
    int left_op;
    int right_op;

    opr() {processed = 0;};
    float doOperation(float fOperandL, float fOperandR);
};

class subexp {
public:
    // Use contstructor instead default value
    char exp[SZ_RAW_STRING]="\0";
    int level = 0;
    float result;
};

class raw_string{
public:
    char cRawString[SZ_RAW_STRING];

    void removeWhitesp();
    void splitOnSubExp(subexp *pSubExp);
    int checkParentheses();
};

class raw_materials{
public:
    int iCountDigits;
    int iCountSymbols;
    float fOperands[SZ_GLOBAL_SIZE];
    opr opOperations[SZ_GLOBAL_SIZE];

    raw_materials() {iCountDigits = 0; iCountSymbols = 0;};
    int getTokens(char *p, subexp *pSubExp);
    void doOperationGroup(char *pOperationGroup, const int *pOperationGroupLim, int &iProcDigits);
    float getResult();
};


int getStrLen(char *p);
int getParent(subexp *pSubExp, int iChildInd);
int floatToChar(float fDigit, char *pCharDigit, const int iDigitSize=8);
int concatStr(char *pOriginS, char *pAdditStr);
float charToFloat(char *p, int iStartNum, int iEndNum);
char isInArray(char cSymbol,char *pContainer, int iStart = -1, int iEnd = -1);

#endif //C_CALCULATOR_CALC_H
