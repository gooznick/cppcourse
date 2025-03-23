int mul(int,int);

int pow(int a, int b)
{
    int res(1);
    for (int i = 0; i < b; i++)
    {
        res = mul(res, a);
    }
    return res;
}