int add(int,int);

int mul(int a, int b)
{
    int res(0);
    for (int i = 0; i < a; i++)
    {
        res = add(res, b);
    }
    return res;
}