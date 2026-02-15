
#include <cstring>
#include <cstdlib>

int main()
{
    size_t Count = size_t(5) * 1024 * 1024 * 1024; // 5 Gb
    char *array = (char *)malloc(Count);
    memset(array, 0, Count);

    int total(0);
    int index = 0; // Oops !
    for (size_t i = 0; i != Count; i++)
    {
        array[index++] = char(i) | 1;
        total +=array[index];
    }
    return total;
}