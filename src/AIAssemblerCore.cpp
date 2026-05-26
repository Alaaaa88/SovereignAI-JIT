#include "AIAssemblerCore.hpp"

std::vector<uint8_t> AIAssemblerCore::generate_relu_kernel() {
    std::vector<uint8_t> machine_code;

    // x86-64 Function Prologue
    machine_code.push_back(0x55);          // push rbp
    machine_code.push_back(0x48);          // mov rbp, rsp
    machine_code.push_back(0x89); 
    machine_code.push_back(0xE5);

    // الحوسبة القياسية: rdi=input, rsi=weights, rdx=output, rcx=size
    // rax = العداد (index = 0)
    machine_code.insert(machine_code.end(), {0x48, 0x31, 0xC0});

    // --- بداية الـ Loop ---
    // مقارنة العداد بالحجم (cmp rax, rcx)
    machine_code.insert(machine_code.end(), {0x48, 0x39, 0xC1});
    machine_code.insert(machine_code.end(), {0x7D, 0x24}); // القفز للنهاية إذا انتهت المصفوفة

    // تحميل قيمة المدخل الحالي: movss xmm0, [rdi + rax*4]
    machine_code.insert(machine_code.end(), {0xF3, 0x0F, 0x10, 0x04, 0x87});

    // تحميل قيمة الوزن الحالي ومضاعفتها ضرباً: mulss xmm0, [rsi + rax*4]
    machine_code.insert(machine_code.end(), {0xF3, 0x0F, 0x59, 0x04, 0x86});

    // تصفير مسجل مؤقت للمقارنة (ReLU Matrix): xorps xmm1, xmm1
    machine_code.insert(machine_code.end(), {0x0F, 0x57, 0xC9});

    // تطبيق دالة ReLU الفكرية (max(0, value)): maxss xmm0, xmm1
    machine_code.insert(machine_code.end(), {0xF3, 0x0F, 0x5F, 0xC1});

    // حفظ النتيجة الحقيقية في مصفوفة الخرج: movss [rdx + rax*4], xmm0
    machine_code.insert(machine_code.end(), {0xF3, 0x0F, 0x11, 0x04, 0x82});

    // زيادة العداد بمقدار 1 لـ float القياسي (inc rax)
    machine_code.insert(machine_code.end(), {0x48, 0xFF, 0xC0});
    machine_code.insert(machine_code.end(), {0xEB, 0xD3}); // العودة لأول الـ Loop

    // Function Epilogue
    machine_code.push_back(0x5D);          // pop rbp
    machine_code.push_back(0xC3);          // ret

    return machine_code;
}
