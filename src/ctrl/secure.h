#ifndef SECURE_H
#define SECURE_H

#include "ctrl/dataexchange.h"
#include "tools/cipher.h"
#include "tools/file.h"

#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#define SIZE_TEXT 3145728

class CSecure
{
private:
    CDataExchange mDataExchange;

    char mPassword[SIZE_PASSWORD];

    int inSize = SIZE_TEXT;
    unsigned char* inData = new unsigned char[SIZE_TEXT]{ 0 };

    int outSize = SIZE_TEXT;
    unsigned char* outData = new unsigned char[SIZE_TEXT]{ 0 };

    void clear();

public:
    CSecure();
    ~CSecure();

    void loadFileAndDecrypt(CMainData* data);
    void encryptFileAndUpload(CMainData* data);
};

#endif // SECURE_H