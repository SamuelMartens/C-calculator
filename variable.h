#ifndef C_CALCULATOR_VARIABLE_H
#define C_CALCULATOR_VARIABLE_H

#include"string_func.h"

class variable {
	char *name;
	float *value;
	bool *initialized;
public:
	variable() { value = new float; initialized = new bool(false); };
	variable(char *pName);
	variable(char *pName, float fValue);
	~variable() { if(*initialized) delete [] name; delete value; delete initialized; };
	variable(const variable &obj);
	void setValue(float fValue) { *value = fValue; };
	void setName(char *pName);
	void setNameValue(char *pName, float fValue) { setName(pName); setValue(fValue); };
	char *getName() { return name; };
	float getValue() { return *value; };
	bool isInit() { return *initialized;};
};


#endif
