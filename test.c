#include "bigint.h"
#include <math.h>
#include <time.h>
/*
    this testing program should also be able to give results on the speed and performance of the library
*/
//#define START(NAME_OF_THING_TO_TIME) clock_t NAME_OF_THING_TO_TIME = clock()
//#define STOP(NAME_OF_THING_TO_TIME) printf("the function %s took: %g msec.\n", (#NAME_OF_THING_TO_TIME), 1000.0 * ((double)(clock() - (NAME_OF_THING_TO_TIME)) / CLOCKS_PER_SEC))

#define CHECK(exp) \
    ((exp) ? (void)0 : printf("line " BOLDRED "%d" RESET " in file " BOLDGREEN "%s" RESET " expression " BOLDCYAN "%s" RESET " was false\n", __LINE__, __FILE__, #exp))
#define INFO(msg) \
    printf("line: %d in file: %s %s", __LINE__, __FILE__, #msg);
//#define VERBOSE_TESTING

boolean test_create_zero(double* amount_of_time){                 /* FIN */
    const int number_of_tests = 1000;
    boolean test_result = FALSE;
    bigint* test_num[number_of_tests];
    clock_t START = clock();
    for(int i = 0; i < number_of_tests; i++){
        test_num[i] = create_zero();
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < number_of_tests; i++){
        if(!test_num[i])
            return FALSE;
        test_result = (test_num[i]->sign == POSITIVE) ? TRUE : FALSE;
        if(!test_num[i]->data){
            free(test_num[i]);
            return FALSE;
        }
        test_result = (test_num[i]->data[0] == 0) ? TRUE : FALSE;
        test_result = (test_num[i]->num_of_digits == 1) ? TRUE : FALSE;
        test_result = (test_num[i]->num_allocated >= 1) ? TRUE : FALSE;
        #ifdef VERBOSE_TESTING
            __obj_details(test_num[i], __LINE__, __FILE__);
        #endif
        free(test_num[i]->data);
        free(test_num[i]);
    }
    return test_result;
}				
boolean test_create_one(double* amount_of_time){                  /* FIN */
    const int number_of_tests = 1000;
    boolean test_result = FALSE;
    bigint* test_num[number_of_tests];
    clock_t START = clock();
    for(int i = 0; i < number_of_tests; i++){
        test_num[i] = create_one();
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < number_of_tests; i++){
        if(!test_num[i])
            return FALSE;
        test_result = (test_num[i]->sign == POSITIVE) ? TRUE : FALSE;
        if(!test_num[i]->data){
            free(test_num[i]);
            return FALSE;
        }
        test_result = (test_num[i]->data[0] == 1) ? TRUE : FALSE;
        test_result = (test_num[i]->num_of_digits == 1) ? TRUE : FALSE;
        test_result = (test_num[i]->num_allocated >= 1) ? TRUE : FALSE;
        #ifdef VERBOSE_TESTING
            __obj_details(test_num[i], __LINE__, __FILE__);
        #endif
        free(test_num[i]->data);
        free(test_num[i]);
    }
    return test_result;
}					
boolean test_create_from_int(double* amount_of_time){             /* FIN */
    const int size_of_test = 1000;
    boolean test_result = TRUE;
    bigint* test_num[size_of_test];
    int test_int[size_of_test];
    for(int i = 0; i < size_of_test; i++){
        test_int[i] = rand();
        test_int[i] *= (test_int[i] % 2 == 1) ? -1 : 1;
    }
    clock_t START = clock();
    for(int i = 0; i < size_of_test; i++){
        test_num[i] = create_from_int(test_int[i]);
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
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
boolean test_create_from_string(double* amount_of_time){          /* FIN */
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    char* test_cases[] = {"0", "1", "-1", "10", "123456", "-10000", "2130000000000000909", "-2130000000000000908"};
    int num_tests_per_loop = sizeof(test_cases)/sizeof(test_cases[0]);
    bigint* nums[num_of_tests];
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        nums[i] = create_from_string(test_cases[i%num_tests_per_loop]);
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        char* numstr = to_string(nums[i]);
        #ifdef VERBOSE_TESTING
            printf("This string: %s produced the number: %s", test_cases[i%num_tests_per_loop], numstr);
            __obj_details(nums[i], __LINE__, __FILE__);
        #endif
        if(__strcmp(numstr, test_cases[i%num_tests_per_loop]) != 0)
            retval = FALSE;
        free(numstr);
        destroy(nums[i]);
    }
    return retval;
}
boolean test_to_int(double* amount_of_time){                      /* FIN */
    boolean retval = TRUE;
    const int number_of_tests = 1000;
    int numbers_to_test_with[number_of_tests];
    bigint* bigint_numbers[number_of_tests];
    int numbers_to_compare[number_of_tests];
    for(int i = 0; i < number_of_tests; i++){
        numbers_to_test_with[i] = rand();
        bigint_numbers[i] = create_from_int(numbers_to_test_with[i]);
    }
    clock_t START = clock();
    for(int i = 0; i < number_of_tests; i++){
        numbers_to_compare[i] = to_int(bigint_numbers[i]);
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < number_of_tests; i++){
        if(numbers_to_compare[i] != numbers_to_test_with[i]){
            printf("There was an issue, %d is what the number should be and %d is what to_int() produced\n", numbers_to_test_with[i], numbers_to_compare[i]);
            retval = FALSE;
        }
        #ifdef VERBOSE_TESTING
            __obj_details(bigint_numbers[i], __LINE__, __FILE__);
        #endif
        destroy(bigint_numbers[i]);
    }
    return retval;
}					
boolean test_to_string(double* amount_of_time){                   /* FIN */
    const int size_of_test = 1000;
    boolean test_result = TRUE;
    bigint* test_num[size_of_test];
    int test_int[size_of_test];
    char* result_to_compare[size_of_test];
    for(int i = 0; i < size_of_test; i++){
        test_int[i] = rand();
        test_num[i] = create_from_int(test_int[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(test_num[i], __LINE__, __FILE__);
        #endif
    }
    clock_t START = clock();
    for(int i = 0; i < size_of_test; i++){
        result_to_compare[i] = to_string(test_num[i]);
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < size_of_test; i++){
        int a = 0;
        for(int j = test_num[i]->num_of_digits - 1; j >= 0; j--){
            if(result_to_compare[i][a] != (test_num[i]->data[j] + 48)){
                printf("Test failed at line %d due to %c not equaling %c\n", __LINE__, result_to_compare[i][a], test_num[i]->data[j]);
                printf("The number is supposed to be: %d\n", test_int[i]);
                printf("The string generated by to_string was: %s\n", result_to_compare[i]);
                return FALSE;
            }
            a++;
        }
        if(result_to_compare[i][test_num[i]->num_of_digits] != '\0'){
            printf("Test failed at line %d due to %c not equaling null\n", __LINE__, result_to_compare[i][test_num[i]->num_of_digits]);
            printf("The number is supposed to be: %d\n", test_int[i]);
            return FALSE;
        }
        free(result_to_compare[i]);
        destroy(test_num[i]);
    }
    return test_result;
}				
boolean test_destroy(double* amount_of_time){                     /* FIN */
    boolean test_result =  FALSE;
    const int number_of_tests = 1000;
    bigint* a_num[number_of_tests];
    clock_t START = clock();
    for(int i = 0; i < number_of_tests; i++){
        a_num[i] = create_one();
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < number_of_tests; i++){
        #ifdef VERBOSE_TESTING
            __obj_details(a_num[i], __LINE__, __FILE__);
        #endif
        test_result = destroy(a_num[i]);
    }
    return test_result;
}				
boolean test_bigint_add(double* amount_of_time){                  /* FIN */
    boolean retval = TRUE;
    const int number_of_tests = 1000;
    int numbers_to_test_with[number_of_tests];
    bigint* bigint_numbers[number_of_tests];
    bigint* sums[number_of_tests];
    for(int i = 0; i < number_of_tests; i++){
        numbers_to_test_with[i] = (rand() / 3);   //make sure that we don't overflow the int
        numbers_to_test_with[i] *= (numbers_to_test_with[i] % 2 == 1) ? -1 : 1;
        bigint_numbers[i] = create_from_int(numbers_to_test_with[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(bigint_numbers[i], __LINE__, __FILE__);
        #endif
        sums[i] = create_zero();
    }
    clock_t START = clock();
    for(int i = 0; i < number_of_tests; i++){
        int first, second;
        first = i;
        second = (i == number_of_tests - 1) ? i : i+1;
        bigint_add(sums[i], bigint_numbers[first], bigint_numbers[second]);
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < number_of_tests; i++){
        int first, second;
        first = i;
        second = (i == number_of_tests - 1) ? i : i+1;
        #ifdef VERBOSE_TESTING
            printf("the result of: %d plus: %d is: %d\n", 
            numbers_to_test_with[first], 
            numbers_to_test_with[second],
            numbers_to_test_with[first] + numbers_to_test_with[second]);
            printf("we got: %d\n", to_int(sums[i]));
            __obj_details(sums[i], __LINE__, __FILE__);
        #endif
        if(to_int(sums[i]) != numbers_to_test_with[first] + numbers_to_test_with[second])
            retval = FALSE;
        destroy(sums[i]);
        destroy(bigint_numbers[i]);
    }
    return retval;
} 
boolean test_bigint_sub(double* amount_of_time){                  /* FIN */
    boolean retval = TRUE;
    const int number_of_tests = 1000;
    int numbers_to_test_with[number_of_tests];
    bigint* bigint_numbers[number_of_tests];
    bigint* diffs[number_of_tests];
    for(int i = 0; i < number_of_tests; i++){
        numbers_to_test_with[i] = (rand() / 2);   //make sure that we don't overflow the int
        numbers_to_test_with[i] *= (numbers_to_test_with[i] % 2 == 1) ? -1 : 1;
        bigint_numbers[i] = create_from_int(numbers_to_test_with[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(bigint_numbers[i], __LINE__, __FILE__);
        #endif
        diffs[i] = create_zero();
    }
    clock_t START = clock();
    for(int i = 0; i < number_of_tests; i++){
        int first, second;
        first = i;
        second = (i == number_of_tests - 1) ? i : i+1;
        bigint_sub(diffs[i], bigint_numbers[first], bigint_numbers[second]);
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < number_of_tests; i++){
        int first, second;
        first = i;
        second = (i == number_of_tests - 1) ? i : i+1;
        #ifdef VERBOSE_TESTING
            printf("the result of: %d minus: %d is: %d\n", 
            numbers_to_test_with[first], 
            numbers_to_test_with[second],
            numbers_to_test_with[first] - numbers_to_test_with[second]);
            printf("we got: %d\n", to_int(diffs[i]));
            __obj_details(diffs[i], __LINE__, __FILE__);
        #endif
        if(to_int(diffs[i]) != numbers_to_test_with[first] - numbers_to_test_with[second])
            retval = FALSE;
        destroy(diffs[i]);
        destroy(bigint_numbers[i]);
    }
    return retval;
} 
boolean test_bigint_mul(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
} 
boolean test_bigint_div(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
} 
boolean test_bigint_mod(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
} 
boolean test_bigint_divmod(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
} 
boolean test_bigint_pow(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
} 
boolean test_bigint_sqr(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
}			  
boolean test_bigint_cube(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
}		   	  
boolean test_bigint_isqrt(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
}          
boolean test_bigint_factorial(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
}	  
boolean test_bigint_n_choose_k(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
} 
boolean test_bigint_random(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
}
boolean test_bigint_cmp(double* amount_of_time){                  /* FIN */
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    bigint* nums[num_of_tests];
    int check_numbers[num_of_tests];
    for(int i = 0; i < num_of_tests; i++){
        check_numbers[i] = rand();
        check_numbers[i] *= (check_numbers[i] % 3 == 0) ? -1 : 1;
        if(i == 0 || i == 1){
            check_numbers[i] = 10 * ((i == 0) ? -1 : 1);
        }
        nums[i] = create_from_int(check_numbers[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(nums[i], __LINE__, __FILE__);
        #endif
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        int real_result = (check_numbers[i] == check_numbers[(i+1)%num_of_tests]) ? 0 : 
            (check_numbers[i] > check_numbers[(i+1)%num_of_tests]) ? 1 : -1;
        if(bigint_cmp(nums[i], nums[(i+1)%num_of_tests]) != real_result)
            retval = FALSE;
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        destroy(nums[i]);
    }
    return retval;
}
boolean test_bigint_cmp_abs(double* amount_of_time){              /* FIN */
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    bigint* nums[num_of_tests];
    int check_numbers[num_of_tests];
    for(int i = 0; i < num_of_tests; i++){
        check_numbers[i] = rand();
        check_numbers[i] *= (check_numbers[i] % 3 == 0) ? -1 : 1;
        if(i == 0 || i == 1){
            check_numbers[i] = 10 * ((i == 0) ? -1 : 1);
        }
        nums[i] = create_from_int(check_numbers[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(nums[i], __LINE__, __FILE__);
        #endif
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        int real_result = (abs(check_numbers[i]) == abs(check_numbers[(i+1)%num_of_tests])) ? 0 : 
            (abs(check_numbers[i]) > abs(check_numbers[(i+1)%num_of_tests])) ? 1 : -1;
        if(bigint_cmp_abs(nums[i], nums[(i+1)%num_of_tests]) != real_result)
            retval = FALSE;
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        destroy(nums[i]);
    }
    return retval;
}     	
boolean test_int_cmp(double* amount_of_time){                     /* FIN */
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    bigint* nums[num_of_tests];
    int check_numbers[num_of_tests];
    for(int i = 0; i < num_of_tests; i++){
        check_numbers[i] = rand();
        check_numbers[i] *= (check_numbers[i] % 3 == 0) ? -1 : 1;
        if(i == 0 || i == 1){
            check_numbers[i] = 10 * ((i == 0) ? -1 : 1);
        }
        nums[i] = create_from_int(check_numbers[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(nums[i], __LINE__, __FILE__);
        #endif
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        int real_result = (check_numbers[i] == check_numbers[(i+1)%num_of_tests]) ? 0 : 
            (check_numbers[i] > check_numbers[(i+1)%num_of_tests]) ? 1 : -1;
        if(int_cmp(nums[i], check_numbers[(i+1)%num_of_tests]) != real_result){
            #ifdef VERBOSE_TESTING
                printf("the comparison of: %d and %d should yield: %d instead we got: %d\n", 
                to_int(nums[i]), check_numbers[(i+1)%num_of_tests], 
                real_result, int_cmp(nums[i], check_numbers[(i+1)%num_of_tests]));
            #endif
            retval = FALSE;
        }
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        destroy(nums[i]);
    }
    return retval;
}
boolean test_int_cmp_abs(double* amount_of_time){                 /* FIN */
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    bigint* nums[num_of_tests];
    int check_numbers[num_of_tests];
    for(int i = 0; i < num_of_tests; i++){
        check_numbers[i] = rand();
        check_numbers[i] *= (check_numbers[i] % 3 == 0) ? -1 : 1;
        if(i == 0 || i == 1){
            check_numbers[i] = 10 * ((i == 0) ? -1 : 1);
        }
        nums[i] = create_from_int(check_numbers[i]);
        #ifdef VERBOSE_TESTING
            __obj_details(nums[i], __LINE__, __FILE__);
        #endif
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        int real_result = (abs(check_numbers[i]) == abs(check_numbers[(i+1)%num_of_tests])) ? 0 : 
            (abs(check_numbers[i]) > abs(check_numbers[(i+1)%num_of_tests])) ? 1 : -1;
        if(int_cmp_abs(nums[i], check_numbers[(i+1)%num_of_tests]) != real_result){
            #ifdef VERBOSE_TESTING
                printf("the comparison of: %d and %d should yield: %d instead we got: %d\n", 
                to_int(nums[i]), check_numbers[(i+1)%num_of_tests], 
                real_result, int_cmp_abs(nums[i], check_numbers[(i+1)%num_of_tests]));
            #endif
            retval = FALSE;
        }
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        destroy(nums[i]);
    }
    return retval;
}
boolean test_bigint_is_zero(double* amount_of_time){              /* FIN */
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    bigint* nums[num_of_tests];
    for(int i = 0; i < num_of_tests; i++){
        if(i%3 == 0)
            nums[i] = create_zero();
        if(i%3 == 1)
            nums[i] = create_from_int(0);
        if(i%3 == 2)
            nums[i] = create_from_string("0");
        #ifdef VERBOSE_TESTING
            __obj_details(nums[i], __LINE__, __FILE__);
        #endif
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        if(!bigint_is_zero(nums[i]))
            retval = FALSE;
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        destroy(nums[i]);
    }
    return retval;
}                      
boolean test_bigint_inc(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
}                             
boolean test_bigint_dec(double* amount_of_time){
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    for(int i = 0; i < num_of_tests; i++){
        //initialize everything
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        //do the thing a bunch of times
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        //destroy(nums[i]);   //clean up after ourselves
    }
    return FALSE;//retval;
}                             
boolean test_bigint_copy(double* amount_of_time){                 /* FIN */
    boolean retval = TRUE;
    const int num_of_tests = 1000;
    bigint* sources[num_of_tests];
    bigint* dests[num_of_tests];
    for(int i = 0; i < num_of_tests; i++){
        sources[i] = create_from_int(rand());
        dests[i] = create_zero();
        #ifdef VERBOSE_TESTING
            __obj_details(sources[i], __LINE__, __FILE__);
        #endif
    }
    clock_t START = clock();
    for(int i = 0; i < num_of_tests; i++){
        bigint_copy(dests[i], sources[i]);
    }
    *amount_of_time = 1000.0 * ((double)(clock() - (START)) / CLOCKS_PER_SEC);
    for(int i = 0; i < num_of_tests; i++){
        if(to_int(sources[i]) != to_int(dests[i]))
            retval = FALSE;
        destroy(sources[i]);   //clean up after ourselves
        destroy(dests[i]);
    }
    return retval;
}


boolean test_everything(void){                                    /* FIN */
    boolean result_of_tests = TRUE;
    double function_speed = 0.0;
    printf("\nEntering the main testing system...\n");
    if(test_create_zero(&function_speed)){
        printf("Test of function: "BOLDWHITE"create_zero "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"create_zero "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;
    }
    if(test_create_one(&function_speed)){
        printf("Test of function: "BOLDWHITE"create_one "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"create_one "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;
    }
    if(test_create_from_int(&function_speed)){
        printf("Test of function: "BOLDWHITE"create_from_int "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"create_from_int "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;		
    }
    if(test_create_from_string(&function_speed)){
        printf("Test of function: "BOLDWHITE"create_from_string "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"create_from_string "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;   	
    }
    if(test_to_int(&function_speed)){
        printf("Test of function: "BOLDWHITE"to_int "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"to_int "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 					
    }
    if(test_to_string(&function_speed)){
        printf("Test of function: "BOLDWHITE"to_string "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"to_string "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  				
    }
    if(test_destroy(&function_speed)){
        printf("Test of function: "BOLDWHITE"destroy "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"destroy "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;			
    }
    if(test_bigint_add(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_add "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_add "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_sub(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_sub "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_sub "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_mul(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_mul "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_mul "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_div(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_div "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_div "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  
    }
    if(test_bigint_mod(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_mod "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_mod "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  
    }
    if(test_bigint_divmod(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_divmod "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_divmod "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;   
    }
    if(test_bigint_pow(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_pow "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_pow "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_sqr(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_sqr "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_sqr "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;			  
    }
    if(test_bigint_cube(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_cube "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_cube "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;	   	  
    }
    if(test_bigint_isqrt(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_isqrt "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_isqrt "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;      
    }
    if(test_bigint_factorial(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_factorial "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_factorial "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;  
    }
    if(test_bigint_n_choose_k(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_n_choose_k "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_n_choose_k "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_random(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_random "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_random "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE; 
    }
    if(test_bigint_cmp(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_cmp "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_cmp "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;        
    }
    if(test_bigint_cmp_abs(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_cmp_abs "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_cmp_abs "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;      	
    }
    if(test_int_cmp(&function_speed)){
        printf("Test of function: "BOLDWHITE"int_cmp "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"int_cmp "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;        
    }
    if(test_int_cmp_abs(&function_speed)){
        printf("Test of function: "BOLDWHITE"int_cmp_abs "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"int_cmp_abs "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;      	
    }
    if(test_bigint_is_zero(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_is_zero "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_is_zero "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;                         
    }
    if(test_bigint_inc(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_inc "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_inc "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;                                
    }
    if(test_bigint_dec(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_dec "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_dec "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;                               
    }
    if(test_bigint_copy(&function_speed)){
        printf("Test of function: "BOLDWHITE"bigint_copy "BOLDGREEN"PASSED"RESET" and took: %g msec\n", function_speed);
        function_speed = 0.0;
    }
    else{
        printf("Test of function: "BOLDWHITE"bigint_copy "BOLDRED"FAILED\n"RESET);
        result_of_tests = FALSE;
    }
    printf("ALL TESTS RESULT: %s\n\n", (result_of_tests == TRUE) ? BOLDGREEN"PASSED"RESET : BOLDRED"FAILED"RESET);
    return result_of_tests;
}


int main(){
    test_everything();
    return 0;
}