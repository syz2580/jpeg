#include <cstdio>
#include <cstdlib>
#include "CHuffman.h"

int main() {
	unsigned short cmap[10] = { 1,1,1,1,1,1,1,1,1,1 };
	CHuffman huff(10,  cmap);
	
	for (int i = 0; i < 10; i++) {
		printf("%u ", cmap[i]);
	}
	printf("\n");
	
	for (int i = 0; i < 10; i++) {
		printf("%u ", huff.get_code(cmap[i]));
	}
	printf("\n");
	for (int i = 0; i < 10; i++) {
		printf("%u ", huff.get_depth(cmap[i]));
	}
	
	system("pause");
}