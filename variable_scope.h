#ifndef C_CALCULATOR_VARIABLE_SCOPE_H
#define C_CALCULATOR_VARIABLE_SCOPE_H

#include"variable.h"

const int VARIABLE_SCOPE_SIZE = 100;

class variable_scope {
public:
	variable vScope[VARIABLE_SCOPE_SIZE];
	int iVarNum = 0;
	int isExistedVar(char *pVarName);
	// !Do here increment method
};

#endif //C_CALCULATOR_VARIABLE_SCOPE_H