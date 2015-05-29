# include <cstdio>
# include <iostream>

void printBit(int a){
	int * pnum = &a;
	char* pi = (char*)pnum;
	int length = sizeof(int)* 8;
	for (int i = 1; i <= length; i += 8) {
		for (int j = 0; j < 8 ; j++){
			if ((*pi) & 1 << j) std:: cout << "1";
			else std::cout << "0";
		}
		std::cout << std::endl;
		++(pi);
	}
}