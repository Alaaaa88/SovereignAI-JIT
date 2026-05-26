#ifndef AI_ASSEMBLER_CORE_HPP
#define AI_ASSEMBLER_CORE_HPP
#include <vector>
#include <cstdint>
class AIAssemblerCore {
public:
    static std::vector<uint8_t> generate_relu_kernel();
};
#endif
