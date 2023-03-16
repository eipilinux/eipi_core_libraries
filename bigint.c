#include "bigint.h"

bigint* create_zero(void){                              /* FIN */
    bigint* retval = malloc(sizeof(bigint));
    retval->num_of_digits = 1;
    retval->num_allocated = 2*retval->num_of_digits+1;
    retval->data = malloc(retval->num_allocated*sizeof(byte));
    retval->sign = POSITIVE;
    retval->data[0] = 0;
    return retval;
}
bigint* create_one(void){                               /* FIN */
    bigint* retval = create_zero();
    retval->data[0] = 1;
    return retval;
}
bigint* create_from_int(int input){                     /* FIN */
    if(input == 0)
        return create_zero();
    if(input == 1)
        return create_one();
    int stepwise_remainder = abs(input), iter = 0;
    bigint* retval = malloc(sizeof(bigint));
    retval->sign = (input >= 0) ? POSITIVE : NEGATIVE;
    retval->num_of_digits = __fast_int_log10(stepwise_remainder) + 1;//(unsigned int)log10((double)stepwise_remainder) + 1;
    retval->num_allocated = 2*retval->num_of_digits+1;
    retval->data = malloc(retval->num_allocated*sizeof(byte));
    do{
        retval->data[iter] = stepwise_remainder % 10;
        stepwise_remainder /= 10;
        iter++;
    }while(stepwise_remainder > 0);
    return retval;
}
bigint* create_from_string(char* input){                /* FIN */
    if(__strcmp(input,"0") == 0)
        return create_zero();
    if(__strcmp(input,"1") == 0)
        return create_one();
    bigint* retval = malloc(sizeof(bigint));
    retval->sign = (input[0] == '-') ? NEGATIVE : POSITIVE;
    retval->num_of_digits = (retval->sign == POSITIVE) ? __strlen(input) : __strlen(input) - 1;
    retval->num_allocated = 2*retval->num_of_digits+1;
    retval->data = malloc(retval->num_allocated*sizeof(byte));
    int index_in_number = retval->num_of_digits - 1; //the most significant end of the number
    int i = (retval->sign == POSITIVE) ? 0 : 1;
    for(; i < __strlen(input); i++){
        retval->data[index_in_number] = input[i]-48;
        index_in_number--;
    }
    return retval;
}
int to_int(bigint* num){                                /* FIN */
    if(num->num_of_digits > 10){
        printf("Error number is out of range!\n");
        return 0;
    }
    int return_int = 0;
    for(int i = 0; i < num->num_of_digits; i++){
        return_int += num->data[i] * __fast_pow_10(i);//(int)pow(10.0, (double)i); //this is guaranteed to be less than 11 digits as max int is not that big
    }
    return (num->sign == POSITIVE) ? return_int : -1*return_int;
}
char* to_string(bigint* num){                           /* FIN */
    int strlength = num->num_of_digits + ((num->sign == POSITIVE) ? 0 : 1) + 1; //the +1 is for the terminating \0
    char* retstr = malloc(strlength);
    retstr[0] = (num->sign == POSITIVE) ? ' ' : '-';
    for(int i = 0; i < num->num_of_digits; i++){
        retstr[strlength - i - 2] = (char)(48 + num->data[i]);
    }
    retstr[strlength - 1] = '\0';
    return retstr;
}
boolean destroy(bigint* num){                           /* FIN */
    if(!num)
        return TRUE;
    if(num->num_allocated > 0)
        free(num->data);
    free(num);
    return TRUE;
}
void bigint_add(bigint* result, bigint* a, bigint* b){  /* FIN */
    if(a->sign == b->sign){
        __add_abs(result, a, b);
        result->sign = a->sign;
    }
    else{
        __positive_difference(result, a, b);    //this is effectively some kind of subtraction we just don't know what the final sign will be
        result->sign = (bigint_cmp_abs(a, b) == 0) ? POSITIVE : (bigint_cmp_abs(a, b) > 0) ? a->sign : b->sign;  //figure out the dominant sign
    }
}
void bigint_sub(bigint* result, bigint* a, bigint* b){  /* FIN */
    if(a->sign == b->sign){
        if(a->sign == POSITIVE){    //if both are positive then it is just a vanilla subtraction of a-b
            __positive_difference(result, a, b);
            result->sign = (bigint_cmp_abs(a, b) == 0) ? POSITIVE : (bigint_cmp_abs(a, b) > 0) ? POSITIVE : NEGATIVE;  //figure out the sign of the number
        }
        else{   //if both are negative then we have (-a)-(-b) -> -1(a-b) thus we can just do regular subtraction and then change the sign
            __positive_difference(result, a, b);
            result->sign = (bigint_cmp_abs(a, b) == 0) ? POSITIVE : (bigint_cmp_abs(a, b) > 0) ? NEGATIVE : POSITIVE;  //figure out the sign of the number
        }
    }
    else{
        __add_abs(result, a, b); //this is effectively addition just with different signs
        result->sign = a->sign; //since (+a) - (-b) is positive and (-a) - (+b) is negative
    }
}
void bigint_mul(bigint* result, bigint* a, bigint* b){
    bigint* retval;
}
void bigint_div(bigint* result, bigint* a, bigint* b){
    bigint* retval;
    
}
void bigint_mod(bigint* result, bigint* a, bigint* b){
    bigint* retval;
    
}
void bigint_divmod(bigint* result, bigint* rem, bigint* a, bigint* b){
    bigint* retval;
    
}
void bigint_pow(bigint* result, bigint* a, bigint* b){
    bigint* retval;
    
}
void bigint_sqr(bigint* result, bigint* a){
    bigint* retval;
    
}
void bigint_cube(bigint* result, bigint* a){
    bigint* retval;
    
}
void bigint_isqrt(bigint* result, bigint* a){
    bigint* retval;
    
}
void bigint_factorial(bigint* result, bigint* a){
    bigint* retval;
    
}
void bigint_n_choose_k(bigint* result, bigint* n, bigint* k){
    bigint* retval;
    
}
void bigint_random(bigint* result, bigint* lower_bound, bigint* upper_bound){
    bigint* retval;

}
int bigint_cmp(bigint* a, bigint* b){              /* FIN */
    if(a->sign != b->sign)
        return (a->sign > b->sign) ? 1 : -1;
    if(a->sign)
        return bigint_cmp_abs(a, b);
    return bigint_cmp_abs(b, a);
}
int bigint_cmp_abs(bigint* a, bigint* b){          /* FIN */
    if(a->num_of_digits != b->num_of_digits)
        return (a->num_of_digits > b->num_of_digits) ? 1 : -1;
    for(int i = a->num_of_digits - 1; i >= 0; i--){
        if(a->data[i] != b->data[i])
            return (a->data[i] > b->data[i]) ? 1 : -1;
    }
    return 0;
}
int int_cmp(bigint* a, int b){                     /* FIN */
    boolean sign_of_b = (b >= 0) ? POSITIVE : NEGATIVE;
    if(sign_of_b == a->sign)
        return (a->sign == POSITIVE) ? int_cmp_abs(a, b) : -1*int_cmp_abs(a, b);
    return (a->sign == POSITIVE) ? 1 : -1;
}
int int_cmp_abs(bigint* a, int b){                 /* FIN */
    if(a->num_of_digits > 10) //this makes sure that if we are going to exceed the int bounds we can return quickly and correctly that abs(a) > abs(b)
        return 1;
    int number_to_compare = abs(to_int(a));
    if(number_to_compare == abs(b))
        return 0;
    return (number_to_compare > abs(b)) ? 1 : -1;
}
boolean bigint_is_zero(bigint* n){                 /* FIN */
    if(n->num_of_digits == 1 && n->data[0] == 0)
        return TRUE;
    return FALSE;
}
void bigint_inc(bigint* n){
    if(n->sign){
        __internal_make_correct_digit_allocation(n, n->num_of_digits + 1);
        byte carry_over = 1; //this is how we add one
        int i = 0;
        do{
            n->data[i] = ((n->num_of_digits > i) ? n->data[i] : 0) + carry_over;
            carry_over = (n->data[i] > 9) ? n->data[i] - 9 : 0;
            n->data[i] %= 10;
            i++;
        }while (carry_over > 0);
        n->num_of_digits = (n->num_of_digits > i) ? n->num_of_digits : n->num_of_digits + 1;
    }
    //handle negative incriments
}
void bigint_dec(bigint* n){
    
}
void bigint_copy(bigint* dst, bigint* src){
    __internal_make_correct_digit_allocation(dst, src->num_of_digits);
    dst->sign = src->sign;
    dst->num_of_digits = src->num_of_digits;
    memcpy(dst->data, src->data, dst->num_of_digits); //memcpy is very fast, unfortunetly it uses the string.h lib which i would prefer not to use
}
boolean __internal_make_correct_digit_allocation(bigint* num, int num_digits_needed){       /* FIN */
    if(num->num_allocated >= num_digits_needed)
        return TRUE;
    byte* tmp = realloc(num->data, 2*num_digits_needed);
    if (tmp == NULL){
        printf("allocation failed"); //i should really implement a better error function to report errors to a file
        return FALSE;
    }
    num->data = tmp;
    num->num_allocated = 2*num_digits_needed;
    return TRUE;
}
void __add_abs(bigint* sum, bigint* a, bigint* b){
    int length = MAX(a->num_of_digits, b->num_of_digits);
    __internal_make_correct_digit_allocation(sum, length + 1);
    byte carry_over = 0;
    for(int i = 0; i < length + 1; i++){
        sum->data[i] = ((a->num_of_digits > i) ? a->data[i] : 0) + ((b->num_of_digits > i) ? b->data[i] : 0) + carry_over;
        carry_over = (sum->data[i] > 9) ? sum->data[i] / 10 : 0;
        sum->data[i] %= 10;
    }
    sum->num_of_digits = (sum->data[length] > 0) ? length+1 : length;
    sum->sign = POSITIVE;
}
void __positive_difference(bigint* diff, bigint* a, bigint* b){
    bigint* larger;
    bigint* smaller;
    larger = (bigint_cmp_abs(a, b) > 0) ? a : b;
    smaller = (bigint_cmp_abs(a, b) > 0) ? b : a;
    int length = MAX(larger->num_of_digits, smaller->num_of_digits);
    __internal_make_correct_digit_allocation(diff, length);
    byte borrow_over = 0;
    int tmp_plc_data = 0;
    diff->num_of_digits = 0;
    for(int i = 0; i < length; i++){
        tmp_plc_data = (int)(((larger->num_of_digits > i) ? larger->data[i] : 0) - ((smaller->num_of_digits > i) ? smaller->data[i] : 0) - borrow_over);
        borrow_over = (tmp_plc_data < 0) ? 1 : 0;
        diff->data[i] = (tmp_plc_data < 0) ? tmp_plc_data+10 : tmp_plc_data;
        tmp_plc_data = 0;
        diff->num_of_digits = (diff->data[i] > 0) ? i+1 : diff->num_of_digits;
    }
    diff->sign = POSITIVE;
}
void __obj_details(bigint* obj, int lineno, char* file){
    printf("\nThe object at line number: "BOLDMAGENTA"%d"RESET" in file: "BOLDMAGENTA"%s\n"RESET, lineno, file);
    if(!obj){
        printf(BOLDRED"Warning:"RESET"This object is NULL\n");
    }
    else{
        printf("    The object pointer points to: "BOLDWHITE"%p\n"RESET, (void*)obj);
        printf("    The object "BOLDCYAN"data pointer"RESET" points to: "BOLDWHITE"%p\n"RESET, (void*)obj->data);
        printf("    The object "BOLDCYAN"sign"RESET" is: "BOLDYELLOW"%s\n"RESET, (obj->sign > 0) ? "POSITIVE" : "NEGATIVE");
        printf("    The object "BOLDCYAN"num_of_digits"RESET" is: "BOLDYELLOW"%d\n"RESET, obj->num_of_digits);
        printf("    The object "BOLDCYAN"num_allocated"RESET" is: "BOLDYELLOW"%d\n"RESET, obj->num_allocated);
        printf("    The object "BOLDCYAN"data"RESET" holds the values (this is in reverse order): "BOLDYELLOW);
        for(int i = 0; i < obj->num_of_digits; i++){
            printf("%d ", obj->data[i]);
        }
        printf(RESET"\n");
    }
    printf("\n");
}
int __strcmp(const char *s1, const char *s2){
   while (*s1 != '\0' && *s2 != '\0'  && *s1 == *s2) {
      s1++;
      s2++;
   }
   return *s1 - *s2;
}
size_t __strlen(const char *str){
  const char *char_ptr;
  const unsigned long int *longword_ptr;
  unsigned long int longword, himagic, lomagic;
  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.  */
  for (char_ptr = str; ((unsigned long int) char_ptr
			& (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == '\0')
      return char_ptr - str;
  /* All these elucidatory comments refer to 4-byte longwords,
     but the theory applies equally well to 8-byte longwords.  */
  longword_ptr = (unsigned long int *) char_ptr;
  /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
     the "holes."  Note that there is a hole just to the left of
     each byte, with an extra at the end:
     bits:  01111110 11111110 11111110 11111111
     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
     The 1-bits make sure that carries propagate to the next 0-bit.
     The 0-bits provide holes for carries to fall into.  */
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
  if (sizeof (longword) > 4)
    {
      /* 64-bit version of the magic.  */
      /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
      himagic = ((himagic << 16) << 16) | himagic;
      lomagic = ((lomagic << 16) << 16) | lomagic;
    }
  if (sizeof (longword) > 8)
    abort ();
  /* Instead of the traditional loop which tests each character,
     we will test a longword at a time.  The tricky part is testing
     if *any of the four* bytes in the longword in question are zero.  */
  for (;;)
    {
      longword = *longword_ptr++;
      if (((longword - lomagic) & ~longword & himagic) != 0)
	{
	  /* Which of the bytes was the zero?  If none of them were, it was
	     a misfire; continue the search.  */
	  const char *cp = (const char *) (longword_ptr - 1);
	  if (cp[0] == 0)
	    return cp - str;
	  if (cp[1] == 0)
	    return cp - str + 1;
	  if (cp[2] == 0)
	    return cp - str + 2;
	  if (cp[3] == 0)
	    return cp - str + 3;
	  if (sizeof (longword) > 4)
	    {
	      if (cp[4] == 0)
		return cp - str + 4;
	      if (cp[5] == 0)
		return cp - str + 5;
	      if (cp[6] == 0)
		return cp - str + 6;
	      if (cp[7] == 0)
		return cp - str + 7;
	    }
	}
    }
}
int __fast_int_log10(int input){
    return (input >= 1000000000) ? 9 : (input >= 100000000) ? 8 : 
        (input >= 10000000) ? 7 : (input >= 1000000) ? 6 : 
        (input >= 100000) ? 5 : (input >= 10000) ? 4 :
        (input >= 1000) ? 3 : (input >= 100) ? 2 : (input >= 10) ? 1 : 0; 
}
int __fast_pow_10(int pow){
    if(pow > 9 || pow < 0){
        printf("error the exponent in __fast_pow_10 was out of bounds\n");
        return -1;
    }
    return (pow == 0) ? 1 : (pow == 1) ? 10 : (pow == 2) ? 100 : (pow == 3) ? 1000 :
    (pow == 4) ? 10000 : (pow == 5) ? 100000 : (pow == 6) ? 1000000 : (pow == 7) ? 10000000 : 
    (pow == 8) ? 100000000 : 1000000000;
}