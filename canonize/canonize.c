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
char *clean_path(char *path) {
	int i, j;
	size_t len = strlen(path);
	for(i = 0; i < len; i++) {
	}
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

char *canonize_path(char *prefix, char *path) {
	char *p;
	int len;
	if (path[0] == '/') {
		p = strdup(path);
	} else {
		if (prefix[0] != '/') {
			printf("Error - prefix should start with /\n");
			return NULL;
		}
		len = strlen(prefix) + strlen(path) + 1;
		p = calloc(1, len);
		strcpy(p, prefix);
		strcat(p, "/");
		strcat(p, path);
	}
	if (!(clean_path(p))) {
		printf("Cannot clean path: %s\n", p);
		return NULL;
	}
	printf("[%s]\n", p);
	return p;
}

// path 'smart' copy //
int eq(char *str1, char *str2) {
	if (strcmp(str1, str2) != 0) {
		printf("Error %s != %s\n", str1, str2);
		return 0;
	}
	printf(".");
	return 1;
}

int main() {

	char *c;
//	c = canonize_path("/", "dev");
//	assert((eq(c, "/dev")));
//	free(c);
//	c = canonize_path("//", "//dev");
//	assert((eq(c, "/dev")));
//	free(c);
//	c = canonize_path("/./", "dev/../");
//	assert((eq(c, "/")));
//	free(c);
//	c = canonize_path("//././", "/dev/../");
//	assert((eq(c, "/")));
//	free(c);
//	c = canonize_path("/dev/", "asddsa/../");
//	assert((eq(c, "/dev")));
//	free(c);
//	c = canonize_path("/asd/", "dev/");
//	assert((eq(c, "/asd/dev")));
//	free(c);
//	c = canonize_path("//", "dev/../");
//	assert((eq(c, "/")));
//	free(c);
//	c = canonize_path("/./././aaaaa/..", "bbbb");
//	assert((eq(c, "/bbbb")));
//	free(c);
//	c = canonize_path("/", "aaaa/.");
//	assert((eq(c, "/aaaa")));
//	free(c);
//	c = canonize_path("////aa", "bbb////");
//	assert((eq(c, "/aa/bbb")));
//	free(c);
//	c = canonize_path("/test/baba", "../aaa../");
//	assert((eq(c, "/test/aaa..")));
//	free(c);
//	c = canonize_path("/dev", "..");
//	assert((eq(c, "/")));
//	free(c);
//	c = canonize_path("//.", "..");
//	assert((eq(c, "/")));
//	free(c);
//	c = canonize_path("/", "s");
//	assert((eq(c, "/s")));
//	free(c);
//	c = canonize_path("/", "ls");
//	assert((eq(c, "/ls")));
//	free(c);
//	c = canonize_path("/", "");
//	assert((eq(c, "/")));
//	free(c);
//	c = canonize_path("/asf", "");
//	assert((eq(c, "/asf")));
//	free(c);
//	c = canonize_path("", "/aaaa");
//	assert((eq(c, "/aaaa")));
//	free(c);
//	c = canonize_path("/", "/");
//	assert((eq(c, "/")));
//	free(c);
//	c = canonize_path("/a/bb", "/mkdir");
//	assert((eq(c, "/mkdir")));
//	free(c);
//	c = canonize_path("/a/bb", "/a/bb");
//	assert((eq(c, "/a/bb")));
//	free(c);
	c = canonize_path("/pp/a", "./..");
	assert((eq(c, "/pp")));
	free(c);
	return 0;
}
