#include <stdio.h>

extern "C" int main(void) {
	printf("%s, %d == %x\n\r", "Hello World", 42, 42);
	return 0;
}
