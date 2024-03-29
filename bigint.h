#ifndef BIGINT
#define BIGINT
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <string.h> 
//#include <math.h>
//#include <limits.h>
//========================================================================================================================================================
// perhaps the end goal could be to replicate an efficient version of the python standard library big int implementation for C with minimal dependencies
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

static const char* MAX_RANDOM_NUMBER_SIZE = "1000";
static unsigned int RANDOM_NUMBER_GENERATOR_SEED = 622336;


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
	byte* data;	//holds the digits of the number (least significant at index 0 and most significant at the higher indicies)
	boolean sign;
	unsigned int num_of_digits;
	unsigned int num_allocated;
}bigint;

//============================================================================
// Basic Interface Setup & User Functions
//============================================================================

bigint* create_zero(void);					/* creates a new bigint and sets it equal to 0 */
bigint* create_one(void);					/* creates a new bigint and sets it equal to 1 */
bigint* create_from_int(int input);			/* creates a new bigint and sets it equal to the int value provided */
bigint* create_from_string(const char* input);	/* creates a new bigint and sets it equal to the string provided */
/* string stuff */
int to_int(bigint* num);					/* this simply truncates to whatever fits in an int so be warned */
char* to_string(bigint* num);				/* returns a string of the integer */
/* memory de-allocation function */
boolean destroy(bigint* num);				/* this frees the object and sets it to null */

//============================================================================
// Mathematical Function Prototypes
//============================================================================
/* Basic math operations */
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

/* More complex math operations */
void next_prime(bigint* result, bigint* a);			   /* returns the next prime number above the input a */
boolean is_prime(bigint* a);						   /* returns true if a is prime and false otherwise */
void bigint_factorial(bigint* result, bigint* a);	   /* result = a! */
void bigint_n_choose_k(bigint* result, bigint* n, bigint* k); /* result = n choose k = n!/(k!(n-k)!) */
void bigint_random_seed(void); /* seeds the random number generator so that the random number sequences are unique */
void bigint_random(bigint* result); /* generate a cryptographically secure psuedo random number between 0 and MAX_RANDOM_NUMBER_SIZE */

/* Special operators and comparison */
int bigint_cmp(bigint* a, bigint* b);               	/* Compare: returns 1 if a > b, 0 if a == b, and -1 if a < b */
int bigint_cmp_abs(bigint* a, bigint* b);               /* Compare: returns 1 if abs(a) > abs(b), 0 if abs(a) == abs(b), and -1 if abs(a) < abs(b) */
int int_cmp(bigint* a, int b);							/* Compare to int: returns 1 if a > b, 0 if a == b, -1 if a < b */
int int_cmp_abs(bigint* a, int b);						/* Compare to int abs: returns 1 if abs(a) > abs(b), 0 if abs(a) == abs(b), and -1 if abs(a) < abs(b) */
boolean bigint_is_zero(bigint* n);                      /* For comparison with zero this returns TRUE if n is 0 and FALSE otherwise */
void bigint_inc(bigint* n);                             /* Increment: add one to n */
void bigint_dec(bigint* n);                             /* Decrement: subtract one from n */
void bigint_copy(bigint* dst, bigint* src);        		/* Copy src into dst*/

//============================================================================
// Internal Library Functions
//============================================================================

boolean __internal_make_correct_digit_allocation(bigint* num, int num_digits_needed);	/* this makes sure there is enough allocated memory for the num_digits_needed */
void __add_abs(bigint* sum, bigint* a, bigint* b); /* this adds the absolute values of the two numbers abs(a) + abs(b) */
void __fast_shift_10(bigint* num); /* this effectively is a bit shift but for a base 10 number */
void __fast_shift_10x(bigint* num, unsigned int places_to_shift); /* this effectively is a bit shift but for a base 10 number by an arbitrary amount */
void __fast_div_10(bigint* num); /* this effectively is a bit shift division but for a base 10 number */
void __fast_div_10x(bigint* num, unsigned int places_to_remove); /* this effectively is a bit shift division but for a base 10 number by an arbitrary amount */
bigint* __create_bigint_zero_of_size(unsigned int size); /* this just makes a bigint zero with size as the number of allocated digits */
void __positive_difference(bigint* diff, bigint* a, bigint* b); /* this calculates abs(abs(a)-abs(b)) the positve difference between the two ie (20, -300) is 280 */
void __obj_details(bigint* obj, int lineno, char* file); /* this is useful for debugging */
void __internal_memcpy(void* dest, const void* source, int size); /* replaces the string.h implementation */
int __strcmp(const char *s1, const char *s2);	/* this is just a copy of the standard string.h implementation of strcmp since that is one of the only function from that library that we utilize */
size_t __strlen(const char *str);	/* this is just a copy of the standard string.h implementation of strlen since that is one of the only function from that library that we utilize */
int __fast_int_log10(int input);	/* this is to replace the standard log10() function from math.h and significantly improve performance */
int __fast_pow_10(int pow);			/* this is to eliminate the need for the pow() function from math.h it is also much faster as it only handles 9 exponents and 1 base */
void __fast_div_2(bigint* result, bigint* num); /* this is an internal way to very quickly divide by 2 it is integrated into the user facing bigint_div function it */
void __smallnum_internal_divmod(bigint* result, bigint* rem, bigint* a, bigint* b);
#endif
