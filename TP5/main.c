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


int file_exists(char *filename){

    int fd;

    fd = open(filename, O_RDONLY);

    if (fd == -1){
        return -1;
    }

    close(fd);  /* pour fermer le fichier on lui donne un numéro */

    return 0;

}

typedef struct point {
    int x;
    int y;
    int z;
} point_t;


/*
 * @pre pt != NULL, pointer to the first point_t in the array
 *      size > 0, the length of the array.
 *      filename != NULL
 * @post writes the array of point_t in the file.
 *       return 0 is everything worked correctly
 *       -1 if open() failed.
 *       -2 if close() failed.
 *       -3 if mmap() failed.
 *       -4 if munmap() failed.
 *       -5 if msync() failed.
 *       -6 if ftruncate() failed.
 *
 */
int save(point_t *pt, int size, char *filename) {

    int fd; /*open*/

    fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR);

    if (fd < 0){
        return -1;
    }

    int err = ftruncate(fd, size*sizeof(point_t));
    if (err == -1){close(fd);return -6;}

    void *mptr = mmap(NULL, size*sizeof(point_t), PROT_WRITE, MAP_SHARED,fd,0);
    if (mptr == MAP_FAILED){
        close(fd);
        return -3;
    }

    memcpy(mptr,pt, size*sizeof(point_t));

    err = msync(mptr, size*sizeof(point_t), MS_ASYNC);
    if(err == -1){close(fd); return -5;}

    err = munmap(mptr,size*sizeof(point_t));

    if (err == -1){close(fd);return -4;}
    err = close(fd);
    if(err == -1){return -2;}

    return 0;

}



/*
 * @pre filename != NULL, name of the file
 * @post returns the sum of all integers stored in the binary file.
 *       return -1 if the file can not be open.
 *       return -2 if the file can not be closed.
 *       return -3 if mmap() fails.
 *       return -4 if munmap() fails.
 *       return -5 if fstat() fails.
 */
int sum_file(char *filename) {

    int fd;

    fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        return -1;
    }

    struct stat buffer;

    if (fstat(fd, &buffer) < 0)
    {
        return -5;
    }

    if (buffer.st_size == 0){
        close(fd);
        return 0;
    }

    int result = 0;
    int *mapper = mmap(NULL, buffer.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (mapper == MAP_FAILED){
        close(fd);
        return -3;
    }

    for (int i = 0; i < (buffer.st_size/sizeof(int)); i++)
    {
        result += mapper[i];

    }

    if (close(fd) == -1){
        munmap((void *) mapper, buffer.st_size);
        return -2;
    }

    if (munmap((void *) mapper, buffer.st_size) < 0)
    {
        return -4;
    }

    return result;

}


int get(char *filename, int index)
{

    int fd;

    fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        return -1;
    }

    struct stat buffer;

    if (fstat(fd, &buffer) < 0)
    {
        return -1;
    }

    if (buffer.st_size/sizeof(int) <= index){
        
        return -2;
    }

    
    int *mapper = mmap(NULL, buffer.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (mapper == MAP_FAILED){
        
        return -1;
    }

    int value = mapper[index];

    if (close(fd) == -1){
        munmap((void *) mapper, buffer.st_size);
        return -1;
    }

    if (munmap((void *) mapper, buffer.st_size) < 0)
    {
        return -1;
    }


    return value;

}

void set(char *filename, int index, int value) 
{

    int fd;

    fd = open(filename,O_RDWR,S_IRUSR);

    if (fd < 0)
    {
        close(fd);
        return;
    }

    struct stat buffer;

    if (fstat(fd, &buffer) < 0)
    {
        close(fd);
        return;
    }

    if (buffer.st_size/sizeof(int) <= index){
        close(fd);
        
        return;
    }


    int *mapper = mmap(NULL, buffer.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    if (mapper == MAP_FAILED){
        close(fd);
        
        return;
    }

    mapper[index] = value;

    if (msync( fd, sizeof(int), MS_SYNC) <0 )
    {
        close(fd);
        return;
    }



    if (close(fd) == -1){
        munmap((void *) mapper, buffer.st_size);
        return;
    }

    if (munmap((void *) mapper, buffer.st_size) < 0)
    {
        close(fd);
        return;
    }

}

/*
 * @pre file_name != NULL, name of the original file
 *      new_file_name != NULL, name of the new file (the copy)
 *
 * @post copy the contents of {file_name} to {new_file_name}.
 *       return 0 if the function terminates with success, -1 in case of errors.
 */
int copy(char *file_name, char *new_file_name) 
{
    int fd_src = open(file_name, O_RDWR);
    if (fd_src < 0)
    {
        return -1;
    }

    struct stat buffer_src;

    if (fstat(fd_src, &buffer_src) < 0)
    {
        return -1;
    }
    /* a retenir mode_t*/

    
    mode_t autorisations = buffer_src.st_mode;

    int fd_dest = open(new_file_name, O_RDWR|O_CREAT|O_TRUNC, autorisations);

    if (fd_dest < 0)
    {
        return -1;
    }

    if (ftruncate(fd_dest,buffer_src.st_size) < 0)
    {
        return -1;
    }

    void *mapper_src = mmap(NULL, buffer_src.st_size, PROT_READ, MAP_SHARED, fd_src, 0);

    if (mapper_src == MAP_FAILED)
    {
        return -1;
    }

    void *mapper_dest = mmap(NULL, buffer_src.st_size, PROT_WRITE, MAP_SHARED, fd_dest, 0);

    if (mapper_dest == MAP_FAILED)
    {
        munmap(mapper_src,buffer_src.st_size);
        return -1;
    }

    memcpy(mapper_dest, mapper_src, buffer_src.st_size);

    if (msync(mapper_dest, buffer_src.st_size, MS_SYNC) < 0)
    {
        munmap(mapper_src, buffer_src.st_size);
        munmap(mapper_dest, buffer_src.st_size);
        return -1;
    }

    if ((close(fd_src) < 0) | (close(fd_dest) < 0) )
    {
        munmap(mapper_src, buffer_src.st_size);
        munmap(mapper_dest, buffer_src.st_size);
        return -1;
    }

    if (((munmap(mapper_dest, buffer_src.st_size) < 0) | (munmap(mapper_src, buffer_src.st_size) < 0)))
    {
        return -1;
    }

    return 0;


}



typedef struct points {
    int NOMA;
    float LEPL1103;
    float LEPL1203;
    float LEPL1108;
    float LEPL1302;
    float LEPL1402;
    float average;
} points_t;


/*
* Function used to compare two structures based on their averages.
*
* @s1 : a pointer to the first structure
* @s2 : a pointer to the second structure
*
* @return an integer less than, equal to, or greater than zero if the first argument is considered to be
*        respectively less than, equal to, or greater than the second
*/
int compar(const void* s1, const void* s2)
{

    int result;
    points_t* first = (points_t*)malloc(sizeof(points_t));
    points_t* second = (points_t*)malloc(sizeof(points_t));
    first = (points_t *) s1;
    second =(points_t*) s2;


    result = ((first->average) - (second->average))*100;
    
    return result;

}


/*
* Function used to update the file with the structures stored in it
*
* @filename : the path indicating where to find the file (!= NULL)
*
* @return 0 if no error
*        -1 if open failed
*        -2 if mmap failed
*        -3 if munmap failed
*        -4 if msync failed
*        -5 if fstat failed
*        -6 if close failed
*/
int sort(char* filename)
{

    int fd = open(filename, O_RDWR);

    if (fd == -1)
    {
        return -1;
    }

    struct stat buffer;

    if (fstat(fd, &buffer) == -1)
    {
        close(fd);
        return -5;
    }

    points_t* mapper = mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 0);
    if (mapper == (void*) (-1))
    {
        close(fd);
        return -2;
    }

    for (int i = 0; i < buffer.st_size/sizeof(points_t); i++)
    {
        mapper[i].average = (mapper[i].LEPL1103 + mapper[i].LEPL1203 +mapper[i].LEPL1108 + mapper[i].LEPL1302 + mapper[i].LEPL1402)/5;
    }

    qsort(mapper, buffer.st_size/sizeof(points_t), sizeof(points_t), compar);

    if (msync(mapper, buffer.st_size, MS_SYNC) == -1)
    {
        munmap(mapper, buffer.st_size);
        return -4;
    }

    if (close(fd) == -1)
    {
        munmap(mapper, buffer.st_size);
        return -6;
    }

    if ((munmap(mapper, buffer.st_size))  == -1)
    {
        return -3;
    }

    return 0;


}




 /*
* Function used to protect a global variable used in the function inc().
*
* @inc : the function that must be called in critical (= protected) section
* @nb : the number of times the function inc() has to be called
* @mutex : an initialised mutex that has to be used for the protection
*
* @return 0 if no error, -1 otherwise
*/
int protect(void inc(void), int nb, pthread_mutex_t* mutex)
{

    for (int i = 0; i < nb; i++)
    {
        if (pthread_mutex_lock(mutex) != 0)
        {
            return -1;
        }
        inc();
        if (pthread_mutex_unlock(mutex) != 0)
        {
            return -1;
        }


        
    }

    return 0;
    

}


/*
* Function used to launch threads that will do the work for us.
*
* @long_computing : a function that will do a really long computing and that must be started by the thread.
*                 This function initially returns a pointer to an int and takes as argument a string.
*                 These have been casted into void* to match pthread_create() function specifications.
*                 You have to launch this function with the argument *string* and compare the returned value
*                 to *ret_value*. If the return value doesn't match *ret_value*,
*                 you have to launch another thread on the same argument with the function *backup_computing*.
*
* @backup_computing :  a backup function that must be used if the first thread didn't do the job correctly.
*                    As for *long_computing*, this function initially takes a string as argument and returns
*                    a pointer to an int.
*
* @string : the string that must be used as a parameter to both functions described above.
*
* @ret_value : the value to which the first returned code must be compared to to know if the first thread
*             correctly did the job or not.
*
* @return the int returned by the function started by the thread if no problem
*         -1 if you can't create the first thread
*         -2 if you can't join the first thread
*         -3 if you can't create the second thread
*         -4 if you can't join the second thread
*/
int doMyWork(void* long_computing(void*), void* backup_computing(void*), char* string, int ret_value)
{
    pthread_t first;

    int *ret;

    if (pthread_create(&first, NULL, long_computing, (void *) string) != 0)
    {
        return -1;
    }

    if (pthread_join(first, (void **)&ret) != 0)
    {
        return -2;
    }

    if (*ret == ret_value)
    {
        return *ret;
    }else
    {
        pthread_t second;
        if (pthread_create(&second, NULL, backup_computing, (void *) string) != 0)
        {
            return -3;
        }

        if (pthread_join(second, (void *) &ret) != 0)
        {
            return -4;
        }

        return *ret;
    }
    


}


typedef struct data {
    int longitude;
    int latitude;
    float avg_temp;
} data_t;



/*
* Function used to put a new value in the shared buffer.
*
* @buf : the shared buffer to fill in with the adresses pointing to the data_t's
* @len : the length of the shared buffer
* @first : the pointer to the array index where you can find the first inserted element that's still in the buffer
*         (or more exactly the pointer to the first element, **if any**)
* @last : the pointer to the array index where you can find the first empty space in the buffer
*         (or more exactly the first NULL pointer in the array, **if any**)
* @in : the number of data_t* pointers in the buffer
* @d : the data_t* that has to be inserted in the buffer
*
* @return 0 if no error, -1 otherwise
*/
int put(data_t** buf, int len, int* first, int* last, int* in, data_t* d)
{
    if (*in == len)
    {
        return -1;
    }

    if (*in == 0)
    {
        *first = * last;
    }

    buf[(*last)++] = d;

    if (*last >= len)
    {
        *last = 0;
    }

    (*in) ++ ;

    return 0;
    


}


/*
* Function used to get a value from the shared buffer.
*
* @buf : the shared buffer to fill out
* @len : the length of the shared buffer
* @first : the pointer to the array index where you can find the first element inserted that's still in the buffer
*         (or more exactly the pointer to the first element, **if any**)
* @last : the pointer to the array index where you can find the first empty space in the buffer
*         (or more exactly the first NULL pointer in the array, **if any**)
* @in : the number of data_t* pointers in the buffer
*
* @return the pointer to the element that you get if no error, NULL otherwise
*/
data_t* get(data_t** buf, int len, int* first, int* last, int* in)
{

    if (*in == 0)
    {
        return NULL;
    }

    data_t *rep = buf[*first];
    buf[(*first) ++] = NULL;
    if ((*first) >= len)
    {
        *first = 0;
    }

    (*in) --;

    return rep;
    


}



int main(int argc, char **argv){





    
}