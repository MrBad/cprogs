#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
//	Few functions to clean a path
//		resolving multiple / or ./ or ../
//

// cleans and solves multiple /, ./, ../, /.$ /..$
// expects first character to be '/'
// if last character is '/' and last is not first, trim it
// returns reference to path on success or NULL on failure 
char *clean_path(char *path) 
{
	printf("path: %s\n",path);
	int i,j,len;
	len=strlen(path);
	for(i = 0, j = 0; i < len && j < len; i++) {
		if(path[i]=='/') {
			while(i < len && path[i]=='/')
				i++;
			if(i < len && path[i]=='.') {
				if(i+1==len || path[i+1]=='/') {
					continue;
				}
				if(path[i+1]=='.') {
					if(i+2==len || path[i+2]=='/') {
						i+=2;
						if(j > 1 && path[j]=='/') j--;
						while(j > 1 && path[j-1]!='/') j--;
						if(i+1==len && j > 1 && path[j-1]=='/') j--;
						continue;
					}
				}
			}
			assert(j <= i);
			if(i<len || j==0) path[j++]='/';
		}
		path[j++]=path[i];
	}
	if(j==0)j++;
	path[j]=0;
	assert(j<=len);
	assert(j>0);
	return path;
}

//
//	giving a prefix and path, if path is relative constructs a string 
//		and cleans it
//	I will use this in cOSiris, prefix being process current working directory
//	and path - the path. If path is relative, we will construct it like
//	prefix/path
//	Expects prefix to start with /
//	return a new string with path cleaned on success, NULL on failure. 
//	It's the caller responsability to free it
//
char *canonize_path(char *prefix, char *path) 
{
	char *p;
	int len;
	if(path[0] == '/') {
		p = strdup(path);
	} else {
		if(prefix[0]!='/') {
			printf("Error - prefix should start with /\n");
			return NULL;
		}
		len = strlen(prefix)+strlen(path)+1;
		p = calloc(1,len);
		strcpy(p, prefix);
		strcat(p, "/");
		strcat(p, path);
	}
	if(!(clean_path(p))){
		printf("Cannot clean path: %s\n", p);
		return NULL;
	}
	printf("[%s]\n",p);
	return p;
}

// path 'smart' copy //
int eq(char *str1, char *str2) {
	if(strcmp(str1,str2)!=0) {
		printf("Error %s != %s\n", str1, str2);
		return 0;
	}
	printf(".");
	return 1;
}

int main()
{
	char *c;
	assert((eq(canonize_path("/", "dev"), "/dev")));
	c = malloc(10);free(c);
	assert((eq(canonize_path("//", "//dev"), "/dev")));
	c = malloc(2);free(c);
	assert((eq(canonize_path("/./", "dev/../"), "/")));
	assert((eq(canonize_path("//././", "/dev/../"), "/")));
	c = malloc(3);free(c);
	assert((eq(canonize_path("/dev/", "asddsa/../"), "/dev")));
	assert((eq(canonize_path("/asd/", "dev/"), "/asd/dev")));
	assert((eq(canonize_path("//", "dev/../"), "/")));
	assert((eq(canonize_path("/./././aaaaa/..", "bbbb"), "/bbbb")));
	assert((eq(canonize_path("/","aaaa/."), "/aaaa")));
	assert((eq(canonize_path("////aa", "bbb////"), "/aa/bbb")));
	assert((eq(canonize_path("/test/baba", "../aaa../"), "/test/aaa..")));
	assert((eq(canonize_path("/dev", ".."), "/")));
	assert((eq(canonize_path("//.", ".."), "/")));
	assert((eq(canonize_path("/","s"), "/s")));
	assert((eq(canonize_path("/","ls"), "/ls")));
	assert((eq(canonize_path("/a","/ls"), "/ls")));
	assert((eq(canonize_path("/",""), "/")));
	assert((eq(canonize_path("/asf",""), "/asf")));
	assert((eq(canonize_path("","/aaaa"), "/aaaa")));

	return 0;
}
