#include "AIInferenceEngine.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

// Mock function representing elite real-world network packet extraction
// In production, this ingests raw structures directly from a PCAP stream or socket
std::vector<float> capture_network_packet_features() {
    // 16 features representing normalized packet telemetry (lengths, flags, injection signatures)
    return {
        1.5f, 0.0f, 3.2f, -1.1f, 0.5f, 2.1f, 0.0f, 0.0f,
        0.8f, 1.2f, -0.4f, 5.0f, 0.0f, 1.1f, 9.4f, -2.5f
    };
}

int main() {
    std::cout << "==================================================\n";
    std::cout << "  SovereignAI-JIT Bare-Metal Inference Engine     \n";
    std::cout << "==================================================\n\n";

    const int INPUT_DIMENSION = 16;  // Fixed AVX-512 vector lane size (16 float32 elements)
    const int OUTPUT_DIMENSION = 16;

    try {
        // Initialize the runtime environment and reserve the isolated page buffer
        AIInferenceEngine engine(INPUT_DIMENSION, OUTPUT_DIMENSION);

        // Generate synthetic baseline weights for the cyber detection neural network layer
        std::vector<float> trained_weights(INPUT_DIMENSION, 0.75f);
        
        std::cout << "[*] Injecting model weights and compiling AVX-512 kernels via JIT...\n";
        engine.load_trained_model(trained_weights);
        std::cout << "[+] JIT compilation successful. W^X security boundary enforced.\n\n";

        // Ingesting live packet data
        std::vector<float> packet_features = capture_network_packet_features();

        std::cout << "[*] Executing ultra-low latency hardware inference loop...\n";
        
        // Benchmark execution clock cycles at high-resolution microsecond scale
        auto start_time = std::chrono::high_resolution_clock::now();
        
        std::vector<float> classification_output = engine.execute_inference(packet_features);
        
        auto end_time = std::chrono::high_resolution_clock::now();
        auto execution_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        // Print runtime statistics
        std::cout << "[+] Inference complete.\n";
        std::cout << "[+] Total Execution Latency: " << execution_duration << " microseconds.\n\n";

        std::cout << "--- Telemetry Classification Output Vector ---\n";
        for (int i = 0; i < OUTPUT_DIMENSION; ++i) {
            std::cout << std::fixed << std::setprecision(4) << classification_output[i] << " ";
            if ((i + 1) % 4 == 0) std::cout << "\n";
        }
        std::cout << "----------------------------------------------\n";

    } catch (const std::exception& ex) {
        std::cerr << "[-] CRITICAL RUNTIME ERROR: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}