#include <stdio.h>

#ifdef __APPLE__
    #include <OpenCL/cl.h>
#else
    #include <CL/cl.h>
#endif

int main(void) {

    cl_int err;
    cl_uint numPlatforms;

    err = clGetPlatformIDs(0, NULL, &numPlatforms);

    if (CL_SUCCESS == err) {
        printf("Found %d OpenCL platform(s)\n", numPlatforms);
    } else {
        printf("Error calling clGetPlatformIDs. Error code: %d\n", err);
    }

    return 0;

}
