//thread_local int buffer[10 * 1024 * 1024];
// readelf -lW ./a.out | Grep TLS
//  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
//   TLS            0x002df0 0x0000000000003df0 0x0000000000003df0 0x000000 0x2800000 R   0x10

thread_local int buffer[10 * 1024];
// TLS            0x002df0 0x0000000000003df0 0x0000000000003df0 0x000000 0x00a000 R   0x10   40960 = 10*1024*4
int main()
{
    return 0;
}