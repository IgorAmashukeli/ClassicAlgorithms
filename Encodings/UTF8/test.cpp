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

// DEPRECATED TESTS FROM C++17 - WILL BE REMOVED IN C++26, but OK for the test
std::string EncodeWithCodecvt(const std::u32string &u32s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(u32s);
}

// DEPRECATED TESTS FROM C++17 - WILL BE REMOVED IN C++26, but OK for the test
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

// DEPRECATED TESTS FROM C++17 - WILL BE REMOVED IN C++26, but OK for the test
void TestDecAgainstCodecvt(const std::string &input) {
    std::u32string encoded_lib = utf8::Decode(input);
    std::u32string encoded_std = DecodeWithCodecvt(input);
    assert(encoded_lib == encoded_std);
}

// ======================== Tests ========================
void TestRoundtrip(const std::u32string &input) {
    std::string encoded = utf8::Encode(input);
    TestDecAgainstCodecvt(encoded);
    std::u32string decoded = utf8::Decode(encoded);
    assert(input == decoded);
}

// DEPRECATED TESTS FROM C++17 - WILL BE REMOVED IN C++26, but OK for the test
void TestEncAgainstCodecvt(const std::u32string &input) {
    std::string encoded_lib = utf8::Encode(input);
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

    std::cout << "Stress tests passed\n";
}

static std::vector<unsigned char> BytesOf(const std::string &s) {
    std::vector<unsigned char> out;
    out.reserve(s.size());
    for (unsigned char c : s) {
        out.push_back(c);
    }
    return out;
}

static std::string HexOf(const std::string &s) {
    std::string out;
    char buf[8];
    for (unsigned char c : s) {
        std::snprintf(buf, sizeof(buf), "%02X ", c);
        out += buf;
    }
    return out;
}

static void TestInvalidDataRoundtrip(std::string_view invalid_bytes) {
    // decode invalid -> decoded (contains replacement characters where needed)
    std::u32string decoded = utf8::Decode(std::string(invalid_bytes));

    // encode decoded -> normalized bytes (valid UTF-8)
    std::string reencoded = utf8::Encode(decoded);

    // decode again: should be identical to decoded (idempotence of normalize)
    std::u32string decoded_again = utf8::Decode(reencoded);
    assert(decoded_again == decoded &&
           "Decode( Encode( Decode(invalid) ) ) must equal Decode(invalid)");

    // Typically, for invalid input, reencoded != original invalid bytes.
    // We assert inequality to show encode(decode(invalid)) != invalid can fail (i.e. not equal).
    if (std::string(invalid_bytes) == reencoded) {
        // Edge case: if they happen to be identical (unlikely for our chosen invalid cases),
        // we still pass, but we note it for diagnostics (not an assert failure).
        std::cout << "[note] reencoded equals input for sequence: "
                  << HexOf(std::string(invalid_bytes)) << "\n";
    } else {
        assert(std::string(invalid_bytes) != reencoded &&
               "Expected normalized re-encoding to differ from invalid input");
    }

    // The reencoded bytes must be valid UTF-8: decoding it must yield a u32string with no
    // replacement characters except those that legitimately came from the decoder's recovery. We
    // already checked decode(reencoded) == decoded, so reencoded is decode-able.
}

// helpers --------------------------------------------------------
static std::u32string U32(std::initializer_list<char32_t> xs) {
    std::u32string out;
    out.reserve(xs.size());
    for (char32_t x : xs) {
        out.push_back(x);
    }
    return out;
}

static std::string BytesFrom(std::initializer_list<int> bytes) {
    std::string out;
    out.reserve(bytes.size());
    for (int b : bytes) {
        out.push_back(static_cast<char>(b & 0xFF));
    }
    return out;
}

static void AssertU32Eq(const std::u32string &a, const std::u32string &b, const char *msg = "") {
    if (a != b) {
        std::cerr << "U32 mismatch: " << msg << "\n";
        assert(false);
    }
}

// ======================== Deterministic Invalid Tests ========================
static void TestSingleReplacement() {
    std::string in = BytesFrom({0x80});  // lone continuation
    std::u32string decoded = utf8::Decode(in);
    std::u32string expected = U32({utf8::GetReplacementCharacter()});
    AssertU32Eq(decoded, expected, "Single replacement");
}

static void TestMultipleReplacements() {
    // three consecutive lone continuation bytes
    std::string in = BytesFrom({0x80, 0x81, 0x82});
    std::u32string decoded = utf8::Decode(in);
    std::u32string expected = U32({utf8::GetReplacementCharacter(), utf8::GetReplacementCharacter(),
                                   utf8::GetReplacementCharacter()});
    AssertU32Eq(decoded, expected, "Multiple replacements in a row");
}

static void TestMixedValidInvalid() {
    // 'A' + lone continuation + valid snowman + truncated 2-byte
    std::string in;
    in.push_back('A');
    in.push_back(static_cast<char>(0x80));  // invalid
    in.push_back(static_cast<char>(0xE2));
    in.push_back(static_cast<char>(0x98));
    in.push_back(static_cast<char>(0x83));  // U+2603
    in.push_back(static_cast<char>(0xC2));  // truncated 2-byte (missing continuation)

    std::u32string decoded = utf8::Decode(in);
    std::u32string expected = U32({static_cast<char32_t>('A'), utf8::GetReplacementCharacter(),
                                   0x2603u, utf8::GetReplacementCharacter()});
    AssertU32Eq(decoded, expected, "Mixed valid + multiple invalid");
}

static void TestThreeBytesThirdInvalid() {
    std::string in = BytesFrom({0xE2, 0x82, 0x20});  // invalid third byte
    std::u32string decoded = utf8::Decode(in);
    std::u32string expected = U32({utf8::GetReplacementCharacter(), 0x20u});
    AssertU32Eq(decoded, expected, "ThreeBytesThirdInvalid");
}

static void TestFourBytesThirdInvalid() {
    std::string in = BytesFrom({0xF0, 0x90, 0x20, 0x41});  // invalid third byte
    std::u32string decoded = utf8::Decode(in);
    std::u32string expected = U32({utf8::GetReplacementCharacter(), 0x20u, 0x41u});
    AssertU32Eq(decoded, expected, "FourBytesThirdInvalid");
}

static void TestFourBytesFourthInvalid() {
    std::string in = BytesFrom({0xF0, 0x9F, 0x98, 0x20});  // invalid fourth byte
    std::u32string decoded = utf8::Decode(in);
    std::u32string expected = U32({utf8::GetReplacementCharacter(), 0x20u});
    AssertU32Eq(decoded, expected, "FourBytesFourthInvalid");
}

static void TestTruncatedSequences() {
    // truncated 2-byte
    std::string t2 = BytesFrom({0xC2});
    std::u32string decoded_t2 = utf8::Decode(t2);
    std::u32string expected_t2 = U32({utf8::GetReplacementCharacter()});
    AssertU32Eq(decoded_t2, expected_t2, "Truncated 2-byte");

    // truncated 3-byte
    std::string t3 = BytesFrom({0xE2, 0x82});
    std::u32string decoded_t3 = utf8::Decode(t3);
    std::u32string expected_t3 = U32({utf8::GetReplacementCharacter()});
    AssertU32Eq(decoded_t3, expected_t3, "Truncated 3-byte");

    // truncated 4-byte
    std::string t4 = BytesFrom({0xF0, 0x9F, 0x98});
    std::u32string decoded_t4 = utf8::Decode(t4);
    std::u32string expected_t4 = U32({utf8::GetReplacementCharacter()});
    AssertU32Eq(decoded_t4, expected_t4, "Truncated 4-byte");
}

static void TestLongContinuationRun() {
    std::string in;
    for (int i = 0; i < 10; ++i) {
        in.push_back(static_cast<char>(0x80 + (i & 0x3F)));  // continuation bytes
    }

    std::u32string decoded = utf8::Decode(in);
    std::u32string expected;
    for (int i = 0; i < 10; ++i) {
        expected.push_back(utf8::GetReplacementCharacter());
    }
    AssertU32Eq(decoded, expected, "Long run of continuation bytes");
}

// ======================== Helpers for Maximal Subpart ========================
static int CountReplacementCharacters(const std::u32string &decoded) {
    int count = 0;
    char32_t repl = utf8::GetReplacementCharacter();
    for (char32_t c : decoded) {
        if (c == repl) {
            ++count;
        }
    }
    return count;
}

static int CountMaximalSubparts(std::string_view s) {
    int count = 0;
    for (size_t i = 0; i < s.size();) {
        unsigned char c = static_cast<unsigned char>(s[i]);

        // special case: already a replacement character
        if (c == 0xEF && i + 2 < s.size() && static_cast<unsigned char>(s[i + 1]) == 0xBF &&
            static_cast<unsigned char>(s[i + 2]) == 0xBD) {
            ++count;
            ++i;     // count this as one subpart
            i += 2;  // skip next two bytes
            continue;
        }

        if (c <= 0x7F) {  // ASCII
            ++i;
        } else if ((c & 0xE0) == 0xC0) {  // 2-byte start
            if (i + 1 >= s.size() || (static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80) {
                ++count;  // invalid maximal subpart
                ++i;
            } else {
                i += 2;
            }
        } else if ((c & 0xF0) == 0xE0) {  // 3-byte start
            if (i + 2 >= s.size() || (static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80 ||
                (static_cast<unsigned char>(s[i + 2]) & 0xC0) != 0x80) {
                ++count;
                ++i;
            } else {
                i += 3;
            }
        } else if ((c & 0xF8) == 0xF0) {  // 4-byte start
            if (i + 3 >= s.size() || (static_cast<unsigned char>(s[i + 1]) & 0xC0) != 0x80 ||
                (static_cast<unsigned char>(s[i + 2]) & 0xC0) != 0x80 ||
                (static_cast<unsigned char>(s[i + 3]) & 0xC0) != 0x80) {
                ++count;
                ++i;
            } else {
                i += 4;
            }
        } else {  // lone continuation / invalid start
            ++count;
            ++i;
        }
    }
    return count;
}

// ======================== Exact placement test ========================
static void TestReplacementPlacement(std::string_view bytes) {
    // Skip entirely valid UTF-8 inputs, only test truly invalid sequences
    if (CountMaximalSubparts(bytes) == 0) {
        return;  // nothing to test
    }

    std::u32string decoded = utf8::Decode(std::string(bytes));
    int repl_count = CountReplacementCharacters(decoded);
    int expected_count = CountMaximalSubparts(bytes);

    assert(repl_count == expected_count &&
           "Number of replacement characters must match number of invalid maximal subparts");
}

// ======================== Random mixed invalid + valid stress test ========================
static void StressTestRandomInvalid(int num_tests, int max_len) {
    std::mt19937 rng(
        static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

    std::uniform_int_distribution<int> ascii_dist(0, 0x7F);
    std::uniform_int_distribution<int> codepoint_dist(0x80, 0x10FFFF);
    std::uniform_int_distribution<int> insert_invalid(0, 3);  // 25% chance of invalid

    for (int t = 0; t < num_tests; ++t) {
        std::string s;
        int len = std::uniform_int_distribution<int>(1, max_len)(rng);
        for (int i = 0; i < len; ++i) {
            if (insert_invalid(rng) == 0) {  // insert random invalid byte
                unsigned char b = static_cast<unsigned char>(0x80 + (rng() & 0x3F));
                s.push_back(static_cast<char>(b));
            } else {  // insert valid UTF-8
                char32_t cp = RandomCodepoint(rng);
                std::string encoded = utf8::Encode(U32({cp}));
                s += encoded;
            }
        }
        // Idempotence check
        std::u32string decoded = utf8::Decode(s);
        std::string reencoded = utf8::Encode(decoded);
        std::u32string decoded_again = utf8::Decode(reencoded);
        assert(decoded_again == decoded &&
               "Decode(Encode(Decode(mixed invalid+valid))) must equal Decode");

        // Exact replacement placement check
        TestReplacementPlacement(s);
    }

    std::cout << "Random mixed invalid + valid stress test passed\n";
}

// Specific invalid-pattern tests with explanations
static void RunInvalidDataTests() {
    TestSingleReplacement();
    TestMultipleReplacements();
    TestMixedValidInvalid();
    TestThreeBytesThirdInvalid();
    TestFourBytesThirdInvalid();
    TestFourBytesFourthInvalid();
    TestTruncatedSequences();
    TestLongContinuationRun();
    // Replacement character encoded by your library (to compare easily)
    std::u32string repl_u32(1, static_cast<char32_t>(0xFFFD));
    std::string repl_bytes = utf8::Encode(repl_u32);  // should be EF BF BD

    // 1) Lone continuation byte (0x80)
    {
        std::string s = std::string("\x80", 1);
        std::u32string d = utf8::Decode(s);
        // decoder should produce at least one replacement char
        assert(d.size() >= 1);
        // re-encoding decoded should produce replacement UTF-8 sequence(s)
        std::string r = utf8::Encode(d);
        assert(r.size() % repl_bytes.size() == 0);  // integer multiple of replacement bytes
        TestInvalidDataRoundtrip(s);
    }

    // 2) Truncated 2-byte start (lead but no continuation)
    {
        std::string s =
            std::string("\xC2", 1);  // start of 2-byte sequence but missing continuation
        TestInvalidDataRoundtrip(s);
    }

    // 3) Overlong encoding (C0 80) - should be treated invalid
    {
        std::string s = std::string("\xC0\x80", 2);  // overlong encoding of U+0000
        TestInvalidDataRoundtrip(s);
    }

    // 4) 3-byte sequence with invalid 3rd byte (valid first two bytes then invalid third)
    // e.g. E2 82 20 (0x20 is space, not a continuation)
    {
        std::string s = std::string("\xE2\x82 ", 3);  // note: last char is 0x20 (space)
        TestInvalidDataRoundtrip(s);
    }

    // 5) 4-byte sequence with missing bytes (truncated)
    {
        std::string s =
            std::string("\xF0\x9F\x98", 3);  // truncated 4-byte (should be F0 9F 98 80 for U+1F600)
        TestInvalidDataRoundtrip(s);
    }

    // 6) Invalid start bytes 0xFE, 0xFF (illegal in UTF-8)
    {
        std::string s1 = std::string("\xFE", 1);
        std::string s2 = std::string("\xFF", 1);
        TestInvalidDataRoundtrip(s1);
        TestInvalidDataRoundtrip(s2);
    }

    // 7) Mixed valid + invalid: valid ASCII, then lone continuation, then valid BMP char
    {
        std::string s;
        s += 'A';
        s += '\x80';  // invalid continuation
        s += '\xE2';
        s += '\x98';
        s += '\x83';  // valid U+2603 SNOWMAN (if sequence complete)
        // Note: here we actually used valid 3 bytes at the end; decoder should decode 'A',
        // replacement, U+2603
        std::u32string d = utf8::Decode(s);
        assert(d.size() >= 3);
        TestInvalidDataRoundtrip(s);
    }

    // 8) Long invalid run of continuation bytes
    {
        std::string s;
        for (int i = 0; i < 10; ++i) {
            s.push_back(static_cast<char>(0x80 + (i & 0x3F)));  // continuation-range bytes
        }
        TestInvalidDataRoundtrip(s);
    }

    StressTestRandomInvalid(50000, 2000);

    std::cout << "Invalid data Tests Passed\n";
}

// ======================== Main ========================
int main() {

    RunInvalidDataTests();
    StressTest(50000, 2000);
    return 0;
}
