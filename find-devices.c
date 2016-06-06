#include <stdio.h>
#include <stdarg.h>

#ifdef __APPLE__
    #include <OpenCL/cl.h>
#else
    #include <CL/cl.h>
#endif

#define PERFORM(fn) { \
    cl_int error = (fn); \
    if (error != CL_SUCCESS) { \
        printf("An error occured on line %d. Error code : %d\n", __LINE__, error); \
        exit(1); \
    } \
}

void printDeviceParameterString(cl_device_id device, cl_device_info param_id, const char* param_name) {

    char value[1024];
    PERFORM(clGetDeviceInfo(device, param_id, sizeof(value), value, NULL));

    printf("%s: %s\n", param_name, value);

}

void printDeviceParameterUInt(cl_device_id device, cl_device_info param_id, const char* param_name) {

    cl_uint value;
    PERFORM(clGetDeviceInfo(device, param_id, sizeof(value), &value, NULL));

    printf("%s: %u\n", param_name, value);

}

void printDeviceParameterULong(cl_device_id device, cl_device_info param_id, const char* param_name) {

    cl_ulong value;
    PERFORM(clGetDeviceInfo(device, param_id, sizeof(value), &value, NULL));

    printf("%s: %lu\n", param_name, (unsigned long)value);

}

void printDeviceParameterSize_t(cl_device_id device, cl_device_info param_id, const char* param_name) {

    size_t value;
    PERFORM(clGetDeviceInfo(device, param_id, sizeof(value), &value, NULL));

    printf("%s: %zd\n", param_name, value);

}

void printDeviceInformation(cl_device_id device) {

    printDeviceParameterString(device, CL_DEVICE_NAME, "Name");
    printDeviceParameterString(device, CL_DEVICE_VENDOR, "Vendor");
    printDeviceParameterUInt(device, CL_DEVICE_MAX_COMPUTE_UNITS, "Compute Units");
    printDeviceParameterULong(device, CL_DEVICE_GLOBAL_MEM_SIZE, "Global Memory");
    printDeviceParameterULong(device, CL_DEVICE_LOCAL_MEM_SIZE, "Local Memory");
    printDeviceParameterSize_t(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, "Workgroup size");

}

void printPlatformParameter(cl_platform_id platform, cl_platform_info param_id, const char* param_name) {

    char value[1024];
    PERFORM(clGetPlatformInfo(platform, param_id, sizeof(value), value, NULL));

    printf("%s: %s\n", param_name, value);

}

void printPlatformInformation(cl_platform_id platform) {

    printPlatformParameter(platform, CL_PLATFORM_NAME, "Name");
    printPlatformParameter(platform, CL_PLATFORM_VENDOR, "Vendor");

    cl_uint num_devices;
    PERFORM(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices));

    cl_device_id* devices = (cl_device_id*)malloc(sizeof(cl_device_id) * num_devices);
    PERFORM(clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL));

    printf("\nFound %u device(s)\n", num_devices);

    for (int i = 0; i < num_devices; i += 1) {
        printf("\nDevice %d\n", i);
        printDeviceInformation(devices[i]);
    }

}

int main() {

    cl_uint num_platforms;

    PERFORM(clGetPlatformIDs(0, NULL, &num_platforms));

    cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platforms);

    PERFORM(clGetPlatformIDs(num_platforms, platforms, NULL));

    printf("Found %d OpenCL platform(s)\n\n", num_platforms);

    for (int i = 0; i < num_platforms; i += 1) {
        printf("Platform %d\n", i);
        printPlatformInformation(platforms[i]);
    }

}
