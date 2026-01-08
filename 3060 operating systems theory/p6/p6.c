#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Please input a logical address\n");
		return 1;
	}

	unsigned int logical_address = strtoul(argv[1], NULL, 16);
	unsigned int page_index = logical_address / PAGE_SIZE;
	unsigned int offset = logical_address % PAGE_SIZE;

	printf("Logical Addr:0x%08X - ", logical_address);
	printf("Page Index:0x%08X - ", page_index);
	printf("Offset:0x%08X\n", offset);

	return 0;
}
