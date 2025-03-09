#pragma once

#include "a.h"


inline int b() {
    return 7;
}


inline int sum_a_and_b() {
    return b()+a();
}