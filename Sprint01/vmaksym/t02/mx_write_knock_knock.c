#include <unistd.h>
#include <string.h>
void mx_write_knock_knock(void)
{
	char *str = "Follow the white rabbit.\nKnock. knock, Neo.\n";
	write(1, str, strlen(str));
}
int main(void)
{
	mx_write_knock_knock();
	return 0;
}
