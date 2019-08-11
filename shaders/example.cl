__kernel void add_one(__global int *buffer, int n) {
    const int global_id = get_global_id(0);

    if(global_id < n)
        buffer[global_id]++;
}
