#include "bigint.h"
#include <math.h>
/*
    this testing program should also be able to give results on the speed and performance of the library
*/

#define CHECK(exp) \
    ((exp) ? (void)0 : printf("line " BOLDRED "%d" RESET " in file " BOLDGREEN "%s" RESET " expression " BOLDCYAN "%s" RESET " was false\n", __LINE__, __FILE__, #exp))
#define INFO(msg) \
    printf("line: %d in file: %s %s", __LINE__, __FILE__, #msg);
//#define VERBOSE_TESTING

boolean test_create_zero(void){             /* FIN */
    boolean test_result = FALSE;
    bigint* test_num = create_zero();
    if(!test_num)
        return FALSE;
    test_result = (test_num->sign == POSITIVE) ? TRUE : FALSE;
    if(!test_num->data){
        free(test_num);
        return FALSE;
    }
    test_result = (test_num->data[0] == 0) ? TRUE : FALSE;
    test_result = (test_num->num_of_digits == 1) ? TRUE : FALSE;
    test_result = (test_num->num_allocated >= 1) ? TRUE : FALSE;
    #ifdef VERBOSE_TESTING
        __obj_details(test_num, __LINE__, __FILE__);
    #endif
    free(test_num->data);
    free(test_num);
    return test_result;
}				
boolean test_create_one(void){              /* FIN */
    boolean test_result = FALSE;
    bigint* test_num = create_one();
    if(!test_num)
        return FALSE;
    test_result = (test_num->sign == POSITIVE) ? TRUE : FALSE;
    if(!test_num->data){
        free(test_num);
        return FALSE;
    }
    test_result = (test_num->data[0] == 1) ? TRUE : FALSE;
    test_result = (test_num->num_of_digits == 1) ? TRUE : FALSE;
    test_result = (test_num->num_allocated >= 1) ? TRUE : FALSE;
    #ifdef VERBOSE_TESTING
        __obj_details(test_num, __LINE__, __FILE__);
    #endif
    free(test_num->data);
    free(test_num);
    return test_result;
}					
boolean test_create_from_int(void){         /* FIN */
    const int size_of_test = 10;
    boolean test_result = TRUE;
    bigint* test_num[size_of_test];
    int test_int[size_of_test];
    for(int i = 0; i < size_of_test; i++){
        test_int[i] = rand();
        test_int[i] *= (test_int[i] % 2 == 1) ? -1 : 1;
        test_num[i] = create_from_int(test_int[i]);
    }
    for(int i = 0; i < size_of_test; i++){
        if(!test_num[i])
            return FALSE;
        if(!test_num[i]->data){
            free(test_num[i]);
            return FALSE;
        }
        #ifdef VERBOSE_TESTING
            printf("the actual number is: %d", test_int[i]);
            __obj_details(test_num[i], __LINE__, __FILE__);
        #endif
        int realnum_digits = (test_int[i] != 0) ? (int)(log10((double)abs(test_int[i]))+1) : 1;
        char printable[100];
        for(int j = 0; j < test_num[i]->num_of_digits; j++){
            printable[j] = test_num[i]->data[j];
        }
        if(test_num[i]->num_of_digits != realnum_digits){
            printf("number of digits was wrong: %d in the object: %d in reality\n"
                     "%s is object number %d is real number\n", 
                     test_num[i]->num_of_digits, 
                     realnum_digits, 
                     printable,
                     test_int[i]);
            return FALSE;
        }
        free(test_num[i]->data);
        free(test_num[i]);
    }
    return test_result;
}
boolean test_create_from_string(void){      /* FIN */
    boolean retval = TRUE;
    char* test_cases[] = {"0", "1", "-1", "10", "123456"};
    int num_tests = sizeof(test_cases)/sizeof(test_cases[0]);
    bigint* nums;
    for(int i = 0; i < num_tests; i++){
        nums = create_from_string(test_cases[i]);
        char* numstr = to_string(nums);
        #ifdef VERBOSE_TESTING
            printf("This string: %s produced the number: %s", test_cases[i], numstr);
            __obj_details(nums, __LINE__, __FILE__);
        #endif
        if(__strcmp(numstr, test_cases[i]) != 0)
            retval = FALSE;
        free(numstr);
        destroy(nums);
    }
    return retval;
}
boolean test_to_int(void){                  /* FIN */
    boolean retval = TRUE;
    const int number_of_tests = 10;
    int numbers_to_test_with[number_of_tests];
    bigint* bigint_numbers[number_of_tests];
    for(int i = 0; i < number_of_tests; i++){
        numbers_to_test_with[i] = rand();
        bigint_numbers[i] = create_from_int(numbers_to_test_with[i]);
        if(to_int(bigint_numbers[i]) != numbers_to_test_with[i]){
            printf("There was an issue, %d is what the number should be and %d is what to_int() produced\n", numbers_to_test_with[i], to_int(bigint_numbers[i]));
            retval = FALSE;
        }
        #ifdef VERBOSE_TESTING
            __obj_details(bigint_numbers[i], __LINE__, __FILE__);
        #endif
        destroy(bigint_numbers[i]);
    }
    return retval;
}					
boolean test_to_string(void){               /* FIN */
    const int size_of_test = 10;
    boolean test_result = TRUE;
    bigint* test_num[size_of_test];
    int test_int[size_of_test];
    for(int i = 0; i < size_of_test; i++){
        test_int[i] = rand();
        test_num[i] = create_from_int(test_int[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(test_num[i], __LINE__, __FILE__);
        #endif
    }
    for(int i = 0; i < size_of_test; i++){
        char* result_to_compare = to_string(test_num[i]);
        int a = 0;
        for(int j = test_num[i]->num_of_digits - 1; j >= 0; j--){
            if(result_to_compare[a] != (test_num[i]->data[j] + 48)){
                printf("Test failed at line %d due to %c not equaling %c\n", __LINE__, result_to_compare[a], test_num[i]->data[j]);
                printf("The number is supposed to be: %d\n", test_int[i]);
                printf("The string generated by to_string was: %s\n", result_to_compare);
                return FALSE;
            }
            a++;
        }
        if(result_to_compare[test_num[i]->num_of_digits] != '\0'){
            printf("Test failed at line %d due to %c not equaling null\n", __LINE__, result_to_compare[test_num[i]->num_of_digits]);
            printf("The number is supposed to be: %d\n", test_int[i]);
            return FALSE;
        }
        free(result_to_compare);
        destroy(test_num[i]);
    }
    return test_result;
}				
boolean test_destroy(void){                 /* FIN */
    boolean test_result =  FALSE;
    bigint* a_num = create_one();
    #ifdef VERBOSE_TESTING
        __obj_details(a_num, __LINE__, __FILE__);
    #endif
    test_result = destroy(a_num);
    return test_result;
}				
boolean test_bigint_add(void){              /* FIN */
    boolean retval = TRUE;
    const int number_of_tests = 10;
    int numbers_to_test_with[number_of_tests];
    bigint* bigint_numbers[number_of_tests];
    for(int i = 0; i < number_of_tests; i++){
        numbers_to_test_with[i] = (rand() / 3);   //make sure that we don't overflow the int
        numbers_to_test_with[i] *= (numbers_to_test_with[i] % 2 == 1) ? -1 : 1;
        bigint_numbers[i] = create_from_int(numbers_to_test_with[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(bigint_numbers[i], __LINE__, __FILE__);
        #endif
    }
    for(int i = 0; i < number_of_tests; i++){
        int first, second;
        first = i;
        second = (i == number_of_tests - 1) ? i : i+1;
        bigint* sum = create_zero();
        bigint_add(sum, bigint_numbers[first], bigint_numbers[second]);
        #ifdef VERBOSE_TESTING
            printf("the result of: %d plus: %d is: %d\n", 
            numbers_to_test_with[first], 
            numbers_to_test_with[second],
            numbers_to_test_with[first] + numbers_to_test_with[second]);
            printf("we got: %d\n", to_int(sum));
            __obj_details(sum, __LINE__, __FILE__);
        #endif
        if(to_int(sum) != numbers_to_test_with[first] + numbers_to_test_with[second])
            retval = FALSE;
        destroy(sum);
        destroy(bigint_numbers[i]);
    }
    return retval;
} 
boolean test_bigint_sub(void){
    boolean retval = TRUE;
    const int number_of_tests = 10;
    int numbers_to_test_with[number_of_tests];
    bigint* bigint_numbers[number_of_tests];
    for(int i = 0; i < number_of_tests; i++){
        numbers_to_test_with[i] = (rand() / 2);   //make sure that we don't overflow the int
        numbers_to_test_with[i] *= (numbers_to_test_with[i] % 2 == 1) ? -1 : 1;
        bigint_numbers[i] = create_from_int(numbers_to_test_with[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(bigint_numbers[i], __LINE__, __FILE__);
        #endif
    }
    for(int i = 0; i < number_of_tests; i++){
        int first, second;
        first = i;
        second = (i == number_of_tests - 1) ? i : i+1;
        bigint* sum = create_zero();
        bigint_sub(sum, bigint_numbers[first], bigint_numbers[second]);
        #ifdef VERBOSE_TESTING
            printf("the result of: %d minus: %d is: %d\n", 
            numbers_to_test_with[first], 
            numbers_to_test_with[second],
            numbers_to_test_with[first] - numbers_to_test_with[second]);
            printf("we got: %d\n", to_int(sum));
            __obj_details(sum, __LINE__, __FILE__);
        #endif
        if(to_int(sum) != numbers_to_test_with[first] - numbers_to_test_with[second])
            retval = FALSE;
        destroy(sum);
        destroy(bigint_numbers[i]);
    }
    return retval;
} 
boolean test_bigint_mul(void){
    return FALSE;
} 
boolean test_bigint_div(void){
    return FALSE;
} 
boolean test_bigint_mod(void){
    return FALSE;
} 
boolean test_bigint_divmod(void){
    return FALSE;
} 
boolean test_bigint_pow(void){
    return FALSE;
} 
boolean test_bigint_sqr(void){
    return FALSE;
}			  
boolean test_bigint_cube(void){
    return FALSE;
}		   	  
boolean test_bigint_isqrt(void){
    return FALSE;
}          
boolean test_bigint_factorial(void){
    return FALSE;
}	  
boolean test_bigint_n_choose_k(void){
    return FALSE;
} 
boolean test_bigint_cmp(void){
    return FALSE;
}
boolean test_bigint_cmp_abs(void){
    return FALSE;
}     	
boolean test_int_cmp(void){
    return FALSE;
}
boolean test_int_cmp_abs(void){
    return FALSE;
}
boolean test_bigint_is_zero(void){              /* FIN */
    boolean retval = TRUE;
    bigint* nums[3];
    nums[0] = create_zero();
    nums[1] = create_from_int(0);
    nums[2] = create_from_string("0"); //this may need testing
    for(int i = 0; i < 3; i++){
        #ifdef VERBOSE_TESTING
            __obj_details(nums[i], __LINE__, __FILE__);
        #endif
        if(!bigint_is_zero(nums[i]))
            retval = FALSE;
        destroy(nums[i]);
    }
    return retval;
}                      
boolean test_bigint_inc(void){
    return FALSE;
}                             
boolean test_bigint_dec(void){
    return FALSE;
}                             
boolean test_bigint_copy(void){
    return FALSE;
}


boolean test_everything(void){                  /* FIN */
    boolean result_of_tests = TRUE;
    printf("\nEntering the main testing system...\n");
    if(test_create_zero())
        printf("Test of function: "BOLDWHITE"create_zero "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"create_zero "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;
    }
    if(test_create_one())
        printf("Test of function: "BOLDWHITE"create_one "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"create_one "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;
    }
    if(test_create_from_int())
        printf("Test of function: "BOLDWHITE"create_from_int "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"create_from_int "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;		
    }
    if(test_create_from_string())
        printf("Test of function: "BOLDWHITE"create_from_string "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"create_from_string "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;   	
    }
    if(test_to_int())   
        printf("Test of function: "BOLDWHITE"to_int "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"to_int "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 					
    }
    if(test_to_string())  
        printf("Test of function: "BOLDWHITE"to_string "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"to_string "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  				
    }
    if(test_destroy())    	
        printf("Test of function: "BOLDWHITE"destroy "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"destroy "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;			
    }
    if(test_bigint_add())    
        printf("Test of function: "BOLDWHITE"bigint_add "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_add "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_sub())    
        printf("Test of function: "BOLDWHITE"bigint_sub "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_sub "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_mul())    
        printf("Test of function: "BOLDWHITE"bigint_mul "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_mul "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_div())   
        printf("Test of function: "BOLDWHITE"bigint_div "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_div "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  
    }
    if(test_bigint_mod())   
        printf("Test of function: "BOLDWHITE"bigint_mod "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_mod "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  
    }
    if(test_bigint_divmod())  
        printf("Test of function: "BOLDWHITE"bigint_divmod "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_divmod "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;   
    }
    if(test_bigint_pow())    
        printf("Test of function: "BOLDWHITE"bigint_pow "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_pow "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_sqr())    
        printf("Test of function: "BOLDWHITE"bigint_sqr "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_sqr "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;			  
    }
    if(test_bigint_cube())    	
        printf("Test of function: "BOLDWHITE"bigint_cube "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_cube "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;	   	  
    }
    if(test_bigint_isqrt())        
        printf("Test of function: "BOLDWHITE"bigint_isqrt "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_isqrt "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;      
    }
    if(test_bigint_factorial())    	
        printf("Test of function: "BOLDWHITE"bigint_factorial "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_factorial "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  
    }
    if(test_bigint_n_choose_k())    
        printf("Test of function: "BOLDWHITE"bigint_n_choose_k "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_n_choose_k "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_cmp())        
        printf("Test of function: "BOLDWHITE"bigint_cmp "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_cmp "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;        
    }
    if(test_bigint_cmp_abs())        
        printf("Test of function: "BOLDWHITE"bigint_cmp_abs "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_cmp_abs "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;      	
    }
    if(test_int_cmp())        
        printf("Test of function: "BOLDWHITE"int_cmp "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"int_cmp "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;        
    }
    if(test_int_cmp_abs())        
        printf("Test of function: "BOLDWHITE"int_cmp_abs "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"int_cmp_abs "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;      	
    }
    if(test_bigint_is_zero()) 
        printf("Test of function: "BOLDWHITE"bigint_is_zero "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_is_zero "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;                         
    }
    if(test_bigint_inc()) 
        printf("Test of function: "BOLDWHITE"bigint_inc "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_inc "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;                                
    }
    if(test_bigint_dec())  
        printf("Test of function: "BOLDWHITE"bigint_dec "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_dec "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;                               
    }
    if(test_bigint_copy()) 
        printf("Test of function: "BOLDWHITE"bigint_copy "BOLDGREEN"PASSED\n"RESET);
    else{
        printf("Test of function: "BOLDWHITE"bigint_copy "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;
    }
    printf("ALL TESTS RESULT: %s\n\n", (result_of_tests == TRUE) ? BOLDGREEN"PASSED"RESET : BOLDRED"FAILED"RESET);
    return result_of_tests;
}


int main(){
    // bigint* zero = create_zero();
    // char* printable = to_string(zero);
    // printf("The zero number is: %s\n", printable);
    // free(printable);
    // destroy(zero);
    // bigint* number = create_from_int(-102);
    // char* numprintable = to_string(number);
    // printf("The other number is: %s\nand data is: ", numprintable);
    // for(int i = 0; i<number->num_of_digits; i++){
    //     printf("%d", (int)number->data[i]);
    // }
    // printf("\n");
    // bigint* second = create_from_int(12);
    // char* printnum = to_string(second);
    // bigint* added = create_zero();
    // bigint_add(added, number, second);
    // char* printnum2 = to_string(added);
    // printf("%s plus %s is: %s\n", numprintable, printnum, printnum2);
    // printf("compare(12, -102): %d\n", bigint_cmp(second, number));
    // printf("abs_compare(12, -102): %d\n", bigint_cmp_abs(second, number));
    // __obj_details(second, __LINE__, __FILE__);
    // //CHECK(21+2==1);
    // free(numprintable);
    // free(printnum);
    // free(printnum2);
    // destroy(second);
    // destroy(added);
    // destroy(number);

    test_everything();

    return 0;
}