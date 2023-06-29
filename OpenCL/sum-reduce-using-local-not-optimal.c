
#define SCRATCH_SIZE 1024

__kernel void sum_by_reduction_many_sums(__global float* in, __global float* out, uint32_t len) {

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

	# The result is that out has group_size / get_global_size elements which are the reducted sums
    # of the local sub-parts of the whole in array (its local scratches)
    # this isn't very efficient because now local can't be used for aggregating the results into one sum,
    # and global should be used for that or at least accessed another time, which is costly.
    # in this case it looks like it is better to fit the work-group into the global size, 
    # which will allow many writes into local memory, and only access of write into global memory out.
	# This is true for the case that for example we have only one image of size 1024 lets say, 
	# and we need to compute only one sum for example, and we think about the work-group as a warp in the GPU
	# (as in the youtube video of AG Guillon).
	
	# this code will be correct for use if for exmaple we want to think about work-group as a thread_block
	# in the GPU, and lets say we have to invoke many thread_blocks (we have in array size as 65536 = 64 * 1024) 
	# and use in each one local memory, for computing one sum of eahc 1024 sized bulk of values, 
	# and we know priorly that our result will end in independent 64 sums which we don't mind to keep 
	# in that shape (1, 64). Then this code makes perfect sense.
	
	
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