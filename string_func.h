#ifndef C_CALCULATOR_STRING_FUNC_H
#define C_CALCULATOR_STRING_FUNC_H


class string_func {
public:
	int concatStr(char *pOriginS, char *pAdditStr);
	int getStrLen(char *p);
	bool isSameStr(char *p1, char *p2);
	void copyStr(char *pCopyTo, char *pCopyFrom);
};

#endif //C_CALCULATOR_STRING_FUNC_H