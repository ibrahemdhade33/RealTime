#ifndef LINES_RREULTS_OPERATIONS
#define LINES_RREULTS_OPERATIONS

#include "header.h"
pthread_rwlock_t rwlock;
key_t key = 1234;
int shm_size = 1024;
int shm_flags = IPC_CREAT | 0777;
int shm_id;
char *shm_ptr;

struct shared_memory
{
    int a;
    int b;
    int c;
    pthread_rwlock_t rwlock;
};

struct thread_args
{
    std::string type;
    struct shared_memory *shm_ptr;
};

void *write_function(void *args)
{
    struct thread_args *t_args = (struct thread_args *)args;
    pthread_rwlock_wrlock(&t_args->shm_ptr->rwlock); // acquire write lock
    if (t_args->type == "a")
    {
        shm_ptr->a++;
    }
    else if (t_args->type == "b")
    {
        shm_ptr->b++;
    }
    else if (t_args->type == "c")
    {
        shm_ptr->c++;
    }
    pthread_rwlock_unlock(&t_args->shm_ptr->rwlock); // release write lock
}
void *read_function(void *args)
{
    struct thread_args *t_args = (struct thread_args *)args;
    pthread_rwlock_rdlock(&t_args->shm_ptr->rwlock); // acquire read lock
    if (t_args->type == "a")
    {
        std::cout << "a: " << --t_args->shm_ptr->a << std::endl;
    }
    else if (t_args->type == "b")
    {
        std::cout << "b: " << --t_args->shm_ptr->b << std::endl;
    }
    else if (t_args->type == "c")
    {
        std::cout << "c: " << --t_args->shm_ptr->c << std::endl;
    }
    pthread_rwlock_unlock(&t_args->shm_ptr->rwlock); // release read lock
}

void create_shared_memory()
{
    int shm_id = shmget(key, sizeof(struct shared_memory), shm_flags);
    if (shm_id < 0)
    {
        // handle error
    }
    shm_ptr = (struct shared_memory *)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *)-1)
    {
        // handle error
    }
    pthread_rwlock_init(&shm_ptr->rwlock, NULL); // initialize read-write lock
}

void create_shared_memory(struct thread_args *t_args)
{
    int shm_id = shmget(key, sizeof(struct shared_memory), shm_flags);
    if (shm_id < 0)
    {
        // handle error
    }
    t_args->shm_ptr = (struct shared_memory *)shmat(shm_id, NULL, 0);
    if (t_args->shm_ptr == (void *)-1)
    {
        // handle error
    }
    pthread_rwlock_init(&t_args->shm_ptr->rwlock, NULL); // initialize read-write lock
}
#endif