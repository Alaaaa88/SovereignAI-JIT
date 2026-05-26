#include "AIInferenceEngine.hpp"
#include "CryptoShield.hpp"
#include "AIAssemblerCore.hpp"
#include <iostream>

AIInferenceEngine::AIInferenceEngine(int input_size, int output_size)
    : jit_allocator(4096), input_dim(input_size), output_dim(output_size) {}

void AIInferenceEngine::load_trained_model(const std::vector<float>& raw_weights) {
    encrypted_model_weights = raw_weights;
    // حماية الأوزان فور تحميلها في الذاكرة
    CryptoShield::encrypt_weights(encrypted_model_weights);

    // توليد كود الآلة (OpCodes) الخاص بمعالجة المصفوفات و الـ ReLU
    // سنقوم ببرمجة مولد البايتات بالتفصيل في AIAssemblerCore
    std::vector<uint8_t> generated_code = AIAssemblerCore::generate_relu_kernel();

    // حقن الكود وتأمين صفحة الذاكرة وتحويلها للتنفيذ
    jit_allocator.inject_instructions(generated_code);
    jit_allocator.convert_to_executable();
}

std::vector<float> AIInferenceEngine::execute_inference(const std::vector<float>& input_features) {
    // فك تشفير مؤقت للأوزان لتمريرها للمعالج
    std::vector<float> active_weights = encrypted_model_weights;
    CryptoShield::decrypt_weights_inline(active_weights);

    // استدعاء مؤشر الدالة من صفحة الذاكرة التي تم توليدها بالـ JIT
    typedef void (*jit_kernel_fn)(const float*, const float*, float*, int);
    jit_kernel_fn run_kernel = jit_allocator.get_function_pointer<jit_kernel_fn>();

    std::vector<float> output_predictions(output_dim, 0.0f);

    // تشغيل الكود المولد عتادياً مباشرة لتصنيف بيانات حزم الشبكة الحقيقية
    run_kernel(input_features.data(), active_weights.data(), output_predictions.data(), input_dim);

    return output_predictions;
}