
__kernel void mulMatrices(  __global const float* inputA,
                            __global const float* inputB,
                            __global float* output) {

    int i = get_global_id(0);
    int j = get_global_id(1);

    int width = get_global_size(0);
    int height = get_global_size(1);

    int index = j * width + i;
    output[index] = 0;

    for (int k = 0; k < width; k++) {
      output[index] += inputA[j * width + k] * inputB[k * width + i];
    }

}
