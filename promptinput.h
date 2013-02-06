/** @file promptinput.h
 * Promptinput - read input from user and provide input error handling
 *
 * Promptinput provides a wrapper around printf, fgets and sscanf which 
 * collectively print a prompt, read and process input according to a 
 * scanf-style format string.
 */
#ifndef PROMPTINPUT_H
#define PROMPTINPUT_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// Default error message used if validation fails
#define PROMPTINPUT_DEFERROR "Sorry. The input you gave was not correct.\n"

// PROMPTINPUT_S - Structure to specify parameters when calling promptinput
typedef struct PROMPTINPUT_S PROMPTINPUT_S;
struct PROMPTINPUT_S {
	const char *prompt;
	const char *error;
	bool (*validators[10])(const void *answer);
	bool (*all_validator)(size_t count, const void *args[count]);
};

// promptinput_dummy_char - used internally by promptinput
extern char promptinput_dummy_char;

// SYNOPSIS - promptinput(PROMPTINPUT_S PSTR, fmt, ...) 
// 		display a prompt and read formatted input
// RETURNS
// 		true - normal return
// 		false - user signalled EOF
#define promptinput(PSTR, ...) \
	promptinput_((PROMPTINPUT_S)PSTR, __VA_ARGS__, &promptinput_dummy_char)

bool promptinput_(PROMPTINPUT_S p, const char *fmt, ...);

#endif // PROMPTINPUT_H
