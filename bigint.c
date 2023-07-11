#include "bigint.h"

//============================================================================
// Basic Interface Setup & User Functions
//============================================================================
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

/* string stuff */
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

/* memory de-allocation function */
boolean destroy(bigint* num){                           /* FIN */
    if(!num)
        return TRUE;
    if(num->num_allocated > 0)
        free(num->data);
    free(num);
    return TRUE;
}

//============================================================================
// Mathematical Function Prototypes
//============================================================================
/* Basic math operations */
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
void bigint_mul(bigint* result, bigint* a, bigint* b){  /* FIN */
    //we will be first using a nieve approach and then a variation of the Karatsuba fast multiplication algorithm
    int possible_num_of_digits = a->num_of_digits + b->num_of_digits;
    __internal_make_correct_digit_allocation(result, possible_num_of_digits);
    //chose the shorter one for the outside
    //choose the longer one for the inside
    bigint* larger = (bigint_cmp_abs(a, b) > 0) ? a : b;
    bigint* smaller = (bigint_cmp_abs(a, b) > 0) ? b : a;

    // bigint* stepwise_addend = create_zero();
    // bigint* intermediary = create_zero();
    // bigint* running_total = create_zero();
    // __internal_make_correct_digit_allocation(stepwise_addend, possible_num_of_digits);
    // __internal_make_correct_digit_allocation(intermediary, possible_num_of_digits);
    // __internal_make_correct_digit_allocation(running_total, possible_num_of_digits);
    bigint* stepwise_addend = __create_bigint_zero_of_size(possible_num_of_digits);
    bigint* intermediary = __create_bigint_zero_of_size(possible_num_of_digits);
    bigint* running_total = __create_bigint_zero_of_size(possible_num_of_digits);
    
    for(int i = 0; i < smaller->num_of_digits; i++){
        byte carry_over = 0;
        for(int j = 0; j < larger->num_of_digits; j++){
            stepwise_addend->data[j] = smaller->data[i] * larger->data[j] + carry_over;
            carry_over = 0;
            if(stepwise_addend->data[j] > 9){
                carry_over = stepwise_addend->data[j] / 10;
                stepwise_addend->data[j] %= 10;
            }
        }
        if(carry_over != 0){
            stepwise_addend->data[larger->num_of_digits] = carry_over;
            stepwise_addend->num_of_digits = larger->num_of_digits + 1;
        }
        else{
            stepwise_addend->num_of_digits = larger->num_of_digits;
        }
        //__obj_details(stepwise_addend, __LINE__, __FILE__);
        //do the appropriate decimal place shift for this round
        __fast_shift_10x(stepwise_addend, i);
        bigint_add(intermediary, running_total, stepwise_addend);
        bigint* switcheroo = running_total;
        running_total = intermediary;
        intermediary = switcheroo;
    }
    bigint_copy(result, running_total);
    result->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;
    destroy(stepwise_addend);
    destroy(intermediary);
    destroy(running_total);
}
void bigint_div(bigint* result, bigint* a, bigint* b){  /* FIN */
    bigint* remainder = create_zero();
    bigint_divmod(result, remainder, a, b);
    destroy(remainder);
}
void bigint_mod(bigint* result, bigint* a, bigint* b){  /* FIN */
    bigint* dividend = create_zero();
    bigint_divmod(dividend, result, a, b);
    destroy(dividend);
}
void bigint_divmod(bigint* result, bigint* rem, bigint* a, bigint* b){  /* FIN */
    if(bigint_is_zero(b) || bigint_is_zero(a)){
        rem->data[0] = 0;
        rem->num_of_digits = 1;
        rem->sign = POSITIVE;

        if(bigint_is_zero(b))
            printf("error: cannot divide by zero\n");
        result->data[0] = 0;
        result->num_of_digits = 1;
        result->sign = POSITIVE;
    }
    else if(bigint_cmp_abs(a, b) < 0){
        bigint_copy(rem, a);

        result->data[0] = 0;
        result->num_of_digits = 1;
        result->sign = POSITIVE;
    }
    else if(bigint_cmp_abs(a, b) == 0){
        rem->data[0] = 0;
        rem->num_of_digits = 1;
        rem->sign = POSITIVE;

        result->data[0] = 1;
        result->num_of_digits = 1;
        result->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;
    }
    else if(int_cmp_abs(b, 1) == 0){
        rem->data[0] = 0;
        rem->num_of_digits = 1;
        rem->sign = POSITIVE;

        bigint_copy(result, a);
        result->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;
    }
    else if(int_cmp_abs(b, 2) == 0){
        rem->data[0] = a->data[0] % 2;
        rem->num_of_digits = 1;
        rem->sign = POSITIVE;

        __fast_div_2(result, a);
        result->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;
    }
    else{
        bigint* total = __create_bigint_zero_of_size(a->num_of_digits);
        total->num_of_digits = 0;
        bigint* step = __create_bigint_zero_of_size(a->num_of_digits);
        bigint* working_dividend = __create_bigint_zero_of_size(a->num_of_digits);
        bigint* mod = __create_bigint_zero_of_size(a->num_of_digits);
        boolean first_digit_trip = FALSE;
        for(int i = a->num_of_digits - 1; i >= 0; i--){
            __fast_shift_10(working_dividend);
            working_dividend->data[0] = a->data[i];
            __smallnum_internal_divmod(step, mod, working_dividend, b);
            //this is where we can improve things by skipping all the inevitible 0's created by looping until we get something larger than the divisor
            if(int_cmp(step, 0) > 0){
                total->num_of_digits++;
                total->data[total->num_of_digits - 1] = step->data[0];
                bigint* tmp = working_dividend;
                working_dividend = mod;
                mod = tmp;
                first_digit_trip = TRUE;
            }
            else if(first_digit_trip){
                total->num_of_digits++;
                total->data[total->num_of_digits - 1] = step->data[0];
            }
        }
        bigint_copy(rem, working_dividend);
        //__obj_details(total, __LINE__, __FILE__);
        __internal_make_correct_digit_allocation(result, total->num_of_digits);
        result->num_of_digits = total->num_of_digits;
        result->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;

        for(int i = 0; i < total->num_of_digits; i++){
            result->data[i] = total->data[total->num_of_digits - i - 1];
        }

        destroy(total);
        destroy(step);
        destroy(working_dividend);
        destroy(mod);
    }
}
void bigint_pow(bigint* result, bigint* a, bigint* b){  /* FIN */
    if(bigint_is_zero(b)){
        result->data[0] = 1;
        result->sign = POSITIVE;
    }
    else{
        if(bigint_is_zero(a)){
            result->data[0] = 0;
            result->sign = POSITIVE;
        }
        else{
            bigint* work_in_progress = create_one();
            bigint* working_copy_b = create_zero();
            bigint* placeholder = create_zero();
            bigint* tmp;
            bigint_copy(working_copy_b, b);
            while(int_cmp(working_copy_b, 0) > 0){
                bigint_mul(placeholder, work_in_progress, a);
                tmp = placeholder;
                placeholder = work_in_progress;
                work_in_progress = tmp;
                bigint_dec(working_copy_b);
            }
            bigint_copy(result, work_in_progress);
            destroy(work_in_progress);
            destroy(working_copy_b);
            destroy(placeholder);
        }
    }
}
void bigint_sqr(bigint* result, bigint* a){             /* FIN */
    bigint* internal_copy = create_zero();
    bigint_copy(internal_copy, a);
    bigint_mul(result, a, internal_copy);
    destroy(internal_copy);
}
void bigint_cube(bigint* result, bigint* a){            /* FIN */
    bigint* internal_copy = create_zero();
    bigint_sqr(internal_copy, a);
    bigint_mul(result, a, internal_copy);
    destroy(internal_copy);
}
void bigint_isqrt(bigint* result, bigint* a){           /* FIN */
    if(bigint_is_zero(a)){
        result->data[0] = 0;
        result->num_of_digits = 1;
        result->sign = POSITIVE;
    }
    else{
        /*
            initial guess is scaled to match the approximate size of the input number based on the number of digits in the input
            ie for a 1 digit input the guess is 3 since the largest 1 digit number is 9 and sqrt(9) <= 3
            for 2 digit number the guess is 10 since (int)sqrt(99) <= 10
            for 3 digit number the guess is 32 since (int)sqrt(999) <= 32
            for 4 digit number the guess is 100 since (int)sqrt(9999) <= 100
            for 5 digit number the guess is 320 since (int)sqrt(99999) <= 320
            for 6 digit number the guess is 1000 since (int)sqrt(999999) <= 1000
            etc

            in addition we can do a leading digit refinement by noting that
            ie for a 1 digit input the guess is 3 since the largest 1 digit number is 9 and sqrt(9) <= 3
            for 2 digit number the guess is 10 since (int)sqrt(99) <= 10 and if the leading digit is <5 we change it to 7
            for 3 digit number the guess is 32 since (int)sqrt(999) <= 32
            for 4 digit number the guess is 100 since (int)sqrt(9,999) <= 100 and if the leading digit is <5 we change it to 71
            for 5 digit number the guess is 320 since (int)sqrt(99,999) <= 320
            for 6 digit number the guess is 1000 since (int)sqrt(999,999) <= 1000 and if the leading digit is <5 we change it to 710
            for 7 digit number the guess is 3200 since (int)sqrt(9,999,999) <= 3200 
            for 8 digit number the guess is 10,000 since (int)sqrt(99,999,999) <= 10,000
            for 9 digit number the guess is 32,000 since (int)sqrt(999,999,999) <= 32,000 
            for 10 digit number the guess is 100,000 since (int)sqrt(9,999,999,999) <= 100,000
            for 11 digit number the guess is 320,000 since (int)sqrt(99,999,999,999) <= 320,000 
        */
        bigint* initial_guess_n;
        if(a->num_of_digits == 1){
            initial_guess_n = create_from_int(3);
        }
        else{
            if(a->num_of_digits % 2 == 0){
            initial_guess_n = create_one();
            int k = a->num_of_digits/2;
            __fast_shift_10x(initial_guess_n, k);
            }
            else{
                initial_guess_n = create_from_int(32);
                int k = (a->num_of_digits - 1)/2;
                __fast_shift_10x(initial_guess_n, k-1);
            }
        }


        /*
        char* guess_print = to_string(initial_guess_n);
        char* input_print = to_string(a);
        printf("number was: %s, initial guess was: %s\n", input_print, guess_print);
        free(input_print);
        free(guess_print);
        */
        
        bigint* cycle = create_one();
        bigint* intermediate = create_one();
        bigint* intermediate2 = create_one();
        //bigint* big_2 = create_from_int(2);
        int max_num_iterations_to_run = 50;
        // char* input_print = to_string(a);
        // printf("Number: %s\n", input_print);
        // free(input_print);
        int i;
        for(i = 0; i < max_num_iterations_to_run; i++) 
        {
            //this basically is next_guess = (initial_guess + a/initial_guess)/2
            bigint_div(intermediate, a, initial_guess_n);
            bigint_add(intermediate2, initial_guess_n, intermediate);
            //bigint_div(cycle, intermediate2, big_2);
            __fast_div_2(cycle, intermediate2);

            //if we already converged or (overshot) then quit
            if(bigint_cmp(initial_guess_n, cycle) == 0 || (bigint_cmp(initial_guess_n, cycle) < 0 && i > 0))
                break;

            bigint* switcheroo = initial_guess_n;
            initial_guess_n = cycle;
            cycle = switcheroo;
            // char* printout = to_string(initial_guess_n);
            // printf("    %s  <--guess   ...   iteration-->  %d\n", printout, i);
            // free(printout);
        }
        // printf("we did %d iterations\n", i);
        // printf("\n");
        bigint_copy(result, initial_guess_n);
        destroy(initial_guess_n);
        destroy(cycle);
        destroy(intermediate);
        destroy(intermediate2);
        //destroy(big_2);
    }
}

/* More complex math operations */
void bigint_factorial(bigint* result, bigint* a){       /* FIN */
    if(a->sign == NEGATIVE){
        printf("error: unable to compute negative factorials\n");
    }
    else{
        if(int_cmp(a, 0) == 0 || int_cmp(a, 1) == 0){
            result->data[0] = 1;
            result->num_of_digits = 1;
            result->sign = POSITIVE;
        }
        else{
            bigint* temp_result = create_zero();
            bigint* intermediary = create_one();
            bigint* start = create_zero();
            bigint_copy(start, a);

            while(int_cmp_abs(start, 1) > 0){
                bigint_mul(temp_result, intermediary, start);
                bigint* floater = intermediary;
                intermediary = temp_result;
                temp_result = floater;
                bigint_dec(start);
            }
            bigint_copy(result, intermediary);
            destroy(intermediary);
            destroy(temp_result);
            destroy(start);
        }
    }
}
void bigint_n_choose_k(bigint* result, bigint* n, bigint* k){
    bigint* retval = create_zero();

    bigint* n_factorial = create_zero();
    bigint_factorial(n_factorial, n);

    bigint* k_factorial = create_zero();
    bigint_factorial(k_factorial, k);

    bigint* n_minus_k = create_zero();
    bigint_sub(n_minus_k, n, k);
    bigint* n_minus_k_factorial = create_zero();
    bigint_factorial(n_minus_k_factorial, n_minus_k);
    destroy(n_minus_k);

    bigint* denom = create_zero();
    bigint_mul(denom, n_minus_k_factorial, k_factorial);
    destroy(k_factorial);
    destroy(n_minus_k_factorial);

    bigint_div(retval, n_factorial, denom);
    destroy(denom);
    destroy(n_factorial);
}
void bigint_random(bigint* result, bigint* lower_bound, bigint* upper_bound){
    bigint* retval;

}

/* Special operators and comparison */
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
void bigint_inc(bigint* n){                        /* FIN */
    if(n->sign == POSITIVE){
        if(n->data[0] == 9){ //this means we will possibly need another digit and have to go through the additon routine
            bigint* addon = create_one();
            bigint* old_val = create_zero();
            bigint_copy(old_val, n);
            bigint_add(n, old_val, addon);
        }
        else{
            n->data[0] += 1;
        }
    }
    else{
        if(n->data[0] == 0){ //this means we will have to possibly reduce the number of digits and go through the subtraction routine
            bigint* addon = create_one();
            bigint* old_val = create_zero();
            bigint_copy(old_val, n);
            bigint_add(n, old_val, addon);
        }
        else{
            n->data[0] -= 1;
        }
    }
}
void bigint_dec(bigint* n){                        /* FIN */
    if(n->sign == NEGATIVE){
        if(n->data[0] == 9){ //this means we will possibly need another digit and have to go through the additon routine
            bigint* addon = create_one();
            bigint* old_val = create_zero();
            addon->sign = NEGATIVE;
            bigint_copy(old_val, n);
            bigint_add(n, old_val, addon);
        }
        else{
            n->data[0] += 1;
        }
    }
    else{
        if(n->data[0] == 0){ //this means we will have to possibly reduce the number of digits and go through the subtraction routine
            bigint* addon = create_one();
            bigint* old_val = create_zero();
            addon->sign = NEGATIVE;
            bigint_copy(old_val, n);
            bigint_add(n, old_val, addon);
        }
        else{
            n->data[0] -= 1;
        }
    }
}
void bigint_copy(bigint* dst, bigint* src){        /* FIN */
    __internal_make_correct_digit_allocation(dst, src->num_of_digits);
    dst->sign = src->sign;
    dst->num_of_digits = src->num_of_digits;
    __internal_memcpy(dst->data, src->data, dst->num_of_digits); //memcpy is very fast, unfortunetly it uses the string.h lib which i would prefer not to use
}

//============================================================================
// Internal Library Functions
//============================================================================
boolean __internal_make_correct_digit_allocation(bigint* num, int num_digits_needed){
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
void __fast_shift_10(bigint* num){
    if(!bigint_is_zero(num)){
        __internal_make_correct_digit_allocation(num, num->num_of_digits+1);
        for(int i = num->num_of_digits - 1; i >= 0; i--){
            num->data[i+1] = num->data[i];
        }
        num->data[0] = 0;
        num->num_of_digits++;
    }
}
void __fast_shift_10x(bigint* num, unsigned int places_to_shift){
    if(!bigint_is_zero(num)){
        __internal_make_correct_digit_allocation(num, num->num_of_digits+places_to_shift);
        for(int i = num->num_of_digits - 1; i >= 0; i--){
            num->data[i+places_to_shift] = num->data[i];
        }
        for(int i = 0; i < places_to_shift; i++){
            num->data[i] = 0;
        }
        num->num_of_digits += places_to_shift;
    }
}
void __fast_div_10(bigint* num){
    if(num->num_of_digits == 1){
        num->data[0] = 0;
        num->num_of_digits = 1;
        num->sign = POSITIVE;
    }
    else{
        for(int i = 0; i < num->num_of_digits - 1; i++){
            num->data[i] = num->data[i+1];
        }
        num->num_of_digits--;
    }
}
void __fast_div_10x(bigint* num, unsigned int places_to_remove){
    if(num->num_of_digits <= places_to_remove){
        num->data[0] = 0;
        num->num_of_digits = 1;
        num->sign = POSITIVE;
    }
    else{
        for(int i = 0; i < num->num_of_digits - places_to_remove; i++){
            num->data[i] = num->data[i+places_to_remove];
        }
        num->num_of_digits -= places_to_remove;
    }
}
bigint* __create_bigint_zero_of_size(unsigned int size){
    int realsz = (size > 0) ? size : 2;
    bigint* retval = malloc(sizeof(bigint));
    retval->num_of_digits = 1;
    retval->num_allocated = realsz;
    retval->data = malloc(retval->num_allocated*sizeof(byte));
    retval->sign = POSITIVE;
    retval->data[0] = 0;
    return retval;
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
void __internal_memcpy(void* dest, const void* source, int size){
    char* a = (char*)dest;
    char* b = (char*)source;
    while(size--)
        *a++ = *b++;
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
void __fast_div_2(bigint* result, bigint* num){
    bigint* big_5 = create_from_int(5);
    bigint_mul(result, num, big_5);
    __fast_div_10(result);
}
void __smallnum_internal_divmod(bigint* result, bigint* rem, bigint* a, bigint* b){
    int test_condition = bigint_cmp_abs(a, b);
    if(bigint_is_zero(b)){
        printf("error: cannot divide by zero\n");
        result->data[0] = 0;
        result->num_of_digits = 1;
        result->sign = POSITIVE;
    }
    else if(test_condition == 0){
        result->data[0] = 1;
        result->num_of_digits = 1;
        result->sign = POSITIVE;//result->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;
        rem->data[0] = 0;
        rem->num_of_digits = 1;
        rem->sign = POSITIVE;
    }
    else if(test_condition < 0){
        bigint_copy(rem, a);
        result->data[0] = 0;
        result->num_of_digits = 1;
        result->sign = POSITIVE;
    }
    else{
        bigint* working_copy_of_this = create_zero();
        bigint* switcheroo = create_zero();
        bigint* tmp;
        bigint* abs_b = create_zero();
        bigint_copy(abs_b, b);
        abs_b->sign = POSITIVE;
        bigint_copy(working_copy_of_this, a);
        working_copy_of_this->sign = POSITIVE;
        result->data[0] = 0;
        result->num_of_digits = 1;
        result->sign = POSITIVE;
        while(int_cmp(working_copy_of_this, 0) >= 0){
            bigint_sub(switcheroo, working_copy_of_this, abs_b);
            if(switcheroo->sign == NEGATIVE){
                break;
            }
            tmp = working_copy_of_this;
            working_copy_of_this = switcheroo;
            switcheroo = tmp;
            bigint_inc(result);
        }
        bigint_copy(rem, working_copy_of_this);
        destroy(switcheroo);
        destroy(working_copy_of_this);
        destroy(abs_b);
        result->sign = POSITIVE;//result->sign = (a->sign == b->sign) ? POSITIVE : NEGATIVE;
    }
}
// end of file