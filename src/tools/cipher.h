#ifndef CIPHER_H
#define CIPHER_H

#include <openssl/evp.h>

#define SIZE_PASSWORD 4

class CCipher
{
private:
    /*int ivSize = SIZE_IV;
    unsigned char iv[SIZE_IV] = { 0 };

    int saltSize = SIZE_SALT;
    unsigned char salt[SIZE_SALT] = { 0 };

    unsigned char key[SIZE_KEY] = { 0 };*/

    static int
    encrypt(unsigned char* inText, int inSize, unsigned char* key, unsigned char* iv, unsigned char* outText);

    static int
    decrypt(unsigned char* inText, int inSize, unsigned char* key, unsigned char* iv, unsigned char* outText);

    static void getKey(const char* password, unsigned char* salt, unsigned char* key);

public:
    CCipher();
    ~CCipher();

    static void encrypt(const char* password, unsigned char* inText, int inSize, unsigned char* outText, int& outSize);
    static void decrypt(const char* password, unsigned char* inText, int inSize, unsigned char* outText, int& outSize);
};

#endif // CIPHER_H