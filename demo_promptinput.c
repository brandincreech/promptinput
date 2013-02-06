/** @file demo_promptinput.c
 * Demonstrate use of the promptinput library.
 *
 * This demo program issues a few prompts for user input and demonstrates how
 * the callback functions can be defined to handle input validation.
 */
#include "standard_headers.h"
#include "promptinput.h"

bool checkhours(const void *ans);
bool checkminutes(const void *ans);
bool checkage(const void *ans);
bool checkyesno(const void *ans);
bool datecheck(size_t count, const void *args[count]);

int main(void) {
	int hr, min;
	if (!promptinput({.prompt="Enter the time of day (e.g. 20:57): ",
			.error="Sorry. It must be a valid time of day in 24-hour format\n",
			.validators = {checkhours, checkminutes}}, "%d:%d", &hr, &min)) {
		goto end;
	}
	
	int age;
	if (!promptinput({.prompt="Enter your age: ",
			.error="Sorry. You must be between 18 and 125 years of age\n",
			.validators = {checkage}}, "%d", &age)) {
		goto end;
	}

	int day, month, year;
	if (!promptinput({.prompt="Enter a date in the format MM/DD/YYYY: ",
			.all_validator = datecheck}, "%d/%d/%d", &month, &day, &year)) {
		goto end;
	}
		
	char yesno;
	if (!promptinput({.prompt="Print a summary (y or n): ",
			.validators = {checkyesno}}, " %c", &yesno)) {
		goto end;
	}
	
	// Show summary if user requested it
	if (toupper(yesno) == 'Y') {
		printf("You entered a time of %02d:%02d\n", hr, min);
		printf("Your age is %d\n", age);
		printf("You entered a date with day %d on month %d of the year %d\n",
				day, month, year);
	}else{
		printf("No summary requested. Bye.\n");
	}
	return EXIT_SUCCESS;
	
	// Jump here in case user terminated the input
end:
	printf("User terminated the input process.\n");
	return EXIT_FAILURE;
}

/*----------------------------------------------------------------------------*/
// Define validation functions
/*----------------------------------------------------------------------------*/

bool checkhours(const void *ans) {
	const int *hour = ans;
	if (*hour >= 0 && *hour <= 23)
		return true;
	return false;
}

bool checkminutes(const void *ans) {
	const int *min = ans;
	if (*min >= 0 && *min <= 59)
		return true;
	return false;
}

bool checkage(const void *ans) {
	const int *age = ans;
	if (*age >= 18 && *age <= 125)
		return true;
	return false;
}

bool checkyesno(const void *ans) {
	const char *c = ans;
	if (*c == 'y' || *c == 'Y' || *c == 'n' || *c == 'N')
		return true;
	return false;
}

bool is_leap_year(int year);

bool datecheck(size_t count, const void *args[count]) {
	if (count != 3)
		return false;
	const int *m = args[0];
	const int *d = args[1];
	const int *y = args[2];
	if (*m < 1 || *m > 12) {
		return false;
	}
	if (*d < 1 || *d > 31) {
		return false;
	}
	// 30 days hath september, april, june and november...
	if ((*m == 9 || *m == 4 || *m == 6 || *m == 11) && *d > 30) {
		return false;
	}
	if (*m == 2 && is_leap_year(*y) && *d > 29) {
		return false;
	}
	if (*m == 2 && !is_leap_year(*y) && *d > 28) {
		return false;
	}
	if (*y < 1 || *y > 9999) {
		return false;
	}
	return true;
}

bool is_leap_year(int y) { 
	if (y % 400 == 0) return true;
	if (y % 100 == 0) return false;
	if (y % 4 == 0) return true;
	return false;
}
