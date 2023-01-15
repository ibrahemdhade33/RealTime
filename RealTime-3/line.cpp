#include "shared_data.h"
using namespace std;
int x = 0;
pthread_mutex_t mutex_s;
bool first_seq_threads_done = false;
bool running = true;
int num_threads;
int num_seq_threads;
int sleep_time;
int num_of_pices = 0;
pthread_cond_t cond;
string type = "";
void *thread_function(void *id)
{
    int thread_id = *((int *)id);
    int local_var = 0;
    while (running)
    {
        pthread_mutex_lock(&mutex_s);
        if (thread_id < num_seq_threads)
        {
            while (!first_seq_threads_done)
            {
                pthread_cond_wait(&cond, &mutex_s);
            }
            local_var = x;
            x = local_var + 1;
            sleep(sleep_time * 100000);
        }
        else
        {
            local_var = x;
            x = local_var + 1;

            usleep(sleep_time * 100000);
        }
        if (x == 8)
        {
            x = 0;
            num_of_pices++;
            cout << "Number of pices for type : " << type << "--->" << num_of_pices << endl;
        }
        pthread_mutex_unlock(&mutex_s);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage: " << argv[0] << " <num_threads> <num_seq_threads> <sleep_time>" << std::endl;
        return 1;
    }
    num_threads = atoi(argv[1]);
    num_seq_threads = atoi(argv[2]);
    sleep_time = atoi(argv[3]);
    type = argv[4];
    pthread_t threads[num_threads];
    int thread_ids[num_threads];
    pthread_mutex_init(&mutex_s, NULL);
    pthread_cond_init(&cond, NULL);
    while (true)
    {
        first_seq_threads_done = false;
        for (int i = 0; i < num_threads; i++)
        {
            thread_ids[i] = i;
            pthread_create(&threads[i], NULL, thread_function, (void *)&thread_ids[i]);
        }
        for (int i = 0; i < num_seq_threads; i++)
        {
            pthread_join(threads[i], NULL);
        }
        pthread_mutex_lock(&mutex_s);
        first_seq_threads_done = true;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex_s);
        for (int i = 0; i < num_threads; i++)
        {
            pthread_join(threads[i], NULL);
        }
    }
    pthread_mutex_destroy(&mutex_s);
    return 0;
}