/*Create simple calculator*/

#include <iostream>
#include <cstdio>
using namespace std;

void remove_whitesp(char *p);
int get_tokens(char *p, int *operands_p, char *operations_p, int *count_digits_p, int *count_symbols_p);
int char_to_int(char *p, int start_num, int end_num);
int get_result(char *operations_p, int *operands_p, int *count_digits_p);
int do_operation(int operand_l_ind, int operand_r_ind, char operation);
bool in_array(char *p,char *container_p);

int main()
{
	char raw_string[80], operations[20];
	char *p=0, *operations_p= operations;
	int operands[20];
	int i, result, count_digits=0, count_symbols=0;
	int *operands_p = operands, *count_digits_p=&count_digits, *count_symbols_p=&count_symbols;
	int error = 0;

	cout << "Print your equation: ";
	gets_s(raw_string);

	p = raw_string;
	remove_whitesp(p);
	error = get_tokens(p, operands_p, operations_p, count_digits_p, count_symbols_p);
	if (error != 0) cout << "Error in your equation";
	else {
		result = get_result(operations_p, operands_p, count_digits_p);
		cout << "Result " << result << "\n";
	}

	return 0;
}


void remove_whitesp(char *p)
{	
	char next;
	int i, k;
	bool wt_left = true;
	 
	while (wt_left){
		wt_left = false;
		for (i = 0; *(p + i); i++) {
			if (*(p + i) == ' ') {
				wt_left = true;
				for (k = i; *(p + k); k++) {
					*(p + k) = *(p + k + 1);
				}
			}
		}
	}
}


int get_tokens(char *p, int *operands_p, char *operations_p, int *count_digits_p, int *count_symbols_p )
{
	int i, start_num = -1, end_num = 0, operands_pos = 0, operations_pos = 0;
	char symbols[] = "+-", digits[] = "0123456789";
	char *digits_p = digits, *symbols_p = symbols;

	for (i = 0; *(p + i); i++) {
		
		if (in_array((p + i), digits_p)) {

			if (start_num == -1) start_num = i;
			else end_num = i;

			if (in_array((p + i + 1), symbols_p) || !(*(p + i + 1))) {
				if (end_num < start_num) end_num = start_num;
				*(operands_p + operands_pos) = char_to_int(p, start_num, end_num);
				(*count_digits_p)++;
			} 

		}
		else if (in_array((p + i), symbols_p)) {

			if (start_num != -1) { 
				start_num = -1;
				operands_pos++;
			}
			*(operations_p + operations_pos) = *(p + i);
			operations_pos++;
			(*count_symbols_p)++;
		}
		else {
			return 1;
		}
	}

	if (*count_digits_p - *count_symbols_p != 1 || *count_digits_p < 2) return 1;

	return 0;
}


bool in_array(char *p, char *container_p) 
{
	int i;

	for (i = 0; *(container_p + i); i++) {
		if (*p == *(container_p + i)) return true;
	}

	return false;
}


int char_to_int(char *p, int start_num, int end_num) 
{
	/* 
	ten_num - кол-во разрядов 10
	cur_num - число над которым мы работаем в данный момент
	*/
	
	int i, int_num, cur_num;
	int ten_num = 1, final_num = 0;

	//cout << "DDD\n";
	for (i = end_num; i >= start_num; i-- ) {
	//	cout << "char cur_num" << *(p + i) << "\n";
		cur_num = (int) *(p + i);
		cur_num = cur_num - 48;
	//	cout << "int  cur_num" << cur_num << "\n";
		if (cur_num !=0 ) final_num = final_num + cur_num * ten_num;
		ten_num = 10 * ten_num;
	}

	return final_num;
}


int get_result(char *operations_p, int *operands_p, int *count_digits_p) 
{
	int i, k=1;
    int result;

	result = do_operation(*(operands_p), *(operands_p + 1), *operations_p);

	for (i = 2; i < *count_digits_p; i++) {
		result = do_operation(result, *(operands_p + i), *(operations_p + k));
	}

	return result;
}


int do_operation(int operand_l_ind, int operand_r_ind, char operation)
{
	switch (operation)
	{
	case '+':
		return operand_l_ind + operand_r_ind;
	case '-':
		return operand_l_ind - operand_r_ind;
	}

	return 0;
}