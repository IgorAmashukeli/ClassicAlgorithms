#include <bits/stdc++.h>
#include <codecvt>
#include <locale>
#include <cassert>
#include "UTF8.h"  // your library with Encode/Decode

// ======================== Helpers ========================
char32_t RandomCodepoint(std::mt19937 &rng) {
    std::uniform_int_distribution<int> dist(0, 0x10FFFF);
    while (true) {
        char32_t cp = dist(rng);
        // skip surrogates
        if ((cp >= 0xD800 && cp <= 0xDFFF) || cp > 0x10FFFF) {
            continue;
        }
        return cp;
    }
}

// DEPRECATED TESTS FROM C++17 - WILL BE REMOVED IN C++26
std::string EncodeWithCodecvt(const std::u32string &u32s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(u32s);
}

std::u32string DecodeWithCodecvt(const std::string &s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(s);
}

std::u32string GenerateRandomU32(std::mt19937 &rng, int max_len) {
    std::uniform_int_distribution<int> len_dist(1, max_len);
    int len = len_dist(rng);

    std::u32string u32s;
    u32s.reserve(len);
    for (int i = 0; i < len; i++) {
        u32s.push_back(RandomCodepoint(rng));
    }
    return u32s;
}

void TestDecAgainstCodecvt(const std::string &input) {
    std::u32string encoded_lib = UTF8::Decode(input);
    std::u32string encoded_std = DecodeWithCodecvt(input);
    assert(encoded_lib == encoded_std);
}

// ======================== Tests ========================
void TestRoundtrip(const std::u32string &input) {
    std::string encoded = UTF8::Encode(input);
    TestDecAgainstCodecvt(encoded);
    std::u32string decoded = UTF8::Decode(encoded);
    assert(input == decoded);
}

void TestEncAgainstCodecvt(const std::u32string &input) {
    std::string encoded_lib = UTF8::Encode(input);
    std::string encoded_std = EncodeWithCodecvt(input);
    assert(encoded_lib == encoded_std);
}

void StressTest(int num_tests, int max_len) {
    std::mt19937 rng(
        static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

    for (int t = 0; t < num_tests; t++) {
        std::u32string original = GenerateRandomU32(rng, max_len);
        TestRoundtrip(original);
        TestEncAgainstCodecvt(original);
    }

    std::cout << "✅ All " << num_tests << " stress tests passed\n";
}

// ======================== Main ========================
int main() {
    StressTest(50000, 2000);
    return 0;
}
