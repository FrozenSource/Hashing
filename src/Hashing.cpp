#include <Hashing.hpp>

bool HashFile(const string & sFileName, string & sHashOut) {
        if (!FSFileExists(sFileName)) return false;
        string sFileData = FSReadFile(sFileName);
        size_t uiSize = 0;
#if defined(_WIN64)
        byte aBuffer[BLAKE2B_OUTBYTES] {0};
        blake2b_state state = {};
        if (blake2b_init(&state, BLAKE2B_OUTBYTES) != 0) return false;
        if (blake2b_update(&state, sFileData.c_str(), sFileData.size()) != 0) return false;
        if (blake2b_final(&state, aBuffer, BLAKE2B_OUTBYTES) != 0) return false;
#else
        byte aBuffer[BLAKE2S_OUTBYTES];
        blake2s_state state = {};
        if (blake2s_init(&state, BLAKE2B_OUTBYTES) != 0) return false;
        if (blake2s_update(&state, sFileData.c_str(), sFileData.size()) != 0) return false;
        if (blake2s_final(&state, aBuffer, BLAKE2S_OUTBYTES) != 0) return false;
#endif
        sHashOut = EncodeBase64(aBuffer, sizeof(aBuffer));
        return true;
    }