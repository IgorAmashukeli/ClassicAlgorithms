#pragma once
#include <cstdint>
#include <string>

// UTF8 Encoder and Decoder
struct UTF8 {
    // Forward declaration
private:
    struct DecodeResult;

public:
    // Uses Replacement Character for Invalid Char32
    static std::string Encode(std::u32string_view s) {
        std::string result;
        for (size_t i = 0; i < s.size(); ++i) {
            result += EncodeCodePoint(s[i]);
        }
        return result;
    }

    // Uses Maximal Subpart Replacement politics
    static std::u32string Decode(std::string_view s) {
        std::u32string result;
        for (size_t i = 0; i < s.size(); ++i) {
            auto [codepoint, additional_shift] = CreateFromBytes(s, i);
            result.push_back(codepoint);
            i += additional_shift;
        }
        return result;
    }

    // Uses Replacement Character for Invalid Char32
    static std::string Encode(std::initializer_list<char32_t> il) {
        return Encode(std::u32string(il));
    }
    // Uses Maximal Subpart Replacement Politic
    static std::u32string Decode(std::initializer_list<char> il) {
        return Decode(std::string(il));
    }

    static char32_t GetReplacementCharacter() {
        return kReplacementCharacter;
    }

private:
    static DecodeResult CreateFromBytes(std::string_view s, size_t i) {
        if (IsUTF8OneByte(s, i)) {
            return CreateFromOneByte(s, i);
        } else if (IsUTF8TwoBytes(s, i)) {
            return CreateFromTwoBytes(s, i);
        } else if (IsUTF8ThreeBytes(s, i)) {
            return CreateFromThreeBytes(s, i);
        } else if (IsUTF8FourBytes(s, i)) {
            return CreateFromFourBytes(s, i);
        } else {
            return CreateFromInvalid(s, i);
        }
    }

    static DecodeResult CreateFromOneByte(std::string_view s, size_t i) {
        uint8_t x = static_cast<uint8_t>(s[i]);
        uint32_t res = static_cast<uint32_t>(x);
        if (IsInvalid(res)) {
            return {kReplacementCharacter, 0};
        }
        return {static_cast<char32_t>(res), 0};
    }

    static DecodeResult CreateFromTwoBytes(std::string_view s, size_t i) {
        uint8_t x = static_cast<uint8_t>(s[i]) & kStartTwoMask;
        uint8_t y = static_cast<uint8_t>(s[i + 1]) & kAddMask;
        uint32_t res = (static_cast<uint32_t>(x) << kShiftOne) | (static_cast<uint32_t>(y));
        if (IsInvalid(res) || IsOverLong(res, 2)) {
            return {kReplacementCharacter, 1};
        }
        return {static_cast<char32_t>(res), 1};
    }

    static DecodeResult CreateFromThreeBytes(std::string_view s, size_t i) {
        uint8_t x = static_cast<uint8_t>(s[i]) & kStartThreeMask;
        uint8_t y = static_cast<uint8_t>(s[i + 1]) & kAddMask;
        uint8_t z = static_cast<uint8_t>(s[i + 2]) & kAddMask;
        uint32_t res = (static_cast<uint32_t>(x) << kShiftTwo) |
                       (static_cast<uint32_t>(y) << kShiftOne) | static_cast<uint32_t>(z);
        if (IsInvalid(res) || IsOverLong(res, 3)) {
            return {kReplacementCharacter, 2};
        }
        return {static_cast<char32_t>(res), 2};
    }

    static DecodeResult CreateFromFourBytes(std::string_view s, size_t i) {
        uint8_t x = static_cast<uint8_t>(s[i]) & kStartFourMask;
        uint8_t y = static_cast<uint8_t>(s[i + 1]) & kAddMask;
        uint8_t z = static_cast<uint8_t>(s[i + 2]) & kAddMask;
        uint8_t u = static_cast<uint8_t>(s[i + 3]) & kAddMask;
        uint32_t res = (static_cast<uint32_t>(x) << kShiftThree) |
                       (static_cast<uint32_t>(y) << kShiftTwo) |
                       (static_cast<uint32_t>(z) << kShiftOne) | (static_cast<uint32_t>(u));
        if (IsInvalid(res) || IsOverLong(res, 4)) {
            return {kReplacementCharacter, 3};
        }
        return {static_cast<char32_t>(res), 3};
    }

    static DecodeResult CreateFromInvalid(std::string_view s, size_t i) {
        return {kReplacementCharacter, FindShift(s, i) - 1};
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
        if (IsInvalid(ch_x)) {
            return EncodeCodePoint(kReplacementCharacter);
        }
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

    static bool IsUTF8OneByte(std::string_view s, size_t i) {
        char ch_x = s[i];
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

    static bool IsUTF8TwoBytes(std::string_view s, size_t i) {
        return i + 1 < s.size() && IsUTF8StartTwoByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]);
    }

    static bool IsUTF8ThreeBytes(std::string_view s, size_t i) {
        return i + 2 < s.size() && IsUTF8StartThreeByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]) &&
               IsUTF8AdditionalByte(s[i + 2]);
    }

    static bool IsUTF8FourBytes(std::string_view s, size_t i) {
        return i + 3 < s.size() && IsUTF8StartFourByte(s[i]) && IsUTF8AdditionalByte(s[i + 1]) &&
               IsUTF8AdditionalByte(s[i + 2]) && IsUTF8AdditionalByte(s[i + 3]);
    }

    struct DecodeResult {
        char32_t codepoint;
        size_t additional_shift;
    };

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
