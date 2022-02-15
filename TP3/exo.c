
#include <stdio.h>
#include <stdlib.h>


void print_digit(int number){
    printf("The magic number is %d\n", number );

}

void format_str(char *buffer, unsigned int d, char *name, char initial) {

    printf(buffer, "Mister %s %c. has %d eggs", name, initial, d);

}

size_t str_len(const char* s){

    int i = 0;

    while ( *(s+i) != '\0')
        i++;

    return i;




}





int strcasecmp(const char *s1, const char *s2) {

    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    int result;
    if (p1 == p2)
        return 0;


    return 1;

}



char *buf_strcpy(const char *src) {

    if (src == NULL)
    {
        return NULL;
    }

    char *x;
    x = (char *)malloc((strlen(src)+1)*sizeof(char));  /*    Multiplier la longueur du string par la taille du type de donnée*/

    
    

    for (int i = 0; i < strlen(src) +1; i++)
    {
        
        *(x+i) = src[i];

    }

    *(x+strlen(src)) = '\0';


    


    return x;

    
    


}


int main()
{
    printf("Hello world!\n");

    print_digit(10);

    

    return 0;


}
