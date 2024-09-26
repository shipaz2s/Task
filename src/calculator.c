#include "calculator.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

sFixed str2Fixed(const char* str)
{
	sFixed result = Fixed_def;

	int size = 0;
	while (str[size] != '\0') {
		++size;
	}


	int8_t has_dot = 0;
	int cur_precision = 0;
	for (int i = size - 1; (i > - 1); --i) {
		if ((str[i] == '.') || (str[i] == ',' )) {
			has_dot = 1;
			break;
		}
		++cur_precision;
	}

	if (has_dot == 0) {
		cur_precision = 0;
	}

	if (cur_precision > PRECISION) {
		cur_precision = 0;
	}

	int last_element_pos = NUM_SIZE - (PRECISION - cur_precision) - 1;

	char* num = (char*) malloc(1 * sizeof(char));

	int j = last_element_pos;
	for (int i = size - 1; i > -1 && j > -1; --i) {
		if (str[i] == '-') {
			result.flag = 1;
			break;
		}
		if (str[i] == '.' || str[i] == ',') {
			continue;
		}
		memcpy(num, str + i, 1 * sizeof(char));
		result.num[j--] = strtoul(num, NULL, NUM_BASE);
	}

	free(num);

	return result;
}

char* Fixed2str(sFixed* const val, char* str)
{
	// free(str);
	if ( (val->flag & 2) == 2) {
		str = (char*) malloc(sizeof(char) * 6);
		// str = "error\0";
		str[0] = 'e';
		str[1] = 'r';
		str[2] = 'r';
		str[3] = 'o';
		str[4] = 'r';
		str[5] = '\0';
		return str;
	}

	int size = NUM_SIZE + 2;
	if (val->flag == 0) {
		--size;
	}

	int first_num = 0;
	int last_num = NUM_SIZE;

	while ( val->num[first_num] == 0 && first_num != NUM_SIZE - PRECISION)
	{
		++first_num;
		--size;
	}

	if (first_num == NUM_SIZE - PRECISION) {
		++size;
		--first_num;
	}

	while (val->num[--last_num] == 0 && last_num != NUM_SIZE - PRECISION - 1)
	{
		--size;
	}

	uint8_t dot = 0;
	if (last_num >= NUM_SIZE - PRECISION && val->num[last_num] != 0) {
		++size;
		dot = 1;
	}

	if (size == 0) {
		str = (char*) malloc ( 2 * sizeof(char));
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	str = (char*) malloc ( size * sizeof(char));

	int str_i = 0;
	if (val->flag != 0) {
		str[str_i] = '-';
		++str_i;
	}

	char* tmp = (char*)malloc(sizeof(char));
	for (int i = first_num; i < NUM_SIZE - PRECISION; ++i) {
		//_itoa(val->num[i], tmp, 16);
		sprintf(tmp, "%X", val->num[i]);
		str[str_i] = toupper(tmp[0]);
		++str_i;
	}
	
	if (dot != 0) {
		str[str_i] = ',';
		++str_i;
	}

	for (int i = NUM_SIZE - PRECISION; i <= last_num; ++i) {
		//_itoa(val->num[i], tmp, 16);
		sprintf(tmp, "%X", val->num[i]);
		str[str_i] = toupper(tmp[0]);
		++str_i;
	}

	str[str_i] = '\0';
	
	return str;
}

uint8_t FixedLess(sFixed const* v1, sFixed const* v2)
{
	if ( v1->flag && !v2->flag ) {
		return 1;
	} else if ( !v1->flag && v2->flag ) {
		return 0;
	}

	if ( ! v1->flag ) {
		for (int i = 0; i < NUM_SIZE; ++i) {
			if ( v1->num[i] < v2->num[i] ) {
				return 1;
			} else if (v1->num[i] > v2->num[i]) {
				return 0;
			}
		}
		return 0;
	}

	if (v1->flag) {
		for (int i = 0; i < NUM_SIZE; ++i) {
			if ( v1->num[i] > v2->num[i] ) {
				return 1;
			} else if (v1->num[i] < v2->num[i]) {
				return 0;
			}
		}
		return 0;
	}
}

uint8_t FixedEqual(sFixed const* v1, sFixed const* v2)
{
	if (v1->flag != v2->flag) {
		return 0;
	}

	for (int i = 0; i < NUM_SIZE; ++i) {
		if (v1->num[i] != v2->num[i]) {
			return 0;
		}
	}

	return 1;
}

sFixed FixedAbs(sFixed const* v)
{
	sFixed result = *v;
	result.flag = 0;

	return result;
}

sFixed FixedShiftL(sFixed const* v, int n)
{
	sFixed result = Fixed_def;

	result.flag = v->flag;
	for (int i = 0; i < NUM_SIZE - n; ++i) {
		result.num[i] = v->num[i+n];
	}

	return result;
}

sFixed FixedShiftR(sFixed const* v, int n)
{
	sFixed result = Fixed_def;

	result.flag = v->flag;
	for (int i = NUM_SIZE - 1; i > n - 1; --i) {
		result.num[i] = v->num[i-n];
	}

	return result;
}

sFixed FixedAdd(sFixed const* v1, sFixed const* v2)
{
	sFixed result = Fixed_def;
	
	if (v1->flag == v2->flag) {
		uint8_t carry = 0;
		result.flag = v1->flag;
		for (int i = NUM_SIZE - 1; i > -1; --i) {
			result.num[i] = v1->num[i] + v2->num[i] + carry;
			if (result.num[i] >= NUM_BASE) {
				result.num[i] -= NUM_BASE;
				carry = 1;
			} else {
				carry = 0;
			}
		}
		if (carry != 0) {
			result.flag ^= 2;
		}
		return result;
	}
	sFixed absV1 = FixedAbs(v1);
	sFixed absV2 = FixedAbs(v2);

	if ( FixedLess(&absV1, &absV2) ) {
		result.flag = v2->flag;
		uint16_t carry = 0;
		for (int i = NUM_SIZE - 1; i > -1; --i) {
			result.num[i] = v2->num[i] - v1->num[i] + carry;
			if (result.num[i] < 0) {
				result.num[i] += NUM_BASE;
				carry = -1;
			} else {
				carry = 0;
			}
		}
	} else if ( !FixedEqual(&absV1, &absV2) ) {
		result.flag = v1->flag;
		uint16_t carry = 0;
		for (int i = NUM_SIZE - 1; i > -1; --i) {
			result.num[i] = v1->num[i] - v2->num[i] + carry;
			if (result.num[i] < 0) {
				result.num[i] += NUM_BASE;
				carry = -1;
			} else {
				carry = 0;
			}
		}
	}

	return result;
}

sFixed FixedSub(sFixed const* v1, sFixed const* v2)
{
	sFixed v2_inv = *v2;
	v2_inv.flag ^= 1;

	sFixed result = FixedAdd(v1, &v2_inv);
	

	return result;
}

sFixed FixedMul(sFixed const* v1, sFixed const* v2)
{
	sFixed result = Fixed_def;

	result.flag = v1->flag ^ v2->flag;

	int8_t res[NUM_SIZE * 2] = {0};

	int ir = 0;
	for (int i1 = NUM_SIZE - 1; i1 > -1; --i1) {
		int8_t carry = 0;
		int16_t tmp;
		for (int i2 = NUM_SIZE - 1; i2  > -1; --i2) {
			tmp = carry + v1->num[i1] * v2->num[i2];
			carry = 0;
			carry += tmp / NUM_BASE;
			tmp %= NUM_BASE;

			
			res[NUM_SIZE + i2 - ir] += tmp;
			carry += res[NUM_SIZE + i2 - ir] / NUM_BASE;
			res[NUM_SIZE + i2 - ir] %= NUM_BASE;

		}
		++ir;
	}

	for (int i = 0; i < NUM_SIZE - PRECISION; ++i) {
		if (res[i] != 0) {
			result.flag ^= 2;
			break;
		}
	}

	for (int i = 0; i < NUM_SIZE; ++i) {
		result.num[i] = res[i + NUM_SIZE - PRECISION];
	}
	if (res[NUM_SIZE * 2 - PRECISION] > 7) {
		sFixed add_val = str2Fixed("0.00000001");
		result = FixedAdd(&result, &add_val);
	}

	return result;
}

sFixed FixedDiv(sFixed const* v1, sFixed const* v2)
{
	sFixed result = Fixed_def;
	sFixed tmp = Fixed_def;
	sFixed absTmp = Fixed_def;
	sFixed absV2 = Fixed_def;
	sFixed zero = Fixed_def;
	result.flag = v1->flag ^ v2->flag;

	int8_t res[NUM_SIZE + PRECISION] = {0};

	if (FixedEqual(v2, &zero)) {
		result.flag |= 2;
		return result;
	}

	
	memcpy(tmp.num, v1->num, sizeof(uint8_t) * NUM_SIZE);
	tmp.flag = v1->flag;

	absV2 = FixedAbs(v2);
	absTmp = FixedAbs(&tmp);

	int res_i = NUM_SIZE - PRECISION - 1;
	int offset = 0;
	if (FixedEqual(&absTmp, &absV2)) {
		result.num[NUM_SIZE - PRECISION - 1] = 1;
		return result;
	} else if (FixedLess(&absTmp, &absV2)) {
		do {
			tmp = FixedShiftL(&tmp, 1);
			absTmp = FixedAbs(&tmp);
			++offset;
		} while ( FixedLess(&absTmp, &absV2) );
	} else {
		do {
			tmp = FixedShiftR(&tmp, 1);
			absTmp = FixedAbs(&tmp);
			--offset;
		} while ( FixedLess(&absV2, &absTmp) );
		++offset;
		tmp = FixedShiftL(&tmp, 1);
		tmp.num[NUM_SIZE - 1] = v1->num[NUM_SIZE + offset - 1];
	}

	res_i += offset;

	int v2_last_num = NUM_SIZE - 1;
	for (; v2->num[v2_last_num] == 0; --v2_last_num) {}
	++v2_last_num;

	int r = 0;
	sFixed to_div = tmp;
	to_div.flag = 0;
	do {		
		for (int i = 1; i < NUM_BASE + 1; ++i) {
			sFixed mul = Fixed_def;
			mul.num[NUM_SIZE - PRECISION - 1] = i;
			sFixed mul_res = FixedMul(&absV2, &mul);
			uint8_t to_div_is_smaller = FixedLess(&to_div, &mul_res);

			if (i == NUM_BASE) {
				to_div_is_smaller = 1;
			}

			if ( to_div_is_smaller ) {
				--i;

				mul.num[NUM_SIZE - PRECISION - 1] = i;

				mul_res = FixedMul(&absV2, &mul);
				sFixed modulo = FixedSub(&to_div, &mul_res);
				if (NUM_SIZE - PRECISION + offset + r < NUM_SIZE) {
					modulo.num[v2_last_num] = v1->num[NUM_SIZE - PRECISION + offset + r];
				}
				to_div = FixedShiftL(&modulo, 1);

				res[res_i++] = i;
				++r;

				break;
			}
		}
	} while ( ! FixedEqual(&to_div, &zero) &&  res_i < NUM_SIZE + 1);
	
	memcpy(result.num, res, sizeof(uint8_t)*NUM_SIZE);
	if (res[NUM_SIZE] > 7) {
		sFixed add_val = str2Fixed("0.00000001");
		result = FixedAdd(&result, &add_val);
	}
	

	return result;
}