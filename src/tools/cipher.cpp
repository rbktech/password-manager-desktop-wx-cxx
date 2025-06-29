#include "cipher.h"

// #define SIZE_IV 32
#define SIZE_KEY 32
#define SIZE_SALT 256

extern unsigned char _binary_iv_start;
extern unsigned char _binary_salt_start;

/** PUBLIC */

CCipher::CCipher()
{
}

CCipher::~CCipher()
{
}

void CCipher::encrypt(const char* password, unsigned char* inText, int inSize, unsigned char* outText, int& outSize)
{
    unsigned char key[SIZE_KEY] = { 0 };

    getKey(password, &_binary_salt_start, key);

    outSize = encrypt(inText, inSize, key, &_binary_iv_start, outText);
}

void CCipher::decrypt(const char* password, unsigned char* inText, int inSize, unsigned char* outText, int& outSize)
{
    unsigned char key[SIZE_KEY] = { 0 };

    getKey(password, &_binary_salt_start, key);

    outSize = decrypt(inText, inSize, key, &_binary_iv_start, outText);
}

/** PRIVATE */

void CCipher::getKey(const char* password, unsigned char* salt, unsigned char* key)
{
    int result = PKCS5_PBKDF2_HMAC(password, SIZE_PASSWORD, salt, SIZE_SALT, 1324, EVP_sha1(), SIZE_KEY, key);
    if(result != 1)
        throw "PKCS5_PBKDF2_HMAC: error";
}

int CCipher::encrypt(unsigned char* inText, int inSize, unsigned char* key, unsigned char* iv, unsigned char* outText)
{
    int length = 0;
    int outSize = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
        throw "EVP_CIPHER_CTX_new: error";

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv))
        throw "EVP_EncryptInit_ex: error";

    if(!EVP_EncryptUpdate(ctx, outText, &length, inText, inSize))
        throw "EVP_EncryptUpdate: error";

    outSize += length;

    if(!EVP_EncryptFinal_ex(ctx, outText + length, &length))
        throw "EVP_EncryptFinal_ex: error";

    outSize += length;

    EVP_CIPHER_CTX_free(ctx);

    return outSize;
}

int CCipher::decrypt(unsigned char* inText, int inSize, unsigned char* key, unsigned char* iv, unsigned char* outText)
{
    int length = 0;
    int outSize = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
        throw "EVP_CIPHER_CTX_new: error";

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv))
        throw "EVP_DecryptInit_ex: error";

    if(!EVP_DecryptUpdate(ctx, outText, &length, inText, inSize))
        throw "EVP_DecryptUpdate: error";

    outSize += length;

    if(!EVP_DecryptFinal_ex(ctx, outText + length, &length))
        throw "EVP_DecryptFinal_ex: error";

    outSize += length;

    EVP_CIPHER_CTX_free(ctx);

    return outSize;
}