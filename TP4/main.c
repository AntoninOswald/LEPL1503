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

    struct node *new = (struct node *)malloc(sizeof(struct node));

    if (new == NULL){
        return 1;
    }

    new->name = (char *)malloc(strlen(value) +1);
    
    if(new->name == NULL || strncpy(new->name, value, strlen(value) +1) == NULL){
        return 1;
    }

    new->next = *head;
    *head = new;

    return 0;
}


int cmp_func(uint8_t*func1(uint8_t n), u_int8_t*func2(uint8_t n)){
    for (int i = 0; i < 256; i++)
    {
        if (func1(i) != func2(i))
        {
            return 0;
        }
        
    }

    return 1;
    
}



/* Filtering a linkedList */


/*struct node {
    struct node *next;
    int hash;
    int id;
    char name[20];
    char buffer[100];
    unsigned int timestamp;
    char acl;
    short flow;
    char *parent;
    void *fifo;
}; */

/* pair_filter
 * Make a copy of the linked list starting at head,
 * only taking the elements with an even index
 *
 * @head : head of a linked list, possibly NULL
 * @return: pointer to the filtered linked list, return NULL if error or if head == NULL
 */

/*
struct node *pair_filter(struct node *head) {

    if (head == NULL){
        return NULL;
    }
    
    struct node *new = malloc(sizeof(*head));

    if (new == NULL){
        return NULL;
    }
    
    memcpy(new, head, sizeof(*head));

    if( head->next  != NULL){
        new->next = pair_filter(head->next->next);
    }

    return new;




}   */

typedef struct node{
  struct node* next;
  int value;
} node_t;

typedef struct queue{
  struct node* tail;
  int size;
} queue_t  ;



/*
* Add @val value at the head of the @q queue
*
* @val     : the value to enqueue
* @q     : the queue
*
* @return 0 if no error, -1 otherwise
*
* pre : The queue q will always be valid, you don't need to handle the NULL case.
*/
int enqueue(queue_t* q, int val){

    struct node *new = malloc(sizeof(node_t));

    if (new == NULL){
        return -1;
    }

    if (q->size == 0){
        new->value = val;
        q->tail = new;
        q->tail->next = new;
        q->size++;
        return 0;
    } else if (q->size == 1){
        new->value = val;
        new->next = q->tail;
        q->tail->next = new;
        q->size++;
        return 0;
    } else {
        new->value = val;
        new->next = q->tail->next;
        q->tail->next = new;
        q->size ++;
        return 0;
    }

}


/*
* Remove the node at the tail of the @q queue and return the value of this node
* @q         : the queue
* @return     : the value at the tail
*
* pre         : The given queue will always be valid, you do not need to handle the NULL case.
*/
int dequeue(queue_t* q){

    int val;

    if (q->size == 1){
        val = q->tail->value;
        free(q->tail);
        q->tail->next = NULL;
        q->tail = NULL;
        q->size = 0;
    } else{
        val = q->tail->value;
        node_t *temp = q->tail;

        while (temp->next != q->tail)
        {
            temp = temp->next;


        }

        node_t *fin = temp->next;
        temp->next = q->tail->next;

        q->tail = temp;

        free(fin);

        q->size--;
        
    }

    return val;



}

typedef struct node{
    char val;
    struct node *next;
} node_t;

int compare(char a, char b){
    return a-b;
}


/*
* @value: value to be inserted. If the value is already in the list, no element is added and the list is not modified
* @fun: comparison function which defines the order relationship of the list
* @head: first node of the list, head != NULL
* @return: 0 if success, -1 otherwise
*/
int insert(node_t **head, char val, int (*cmp)(char,char)) {

    if (!*head){
        node_t* newnode = (node_t*)malloc(sizeof(node_t));
        if (newnode == NULL){return -1;}
        newnode->val = val;
        newnode->next = NULL;
        *head = newnode;
        return 0;
    }

    node_t *runner = *head;
    node_t *previous = *head;

    int gol = (*cmp)(val, runner->val);
    if (gol == 0){return 0;}

    if (gol < 0){
        node_t *newNode = (node_t*)malloc(sizeof(node_t));

        if (newNode == NULL){return -1;}
        newNode->val = val;
        newNode->next = runner;
        *head = newNode;
        return 0;
    }

    runner = runner->next;

    while ((runner != NULL) && ((*cmp)(val, runner->val) >= 0))
    {
        if ((*cmp)(val, runner->val) == 0){return 0;}
        previous = runner;
        runner = runner->next;
    }

    node_t*newNode = (node_t*)malloc(sizeof(node_t));

    if(newNode == NULL){return -1;}

    newNode->val = val;
    newNode->next = runner;
    previous->next = newNode;
    return 0;

    


}








int main(int argc, char **argv){



    
}
