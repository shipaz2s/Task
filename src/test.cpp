extern "C" {
#include "calculator.h"
}

#include <iostream>
#include <stdlib.h>
#include <cassert>

int main(int argc, char** argv)
{
	sFixed v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15;
	v1 = str2Fixed("abcd.eab");
	v2 = str2Fixed("111.11");
	v3 = str2Fixed("-abcd.eab");
	v4 = str2Fixed("-111.11");
	v5 = str2Fixed("-Bbcd.eab");
	v6 = str2Fixed("2");
	v7 = str2Fixed("0.00111110");
	v8 = str2Fixed("0.00000020");
	v9 = str2Fixed("-148.742000");
	v10 = str2Fixed("-9.99001000");
	v11 = str2Fixed("51.3");
	v12 = str2Fixed("-F00000000E.00D");
	v13 = str2Fixed("10.2");

	v14 = str2Fixed("222222222222222222222222.22222222");
	v15 = str2Fixed("A");
	
	sFixed res;

	char* str = NULL;

	res = FixedDiv(&v15, &v6);
	str = Fixed2str(&res, str);
	std::cout << "FixedDiv(A, 2) " << str  << std::endl;
	assert( strcmp(str, "5") == 0 );

	res = FixedDiv(&v11, &v6);
	str = Fixed2str(&res, str);
	std::cout << "FixedDiv(51.3, 2) " << str  << std::endl;
	assert( strcmp(str, "28,98") == 0 );

	
	res = FixedDiv(&v6, &v11);
	str = Fixed2str(&res, str);
	std::cout << "FixedDiv(2, 51.3) " << str  << std::endl;
	assert( strcmp(str, "0,064E6F8B") == 0 );

	res = FixedDiv(&v12, &v13);
	str = Fixed2str(&res, str);
	std::cout << "FixedDiv(-F00000000E.00D, 10.2) " << str  << std::endl;

	
	res = FixedDiv(&v14, &v15);
	str = Fixed2str(&res, str);
	std::cout << "FixedDiv(222222222222222222222222,22222222, A) " << str  << std::endl;
	assert( strcmp(str, "369D0369D0369D0369D0369,D0369D03") == 0);
	
	res = FixedMul(&v14, &v15);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(222222222222222222222222,22222222, A) " << str  << std::endl;
	assert( strcmp(str, "error") == 0);

	res = FixedMul(&v12, &v13);
	free(str);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(-F00000000E.00D, 10.2) " << str  << std::endl;
	assert( strcmp(str, "-F1E000000E1,CD1A") == 0);

	
	res = FixedMul(&v2, &v6);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(111.11, 2) " << str  << std::endl;

	res = FixedMul(&v7, &v8);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(0.00111110, 0.00000020) " << str  << std::endl;

	
	res = FixedMul(&v7, &v6);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(0.00111110, 2) " << str  << std::endl;

	
	res = FixedMul(&v1, &v5);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(abcd.eab, -Bbcd.eab) " << str  << std::endl;
	
	res = FixedMul(&v9, &v6);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(-148.742000, 2) " << str  << std::endl;

	res = FixedMul(&v9, &v10);
	str = Fixed2str(&res, str);
	std::cout << "FixedMul(-148.742000, -9.99001000) " << str  << std::endl;

	res = FixedAdd(&v1, &v2);
	str = Fixed2str(&res, str);
	std::cout << "FixedAdd(abcd.eab, 111.11) " << str  << std::endl;

	res = FixedAdd(&v1, &v3);
	str = Fixed2str(&res, str);
	std::cout << "FixedAdd(abcd.eab, -abcd.eab) " << str  << std::endl;
	
	res = FixedAdd(&v2, &v3);
	str = Fixed2str(&res, str);
	std::cout << "FixedAdd(111.11, -abcd.eab) " << str  << std::endl;

	res = FixedAdd(&v3, &v5);
	str = Fixed2str(&res, str);
	std::cout << "FixedAdd(-abcd.eab, -Bbcd.eab) " << str  << std::endl;

	res = FixedAdd(&v1, &v4);
	str = Fixed2str(&res, str);
	std::cout << "FixedAdd(abcd.eab, -111.11) " << str  << std::endl;

	res = FixedSub(&v1, &v2);
	str = Fixed2str(&res, str);
	std::cout << "FixedSub(abcd.eab, 111.11) " << str  << std::endl;

	res = FixedSub(&v3, &v5);
	str = Fixed2str(&res, str);
	std::cout << "FixedSub(-abcd.eab, -Bbcd.eab) " << str  << std::endl;

	res = FixedSub(&v1, &v4);
	str = Fixed2str(&res, str);
	std::cout << "FixedSub(abcd.eab, -111.11) " << str  << std::endl;

	res = FixedSub(&v2, &v4);
	str = Fixed2str(&res, str);
	std::cout << "FixedSub(111.11, -111.11) " << str  << std::endl;

	return 0;
}