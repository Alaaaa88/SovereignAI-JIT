#include "JITMemoryManager.hpp"
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

JITMemoryManager::JITMemoryManager(size_t size) : is_executable(false) {
    size_t sys_page = sysconf(_SC_PAGESIZE);
    // محاذاة حجم الذاكرة مع حجم صفحات النظام لضمان أعلى أداء عتادي
    page_size = ((size + sys_page - 1) / sys_page) * sys_page;

    // حجز الذاكرة مع صلاحيات قراءة وكتابة مبدئياً
    page_ptr = mmap(nullptr, page_size, PROT_READ | PROT_WRITE, 
                    MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (page_ptr == MAP_FAILED) {
        throw std::runtime_error("CRITICAL: Failed to allocate JIT protected memory page via mmap.");
    }
    // تصفير الذاكرة بشكل آمن تماماً
    std::memset(page_ptr, 0, page_size);
}

void JITMemoryManager::inject_instructions(const std::vector<uint8_t>& machine_code) {
    if (is_executable) {
        throw std::runtime_error("SECURITY ERROR: Cannot inject instructions. Memory is locked as executable.");
    }
    if (machine_code.size() > page_size) {
        throw std::runtime_error("MEMORY ERROR: Machine code size exceeds allocated page boundary.");
    }
    std::memcpy(page_ptr, machine_code.data(), machine_code.size());
}

void JITMemoryManager::convert_to_executable() {
    if (is_executable) return;

    // تطبيق حماية W^X الصارمة: سحب صلاحية الكتابة وإعطاء صلاحية التنفيذ فقط
    if (mprotect(page_ptr, page_size, PROT_READ | PROT_EXEC) == -1) {
        throw std::runtime_error("SECURITY ERROR: mprotect failed to lock down JIT execution zone.");
    }
    is_executable = true;
}

JITMemoryManager::~JITMemoryManager() {
    if (page_ptr && page_ptr != MAP_FAILED) {
        // تصفير الذاكرة قبل مسحها لضمان عدم ترك أي أثر للأوزان أو البيانات في الـ RAM
        std::memset(page_ptr, 0, page_size);
        munmap(page_ptr, page_size);
    }
}