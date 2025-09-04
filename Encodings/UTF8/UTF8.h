#pragma once
#include <cstdint>
#include <string>
#include <iostream>

struct UTF8 {
    static std::string Encode(std::u32string_view s) {
        std::cout << "cheap\n";
        std::string result;
        for (size_t i = 0; i < s.size(); ++i) {
            if (IsInvalid(s[i])) {
                result += EncodeCodePoint(kReplacementCharacter);
            } else {
                result += EncodeCodePoint(s[i]);
            }
        }
        return result;
    }

    static std::u32string Decode(std::string_view s) {
        std::u32string result;
        for (size_t i = 0; i < s.size(); ++i) {
            if (IsUTF8OneByte(s[i])) {
                result.push_back(CreateFromOneByte(s[i]));
            } else if (i + 1 < s.size() && IsUTF8TwoBytes(s[i], s[i + 1])) {
                result.push_back(CreateFromTwoBytes(s[i], s[i + 1]));
                i += 1;
            } else if (i + 2 < s.size() && IsUTF8ThreeBytes(s[i], s[i + 1], s[i + 2])) {
                result.push_back(CreateFromThreeBytes(s[i], s[i + 1], s[i + 2]));
                i += 2;
            } else if (i + 3 < s.size() && IsUTF8FourBytes(s[i], s[i + 1], s[i + 2], s[i + 3])) {
                result.push_back(CreateFromFourBytes(s[i], s[i + 1], s[i + 2], s[i + 3]));
                i += 3;
            } else {
                result.push_back(kReplacementCharacter);
                i += (FindShift(s, i) - 1);
            }
        }
        return result;
    }

    static std::string Encode(std::initializer_list<char32_t> il) {
        return Encode(std::u32string(il));
    }
    static std::u32string Decode(std::initializer_list<char> il) {
        return Decode(std::string(il));
    }

private:
    static char32_t CreateFromOneByte(char ch_x) {
        uint8_t x = static_cast<uint8_t>(ch_x);
        uint32_t res = static_cast<uint32_t>(x);
        if (IsInvalid(res)) {
            return kReplacementCharacter;
        }
        return static_cast<char32_t>(res);
    }

    static char32_t CreateFromTwoBytes(char ch_x, char ch_y) {
        uint8_t x = static_cast<uint8_t>(ch_x) & kStartTwoMask;
        uint8_t y = static_cast<uint8_t>(ch_y) & kAddMask;
        uint32_t res = (static_cast<uint32_t>(x) << kShiftOne) | (static_cast<uint32_t>(y));
        if (IsInvalid(res) || IsOverLong(res, 2)) {
            return kReplacementCharacter;
        }
        return static_cast<char32_t>(res);
    }

    static char32_t CreateFromThreeBytes(char ch_x, char ch_y, char ch_z) {
        uint8_t x = static_cast<uint8_t>(ch_x) & kStartThreeMask;
        uint8_t y = static_cast<uint8_t>(ch_y) & kAddMask;
        uint8_t z = static_cast<uint8_t>(ch_z) & kAddMask;
        uint32_t res = (static_cast<uint32_t>(x) << kShiftTwo) |
                       (static_cast<uint32_t>(y) << kShiftOne) | static_cast<uint32_t>(z);
        if (IsInvalid(res) || IsOverLong(res, 3)) {
            return kReplacementCharacter;
        }
        return static_cast<char32_t>(res);
    }

    static char32_t CreateFromFourBytes(char ch_x, char ch_y, char ch_z, char ch_u) {
        uint8_t x = static_cast<uint8_t>(ch_x) & kStartFourMask;
        uint8_t y = static_cast<uint8_t>(ch_y) & kAddMask;
        uint8_t z = static_cast<uint8_t>(ch_z) & kAddMask;
        uint8_t u = static_cast<uint8_t>(ch_u) & kAddMask;
        uint32_t res = (static_cast<uint32_t>(x) << kShiftThree) |
                       (static_cast<uint32_t>(y) << kShiftTwo) |
                       (static_cast<uint32_t>(z) << kShiftOne) | (static_cast<uint32_t>(u));
        if (IsInvalid(res) || IsOverLong(res, 4)) {
            return kReplacementCharacter;
        }
        return static_cast<char32_t>(res);
    }

    static std::string CreateTwoBytes(uint32_t x) {
        std::string result;
        uint8_t lead_byte = kEncTwoBytes | static_cast<uint8_t>(x >> kShiftOne);
        uint8_t additional_byte = kEncAddByte | (kAddMask & static_cast<uint8_t>(x));
        result.push_back(static_cast<char>(lead_byte));
        result.push_back(static_cast<char>(additional_byte));
        return result;
    }

    static std::string CreateThreeBytes(uint32_t x) {
        std::string result;
        uint8_t lead_byte = kEncThreeBytes | static_cast<uint8_t>(x >> kShiftTwo);
        uint8_t first_additional_byte =
            kEncAddByte | (kAddMask & static_cast<uint8_t>(x >> kShiftOne));
        uint8_t second_additional_byte = kEncAddByte | (kAddMask & static_cast<uint8_t>(x));
        result.push_back(static_cast<char>(lead_byte));
        result.push_back(static_cast<char>(first_additional_byte));
        result.push_back(static_cast<char>(second_additional_byte));
        return result;
    }

    static std::string CreateFourBytes(uint32_t x) {
        std::string result;
        uint8_t lead_byte = kResFourBytes | static_cast<uint8_t>(x >> kShiftThree);
        uint8_t first_additional_byte =
            kResAddByte | (kAddMask & static_cast<uint8_t>(x >> kShiftTwo));
        uint8_t second_additional_byte =
            kResAddByte | (kAddMask & static_cast<uint8_t>(x >> kShiftOne));
        uint8_t third_additional_byte = kResAddByte | (kAddMask & static_cast<uint8_t>(x));
        result.push_back(static_cast<char>(lead_byte));
        result.push_back(static_cast<char>(first_additional_byte));
        result.push_back(static_cast<char>(second_additional_byte));
        result.push_back(static_cast<char>(third_additional_byte));
        return result;
    }

    static std::string EncodeCodePoint(char32_t ch_x) {
        if (ch_x < kTwoBytesMin) {
            return {static_cast<char>(ch_x)};
        }
        if (ch_x < kThreeBytesMin) {
            uint32_t x = static_cast<uint32_t>(ch_x);
            return CreateTwoBytes(x);
        }
        if (ch_x < kFourBytesMin) {
            uint32_t x = static_cast<uint32_t>(ch_x);
            return CreateThreeBytes(x);
        }
        uint32_t x = static_cast<uint32_t>(ch_x);
        return CreateFourBytes(x);
    }

    static size_t FindShift(std::string_view s, size_t i) {
        if (ThreeBytesThirdInvalid(s, i) || ThreeBytesThirdMissing(s, i) ||
            FourBytesThirdInvalid(s, i) || FourBytesThirdMissing(s, i)) {
            return 2;
        }
        if (FourBytesFourthInvalid(s, i) || FourBytesFourthMissing(s, i)) {
            return 3;
        }
        return 1;
    }

    static bool IsOverLong(uint32_t res, int k) {
        if (k == 2) {
            return res < kTwoBytesMin;
        } else if (k == 3) {
            return res < kThreeBytesMin;
        } else if (k == 4) {
            return res < kFourBytesMin;
        } else {
            return true;
        }
    }

    static bool ThreeBytesThirdInvalid(std::string_view s, size_t i) {
        return (i + 2 < s.size()) && IsUTF8StartThreeByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]) &&
               !IsUTF8AdditionalByte(s[i + 2]);
    }

    static bool ThreeBytesThirdMissing(std::string_view s, size_t i) {
        return (i + 2 == s.size()) && IsUTF8StartThreeByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]);
    }

    static bool FourBytesThirdInvalid(std::string_view s, size_t i) {
        return (i + 2 < s.size()) && IsUTF8StartFourByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]) &&
               !IsUTF8AdditionalByte(s[i + 2]);
    }

    static bool FourBytesThirdMissing(std::string_view s, size_t i) {
        return (i + 2 == s.size()) && IsUTF8StartFourByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]);
    }

    static bool FourBytesFourthInvalid(std::string_view s, size_t i) {
        return (i + 3 < s.size()) && IsUTF8StartFourByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]) &&
               IsUTF8AdditionalByte(s[i + 2]) && !IsUTF8AdditionalByte(s[i + 3]);
    }

    static bool FourBytesFourthMissing(std::string_view s, size_t i) {
        return (i + 3 == s.size()) && IsUTF8StartFourByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]) &&
               IsUTF8AdditionalByte(s[i + 2]);
    }

    static bool IsInvalid(uint32_t x) {
        return (x > kMaxPoint) || (x >= kMinSurrogateHalf && x <= kMaxSurrogateHalf);
    }

    static bool IsUTF8OneByte(char ch_x) {
        uint8_t x = static_cast<uint8_t>(ch_x);
        return ((x & kMaskOneByte) == kMinByte);
    }

    static bool IsUTF8AdditionalByte(char ch_x) {
        uint8_t x = static_cast<uint8_t>(ch_x);
        return ((x & kMaskAddByte) == kResAddByte);
    }

    static bool IsUTF8StartTwoByte(char ch_x) {
        uint8_t x = static_cast<uint8_t>(ch_x);
        return ((x & kMaskTwoBytes) == kResTwoBytes);
    }

    static bool IsUTF8StartThreeByte(char ch_x) {
        uint8_t x = static_cast<uint8_t>(ch_x);
        return ((x & kMaskThreeBytes) == kResThreeBytes);
    }

    static bool IsUTF8StartFourByte(char ch_x) {
        uint8_t x = static_cast<uint8_t>(ch_x);
        return ((x & kMaskFourBytes) == kResFourBytes);
    }

    static bool IsUTF8TwoBytes(char a, char b) {
        return IsUTF8StartTwoByte(a) && IsUTF8AdditionalByte(b);
    }

    static bool IsUTF8ThreeBytes(char a, char b, char c) {
        return IsUTF8StartThreeByte(a) && IsUTF8AdditionalByte(b) && IsUTF8AdditionalByte(c);
    }

    static bool IsUTF8FourBytes(char a, char b, char c, char d) {
        return IsUTF8StartFourByte(a) && IsUTF8AdditionalByte(b) && IsUTF8AdditionalByte(c) &&
               IsUTF8AdditionalByte(d);
    }

    static constexpr char32_t kReplacementCharacter = 0xFFFDu;
    static constexpr uint32_t kMaxPoint = 0x10FFFFu;
    static constexpr uint32_t kMinSurrogateHalf = 0xD800u;
    static constexpr uint32_t kMaxSurrogateHalf = 0xDFFFu;
    static constexpr uint32_t kTwoBytesMin = 0x80u;
    static constexpr uint32_t kThreeBytesMin = 0x800u;
    static constexpr uint32_t kFourBytesMin = 0x10000u;
    static constexpr uint8_t kMinByte = 0x00u;
    static constexpr uint8_t kMaskOneByte = 0x80u, kResAddByte = 0x80u, kEncAddByte = 0x80u;
    static constexpr uint8_t kMaskAddByte = 0xC0u, kResTwoBytes = 0xC0u, kEncTwoBytes = 0xC0u;
    static constexpr uint8_t kMaskTwoBytes = 0xE0u, kResThreeBytes = 0xE0u, kEncThreeBytes = 0xE0u;
    static constexpr uint8_t kMaskThreeBytes = 0xF0u, kResFourBytes = 0xF0u, kEncFourBytes = 0xF0u;
    static constexpr uint8_t kMaskFourBytes = 0xF8u;
    static constexpr uint8_t kStartTwoMask = 0x1Fu;
    static constexpr uint8_t kStartThreeMask = 0x0Fu;
    static constexpr uint8_t kStartFourMask = 0x07u;
    static constexpr uint8_t kAddMask = 0x3F;
    static constexpr int kShiftOne = 6;
    static constexpr int kShiftTwo = 12;
    static constexpr int kShiftThree = 18;
};
