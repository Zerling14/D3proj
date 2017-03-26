#include "D3Tool.h"

int main ()
{
	if (init_d3tool()) {
		printf("error init\n");
		return 1;
	}
	print_entity_list();
	return 0;
}