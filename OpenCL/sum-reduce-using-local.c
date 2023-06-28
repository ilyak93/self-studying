
#define SCRATCH_SIZE 1024

__kernel void sum_by_reduction_not_optimal(__global float* in, __global float* out, uint32_t len) {

    __local float scratch[SCRATCH_SIZE];

    size_t global_idx = get_global_id(0);
    size_t local_idx = get_local_id(0);
    size_t group_size = get_local_size(0);

    if (global_idx >= len) return;

    // Perform a local operation on the input data using local indices
    scratch[local_idx] = in[global_idx];

    // Synchronize threads within the work-group to ensure all local data is available
    barrier(CLK_LOCAL_MEM_FENCE);

    // Perform a reduction operation on the local data
    for (size_t stride = group_size / 2; stride > 0; stride /= 2) {
        if (local_idx < stride) {
            scratch[local_idx] += scratch[local_idx + stride];
        }

        // Synchronize threads within the work-group to ensure all partial sums are computed
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Write the result of the reduction to the output buffer
    if (local_idx == 0) {
        out[global_idx / group_size] = scratch[0];
    }
    # The result is that out has group_size / get_global_size elements which are the reducted sums
    # of the local sub-parts of the whole in array (its local scratches)
    # this isn't very efficient because now local can't be used for aggregating the results into one sum,
    # and global should be used for that or at least accessed another time, which is costly.
    # in this case it looks like it is better to fit the work-group into the global size, 
    # which will allow many writes into local memory, and only access of write into global memory out.	
	# This implementation is below.
}

#define SCRATCH_SIZE 1024

__kernel void sum_by_reduction_optimal(__global float* in, __global float* out, uint32_t len) {

    __local float scratch[SCRATCH_SIZE];

    size_t global_idx = get_global_id(0);
    size_t local_idx = get_local_id(0);
    size_t group_size = get_local_size(0);

    if (global_idx >= len) return;

    // Perform a local operation on the input data using local indices
    scratch[global_idx] = in[global_idx];

    // Synchronize threads within the work-group to ensure all local data is available
    barrier(CLK_LOCAL_MEM_FENCE);

    // Perform a reduction operation on the local data
    for (size_t stride = group_size / 2; stride > 0; stride /= 2) {
        if (local_idx < stride) {
            scratch[local_idx] += scratch[local_idx + stride];
        }

        // Synchronize threads within the work-group to ensure all partial sums are computed
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Write the result of the reduction to the output buffer
    if (local_idx == 0) {
        out[global_idx / group_size] = scratch[0];
    }
    # The result is that out has group_size / get_global_size elements which are the reducted sums
    # of the local sub-parts of the whole in array (its local scratches)
    # this isn't very efficient because now local can't be used for aggregating the results into one sum,
    # and global should be used for that or at least accessed another time, which is costly.
    # in this case it looks like it is better to fit the work-group into the global size, 
    # which will allow many writes into local memory, and only access of write into global memory out.	
	# This is done in the previous implementation.
}