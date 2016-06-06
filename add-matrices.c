#include <stdio.h>
#include <float.h>

#ifdef __APPLE__
    #include <OpenCL/cl.h>
#else
    #include <CL/cl.h>
#endif

#define WIDTH 700
#define HEIGHT 700
#define NUM_ELEMENTS (WIDTH * HEIGHT)

#define PERFORM(fn) { \
    cl_int error = (fn); \
    if (error != CL_SUCCESS) { \
        printf("An error occured on line %d. Error code : %d\n", __LINE__, error); \
        exit(1); \
    } \
}

char* readKernelSourceCode(const char *filename) {

    FILE* file = fopen(filename, "r");

    if (!file) {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char* program = (char*)malloc(size + 1);
    fread(program, sizeof(char), size, file);
    program[size] = '\0';
    fclose(file);

    return program;

}

void fillArray(cl_float *array, size_t size) {
    for (int i = 0; i < size; i += 1) {
        array[i] = (cl_float)rand() / RAND_MAX;
    }
}

int main() {

    cl_int status;

    cl_platform_id platform;
    PERFORM(clGetPlatformIDs(1, &platform, NULL));

    cl_device_id device;
    PERFORM(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL));

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
    PERFORM(status);

    cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &status);
    PERFORM(status);

    char* source = readKernelSourceCode("add-matrices.cl");
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
    PERFORM(status);
    free(source);

    PERFORM(clBuildProgram(program, 0, NULL, NULL, NULL, NULL));

    cl_kernel kernel = clCreateKernel(program, "addMatrices", &status);
    PERFORM(status);

    cl_float a[NUM_ELEMENTS], b[NUM_ELEMENTS];
    fillArray(a, NUM_ELEMENTS);
    fillArray(b, NUM_ELEMENTS);

    cl_mem inputA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NUM_ELEMENTS, a, &status);
    PERFORM(status);

    cl_mem inputB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * NUM_ELEMENTS, b, &status);
    PERFORM(status);

    cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * NUM_ELEMENTS, NULL, &status);
    PERFORM(status);

    PERFORM(clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputA));
    PERFORM(clSetKernelArg(kernel, 1, sizeof(cl_mem), &inputB));
    PERFORM(clSetKernelArg(kernel, 2, sizeof(cl_mem), &output));

    cl_event profiler_events;
    size_t work_units[] = {WIDTH, HEIGHT};
    PERFORM(clEnqueueNDRangeKernel(queue, kernel, 2, NULL, work_units, NULL, 0, NULL, &profiler_events));

    cl_float results[NUM_ELEMENTS];
    PERFORM(clEnqueueReadBuffer(queue, output, CL_TRUE, 0, sizeof(cl_float) * NUM_ELEMENTS, results, 0, NULL, NULL));

    cl_ulong starttime;
    PERFORM(clGetEventProfilingInfo(profiler_events, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &starttime, NULL));

    cl_ulong endtime;
    PERFORM(clGetEventProfilingInfo(profiler_events, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &endtime, NULL));

    printf("Elapsed time: %lu us\n", (unsigned long)(endtime - starttime) / 1000);

    PERFORM(clReleaseMemObject(inputA));
    PERFORM(clReleaseMemObject(inputB));
    PERFORM(clReleaseMemObject(output));

    PERFORM(clReleaseKernel(kernel));
    PERFORM(clReleaseProgram(program));
    PERFORM(clReleaseContext(context));
    PERFORM(clReleaseCommandQueue(queue));

    return 0;

}
