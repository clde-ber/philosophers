#include "philosophers.h"
int count = 0;

void    *start_routine(void *arg)
{
    unsigned int count;

    count = 0;
    pthread_mutex_lock(&arg);
    count += 1;
    printf("task %d\n finished", count);
    pthread_mutex_lock(&arg);
}

int main(int ac, char **av)
{

    pthread_t   thread[2];
    void        *start_routine;
    void        *arg;
    pthread_mutex_t *mutex;
    int i;
    int ret;
    void *code;

    i =0;
    arg = 0;
    pthread_mutex_init(&mutex, PTHREAD_MUTEX_FAST_NP);
    while (i < 2)
    {
        ret = pthread_create(&thread[i], PTHREAD_CREATE_JOINABLE | SCHED_RR, &start_routine, &mutex);
        if (ret)
            printf("error\n");
        i++;
    }
    ret = phtread_join(thread[0], &code);
    if (ret)
        printf("error\n");
    ret = phtread_join(thread[1], &code);
    if (ret)
        printf("error\n");
    pthread_mutex_destroy(&mutex);
}