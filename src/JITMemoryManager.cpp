#include "JITMemoryManager.hpp"
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
JITMemoryManager::JITMemoryManager(size_t size) : page_ptr(nullptr), page_size(0), is_executable(false) {
    size_t sys_page = sysconf(_SC_PAGESIZE);
    page_size = ((size + sys_page - 1) / sys_page) * sys_page;
    page_ptr = mmap(nullptr, page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (page_ptr == MAP_FAILED) throw std::runtime_error("SYSTEM ERROR: mmap allocation failed.");
    std::memset(page_ptr, 0, page_size);
}
void JITMemoryManager::inject_instructions(const std::vector<uint8_t>& machine_code) {
    if (is_executable) throw std::runtime_error("SECURITY CRITICAL: Attempted write operation.");
    if (machine_code.size() > page_size) throw std::runtime_error("CRITICAL BOUNDS: Payload overflow.");
    std::memcpy(page_ptr, machine_code.data(), machine_code.size());
}
void JITMemoryManager::convert_to_executable() {
    if (is_executable) return;
    if (mprotect(page_ptr, page_size, PROT_READ | PROT_EXEC) == -1) throw std::runtime_error("SECURITY ERROR: mprotect failed.");
    is_executable = true;
}
JITMemoryManager::~JITMemoryManager() {
    if (page_ptr && page_ptr != MAP_FAILED) { std::memset(page_ptr, 0, page_size); munmap(page_ptr, page_size); }
}
