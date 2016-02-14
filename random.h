#ifndef STANDARD_DEFINITIONS
#define STANDARD_DEFINITIONS 1
#define TRUE 1
#define FALSE 0
#define VERBOSE TRUE
typedef char BOOL;
#endif
//end of standard definitions
#ifndef BLUM_BLUM_SHUB
#define BLUM_BLUM_SHUB 1
typedef struct __blum_blum_shub blum_blum_shub;

struct __blum_blum_shub
{
	//B.B.S PRNG works like this: X_n+1 = (X_n^2)mod(M) where M = P*Q and X_0 is
	//co-prime to M and P is congruent to 3mod(4) and q is congruent to 3mod(4)
	//and gcd(phi(p-1), phi(q-1)) < a where a is a relatively small number and phi(X)
	//is euler's totiant function
	//also can directly calculate nth element of set by: X_i = ( X_0^(2^i mod(car(M)))) mod(M)
	//where car is the Charmichael function car(M) = lcm((p-1), (q-1))
};

#endif
//end of conditional blum blum shub implementation
