#include "../h/ConsoleUtils.h"
#include "../h/syscall_cpp.hpp"

struct SharedData
{
    int arr[2];
    int cycles;
    Semaphore* sem;
    bool done[3];
    SharedData()
    {
        sem = new Semaphore();
        arr[0] = arr[1] = 0;
        cycles = 5;
        for (int i = 0; i < 3; i++)
        {
            done[i] = false;
        }
    }
    ~SharedData()
    {
        delete sem;
    }
};

class Greeter: public PeriodicThread
{
public:
    Greeter(): PeriodicThread(25) { }
    void periodicActivation() override
    {
        printString("Hello world!\n");
    }
};

void studentMain()
{
    SharedData data;

    auto thread1 = new Thread([](void*)
        {
            for (int i = 0; i < 2; i++)
            {
                time_sleep(5);
                printString("Hello from A!\n");
                thread_dispatch();
            }
        }, nullptr);
    if (thread1 == nullptr) return;
    thread1->start();

    auto thread2 = new Thread([](void* ptr)
        {
            auto data = (SharedData*)ptr;
    while (!data->done[0] && !data->done[1] && !data->done[2])
    {
        printString("Thread B says hi!!!\n");
        time_sleep(5);
    };
    printString("B done here!!!\n");
        }, &data);

    if (thread2 == nullptr) return;
    thread2->start();

    auto thread3 = new Thread([](void*)
        {
            for (int i = 0; i < 3;i++)
            {
                if (i == 2) time_sleep(20);
                else time_sleep(3);
                printString("Hello from C!\n");
                thread_dispatch();
            }
        }, nullptr);

    if (thread3 == nullptr) return;
    thread3->start();

    auto threadA = new Thread([](void* arg)
        {
            auto data = (SharedData*)arg;
    for (int i = 0; i < data->cycles; i++)
    {
        data->sem->wait();
        data->arr[0] = i;
        data->sem->signal();
        thread_dispatch();
    }
    data->done[0] = true;
        }, &data);

    if (threadA == nullptr) return;
    threadA->start();

    auto threadB = new Thread([](void* arg)
        {
            auto data = (SharedData*)arg;
    for (int i = 3; i < 3 + data->cycles; i++)
    {
        data->sem->wait();
        data->arr[1] = i;
        data->sem->signal();
        thread_dispatch();
    }
    data->done[1] = true;
        }, &data);
    if (threadB == nullptr) return;
    threadB->start();

    auto threadC = new Thread([](void* arg)
        {
            auto data = (SharedData*)arg;
    for (int i = 0; i < data->cycles; i++)
    {
        data->sem->wait();
        char str[5];
        str[0] = data->arr[0] + 48;
        str[1] = ' ';
        str[2] = data->arr[1] + 48;
        str[3] = '\n';
        str[4] = '\0';
        printString(str);
        data->sem->signal();
        thread_dispatch();
    }
    data->done[2] = true;
        }, &data);
    if (threadC == nullptr) return;
    threadC->start();

    while (!data.done[0] && !data.done[1] && !data.done[2])
    {
        thread_dispatch();
    }

    char buffer[20];
    getString(buffer, 15);
    int num = stringToInt(buffer);
    num += 17;
    printString("Calculating...\n");
    Thread::sleep(25);
    printString("Result: ");
    printInt(num, 8);
    putc('\n');
    Thread::sleep(25);

    thread_t phantom;
    thread_create(&phantom, [](void* arg)
        {
            printString("Phantom starting!\n");
    Thread::sleep(25);
    printString("Phantom done!\n");
        }, nullptr);

    Greeter* greeter = new Greeter();
    greeter->start();
}