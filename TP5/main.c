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



int main(int argc, char **argv){





    
}