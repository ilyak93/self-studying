
#define SCRATCH_SIZE 1024

//in case |work-group| == |ndrange| and the expected result's shape is (1,)
__kernel void sum_by_reduction(__global float* in, __global float* out, uint32_t len) {

    __local float scratch[SCRATCH_SIZE];
	
	assert(get_local_size() == get_global_size() == SCRATCH_SIZE);

    size_t global_idx = get_global_id(0);

    if (global_idx >= len) return;

    // Perform a local operation on the input data
    scratch[global_idx] = in[global_idx];

    // Synchronize threads within the work-group to ensure all local data is available
    barrier(CLK_LOCAL_MEM_FENCE);

    // Perform a reduction operation on the local data
    for (size_t stride = get_global_size(0) / 2; stride > 0; stride /= 2) {
        if (global_idx < stride) {
            scratch[global_idx] += scratch[global_idx + stride];
        }

        // Synchronize threads within the work-group to ensure all partial sums are computed
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    // Write the result of the reduction to the output buffer
    if (global_idx == 0) {
        out[global_idx] = scratch[0];
    }
}

#define SCRATCH_SIZE 1024

//in case |work-group| == |thread_block| and the expected result is a vector of size global_size / group_size
__kernel void sum_by_reduction_many(__global float* in, __global float* out, uint32_t len) {

    __local float scratch[SCRATCH_SIZE];

    size_t global_idx = get_global_id(0);
    size_t local_idx = get_local_id(0);
    size_t group_size = get_local_size(0);
	
	assert(get_local_size() == SCRATCH_SIZE);

    if (global_idx >= len) return;

    // Perform a local operation on the input data using local indices - presrves the sub-global-group order ***
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
}
	
	// explanation for ***
	/*
	
	Global ID 0 corresponds to Local ID 0 in the first work group.

    Global ID 1 corresponds to Local ID 1 in the first work group.

    Global ID 2 corresponds to Local ID 2 in the first work group.

    ...

    Global ID 255 corresponds to Local ID 255 in the first work group.

    Global ID 256 corresponds to Local ID 0 in the second work group.

    Global ID 257 corresponds to Local ID 1 in the second work group.

    Global ID 258 corresponds to Local ID 2 in the second work group.

    ...

    Global ID 511 corresponds to Local ID 255 in the second work group.

    Global ID 512 corresponds to Local ID 0 in the third work group.

    Global ID 513 corresponds to Local ID 1 in the third work group.

    Global ID 514 corresponds to Local ID 2 in the third work group.

    ...

    Global ID 767 corresponds to Local ID 255 in the third work group.

    Global ID 768 corresponds to Local ID 0 in the fourth work group.

    Global ID 769 corresponds to Local ID 1 in the fourth work group.

    Global ID 770 corresponds to Local ID 2 in the fourth work group.

    ...

    Global ID 1023 corresponds to Local ID 255 in the fourth work group.
	*/
	

#define SCRATCH_SIZE 1024

// a generalization of previous both, works for both cases without pre-assumptions
__kernel void sum_by_reduction_gen(__global float* in, __global float* out, uint32_t len) {

    __local float scratch[SCRATCH_SIZE];

    size_t local_idx = get_local_id(0);
    size_t global_size = get_global_size(0);
    size_t global_offset = get_global_offset(0);

    assert(get_local_size() == SCRATCH_SIZE);

    // Perform multiple iterations to process the entire input data
    for (size_t index = global_offset + local_idx; index < len; index += global_size) {

        // Check if the current index is within the valid range
        if (index < len) {
            // Perform a local operation on the input data using local indices - preserves the sub-global-group order ***
            scratch[local_idx] = in[index];
        }
        else {
            // Set an identity value for out-of-range indices
            scratch[local_idx] = 0.0f;
        }

        // Synchronize threads within the work-group to ensure all local data is available
        barrier(CLK_LOCAL_MEM_FENCE);

        // Perform a reduction operation on the local data
        for (size_t stride = global_size / 2; stride > 0; stride /= 2) {
            if (local_idx < stride) {
                scratch[local_idx] += scratch[local_idx + stride];
            }

            // Synchronize threads within the work-group to ensure all partial sums are computed
            barrier(CLK_LOCAL_MEM_FENCE);
        }

        // Write the result of the reduction to the output buffer
        if (local_idx == 0 && index < len) {
            out[index / global_size] = scratch[0];
        }
    }
}

// if we desire the sum of a huge number of elements which can't fit even one reduce, 
// more then one read and write from global memory is inevitable because in that case
// one use of local memory isn't enough and the global memory always will serve as a bridge,
// to aggregate the local results and then continue again work on them, untill all are processed,
// i.e reduced to one result. In the above case we will have a vector result of size: global size / local size.
// Although in many case this is not the usual flow of GPU processing and the casual use-case is
// when the tasks are data-separbale and each data-bulk can be independtly processed. 

// But if we face a such situation, there are another approaches like producer-consumer to process 
// more data then a global memory can fit in once.