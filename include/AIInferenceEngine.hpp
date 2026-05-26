#ifndef AI_INFERENCE_ENGINE_HPP
#define AI_INFERENCE_ENGINE_HPP
#include "JITMemoryManager.hpp"
#include <vector>
class AIInferenceEngine {
private:
    JITMemoryManager jit_allocator;
    std::vector<float> encrypted_model_weights;
    int input_dim;
    int output_dim;
public:
    AIInferenceEngine(int input_size, int output_size);
    void load_trained_model(const std::vector<float>& raw_weights);
    std::vector<float> execute_inference(const std::vector<float>& input_features);
};
#endif
