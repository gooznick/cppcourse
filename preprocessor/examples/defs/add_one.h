#pragma once

struct Data
{
#ifdef ADD_ONE_DEBUG
    char debug_string[20];
#endif
    
    int value;
};

void add_one(Data d);