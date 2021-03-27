#include <thread>
#include <mutex>
#include <memory>
#include <iostream>

#define LOOP_COUNT 100000000

std::shared_ptr<int> g_pi;
std::mutex g_pi_mutex;

// Write(reset) global shared_ptr object without mutex
void TestThread1()
{
    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
        g_pi.reset(new int(i));
    }
}

// Write(operator =) global shared_ptr object without mutex
void TestThread2()
{
    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
        std::shared_ptr<int> pi(new int(i));
        g_pi = pi;
    }
}

// Write(reset) global shared_ptr object with mutex
void TestThread3()
{
    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
        std::lock_guard<std::mutex> lock(g_pi_mutex);

        g_pi.reset(new int(i));
    }
}

// Write(operator =) global shared_ptr object with mutex
void TestThread4()
{
    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
        std::lock_guard<std::mutex> lock(g_pi_mutex);

        std::shared_ptr<int> pi(new int(i));
        g_pi = pi;
    }
}

// Read(operator =) global shared_ptr object without mutex
void TestThread5()
{
    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
        std::shared_ptr<int> pi;
        pi = g_pi;
    }
}

// Read(operator =) global shared_ptr object with mutex
void TestThread6()
{
    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
        std::lock_guard<std::mutex> lock(g_pi_mutex);

        std::shared_ptr<int> pi;
        pi = g_pi;
    }
}

// Write-Write without mutex
void Test1()
{
    std::thread t1(TestThread1);
    std::thread t2(TestThread1);

    t1.join();
    t2.join();
}

// Write-Write without mutex
void Test2()
{
    std::thread t1(TestThread2);
    std::thread t2(TestThread2);

    t1.join();
    t2.join();
}

// Write-Write with mutex
void Test3()
{
    std::thread t1(TestThread3);
    std::thread t2(TestThread3);

    t1.join();
    t2.join();
}

// Write-Write with mutex
void Test4()
{
    std::thread t1(TestThread4);
    std::thread t2(TestThread4);

    t1.join();
    t2.join();
}

// Read-Write without mutex
void Test5()
{
    std::thread t1(TestThread2);
    std::thread t2(TestThread5);

    t1.join();
    t2.join();
}

// Read-Write with mutex
void Test6()
{
    std::thread t1(TestThread4);
    std::thread t2(TestThread6);

    t1.join();
    t2.join();
}

// Read-Read without mutex
void Test7()
{
    g_pi.reset(new int(1));

    std::thread t1(TestThread5);
    std::thread t2(TestThread5);

    t1.join();
    t2.join();
}

// Read-Read without mutex
void Test8()
{
    std::thread t1(TestThread5);
    std::thread t2(TestThread5);

    t1.join();
    t2.join();
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [test id 1~8]\n", argv[0]);
        printf("\n* Use AddressSanitizer or ThreadSanitizer to see detail.\n");
        return 0;
    }
    int testID = atoi(argv[1]);

    switch (testID)
    {
    case 1:
        Test1();
        break;
    case 2:
        Test2();
        break;
    case 3:
        Test3();
        break;
    case 4:
        Test4();
        break;
    case 5:
        Test5();
        break;
    case 6:
        Test6();
        break;
    case 7:
        Test7();
        break;
    case 8:
        Test8();
        break;
    }
    std::cout << "Done" << std::endl;
    return 0;
}

