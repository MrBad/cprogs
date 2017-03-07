#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void bname(char *path, char *dirname, char *basename)
{
	char *p;
	unsigned long pos;size_t len;
	len = strlen(path);
	for(p = path + len; p > path; p--) 
		if(*p == '/')
			break;
	if(*p == '/') p++;
	pos = p - path;
	strncpy(dirname, path, pos);
	dirname[pos]=0;
	if(pos > 1 && dirname[pos-1]=='/')
		dirname[pos-1] = 0;
	strncpy(basename, p, len - pos);
	basename[len-pos] = 0;
}


int main()
{
//	char *basename=NULL,
//			*dirname=NULL;
	char basename[128], dirname[128];

	bname("/aaaddd/eeee", dirname, basename);
	printf("%s, %s\n", dirname, basename);
	return 0;
}
