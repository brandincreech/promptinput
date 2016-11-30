#include "promptinput.h"
#include <stdbool.h>
#include <stdlib.h>

char promptinput_dummy_char;

// is_empty(str): return true if str is empty or contains only whitespace
static inline bool is_empty(const char *str) {
	if (sscanf(str, " %c", &promptinput_dummy_char) == EOF)
		return true;
	return false;
}

bool promptinput_(PROMPTINPUT_S p, const char *fmt, ...) {
	// Count arguments ("%d", "%f", etc...) that are in fmt
	char count = 0;
	va_list ap;
	va_start(ap, fmt);
	char *c = (char *)fmt;
	for (; *c != '\0'; c++) {
		if (*c == '%') {
			if (*(c+1) == '*')
				;
			else if (*(c+1) == '%') // XXX What if format string contains "... %% ..."? The first % will be ignored by this line, but then in the next loop iteration, the second '%' will be incorrectly counted as a format specifier.
				;
			else
				count++;
		}
	}
	va_end(ap);

	// all_args: used to store a list of args to give to the all_validator
	void *all_args[count]; 

	for (;;) {
		// Show prompt and read an input line
		printf("%s", p.prompt);
		char linebuf[10000];
		if (fgets(linebuf, 10000, stdin) == NULL) {
			return false;
		}
		
		// If there is empty input, simply redisplay the prompt
		if (is_empty(linebuf))
			continue;
		
		// Read the input line and get the returned result ret
		// A dummy character argument is appended to fmt in order to make sure
		// there are no extra garbage characters leftover in the user's input
		int ret;
		va_start(ap, fmt);
		char newfmt[strlen(fmt)+strlen(" %c")+1];
		strcpy(newfmt, fmt);
		strcat(newfmt, " %c");
		ret = vsscanf(linebuf, newfmt, ap);
		va_end(ap);

		// Display an error if ret does not match count or if any of the 
		// validation functions fail
		bool show_error = false;
		if (ret != count) {
			show_error = true;
			goto done;
		}
		va_start(ap, fmt);
		for (int i=0; i < count; i++) {
			void *arg = va_arg(ap, void*);
			if (p.validators[i] && !(p.validators[i](arg))) {
				show_error = true;
				break;
			}
			all_args[i] = arg;
		}
		va_end(ap);
		// Check all arguments through the all_validator if appropriate
		if (!show_error) {
			if (p.all_validator && !(p.all_validator((size_t)count, 
					(const void**)all_args))) {
				show_error = true;
			}
		}
done:
		// Only break the loop if no error was shown
		if (show_error)
			printf("%s", p.error ? p.error : PROMPTINPUT_DEFERROR);
		else
			break;
	}
	return true;
}
