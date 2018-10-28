#ifndef ARGON2_CUDA_KERNELS_H
#define ARGON2_CUDA_KERNELS_H

#if HAVE_CUDA

#include <cuda_runtime.h>
#include <cstdint>

/* workaround weird CMake/CUDA bug: */
#ifdef argon2
#undef argon2
#endif

namespace argon2 {
namespace cuda {

class KernelRunner
{
private:
    std::uint32_t type, version;
    std::uint32_t passes, lanes, segmentBlocks;
    std::size_t batchSize;
    bool bySegment;
    bool precompute;

    cudaEvent_t start, end;
    cudaStream_t stream;
    void *memory;
    void *refs;

    void precomputeRefs();

    void runKernelSegment(std::uint32_t lanesPerBlock,
                          std::uint32_t jobsPerBlock,
                          std::uint32_t pass, std::uint32_t slice);
    void runKernelOneshot(std::uint32_t lanesPerBlock,
                          std::uint32_t jobsPerBlock);

public:
    std::uint32_t getMinLanesPerBlock() const { return bySegment ? 1 : lanes; }
    std::uint32_t getMaxLanesPerBlock() const { return lanes; }

    std::uint32_t getMinJobsPerBlock() const { return 1; }
    std::uint32_t getMaxJobsPerBlock() const { return batchSize; }

    std::size_t getBatchSize() const { return batchSize; }

    KernelRunner(std::uint32_t type, std::uint32_t version,
                 std::uint32_t passes, std::uint32_t lanes,
                 std::uint32_t segmentBlocks, std::size_t batchSize,
                 std::int32_t deviceIndex,
                 bool bySegment, bool precompute);
    ~KernelRunner();

    void writeInputMemory(std::size_t jobId, const void *buffer);
    void readOutputMemory(std::size_t jobId, void *buffer);

    void run(std::uint32_t lanesPerBlock, std::uint32_t jobsPerBlock);
    float finish();
};

} // cuda
} // argon2

#endif /* HAVE_CUDA */

#endif // ARGON2_CUDA_KERNELS_H
