#ifndef BIGINT
#define BIGINT
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//========================================================================================================================================================
// perhaps the end goal could be to replicate an efficient version of the python standard library for C 
//========================================================================================================================================================

//============================================================================
// Useful preprocessor setup stuff
//============================================================================

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

typedef char boolean;
typedef unsigned char byte;		/* we could use uint_8t from stdint.h for better cross platform uniformity */
#define TRUE 1
#define FALSE 0
#define POSITIVE 1
#define NEGATIVE 0
/* these are constructed in such a way as to make the results the same if a and b are equal */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//============================================================================
// Main Datatype definition
//============================================================================

typedef struct bigint{
	byte* data;
	boolean sign;
	unsigned int num_of_digits;
	unsigned int num_allocated;
}bigint;

//============================================================================
// User Function Initialization and Destruction Prototypes
//============================================================================

bigint* create_zero(void);					/* creates a new bigint and sets it equal to 0 */
bigint* create_one(void);					/* creates a new bigint and sets it equal to 1 */
bigint* create_from_int(int input);			/* creates a new bigint and sets it equal to the int value provided */
bigint* create_from_string(char* input);	/* creates a new bigint and sets it equal to the string provided */
int to_int(bigint* num);					/* this simply truncates to whatever fits in an int so be warned */
char* to_string(bigint* num);				/* returns a string of the integer */
boolean destroy(bigint* num);				/* this frees the object and sets it to null */

//============================================================================
// User Function Basic Math Prototypes
//============================================================================
/* Basic arithmetic operations: */
void bigint_add(bigint* result, bigint* a, bigint* b); /* result = a + b */
void bigint_sub(bigint* result, bigint* a, bigint* b); /* result = a - b */
void bigint_mul(bigint* result, bigint* a, bigint* b); /* result = a * b */
void bigint_div(bigint* result, bigint* a, bigint* b); /* result = a / b */
void bigint_mod(bigint* result, bigint* a, bigint* b); /* result = a % b */
void bigint_divmod(bigint* result, bigint* rem, bigint* a, bigint* b); /* result = a/b, rem = a%b */
void bigint_pow(bigint* result, bigint* a, bigint* b); /* result = a^b ie 10^4 = 10,000 */
void bigint_sqr(bigint* result, bigint* a);			   /* result = a^2 ie 5^2 = 25 */
void bigint_cube(bigint* result, bigint* a);		   /* result = a^3 ie 2^3 = 8 */
void bigint_isqrt(bigint* result, bigint* a);          /* Integer square root ie isqrt(5) => 2 */
void bigint_factorial(bigint* result, bigint* a);	   /* result = a! */
void bigint_n_choose_k(bigint* result, bigint* n, bigint* k); /* result = n choose k = n!/(k!(n-k)!) */

/* Special operators and comparison */
int bigint_cmp(bigint* a, bigint* b);               	/* Compare: returns 1 if a > b, 0 if a == b, and -1 if a < b */
int bigint_cmp_abs(bigint* a, bigint* b);               	/* Compare: returns 1 if abs(a) > abs(b), 0 if abs(a) == abs(b), and -1 if abs(a) < abs(b) */
boolean bigint_is_zero(bigint* n);                      /* For comparison with zero this returns TRUE if n is 0 and FALSE otherwise */
void bigint_inc(bigint* n);                             /* Increment: add one to n */
void bigint_dec(bigint* n);                             /* Decrement: subtract one from n */
void bigint_copy(bigint* dst, bigint* src);        		/* Copy src into dst*/

//============================================================================
// Internal Library Functions
//============================================================================

boolean __internal_make_correct_digit_allocation(bigint* num, int num_digits_needed);	/* this makes sure there is enough allocated memory for the num_digits_needed*/
void __positive_difference(bigint* diff, bigint* a, bigint* b); /* this calculates abs(abs(a)-abs(b)) the positve difference between the two ie (20, -300) is 280 */
void __obj_details(bigint* obj, int lineno, char* file); /* this is useful for debugging */
#endif