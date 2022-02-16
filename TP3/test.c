
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void swap(int *i, int *j){

    int local = *j;
    int local2 = *i;

    *i = local;
    *j = local2;




}

struct fract_t {
    int num;
    int denum;
};

void swap2(struct fract_t *a, struct fract_t *b){


    struct fract_t temp = *a;
    *a = *b;
    *b = temp;
}




int main(int argc, char **argv)
{
    printf("Hello world!\n");

    int i;

    for (i = 0; i < argc; i++){
        i = i+1;
        if (i == argc -1 || i == argc -2 ){
            printf("%s\n", argv[i]);
        } else{
            printf("%s ", argv[i]);
        }
        
    }
    printf("Test");




    

    

    

    

    return 0;


}