#include <Hashing.hpp>
#include <filesystem>
#include <fstream>

#if !defined(__x86_64__) && defined(_WIN64)
#define __x86_64__
#endif

/*
  base64.cpp and base64.h
  Copyright (C) 2004-2008 René Nyffenegger
  This source code is provided 'as-is', without any express or implied
  warranty. In no event will the author be held liable for any damages
  arising from the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  1. The origin of this source code must not be misrepresented; you must not
  claim that you wrote the original source code. If you use this source code
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original source code.
  3. This notice may not be removed or altered from any source distribution.
  René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 */

std::string __HASHING_EncodeBase64(unsigned char const* bytes_to_encode,
                                   unsigned int in_len) {
    static const std::string __HASHING_C_BASE64_CHARS =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                              ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                              ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++) ret += __HASHING_C_BASE64_CHARS[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] =
            ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] =
            ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++) ret += __HASHING_C_BASE64_CHARS[char_array_4[j]];

        while ((i++ < 3)) ret += '=';
    }

    return ret;
}

bool HashFile(const std::string& sFileName, std::string& sHashOut) {
    if (!std::filesystem::is_regular_file(sFileName)) return false;
    std::ifstream in(sFileName, std::ios::in | std::ios::binary);
    if (!in.good()) return false;

    unsigned char read_buffer[4096 * 4096];
#if defined(__x86_64__)
    unsigned char aBuffer[BLAKE2B_OUTBYTES]{0};
    blake2b_state state = {};
#else
    unsigned char aBuffer[BLAKE2S_OUTBYTES]{0};
    blake2s_state state = {};
#endif

#if defined(__x86_64__)
    if (blake2b_init(&state, BLAKE2B_OUTBYTES) != 0) return false;
#else
    if (blake2s_init(&state, BLAKE2S_OUTBYTES) != 0) return false;
#endif

    while (in.gcount() > 0) {
        in.read((char*)read_buffer, 4096 * 4096);
#if defined(__x86_64__)
        if (blake2b_update(&state, (char*)read_buffer, in.gcount()) != 0)
            return false;
#else
        if (blake2s_update(&state, (char*)read_buffer, in.gcount()) != 0)
            return false;
#endif
    }

#if defined(__x86_64__)
    if (blake2b_final(&state, aBuffer, BLAKE2B_OUTBYTES) != 0) return false;
    sHashOut = __HASHING_EncodeBase64(aBuffer, BLAKE2B_OUTBYTES);
#else
    if (blake2s_final(&state, aBuffer, BLAKE2S_OUTBYTES) != 0) return false;
    sHashOut = __HASHING_EncodeBase64(aBuffer, BLAKE2S_OUTBYTES);
#endif
    return true;
}

bool Hash(const std::string& sBuffer, std::string& sHashOut) {
    size_t uiSize = 0;
#if defined(__x86_64__)
    unsigned char aBuffer[BLAKE2B_OUTBYTES]{0};
    blake2b_state state = {};
    if (blake2b_init(&state, BLAKE2B_OUTBYTES) != 0) return false;
    if (blake2b_update(&state, sBuffer.c_str(), sBuffer.size()) != 0)
        return false;
    if (blake2b_final(&state, aBuffer, BLAKE2B_OUTBYTES) != 0) return false;
    sHashOut = __HASHING_EncodeBase64(aBuffer, BLAKE2B_OUTBYTES);
#else
    unsigned char aBuffer[BLAKE2S_OUTBYTES]{0};
    blake2s_state state = {};
    if (blake2s_init(&state, BLAKE2S_OUTBYTES) != 0) return false;
    if (blake2s_update(&state, sBuffer.c_str(), sBuffer.size()) != 0)
        return false;
    if (blake2s_final(&state, aBuffer, BLAKE2S_OUTBYTES) != 0) return false;
    sHashOut = __HASHING_EncodeBase64(aBuffer, BLAKE2S_OUTBYTES);
#endif
    return true;
}