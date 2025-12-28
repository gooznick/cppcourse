


int main()
{
    // This is the allocation (don't do this. Use vector/unique_ptr)
    int* a = new int[1];

    // This will kill the heap metadata
    for (int i = -30; i < 0; ++i) a[i] = 0;

    // This may use not allocated memory
    for (int i = 0; i < 30; ++i) a[i] = 0;

    // this is fine
    a[0] = 123321;

    // this will probably fail
    // ... but this may be very far away from the `new` command, and maybe in destructor
    //delete[] a;

    return 0;
}
