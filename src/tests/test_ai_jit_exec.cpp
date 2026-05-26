#include "JITMemoryManager.hpp"
#include <iostream>
#include <vector>
#include <cassert>

void run_integrity_unit_test() {
    std::cout << "[*] Initializing JIT Memory Manager safety assertion test...\n";

    // Request a standard single page block
    JITMemoryManager test_allocator(1024);

    // Simple x86-64 machine code routine: returns integer value 42
    // mov eax, 42; ret
    std::vector<uint8_t> mock_kernel_code = { 0xB8, 0x2A, 0x00, 0x00, 0x00, 0xC3 };

    // Test phase 1: Instruction Injection
    test_allocator.inject_instructions(mock_kernel_code);
    
    // Test phase 2: Memory state lock down
    test_allocator.convert_to_executable();

    // Test phase 3: Runtime execution pointer jump assertion
    typedef int (*mock_fn)();
    mock_fn execute_test_loop = test_allocator.get_function_pointer<mock_fn>();

    int resulting_value = execute_test_loop();
    
    std::cout << "[*] Returned runtime value from memory: " << resulting_value << "\n";
    assert(resulting_value == 42);

    std::cout << "[+] Pass: Memory transition and execution pointer assertion verified.\n";
}

int main() {
    std::cout << "=== Running SovereignAI-JIT Automated Testing Suite ===\n";
    try {
        run_integrity_unit_test();
        std::cout << "[SUCCESS] All bare-metal execution gates passed natively.\n";
    } catch (const std::exception& error) {
        std::cerr << "[FAILURE] Test suite failed execution: " << error.what() << "\n";
        return 1;
    }
    return 0;
}