#include "myapue.h"

int strregex(char *str1, char *str2)
{
	regex_t regex;
	int ret;

	ret = regcomp(&regex, str1, 0);
	if (ret)
		return -1;
		
	ret = regexec(&regex, str2, 0, NULL, 0);
	regfree(&regex);
	if (!ret) {
		return 1;
	} else if (ret == REG_NOMATCH) {
		return 0;
	} else {
		return -2;
	}
}

