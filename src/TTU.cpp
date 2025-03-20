

#include "TTU.hpp"
#include <string>



TTU::TTU(const std::string &crt, const std::string &key, bool is_server)
        : server_crt(crt), server_key(key), ctx(nullptr) {
    init_openssl();
    ctx = create_context(is_server);
}

TTU::~TTU() {
    if (ctx) SSL_CTX_free(ctx);
    cleanup_openssl();
}

void TTU::init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void TTU::cleanup_openssl() {
    EVP_cleanup();
}

SSL_CTX* TTU::create_context(bool is_server) {
    const SSL_METHOD* method = is_server ? DTLS_server_method() : DTLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);

    if (!ctx) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("Unable to create SSL context");
    }

    return ctx;
}

SSL_CTX* TTU::get_context() const {
    return ctx;
}
