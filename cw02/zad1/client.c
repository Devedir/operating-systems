#include <stdio.h>
#ifndef DYNAMIC
    #include "collatz.h"
#else
    #include <dlfcn.h>
#endif

#define MAX_ITER 10000

int main(void) {
#ifdef DYNAMIC
    void* handle = dlopen("./libcollatz.so", RTLD_LAZY);
    char* error = dlerror();
    if (!handle) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }
    int (*test_collatz_convergence)(int, int) = dlsym(handle, "test_collatz_convergence");
    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        dlclose(handle);
        return 2;
    }
#endif

    printf("Dla 2137: %d\n", test_collatz_convergence(2137, MAX_ITER));
    printf("Dla 321: %d\n", test_collatz_convergence(321, MAX_ITER));

#ifdef DYNAMIC
    dlclose(handle);
#endif

    return 0;
}