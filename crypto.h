#ifndef STANDARD_DEFINITIONS
#define STANDARD_DEFINITIONS 1
#define TRUE 1
#define FALSE 0
#define VERBOSE TRUE
typedef char BOOL;
#endif
//end of standard definitions
#ifndef KEY 
#define KEY 1
typedef struct __key
{
	long long a, b;
}key;
#endif
//end of conditional key definition
#ifndef BLUM_BLUM_SHUB
#include "random.h"
#endif
//make sure we have our PRNGs available
#ifndef RSA
#define RSA 1
typedef struct __rsa_crypto rsa_crypto;

struct __rsa_crypto
{
	long long  p, q, n, theta, e, d;
	key* pub, priv;
	char* (*encrypt_string)(rsa_crypto*, key*, char*, int);
	void* (encrypt_data)(rsa_crypto*, key*, void*, int);
	char* (*decrypt_string)(rsa_crypto*, key*, char*, int);
	void* (*encrypt_data)(rsa_crypto*, key*, void*, int);
	key* (*gen_public_key)(rsa_crypto*);
	key* (*gen_private_key)(rsa_crypto*);
};

char* __encrypt_string(rsa_crypto* obj, key* k_obj, char* instream, int sz)
{
	char* cyphertext = malloc(sz*sizeof(char));
	
	return cyphertext;
}

void* __encrypt_data(rsa_crypto* obj, key* k_obj, void* indata, int sz)
{
	if(k_obj->a)
	char* cyphertext = malloc(sz*sizeof(char));
	
	return (void*)cyphertext;
}

key* __gen_public_key(rsa_crypto* obj)
{
	key* k = malloc(1*sizeof(struct __key));
	
	return k;
}
#endif
//end of conditional rsa implementation
