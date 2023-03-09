//add support for compression when writing to or reading from file,
// add support for encryption when writing/reading from file
//finish str to int/float and int/float to str functions
//add optimizations for str/strsub function
//consider adding an optimization for the calls made to malloc to be sent to a stack
// initialized array for smaller strings
#ifndef STRLIB
//just in case...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define STRLIB 1
typedef char BOOL;
#define TRUE 1
#define FALSE 0
#define OPTLVL 512
typedef struct I_string string;

struct I_string {
  BOOL alloc;
  BOOL opt;
  char *ptr;
  char optimization[OPTLVL]; //set to a size that below this size it is not efficent to allocate memory for every little string
  size_t leng;
  //fin
  void (*add)(string*, string*); //adds charachters to a string and resizes if not existant creates string
  //fin
  int (*strsub)(string*, string*, int, int); //returns starting location of string 2 in string 1
  //fin
  int (*str)(string*, string*); //returns location of first occurence of needle in haystack
  //fin
  int (*len)(string*); //returns length of string
  //fin
  void (*rsz)(string*, int); //resizes string to int charachters if smaller truncates from end of string if bigger adds '\0' to end of original string
  //fin
  void (*ins)(string*, string*, int, int);//inserts substring of string 2 into string 1 staring at int and length int
  //fin
  int (*chrsub)(string*, char, int, int); //returns location of first occurence of char in string
  //fin
  int (*chr)(string*, char);
  //fin
  BOOL (*cmp)(string*, string*); //compares strings if equal returns TRUE if not equal returns FALSE
  //fin
  void (*free)(string*); //frees the memory allocated for string
  //fin
  char* (*c)(string*); //returns a pointer to allocated memory containing c string with string contents, must be freed seperately
  //fin
  void (*print)(string*); //prints the string to stdout
  //fin
  void (*fprint)(string*, FILE*);
  //fin
  void (*println)(string*); //prints the string to stdout and appends a newline
  //fin
  void (*reset)(string*, char*);
  //
  int (*toint)(string*);
  //will only ever be effective up to 6 decimal places due to the precision of the double data type
  double (*tofloat)(string*);
};

//fin
char* I_c(string* s) //returns a pointer to a dynamically allocated charachter array with a copy of the string in it and the terminating null charachter appended
{
    if(s->alloc != FALSE)
    {
        char* c = malloc(s->leng + 1);
        memcpy(c, s->ptr, s->leng);
        c[s->leng] = '\0';
        return c;
    }
    else
    {
        return NULL;
        printf("error at string function c\n");
    }
}

//fin
void I_str_add(string* s, string* s2) //concatenates string 2 onto string 1 leaves string 2 unchainged
{
    if(s->alloc != FALSE && s2->alloc != FALSE)
    {
        if(s->leng + s2->leng >= OPTLVL && s->opt != TRUE)
        {
            s->ptr = realloc(s->ptr, s->leng + s2->leng);
            memcpy( (s->ptr + s->leng), s2->ptr, s2->leng);
            s->leng += s2->leng;
            s->opt = FALSE;
        }
        else if(s->leng + s2->leng >= OPTLVL && s->opt == TRUE)
        {
            s->opt = FALSE;
            s->ptr = NULL;
            s->ptr = malloc((s->leng + s2->leng));
            memcpy(s->ptr, s->optimization, s->leng);
            memcpy( (s->ptr + s->leng), s2->ptr, s2->leng);
            s->leng += s2->leng;
        }
        else if(s->leng + s2->leng < OPTLVL && s->opt == TRUE)
        {
            memcpy( (s->ptr + s->leng), s2->ptr, s2->leng);
            s->leng += s2->leng;
        }
    }
}

//fin
BOOL I_str_cmp(string* s1, string* s2) //compares the two strings if equal returns TRUE if not equal returns FALSE
{
    //return true if equal false if not equal
    if(s1->alloc == TRUE && s2->alloc == TRUE && s1->leng == s2->leng)
    {
        for(int i = 0; i < s1->leng; i++)
        {
            if(s1->ptr[i] != s2->ptr[i])
                return FALSE;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//fin
int I_str_str(string* haystack, string* needle) //returns the position of the first occurence of needle in haystack 
{
    //simply replace start with 0 and len with haystack->leng for regular strstr functionality
    if(haystack->alloc != FALSE && needle->leng <= haystack->leng)
    {
        int s2plc = 0;
        while(s2plc < haystack->leng && s2plc + needle->leng <= haystack->leng)
        {
            int i = 0;
            while(needle->ptr[i] == haystack->ptr[s2plc + i] && i < needle->leng)
                i++;
            if(i >= needle->leng)
            {
                return s2plc;
            }
            s2plc++;
        }
        printf("error finding substring 1\n");
        return -1;
    }
    else
    {
        printf("error finding substring 2\n");
        return -1;
    }
}

//fin
int I_str_strsub(string* haystack, string* needle, int start, int len) //returns the position of the first occurence after start continueing for len of needle in haystack 
{
    //simply replace start with 0 and len with haystack->leng for regular strstr functionality
    if(haystack->alloc != FALSE && needle->leng <= len && len < haystack->leng)
    {
        int s2plc = start;
        while(s2plc < len && s2plc + needle->leng <= len)
        {
            int i = 0;
            while(needle->ptr[i] == haystack->ptr[s2plc + i] && i < needle->leng)
                i++;
            if(i >= needle->leng)
            {
                return s2plc;
            }
            s2plc++;
        }
        printf("error finding substring 1\n");
        return -1;
    }
    else
    {
        printf("error finding substring 2\n");
        return -1;
    }
}

//fin
int I_str_len(string* s) //returns the length of string
{
    if(s->alloc != FALSE)
        return s->leng;
    else
    {
        printf("error getting length of string\n");
        return -1;
    }
}

//fin
void I_str_rsz(string* s1, int sz) //resizes the string to the size specified by sz operation preserves string contents
{
    if(s1->alloc != FALSE)
    {
        if(s1->leng != sz && sz >= OPTLVL && s1->leng >= OPTLVL)
        {
            //going to be bigger than optval and are rn
            char* tmp = malloc(s1->leng);
            memcpy(tmp, s1->ptr, s1->leng);
            s1->ptr = realloc(s1->ptr, sz);
            int nsz = (sz > s1->leng) ? s1->leng : sz;
            memset(s1->ptr, '\0', sz);
            memcpy(s1->ptr, tmp, nsz);
            free(tmp);
            s1->leng = sz;
        }
        else if(s1->leng != sz && sz >= OPTLVL && s1->leng < OPTLVL)
        {
            //going to be bigger than optlvl but arent rn
            s1->opt = FALSE;
            s1->ptr = malloc(sz);
            memcpy(s1->ptr, s1->optimization, s1->leng);
            s1->leng = sz;
        }
        else if(s1->leng != sz && sz < OPTLVL && s1->leng >= OPTLVL)
        {
            //going to be smaller than optval but arent rn
            s1->opt = TRUE;
            memcpy(s1->optimization, s1->ptr, sz);
            free(s1->ptr);
            s1->ptr = &(s1->optimization[0]);
            s1->leng = sz;
        }
        else if(s1->leng != sz && sz < OPTLVL && s1->leng < OPTLVL)
        {
            //going to be smaller than optval and are rn
            s1->leng = sz;
        }
    }
    else
    {
        printf("error resizing string\n");
    }
}

//fin
void I_str_ins(string* s1, string* s2, int start, int len) //inserts string 2 into string 1 at the position start continueing for length len
{
    if(s1->alloc != FALSE && start <= s1->leng && len != 0)
    {
        if(s1->leng + len >= OPTLVL)
        {
            if(s1->opt != TRUE)
            {
                char* tmp = malloc(s1->leng);
                memcpy(tmp, s1->ptr, s1->leng);
                if(realloc(s1->ptr, s1->leng + len) == NULL)
                    printf("error reallocating string\n");
                memcpy(s1->ptr, tmp, start);
                memcpy( (s1->ptr + start), s2->ptr, len);
                if(start != s1->leng)
                    memcpy( (s1->ptr + start + len), (tmp + start), (s1->leng - start));
                free(tmp);
                s1->leng += len;
            }
            else
            {
                s1->opt = FALSE;
                s1->ptr = malloc(s1->leng + len);
                memcpy(s1->ptr, s1->optimization, start);
                memcpy( (s1->ptr + start), s2->ptr, len);
                if(start != s1->leng)
                    memcpy( (s1->ptr + start + len), (s1->optimization + start), (s1->leng - start));
                s1->leng += len;
            }            
        }
        else
        {
            memcpy((s1->ptr + start), s2->ptr, len);
            if(start != s1->leng)
                memcpy((s1->ptr + start + len), (s1->ptr + start), (s1->leng - start));
            s1->leng += len;
        }
    }
    else
    {
        printf("error inserting string\n");
    }
}

//fin
int I_str_chr(string* s, char ch) //returns the integer position of first occurance of the charachter in the string
{
    if(s->alloc != FALSE)
    {
        for(int i = 0; i < s->leng; i++)
        {
            if(s->ptr[i] == ch)
            {
                return i;
            }
        }
        printf("charachter %c not in string\n", ch);
        return -1;
    }
    else
    {
        printf("error finding charachter\n");
        return -1;
    }
}

//fin
int I_str_chrsub(string* s, char ch, int start, int len) //returns the integer position of the charachter in the string after start continueing for len
{
    if(s->alloc != FALSE)
    {
        for(int i = 0; i + start < s->leng && i < len; i++)
        {
            if(s->ptr[i + start] == ch)
            {
                return i + start;
            }
        }
        printf("charachter %c not in string\n", ch);
        return -1;
    }
    else
    {
        printf("error finding charachter\n");
        return -1;
    }
}

//fin
void I_str_free(string* s) //frees the memory allocated by the string
{
    if(s->alloc != FALSE && s->opt == FALSE)
    {
        free(s->ptr);
        s->leng = 0;
        s->alloc = FALSE;
    }
    else
    {
        s->leng = 0;
        s->alloc = FALSE;
    }
    free(s);
    s = NULL;
}

//fin
void I_print(string* s1) //prints the string without the newline charachter
{
    for(int i = 0; i < s1->leng; i++)
    {
        printf("%c", s1->ptr[i]);
    }
}

//fin
void I_fprint(string* s1, FILE* fileptr) //writes string to fileptr fileptr must be opened with w, a, w+ or a+ for this to work
{
    char* dat = malloc(s1->leng + 1);
    memcpy(dat, s1->ptr, s1->leng);
    dat[s1->leng] = '\0';
    if(fputs(dat, fileptr))
        printf("successfully wrote %zu bytes to file\n", s1->leng);
    free(dat);
}

//fin
void I_println(string* s1) //prints the string with a newline charachter appended 
{
    for(int i = 0; i < s1->leng; i++)
    {
        printf("%c", s1->ptr[i]);
    }
    printf("\n");
}

//fin
void I_reset(string* s, char* n)
{
    if(s->alloc != FALSE && s->opt == FALSE)
    {
        free(s->ptr);
        s->leng = 0;
        s->alloc = FALSE;
    }
    else
    {
        s->leng = 0;
        s->alloc = FALSE;
    }
    s->alloc = TRUE;
    s->leng = strlen(n);
    if(s->leng < OPTLVL)
    {
        s->opt = TRUE;
        s->ptr = &s->optimization[0];
    }
    else
    {
        s->ptr = malloc(s->leng);
        s->opt = FALSE;
    }
    memcpy(s->ptr, n, s->leng);
}

//fin
int I_toint(string* s)
{
    int x = 0,ind = 0;
    BOOL neg = FALSE;
    if (s->ptr[ind] == '-') {
        neg = TRUE;
        ++ind;
    }
    while (s->ptr[ind] >= '0' && s->ptr[ind] <= '9') {
        x = (x*10) + (s->ptr[ind] - '0');
        ++ind;
    }
    if (neg) {
        x = -x;
    }
    return x;
}

//fin
double I_tofloat(string* s)
{
    double x = 0.0; 
    double factor = 10;
    int ind = 0;
    BOOL after = FALSE;
    BOOL neg = FALSE;
    if (s->ptr[ind] == '-') {
        neg = TRUE;
        ++ind;
    }
    
    while ( (s->ptr[ind] >= '0' && s->ptr[ind] <= '9') || s->ptr[ind] == '.' || s->ptr[ind] == ' ') {
        if(s->ptr[ind] == ' ')
        {
            ++ind;
            continue;
        }
        if(s->ptr[ind] != '.' && after == FALSE)
            x = (x*10) + (s->ptr[ind] - '0');
        if(s->ptr[ind] == '.' && after == FALSE)
        {
            ++ind;
            after = TRUE;
        }
        if(after == TRUE)
        {
            x += (double)(s->ptr[ind] - '0')/factor;
            factor *= 10;
        }
        ++ind;
    }
    //do some cleaning up by rounding to the number of decimal places in the origional number
    factor /= 10;
    x = (double)(int)(x * factor + 0.5) / factor; 
    if (neg) {
        x = -x;
    }
    return x;
}

//fin
void internal_set_fp(string* s)
{
    s->add = I_str_add;
    s->chr = I_str_chr; 
    s->chrsub = I_str_chrsub;
    s->free = I_str_free;
    s->ins = I_str_ins;
    s->len = I_str_len;
    s->rsz = I_str_rsz;
    s->str = I_str_str;
    s->strsub = I_str_strsub;
    s->c = I_c;
    s->cmp = I_str_cmp;
    s->print = I_print;
    s->fprint = I_fprint;
    s->println = I_println;
    s->reset = I_reset;
    s->toint = I_toint;
    s->tofloat = I_tofloat;
}

//fin
string* substr(string* s, int start, int sz) //returns substring starting at position start and of length sz
{
    if(s->alloc != FALSE)
    {
        string* sub = malloc(sizeof(string));
        sub->alloc = TRUE;
        sub->leng = sz;
        if(sub->leng < OPTLVL)
        {
            sub->opt = TRUE;
            sub->ptr = &sub->optimization[0];
        }
        else
        {
            sub->ptr = malloc(sub->leng);
            sub->opt = FALSE;
        }
        memcpy(sub->ptr, (s->ptr + start), sz);
        internal_set_fp(sub);
        return sub;
    }
    else
    {
        string* is = malloc(sizeof(string));
        printf("error getting substring\n");
        return is;
    }
}

//fin
string* snip(string* s, char before, char after) //returns the string created by the charachters between before and after
{
    if(s->alloc != FALSE)
    { 
        int beg, sz;
        for(int i = 0; i < s->leng; i++)
        {
            if(s->ptr[i] == before)
            {
                beg = i+1; //we only care about the charachters after this and before the after charachter
                break;
            }
        }
        for(int a = beg; a < s->leng; a++)
        {
            if(s->ptr[a] == after)
            {
                sz = a - beg;
                break;
            }
        }
        string* sub = malloc(sizeof(string));
        sub->alloc = TRUE;
        sub->leng = sz;
        if(sub->leng < OPTLVL)
        {
            sub->opt = TRUE;
            sub->ptr = &sub->optimization[0];
        }
        else
        {
            sub->ptr = malloc(sub->leng);
            sub->opt = FALSE;
        }
        memcpy(sub->ptr, (s->ptr + beg), sz);
        internal_set_fp(sub);
        return sub;
    }
    else
    {
        string* is = malloc(sizeof(string));
        is->alloc = FALSE;
        printf("error getting substring\n");
        return is;
    }
}

//fin
//returns the string created by the 
//charachters between before and after and in the substring of string starting at start going for len
string* subsnip(string* s, char before, char after, int start, int len)
{
    if(s->alloc != FALSE)
    { 
        int beg, sz;
        for(int i = start; i < s->leng && i < len + start; i++)
        {
            if(s->ptr[i] == before)
            {
                beg = i+1; //we only care about the charachters after this and before the after charachter
                break;
            }
        }
        for(int a = beg; a < s->leng && a < len + start; a++)
        {
            if(s->ptr[a] == after)
            {
                sz = a - beg;
                break;
            }
        }
        string* sub = malloc(sizeof(string));
        sub->alloc = TRUE;
        sub->leng = sz;
        if(sub->leng < OPTLVL)
        {
            sub->opt = TRUE;
            sub->ptr = &sub->optimization[0];
        }
        else
        {
            sub->ptr = malloc(sub->leng);
            sub->opt = FALSE;
        }
        memcpy(sub->ptr, (s->ptr + beg), sz);
        internal_set_fp(sub);
        return sub;
    }
    else
    {
        string* is = malloc(sizeof(string));
        is->alloc = FALSE;
        printf("error getting substring\n");
        return is;
    }
}

//fin
string* clone(string* s) //clones the contents of string into a new string
{
    string* newstr = malloc(sizeof(string));
    newstr->alloc = TRUE;
    newstr->leng = s->leng;
    if(newstr->leng < OPTLVL)
    {
        newstr->opt = TRUE;
        newstr->ptr = &newstr->optimization[0];
    }
    else
    {
        newstr->ptr = malloc(newstr->leng);
        newstr->opt = FALSE;
    }
    memcpy(newstr->ptr, s->ptr, s->leng);
    internal_set_fp(newstr);
    return newstr;
}

//fin
string* readfile(char* filename)
{
    FILE * pFile;
    long lSize;
    size_t result;
    pFile = fopen ( filename, "r" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    string* newstr = malloc(sizeof(string));
    newstr->alloc = TRUE;
    newstr->leng = lSize;
    if(newstr->leng < OPTLVL)
    {
        newstr->opt = TRUE;
        newstr->ptr = &newstr->optimization[0];
    }
    else
    {
        newstr->ptr = malloc(newstr->leng);
        newstr->opt = FALSE;
    }
    internal_set_fp(newstr);
    result = fread (newstr->ptr,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
    printf("size of file in bytes was: %ld\n", lSize);
    fclose (pFile);
    return newstr;
}

//fin
string* fromint(int i)
{
    if(i == 0)
    {
        string* newstr = malloc(sizeof(string));
        newstr->alloc = TRUE;
        newstr->opt = TRUE;
        newstr->ptr = &newstr->optimization[0];
        internal_set_fp(newstr);
        *newstr->ptr = '0';
        newstr->leng = 1;
        return newstr;
    }
    int neg = 1, l = 0;
    if(i < 0)
    {
        neg = -1;
        l = 1;
    }
    string* newstr = malloc(sizeof(string));
    newstr->alloc = TRUE;
    newstr->leng = (int)log10(i) + 1 + l;
    if(newstr->leng < OPTLVL)
    {
        newstr->opt = TRUE;
        newstr->ptr = &newstr->optimization[0];
    }
    else
    {
        newstr->ptr = malloc(newstr->leng);
        newstr->opt = FALSE;
    }
    internal_set_fp(newstr);
    int copy = neg*i, place = (i < 0) ? 1:0;
    char* base10 = "0123456789";
    newstr->ptr[0] = '-';
    while(copy)
    {
        newstr->ptr[place] = base10[copy % 10];
        copy /= 10;
        place++;
    }
    return newstr;
}

//
string* fromfloat(double f)
{
    if(f == 0.0f)
    {
        string* newstr = malloc(sizeof(string));
        newstr->alloc = TRUE;
        newstr->opt = TRUE;
        newstr->ptr = &newstr->optimization[0];
        internal_set_fp(newstr);
        *newstr->ptr = '0';
        newstr->leng = 1;
        return newstr;
    }
    int neg = 1, l = 0;
    if(f < 0.0f)
    {
        neg = -1;
        l = 1;
    }
    string* newstr = malloc(sizeof(string));
    newstr->alloc = TRUE;
    newstr->leng = (int)log10(f) + 1 + l;
    if(newstr->leng < OPTLVL)
    {
        newstr->opt = TRUE;
        newstr->ptr = &newstr->optimization[0];
    }
    else
    {
        newstr->ptr = malloc(newstr->leng);
        newstr->opt = FALSE;
    }
    internal_set_fp(newstr);
    int copy = neg*f, place = (f < 0.0f) ? 1.0f:0.0f;
    char* base10 = "0123456789";
    newstr->ptr[0] = '-';
    while(copy)
    {
        newstr->ptr[place] = base10[copy % 10];
        copy /= 10;
        place++;
    }
    return newstr;
}

//fin
string* set(char* s2) //creates a new string with contents of s2
{
    string* newstr = malloc(sizeof(string));
    newstr->alloc = TRUE;
    newstr->leng = strlen(s2); //number of charachters != final memory cell number
    //printf("\n%d\n", newstr.leng);
    if(newstr->leng < OPTLVL)
    {
        newstr->opt = TRUE;
        newstr->ptr = &newstr->optimization[0];
    }
    else
    {
        newstr->ptr = malloc(newstr->leng);
        newstr->opt = FALSE;
    }
    memcpy(newstr->ptr, s2, newstr->leng);
    internal_set_fp(newstr);
    return newstr;
}
#endif