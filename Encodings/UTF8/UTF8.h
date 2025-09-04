#pragma once
#include <cstdint>
#include <string>

// UTF8 Encoder and Decoder
namespace utf8 {
// Forward declaration
struct DecodeResult;

// Uses Replacement Character for Invalid Char32
std::string Encode(std::u32string_view s);

// Uses Maximal Subpart Replacement politics
std::u32string Decode(std::string_view s);

// Uses Replacement Character for Invalid Char32
std::string Encode(std::initializer_list<char32_t> il);
// Uses Maximal Subpart Replacement Politic
std::u32string Decode(std::initializer_list<char> il);

char32_t GetReplacementCharacter();

};  // namespace utf8
