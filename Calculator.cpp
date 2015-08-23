/*Create simple calculator*/

#include <iostream>
#include <cstdio>
using namespace std;

// ivan: it would be better to move all function declarations to separated header file
void remove_whitesp(char *p);
int get_tokens(char *p, int *operands_p, char *operations_p, int *count_digits_p, int *count_symbols_p);
int char_to_int(char *p, int start_num, int end_num);
int get_result(char *operations_p, int *operands_p, int *count_digits_p);
int do_operation(int operand_l, int operand_r, char operation);
bool in_array(char symbol,char *container_p);

int main()
{
	const int sz_raw_string = 80 , sz_global_size = 20;
	char raw_string[sz_raw_string], operations[sz_global_size];
	char *operations_p= operations;
	int operands[sz_global_size];
	int result, count_digits=0, count_symbols=0;
	int error = 0;

	cout << "Print your equation: ";
	gets_s(raw_string, sz_raw_string);

	remove_whitesp(raw_string);
	error = get_tokens(raw_string, operands, operations, &count_digits, &count_symbols);
	if (error != 0) {
		cout << "Error in your equation";
		return 1;
	}
	result = get_result(operations, operands, &count_digits);
	cout << "Result " << result << "\n";

	return 0;
}


void remove_whitesp(char *p)
{	
	char next;
	bool wt_left = true;
	 
	while (wt_left){
		wt_left = false;
		for (int i = 0; *(p + i); i++) {
			if (*(p + i) == ' ') {
				wt_left = true;
				for (int k = i; *(p + k); k++) {
					*(p + k) = *(p + k + 1);
				}
			}
		}
	}
}


int get_tokens(char *p, int *operands_p, char *operations_p, int *count_digits_p, int *count_symbols_p )
{
	int start_num = -1, end_num = 0, operands_pos = 0, operations_pos = 0;
	char symbols[] = "+-", digits[] = "0123456789";

	for (int i = 0; *(p + i); i++) {
		
		if (in_array(*(p + i), digits)) {

			if (start_num == -1) start_num = i;
			else end_num = i;

			if (in_array(*(p + i + 1), symbols) || !(*(p + i + 1))) {
				if (end_num < start_num) end_num = start_num;
				*(operands_p + operands_pos) = char_to_int(p, start_num, end_num);
				(*count_digits_p)++;
			} 

		}
		else if (in_array(*(p + i), symbols)) {

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


bool in_array(char symbol, char *container_p) 
{
	for (int i = 0; *(container_p + i); i++) {
		if (symbol == *(container_p + i)) return true;
	}

	return false;
}


int char_to_int(char *p, int start_num, int end_num) 
{
	/* 
	ten_num - кол-во разрядов 10
	cur_num - число над которым мы работаем в данный момент
	*/
	
	int int_num, cur_num;
	int ten_num = 1, final_num = 0;

	//cout << "DDD\n";
	for (int i = start_num; i <= end_num; i++ ) {
	//	cout << "char cur_num" << *(p + i) << "\n";
		cur_num = (int) *(p + i);
		cur_num = cur_num - 48;
	//	cout << "int  cur_num" << cur_num << "\n";
		//if (cur_num !=0 ) final_num = final_num + cur_num * ten_num;
		//ten_num = 10 * ten_num;
		final_num = final_num * 10 + cur_num;
	}

	return final_num;
}


int get_result(char *operations_p, int *operands_p, int *count_digits_p) 
{
	int k=1;
    int result;

	result = do_operation(*(operands_p), *(operands_p + 1), *operations_p);

	for (int i = 2; i < *count_digits_p; i++) {
		result = do_operation(result, *(operands_p + i), *(operations_p + k));
		k++;
	}

	return result;
}


int do_operation(int operand_l, int operand_r, char operation)
{
	switch (operation)
	{
	case '+':
		return operand_l + operand_r;
	case '-':
		return operand_l - operand_r;
	}

	return 0;
}