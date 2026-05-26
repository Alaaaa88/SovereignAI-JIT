#include "CryptoShield.hpp"
void CryptoShield::encrypt_weights(std::vector<float>& weights) {
    uint8_t* raw_byte_pipeline = reinterpret_cast<uint8_t*>(weights.data());
    size_t total_payload_bytes = weights.size() * sizeof(float);
    for (size_t byte_idx = 0; byte_idx < total_payload_bytes; ++byte_idx) {
        raw_byte_pipeline[byte_idx] ^= CRYPTO_KEY;
    }
}
void CryptoShield::decrypt_weights_inline(std::vector<float>& weights) { encrypt_weights(weights); }
