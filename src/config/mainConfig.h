#pragma once

#if __has_include("config.h")
    #include "config.h"
#elif __has_include("config.example.h")
    #include "config.example.h"
#else
    #error "No config.h or config.example.h found"
#endif