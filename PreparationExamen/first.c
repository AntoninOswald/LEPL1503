#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> /* Permet d'utiliser sleep*/

#include <inttypes.h>


/* Truc de filename */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>



#include <pthread.h> 


/* Examen import */

#include <limits.h>


char* strcat(char* dest, const char*src)
{

    size_t dest_len = strlen(dest);
    size_t i;
    size_t n = strlen(src);

    for (i = 0; i < n; i++)
    {
        dest[dest_len +i] = src[i];
    }
    dest[dest_len+i] = '\0';
    


    return dest;

}

int pal(char *str)
{
    if (str == NULL)
    {
        return -1;
    }

    int leftindex = 0;
    int rightindex = strlen(str)-1;


    while (rightindex != 0 && (*(str + leftindex) != '\0'))
    {   
        if (rightindex == leftindex)
        {
            return 1;
        }
        
        if (*(str+leftindex) == ' ')
        {
            leftindex++;
        } else if (*(str+rightindex) == ' ')
        {
            rightindex--;
        } else
        {
            if (*(str+leftindex) == *(str+rightindex))
            {
                leftindex++;
                rightindex--;
            } else
            {
                return 0;
            }
            
            
        }
    
        
    }

    return 1;
    
}

struct fract_t {
        int num;
        int denum;
};

void swap(struct fract_t *a, struct fract_t *b)
{

    struct fract_t temp = *a;
    *a = *b;
    *b = temp;
    

}

/*
* Creates a buffer that has the same size as src, and copies the content of src to this buffer.
*
* @src: string to be copied
* @return: return pointer. if src == NULL or in case of error, return NULL
*
* Remember that strings are terminated with '\0' and that strlen("abc") returns 3 even if 4 bytes are required to store this string.
*/
char *buf_strcpy(const char *src)
{
    if (src == NULL)
    {
        return NULL;
    }
    char *buffer;

    buffer = (char *)malloc((strlen(src)+1)*sizeof(char));

    if (buffer == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < strlen(src); i++)
    {
        buffer[i] = src[i];
    }
    buffer[strlen(src)]= '\0';

    return buffer;

}

void *calloc2(size_t nmemb, size_t size)
{
    if (nmemb == 0 || size == 0)
    {
        return NULL;
    }
    
    int *result = malloc(nmemb*size);
    if (result == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < nmemb*size; i++)
    {
        *(result+i) = 0;
    }
    
    return result;
    
    
}

/* Wrapper to the malloc system call.
 * If malloc fails, sleep for 5 seconds, and then try again.
 * If malloc failed 10 times in a row, return NULL.
 */
void *sleepy_malloc(size_t s) 
{
    int counter = 0;
    
    while (counter < 10)
    {
        void *result = malloc(s);
        if (result == NULL)
        {
            counter++;
            sleep(5);
        } else
        {
            return result;
        }
        
    }
    return NULL;
}


/* -----------------------Question 1----------------------------*/

struct s {
    char name[10];
    int id;
};


/**
* @pre tab est un pointeur vers un tableau et
* @pre size est la taille du tableau, size > 0
* @post retourne un pointeur vers la structure s dont l'id est minimal dans le tableau dont le pointeur est passé en argument.
**/
struct s *minTab(struct s *tab, unsigned int size)
{
    
    if (size == 1)
    {
        return (&tab[0]);
    }

    struct s *min = &tab[0];

    for (int i = 0; i < size; i++)
    {
        if (min->id > tab[i].id)
        {
            min = &tab[i];
        }
        
    }
    
    return min;
    
}


/*-------------------Question 2--------------------------*/


typedef int Item;  // donnée stockée
struct stacknode{   // élement
    Item item;
    struct stacknode * next;
};

/**
 *  @pre head != NULL
 *  @post *head = NULL
 **/
void init_stack_sol(struct stacknode **head){
    *head = NULL;
}

/**
 *  @pre s != NULL
 *  @post un nouveau noeud est crée et renvoyé ayant pour item i
 *  le nouveau noeud devient le head
 **/
struct stacknode* push(struct stacknode **s, Item i)
{
    
    struct stacknode *new = (struct stacknode *)malloc(sizeof(struct stacknode));
    if (new == NULL)
    {
        return NULL;
    }

    new->item = i;
    new->next = *s;
    *s = new;
    return new;

}

/**
 * @pre s!=NULL
 * @post retourne l'item du noeud en tête de liste
 * l'élement suivant devient la tête de liste. Retourne
 * -1 si la pile est vide ou en cas d'erreur.
 **/

Item pop(struct stacknode **s)
{
    if (*s == NULL)
    {
        return -1;
    }

    int val = (*s)->item;

    struct stacknode *tofree = *s;
    *s = (*s)->next;
    free(tofree);
    return val;

}


/* ----------------------- Question 3------------------------ */

/*
 * @pre str!=NULL
 * @ret renvoie un pointeur vers une copie (sur le heap) de la chaîne de caractères
 *      passée en argument dans laquelle les premiers caractères repris dans la
 *      chaîne ``chars`` sont supprimés.  Renvoie ``NULL`` en cas d'erreur.
 */
char *lstrip(char *str, char *chars)
{
    char *result = (char *)malloc((strlen(str) + 1)*sizeof(char));
    if (result == NULL)
    {
        return NULL;
    }
    
    if (chars == NULL)
    {
        int j = 0;
        for (int i = 0; i < strlen(str); i++)
        {
            if (str[i] != ' ')
            {
                result[j] = str[i];
                j++;
            }
            
        }
        result[strlen(str)] = '\0';
        return result;
    } else
    {
        int i,j;
        int resindex = 0;
        

        for ( i = 0; str[i] != '\0'; i++)
        {
            int is_in_chars = 0;
            

            for ( j = 0; chars[j] != '\0'; j++)
            {
                if (chars[j] == str[i])
                {
                    is_in_chars =1;
                    
                }
                 
            }

            if (is_in_chars == 0)
            {
                for (int k = i; str[k] != '\0'; k++)
                {
                    result[resindex] = str[k];
                    resindex++;
                }
                result[resindex] = '\0';
                return result;
            } 
            
        }
        

        return result;
    }
    

}


/*------------------------Question 4------------------------ */

struct set {
  struct node *first; // pointeur vers le premier node
  int n; // nombre total d'éléments (pas nécessairement distincts) dans l'ensemble
};

struct node {
  double real; // partie réelle
  double img; // partie imaginaire
  int count; // nombre d'éléments ayant cette valeur
  struct node *next; // pointeur vers le noeud suivant
} ;




/*
 * @pre set!=NULL
 * @post le nombre complexe (real,img) a été correctement
 *       ajouté à l'ensemble set. La variable n de cet
 *       ensemble contient à tout moment la quantité de
 *       nombres complexes dans l'ensemble (qui est équivalente à
 *       la somme des count de tous les éléments de la liste chainée).
 *       La fonction retourne la quantité de nombres complexes dans
 *       l'ensemble . En cas d'erreur, retourne -1.
 */
int add(double real, double img, struct set *set)
{

    if (set->n == 0)
    {
        struct node *newnode = (struct node *)malloc(sizeof(struct node));
        if (newnode == NULL)
        {
            return -1;
        }
        newnode->count = 1;
        newnode->img = img;
        newnode->real = real;
        newnode->next = NULL;
        set->n ++;
        set->first = newnode;
        return 1;
        
    }
    
    
    struct node *current = set->first;
    
    
    while (current->next != NULL)
    {
        if (current->real == real && current->img == img)
        {
            current->count ++;
            set->n++;
            return set->n;
        }
        
    }

    if (current->img == img && current->real == real)
    {
        current->count++;
        set->n++;
        return set->n;
    } else
    {
        struct node *newnode = (struct node *)malloc(sizeof(struct node));
        if (newnode == NULL)
        {
            return -1;
        }
        newnode->count = 1;
        newnode->img = img;
        newnode->real = real;
        newnode->next = NULL;
        set->n ++;
        current->next = newnode;
        return set->n;
    }

}




int main(int argc, char *argv[])
{

    printf("----------------TestExamen---------------\n");
    struct s tab[]={
    {"Annie", 12},
    {"Alice", 10}
    };
    printf("%s\n", minTab(tab,2)->name);

    char *A = "www.mozilla.org";
    char *B ="cmowz.";
    printf("%s\n", lstrip(A, B));

    struct 

    struct set set; 
    set.n = 0; 
    set.first = NULL;
    add(2.3,2.2,&set);

    printf("%d\n",add(2.3,2.2,&set));
    printf("%d\n", add(2.3,2.3,&set));
    printf("%f\n", (set.first)->next->img);

    
    
}