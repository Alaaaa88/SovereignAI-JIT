#include "AIInferenceEngine.hpp"
#include "CryptoShield.hpp"
#include "AIAssemblerCore.hpp"
AIInferenceEngine::AIInferenceEngine(int input_size, int output_size) : jit_allocator(4096), input_dim(input_size), output_dim(output_size) {}
void AIInferenceEngine::load_trained_model(const std::vector<float>& raw_weights) {
    encrypted_model_weights = raw_weights;
    CryptoShield::encrypt_weights(encrypted_model_weights);
    std::vector<uint8_t> native_opcodes = AIAssemblerCore::generate_relu_kernel();
    jit_allocator.inject_instructions(native_opcodes);
    jit_allocator.convert_to_executable();
}
std::vector<float> AIInferenceEngine::execute_inference(const std::vector<float>& input_features) {
    std::vector<float> active_parameters = encrypted_model_weights;
    CryptoShield::decrypt_weights_inline(active_parameters);
    typedef void (*jit_matrix_kernel)(const float*, const float*, float*, int);
    jit_matrix_kernel compute_pass = jit_allocator.get_function_pointer<jit_matrix_kernel>();
    std::vector<float> inference_results(output_dim, 0.0f);
    compute_pass(input_features.data(), active_parameters.data(), inference_results.data(), input_dim);
    return inference_results;
}
