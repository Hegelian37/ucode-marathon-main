#include "decrypt.h"

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		mx_printerr(MX_NO_ARGUMENTS);
		exit(-1);
	}
	char *a = argv[1];
	char *o = argv[2];
	char *b = argv[3];
	char *r = argv[4];
	mx_args_parse(&a, &o, &b, &r);
	mx_ops(a, o, b, r);
	return 0;
}

