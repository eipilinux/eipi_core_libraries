#include "bigint.h"


#define CHECK(exp) \
    ((exp) ? (void)0 : printf("line " BOLDRED "%d" RESET " in file " BOLDGREEN "%s" RESET " expression " BOLDCYAN "%s" RESET " was false\n", __LINE__, __FILE__, #exp))
#define INFO(msg) \
    printf("line: %d in file: %s %s", __LINE__, __FILE__, #msg);
//#define VERBOSE_TESTING

boolean test_create_zero(void){
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
boolean test_create_one(void){
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
boolean test_create_from_int(void){
    const int size_of_test = 10;
    boolean test_result = TRUE;
    bigint* test_num[size_of_test];
    for(int i = 0; i < size_of_test; i++){
       test_num[i] = create_from_int(rand());
    }
    for(int i = 0; i < size_of_test; i++){
        if(!test_num[i])
            return FALSE;
        if(!test_num[i]->data){
            free(test_num[i]);
            return FALSE;
        }
        #ifdef VERBOSE_TESTING
            __obj_details(test_num[i], __LINE__, __FILE__);
        #endif
        free(test_num[i]->data);
        free(test_num[i]);
    }
    return test_result;
}			
boolean test_create_from_string(void){
    return FALSE;
}	
boolean test_to_int(void){
    return FALSE;
}					
boolean test_to_string(void){
    return FALSE;
}				
boolean test_destroy(void){
    return FALSE;
}				
boolean test_bigint_add(void){
    return FALSE;
} 
boolean test_bigint_sub(void){
    return FALSE;
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
boolean test_bigint_is_zero(void){
    return FALSE;
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

boolean test_everything(void){
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
    test_everything();
    return 0;
}
