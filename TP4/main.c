#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> /* Permet d'utiliser sleep*/

#include <inttypes.h>


/*Question 1 S4 simple stack */



struct node
{
    struct node *next;
    char *name;  /* let us not forget to use malloc*/
    
};

/**
* Remove the top element of the stack and return its content.
*
* @head : pointer to the top of the stack
* @result : pointer to store the popped element
*
* @return 0 if no error, 1 otherwise
*
* pre : @result contains a null-terminated correct string
* post : @result contains the string @name from the element at the top of the stack
*/

int pop(struct node **head, char *result){

    if (head == NULL){
        return 1;
    }

    if (strncpy(result, (*head)->name, strlen((*head)->name)) == NULL){
        return 1;
    }

    /* Comme on a appelé strncopy dans le if, il est appliqué quand meme
       strncopy permet de vérifier que result n'est pas trop petit*/

    struct node *tofree = *head;
    *head = (*head)->next;
    free(tofree->name);
    free(tofree);
    return 0;

}

/**
* Add @name at the "top" of the stack.
*
* @head : pointer to the top of the stack
* @name : the string to be placed in the element at the top of the stack
*
* @return 0 if no error, 1 otherwise
*/

int push(struct node **head, const char *value){

    if (head == NULL){
        return 1;
    }

    struct node *toHead = (struct node *)malloc();



}






int main(int argc, char **argv){



    
}
