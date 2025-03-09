#pragma GCC poison class1

int class1()
{
    return 42;
}

int main()
{
    return class1();
}