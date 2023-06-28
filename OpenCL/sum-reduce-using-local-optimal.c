#define SCRATCH_SIZE 1024

__kernel void sum_by_reduction(__global float* in, __global float* out, uint32_t len) {

    __local float scratch[SCRATCH_SIZE];
	
	assert(get_local_size() == get_global_size() == SCRATCH_SIZE);
	
	# Not in every case this is the best option, but in this case it is optimal, as
	# reduction need to perofrm many read and write access, which is faster on local memory,
	# and we need as the output only one value (total sum), which can be written in the end to
	# the global out memory, thus accessing it only ones in the end (and once for reading in the
	# beginning).

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