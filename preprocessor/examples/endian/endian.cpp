#include <iostream>
#include <boost/detail/endian.hpp>

// Windows :
// #pragma message("The use of BOOST_*_ENDIAN and BOOST_BYTE_ORDER is deprecated. Please include <boost/predef/other/endian.h> and use BOOST_ENDIAN_*_BYTE instead")

// Build started...
// 1>------ Build started: Project: Project1, Configuration: Debug x64 ------
// 1>main.cpp
// 1>The use of BOOST_*_ENDIAN and BOOST_BYTE_ORDER is deprecated. Please include <boost/predef/other/endian.h> and use BOOST_ENDIAN_*_BYTE instead
// 1>Project1.vcxproj -> C:\Users\admin\source\repos\Project1\x64\Debug\Project1.exe
// ========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
// ========== Build started at 4:59 PM and took 01.359 seconds ==========

int main()
{
    #if BOOST_BIG_ENDIAN
    std::cout<<"hello world (BOOST_BIG_ENDIAN)"<<std::endl;
    #else
    std::cout<<"dlrow olleh (BOOST_LITTLE_ENDIAN)"<<std::endl;
    #endif
    return 0;
}