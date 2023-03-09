#include "bigint.h"

bigint* create_zero(void){
    bigint* retval = malloc(sizeof(bigint));
    retval->num_of_digits = 1;
    retval->num_allocated = 2*retval->num_of_digits+1;
    retval->data = malloc(retval->num_allocated*sizeof(byte));
    retval->sign = POSITIVE;
    retval->data[0] = 0;
    return retval;
}
bigint* create_one(void){
    bigint* retval = create_zero();
    retval->data[0] = 1;
    return retval;
}
bigint* create_from_int(int input){
    int stepwise_remainder = abs(input), iter = 0;
    bigint* retval = malloc(sizeof(bigint));
    retval->sign = (input >= 0) ? POSITIVE : NEGATIVE;
    retval->num_of_digits = (unsigned int)log10((double)stepwise_remainder) + 1;
    retval->num_allocated = 2*retval->num_of_digits+1;
    retval->data = malloc(retval->num_allocated*sizeof(byte));
    do{
        retval->data[iter] = stepwise_remainder % 10;
        stepwise_remainder /= 10;
        iter++;
    }while(stepwise_remainder > 0);
    return retval;
}
bigint* create_from_string(char* input){
    bigint* retval = malloc(sizeof(bigint));
    retval->sign = (input[0] == '-') ? NEGATIVE : POSITIVE;
    return retval;
}
int to_int(bigint* num){
    return 1;
}
char* to_string(bigint* num){
    int strlength = num->num_of_digits + ((num->sign == POSITIVE) ? 0 : 1);
    char* retstr = malloc(strlength);
    retstr[0] = (num->sign == POSITIVE) ? ' ' : '-';
    for(int i = 0; i < num->num_of_digits; i++){
        retstr[strlength - i - 1] = (char)(48 + num->data[i]);
    }
    return retstr;
}
boolean destroy(bigint* num){
    if(num->num_allocated > 0)
        free(num->data);
    free(num);
    return TRUE;
}
void bigint_add(bigint* result, bigint* a, bigint* b){
    __internal_make_correct_digit_allocation(result, MAX(a->num_of_digits, b->num_of_digits) + 1);
    byte carry_over = 0;
    int i;
    for(i = 0; i < MAX(a->num_of_digits, b->num_of_digits) + 1; i++){
        result->data[i] = ((a->num_of_digits > i) ? a->data[i] : 0) + ((b->num_of_digits > i) ? b->data[i] : 0) + carry_over;
        carry_over = (result->data[i] > 9) ? result->data[i] - 9 : 0;
        result->data[i] %= 10;
    }
    result->num_of_digits = (result->data[i] > 0) ? i : i-1;
    result->sign = a->sign;
}
void bigint_sub(bigint* result, bigint* a, bigint* b){

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
int bigint_cmp(bigint* a, bigint* b){
    if(a->sign != b->sign)
        return (a->sign > b->sign) ? 1 : -1;
    if(a->sign)
        return bigint_cmp_abs(a, b);
    return bigint_cmp_abs(b, a);
}
int bigint_cmp_abs(bigint* a, bigint* b){
    if(a->num_of_digits != b->num_of_digits)
        return (a->num_of_digits > b->num_of_digits) ? 1 : -1;
    for(int i = a->num_of_digits - 1; i >= 0; i--){
        if(a->data[i] != b->data[i])
            return (a->data[i] > b->data[i]) ? 1 : -1;
    }
    return 0;
}
boolean bigint_is_zero(bigint* n){
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
    bigint* retval;
}
boolean __internal_make_correct_digit_allocation(bigint* num, int num_digits_needed){
    if(num->num_allocated >= num_digits_needed)
        return TRUE;
    byte* tmp = realloc(num->data, 2*num_digits_needed);
    if (tmp == NULL)
    {
        printf("allocation failed"); //i should really implement a better error function to report errors to a file
        return FALSE;
    }
    num->data = tmp;
    num->num_allocated = 2*num_digits_needed;
    return TRUE;
}
void __positive_difference(bigint* diff, bigint* a, bigint* b){
    bigint* larger;
    bigint* smaller;
    larger = (bigint_cmp_abs(a, b) > 0) ? a : b;
    smaller = (bigint_cmp_abs(a, b) > 0) ? b : a;
    __internal_make_correct_digit_allocation(diff, MAX(larger->num_of_digits, smaller->num_of_digits));
    byte borrow_over = 0;
    int i;
    int tmp_plc_data = 0;
    diff->num_of_digits = 0;
    for(i = 0; i < MAX(larger->num_of_digits, smaller->num_of_digits); i++){
        tmp_plc_data = (int)(((larger->num_of_digits > i) ? larger->data[i] : 0) - ((smaller->num_of_digits > i) ? smaller->data[i] : 0) - borrow_over);
        borrow_over = (tmp_plc_data < 0) ? 1 : 0;
        diff->data[i] = (tmp_plc_data < 0) ? tmp_plc_data+10 : tmp_plc_data;
        tmp_plc_data = 0;
        diff->num_of_digits = (diff->data[i] > 0) ? i : diff->num_of_digits;
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
        printf("    The object "BOLDCYAN"sign"RESET" is: "BOLDYELLOW"%s\n"RESET, (obj->sign > 0) ? "POSITIVE" : "NEGATIVE");
        printf("    The object "BOLDCYAN"num_of_digits"RESET" is: "BOLDYELLOW"%d\n"RESET, obj->num_of_digits);
        printf("    The object "BOLDCYAN"num_allocated"RESET" is: "BOLDYELLOW"%d\n"RESET, obj->num_allocated);
        printf("    The object "BOLDCYAN"data"RESET" holds the values: "BOLDYELLOW);
        for(int i = 0; i < obj->num_of_digits; i++){
            printf("%d ", obj->data[i]);
        }
        printf(RESET"\n");
    }
    printf("\n");
}