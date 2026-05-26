#ifndef CRYPTO_SHIELD_HPP
#define CRYPTO_SHIELD_HPP
#include <vector>
#include <cstdint>
class CryptoShield {
private:
    static const uint8_t CRYPTO_KEY = 0x5A; 
public:
    static void encrypt_weights(std::vector<float>& weights);
    static void decrypt_weights_inline(std::vector<float>& weights);
};
#endif
