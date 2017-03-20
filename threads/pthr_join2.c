#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *start_thread(void *message)
{
    printf("%s\n", (const char *)message);
    pthread_exit((void *)message);
}

int main(void)
{
    pthread_t thing1, thing2;
    const char *message1 = "Thing 1";
    const char *message2 = "Thing 2";
    void *tret;

    pthread_create(&thing1, NULL, start_thread, (void *)message1);
    pthread_create(&thing2, NULL, start_thread, (void *)message2);

    /*
     * wait for the threads to exit. if we didn't join here,
     * we'd risk terminating this main thread before the
     * other two threads finished.
     */
    pthread_join(thing1, &tret);
    printf("thing 1 return %s\n", (const char *)tret);
    pthread_join(thing2, &tret);
    printf("thing 2 return %s\n", (const char *)tret);

    return 0;
}

