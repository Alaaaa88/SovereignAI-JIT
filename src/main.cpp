#include "AIInferenceEngine.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
int main() {
    std::cout << "==================================================\n";
    std::cout << "  SovereignAI-JIT Bare-Metal Inference Engine     \n";
    std::cout << "==================================================\n\n";
    try {
        AIInferenceEngine engine(16, 16);
        std::vector<float> trained_weights(16, 0.50f);
        std::cout << "[*] Compiling hardware-level AVX-512 kernels via JIT...\n";
        engine.load_trained_model(trained_weights);
        std::cout << "[+] JIT Memory Protection Enabled (W^X Paradigm Active).\n\n";
        std::vector<float> network_packet = { 2.0f, -0.5f, 4.0f, 1.5f, 0.0f, 8.2f, -1.1f, 3.0f, 0.5f, 1.2f, 6.0f, -2.0f, 1.1f, 0.0f, 10.5f, 5.0f };
        std::cout << "[*] Executing real-time inference directly on CPU registers...\n";
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<float> results = engine.execute_inference(network_packet);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "[+] Process Optimized Successfully.\n";
        std::cout << "[+] Real Latency Performance: " << duration << " Microseconds.\n\n";
        std::cout << "--- Real Classification Output Vectors (Bare-Metal Yield) ---\n";
        for (int i = 0; i < 16; ++i) {
            std::cout << std::fixed << std::setprecision(4) << results[i] << "  ";
            if ((i + 1) % 4 == 0) std::cout << "\n";
        }
        std::cout << "------------------------------------------------------------\n";
    } catch (const std::exception& error) {
        std::cerr << "[-] RUNTIME FATAL: " << error.what() << "\n";
        return 1;
    }
    return 0;
}
