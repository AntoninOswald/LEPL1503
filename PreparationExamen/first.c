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




int main(int argc, char *argv[])
{

    char A[] = "Hello";
    char B[] = "World!";
    printf("%s\n", strcat(A,B));

    char *test = "kayak";

    printf("%d\n",pal(test));
    printf("%s\n", buf_strcpy(test));

}