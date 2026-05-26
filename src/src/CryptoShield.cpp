#include "CryptoShield.hpp"

void CryptoShield::encrypt_weights(std::vector<float>& weights) {
    uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(weights.data());
    size_t total_bytes = weights.size() * sizeof(float);
    
    // تشفير فوري خفيف وعالي الأداء لحماية المصفوفات في الذاكرة
    for (size_t i = 0; i < total_bytes; ++i) {
        byte_ptr[i] ^= CRYPTO_KEY;
    }
}

void CryptoShield::decrypt_weights_inline(std::vector<float>& weights) {
    // عملية فك تشفير عكسية في مكانها (In-place) لتقليل الـ Overhead وحفظ الكاش
    encrypt_weights(weights); 
}