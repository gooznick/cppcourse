int GiveMeFive();
int Add(int,int);

int TheAnswer()
{
    return 42;
}

int main()
{
    return TheAnswer()+GiveMeFive()+ Add(12345,54321);
}