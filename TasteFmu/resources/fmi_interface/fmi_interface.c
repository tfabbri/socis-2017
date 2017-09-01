/* User code: This file will not be overwritten by TASTE. */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "fmi_interface.h"

#include "common.h"

sem_t* semaphore;  // Mutex on the shared memory
struct shm_struct *shm_memory;
int shm_memory_fd;

void fmi_interface_startup()
{
    /* 
     * Using the POSIX API
     *
     * Initialization of the syncronization semaphore
     * Initialization of the shared memory
     *
     */

    semaphore = sem_open(SEM_NAME, O_CREAT, SEM_PERMS, 1);

    if (semaphore == SEM_FAILED) {
        perror("Sem_open(3) error\n");
        exit(1);
    }
    if((shm_memory_fd = shm_open(SHM_NAME,(O_CREAT|O_RDWR), 0666)) == -1){
        perror("Shm get error\n"); 
        exit(1);
    }
    ftruncate(shm_memory_fd, sizeof(struct shm_struct));
    shm_memory = (struct shm_struct *) mmap(0, sizeof(struct shm_struct), PROT_WRITE, MAP_SHARED, shm_memory_fd, 0);

}


asn1SccFmiInteger last_mip = 0;
asn1SccFmiReal last_mrp = 0;
asn1SccFmiBoolean last_mbp = false;


void fmi_interface_PI_Cyclic_Call()
{
    // Enter in critical section
    sem_wait(semaphore);
    //printf("Cyclic call to the Controller function\n");
    asn1SccFmiInteger t_cip;
    asn1SccFmiReal t_crp;
    asn1SccFmiBoolean t_cbp;
    // Read the input from the buffer
    asn1SccFmiInteger t_mip = shm_memory->mip; 
    asn1SccFmiReal t_mrp = shm_memory->mrp;
    asn1SccFmiBoolean t_mbp = shm_memory->mbp;
    //Call to the Controller PI executing the real code 
    printf("IN Port  - mip %d\n", t_mip);
    printf("IN Port  - mrp %f\n", t_mrp);
    printf("IN Port  - mbp %d\n", t_mbp);
    fmi_interface_RI_Controller_Function(&last_mip, &last_mbp, &last_mrp, &t_cip, &t_cbp, &t_crp);
    printf("OUT Port  - cip %d\n", t_cip);
    printf("OUT Port  - crp %f\n", t_crp);
    printf("OUT Port  - cbp %d\n", t_cbp);
    // Copy the output generated to the buffer  
    shm_memory->cip = t_cip;
    shm_memory->cbp = t_cbp;
    shm_memory->crp = t_crp;
    // Store of the current input
    last_mip = t_mip;
    last_mrp = t_mrp;
    last_mbp = t_mbp;
    // Exit critical section
    
    sem_post(semaphore);
    printf("\n");
}

