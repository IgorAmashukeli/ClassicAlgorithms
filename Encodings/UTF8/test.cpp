#include "UTF8.h"
#include <cassert>

// TO DO
int main() {
    assert(UTF8::Decode("привет").size() == 6);
    assert(UTF8::Encode(UTF8::Decode("привет")) == "привет");
}
