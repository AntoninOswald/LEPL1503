
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int equals(int a, int b){
    if (a==b){
        return 0;
    } else{
        return 1;
    }
}

int greaterThan(int a, int b){
    if (a >= b){
        return 0;
    } else{
        return 1;
    }
}

int greater(int a, int b){
    if (a > b){
        return 0;
    } else{
        return 1;
    }
}

int lessThan(int a, int b){
    if (a <= b){
        return 0;
    } else{
        return 1;
    }
}

int less(int a, int b){
    if (a < b){
        return 0;
    } else{
        return 1;
    }
}

int notEqual(int a, int b){
    if (a != b ){
        return 0;
    } else{
        return 1;
    }
}



int main(int argc, char **argv){

    
    int v1 = atoi(argv[1]);
    int v2 = atoi(argv[3]);
    char *command;
    command = argv[2];

    

    if (strcmp(command, "-eq") == 0){return equals(v1,v2);}
    if (strcmp(command,"-ge") == 0){return greaterThan(v1,v2);}
    if (strcmp(command,"-gt") == 0){return greater(v1,v2);}
    if (strcmp(command,"-le")==0){return lessThan(v1,v2);}
    if (strcmp(command,"-lt") == 0){return less(v1,v2);}
    if (strcmp(command,"-ne") == 0){return notEqual(v1,v2);}
    

}