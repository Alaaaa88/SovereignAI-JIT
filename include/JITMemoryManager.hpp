#ifndef JIT_MEMORY_MANAGER_HPP
#define JIT_MEMORY_MANAGER_HPP
#include <cstdint>
#include <cstddef>
#include <vector>
class JITMemoryManager {
private:
    void* page_ptr;
    size_t page_size;
    bool is_executable;
public:
    explicit JITMemoryManager(size_t size);
    void inject_instructions(const std::vector<uint8_t>& machine_code);
    void convert_to_executable();
    template<typename FunctionSignature>
    FunctionSignature get_function_pointer() { return reinterpret_cast<FunctionSignature>(page_ptr); }
    ~JITMemoryManager();
};
#endif
