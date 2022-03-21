
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> /* Permet d'utiliser sleep*/

#include <inttypes.h>



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


/**
* Structure node
*
* @next: pointer to the next node in the list, NULL if last node_t
* @value: value stored in the node
*/
/*typedef struct node {
    struct node *next;
    int value;
} node_t;

/**
* Structure list
*
* @first: first node of the list, NULL if list is empty
* @size: number of nodes in the list
*/
/*
typedef struct list {
    struct node *first;
    int size;
} list_t;

/*
* Create a new node containing the value @value.
*
* @value: value stored in the node
* @next : pointer to next node, initialised to NULL.
* @return: returns the pointer to the newly created node, or NULL if an error occured.
*/

/*
node_t *init_node(int value){

    node_t *first;

    first = (struct node *)malloc(sizeof(struct node));

    if (first == NULL ){
        return NULL;
    }

    first->next = NULL;
    first->value = value;

    return first;
    free(first);


}

/*
* Add a node at the head of the list @list and increment the number of nodes in the list (member `size`)
*
* @l: linked list
* @value: value to add to the list
*
* @return: 0 if success, 1 otherwise
*/
/*
int add_node(list_t *list, int value) {

    if (list == NULL){
        return 1;

    } else if (list == NULL){
        list->first = init_node(value);
        list->size = 1;
        return 0;
    } else{
        node_t *n = malloc(sizeof(node_t));
        if (n == NULL){
            return 1;
        } else{
            n->value = value;
            n->next = list->first;
            list->first = n;
            (list->size)++;
            return 0;
        }

        free(n);
    }


}
 */





/*
 * exo sur les arbres binaires
 */

/*
* Node has a value, @value, and two children, @left and @right.
* All the children of @left and itself have a smaller value than the node and all the children of @right and itself have a larger value than node
*/
typedef struct node{
    int value;
    struct node* left; // to smaller values
    struct node* right; // to larger values
} node_t;



typedef struct bt{
    struct node* root;
} bt_t;


/*
* Return 1 if there is a node containing @value in @tree and 0 otherwise.
*/
/*int contains(bt_t* tree, int value){

    struct node* init = malloc(sizeof(struct node));

    if(init == NULL){
        return 0;
    }
    init = tree->root;


    struct bt* rightTree;
    struct bt* leftTree;

    rightTree = malloc(sizeof(struct bt));
    if (rightTree == NULL){
        return 0;
    }


    leftTree = malloc(sizeof(struct bt));
    if (leftTree == NULL){
        return 0;
    }

    if(tree->root->value == value){
        return 1;
    } else{

        if(init->right != NULL && init->left != NULL){
            rightTree->root = init->right;
            leftTree->root = init->left;

            if (contains(rightTree,value) == 0){
                return contains(leftTree,value);
            } else{
                return 1;
            }

        } else if(init->right == NULL && init->left == NULL){

            if(init->value == value){
                return 1;
            } else{
                return 0;
            }
        } else if (init->right == NULL && init->left != NULL){
            return contains(leftTree,value);
        } else{
            return contains(rightTree,value);
        }


    }

    free(leftTree);
    free(rightTree);
    free(init);


}  */

int search(int value, struct node *root){

    if (root != 0){
        if (value == root->value){
            return 1;
        } else if (value < root->value){
            return search(value, root->left);
        } else{
            return search(value,root->right);
        }
    } else{
        return 0;
    }
}

int contains(bt_t* tree, int value){
    if (tree == NULL){
        return 0;
    } else{
        return search(value,tree->root);
    }

}





/* LAST S3 */


typedef struct product {
    char *name;
    double price;
} product_t;


double getPtrPrice(product_t *ptrProd){

    return ptrProd->price;

}

double getPrice(product_t prod){

    return prod.price;


}

char* getPtrName(product_t *ptrProd){

    char *named = malloc(sizeof(char)*(strlen(ptrProd->name)+1));

    if (named == NULL){
        return NULL;
    }

    strcpy(named,ptrProd->name);

    return named;
}


char* getName(product_t prod){

    char *named = malloc(sizeof(char)*(strlen(prod.name)+1));

    if (named == NULL){
        return NULL;
    }

    strcpy(named,prod.name);

    return named;
    
}

int prodEquals(product_t *ptrProd, product_t prod){
    
    if (strcmp(ptrProd->name, prod.name) == 0)
    {
        if (ptrProd->price == prod.price){
            return 1;
        } else
        {
            return 0;
        }
        
        
        
    } else
    {
        return 0;
    }

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

   /* node_t *p = init_node(3);
    printf("%d\n",p->value); */

    

    return 0;


}


