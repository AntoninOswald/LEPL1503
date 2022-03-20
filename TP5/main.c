#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> /* Permet d'utiliser sleep*/

#include <inttypes.h>


/* Truc de filename */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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


 




int main(int argc, char **argv){





    
}