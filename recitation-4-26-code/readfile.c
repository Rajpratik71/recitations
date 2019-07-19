#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)
{
	if(argc != 2)
		return 0;
	// Open up the file
	FILE *fp = fopen(argv[1], "r");
	// Make a 1 MB buffer for it
	char buf[1024 * 1024] = {0};
	// fread from the file	
	while(fread(buf, sizeof(char), 1024 * 1024 - 1, fp) == 1024 * 1024 - 1) {
		printf("%s", buf);
		memset(buf, 0, sizeof(buf));
	}
	if(feof(fp))
		printf("%s", buf);
	else
		fprintf(stderr, "Something went terribly wrong!");
	fclose(fp);
	return 0;
}
