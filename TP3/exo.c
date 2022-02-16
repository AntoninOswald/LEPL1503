
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> /* Permet d'utiliser sleep*/



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









char *buf_strcpy(const char *src) {

    if (src == NULL)
    {
        return NULL;
    }

    char *x;
    x = (char *)malloc((strlen(src)+1)*sizeof(char));  /*    Multiplier la longueur du string par la taille du type de donnée*/

    if (x == NULL)  /* cette ligne est très importante,  car elle permet de renvoier null si le malloc n'a pas fonctionné*/
    {
        return NULL;
    }




    
    

    for (int i = 0; i < strlen(src) +1; i++)
    {
        
        *(x+i) = src[i];

    }

    *(x+strlen(src)) = '\0';


    


    return x;

    
    


}




int pal(char *str){


    int length;
    int i, j;
        


    if (str == NULL)
    {
        return -1;
    }
    /* Pour copier une chaine de caractère, on peut utiliser strcopy*/
    
    /*   D'abord il faut retirer les espaces*/

    char *str2;

    length = strlen(str);

    str2 = (char *)malloc(length*sizeof(char)+1); /* Ne pas oublier le +1 + Comment faire quand on ne peut pas utiliser malloc ?*/ 

    if (str2 == NULL)
    {
        return -1;
    }

    j = 0;

    for (i = 0; i < length +1; i++)
    {
        if(str[i] != ' ')  /*     NE PAS OUBLIER DE METTRE DES CHAR ' ' et PAS DES " "*/ 
        {
            str2[j] = str[i];
            j++;
            
        }
    }

    str2[j] = '\0';/*   Utile ?*/

    printf("%s\n", str);

    printf("%s\n", str2);

    

    int k;
    int l = strlen(str2);

    for (k = 0 ; k < l+1 ; k++)
    {

        if (str2[k] != str2[l-k-1])
        {

            free(str2);
            return 0;
            
        }
    }
    free(str2);
    return 1;
}

int strcasecmp(const char *s1, const char *s2){
    int i;
    for (i = 0; s1[i] && s2[i]; i++){
        if (s1[1] == s2[i] || (s1[i] ^32) == s2[i]){
            continue;
        } else{
            break;
        }
    }
    if (s1[i] == s2[i]){
        return 0;
    }

    if ((s1[i] | 32) < (s2[i] | 32)){
        return -1;
    }
    return 1;
} 


int pal2(char *str){ /*   Fonction à mettre sur Inginious, car utiliser la version avec pointeur/malloc ne compile pas*/

    if (str == NULL){
        return -1;
    }

    int len = strlen(str);
    if (len == 0){
        return -1;
    }

    int left = 0;
    int right = len -1;

    while(left < right){
        char str_left = str[left];
        char str_right = str[right];

        if (str_left == ' '){
            left++;
        }else if (str_right == ' '){
            right--;
        } else if (str_left != str_right){
            return 0;
        } else{
            left++;
            right--;
        }
    }

    return 1;


}


/* TP3*/


void *calloc2(size_t nmemb, size_t size){

    if ((nmemb == 0) || (size == 0)){
        return NULL;
    }

    void *a = malloc(nmemb*size);
    if (a == NULL){
        return NULL;
    }

    return a;
    free(a);



}

void *sleepy_malloc(size_t s){
    int c = 0;
    while (c < 10){
        void* a = malloc(s);
        if (a == NULL){
            sleep(5);
        } else{
            return a;
            
        }
        c++;
    }

    return NULL;
    
}









int main(int argc, char **argv)
{
    printf("Hello world!\n");

    print_digit(10);

    char *test = " ka y a k";
    char *t = " djqksldjsqkl";
    char *a = NULL;

    int d = pal(test);

    printf("%d\n", d);

    printf("%d\n", pal(t));

    printf("%d\n", pal(a));

    

    return 0;


}


