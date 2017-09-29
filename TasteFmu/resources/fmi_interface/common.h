#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>

#include "dataview-uniq.h"

#define SEM_NAME "/pSem"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define SHM_NAME "/pShm"

struct shm_struct{ 
    // Mutex and condition variables to sync with the FMI / TASTE
    pthread_mutex_t mutex;
    pthread_cond_t put_cond;
    pthread_cond_t get_cond;

    asn1SccFmiInteger mip; // integer input port
    asn1SccFmiInteger cip; // integer output port
    asn1SccFmiBoolean mbp;
    asn1SccFmiBoolean cbp;
    asn1SccFmiReal mrp;
    asn1SccFmiReal crp; 
};

