#include "b64.h"

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace {
struct BIOFreeAll { void operator()(BIO* p) { BIO_free_all(p); } };
}

std::string Base64::Encode(const std::string& data) {
    std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
    BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO* sink = BIO_new(BIO_s_mem());
    BIO_push(b64.get(), sink);
    BIO_write(b64.get(), data.data(), data.length());
    BIO_flush(b64.get());
    const char* encoded;
    const long len = BIO_get_mem_data(sink, &encoded);
    return std::string(encoded, len);
}

std::string Base64::Decode(const std::string& encoded) {
    std::string encoded_copy = encoded;
    std::unique_ptr<BIO, BIOFreeAll> b64(BIO_new(BIO_f_base64()));
    BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO* source = BIO_new_mem_buf(const_cast<char*>(encoded_copy.data()), -1);
    BIO_push(b64.get(), source);
    const int maxlen = encoded.length() / 4 * 3 + 1;
    std::string decoded(maxlen, ' ');
    const int len = BIO_read(b64.get(), const_cast<char*>(decoded.data()), maxlen);
    decoded.resize(len);
    return decoded;
}
