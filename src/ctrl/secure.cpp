#include "secure.h"

#include <cstring>

CSecure::CSecure()
    : mDataExchange()
{
    memset(mPassword, 0, SIZE_PASSWORD);

    inSize = SIZE_TEXT;
    inData = new unsigned char[SIZE_TEXT]{ 0 };

    outSize = SIZE_TEXT;
    outData = new unsigned char[SIZE_TEXT]{ 0 };
}

CSecure::~CSecure()
{
    delete[] inData;
    delete[] outData;
}

void CSecure::clear()
{
    inSize = SIZE_TEXT;
    outSize = SIZE_TEXT;

    memset(inData, 0, inSize);
    memset(outData, 0, outSize);
}

void CSecure::loadFileAndDecrypt(CMainData* data)
{
    wxPasswordEntryDialog* dlg = new wxPasswordEntryDialog(nullptr, wxT(""));
    if(dlg->ShowModal() == wxID_OK) {

        clear();

        const char* password = dlg->GetValue().ToStdString().data();

        mDataExchange.load();

        // CFile::read("../valid/KeyByAllTheDoor.txt", inData, inSize);
        CFile::read("../valid/testall.txt", inData, inSize);

        // CCipher::decrypt(password, inData, inSize, outData, outSize);

        CFile::parse(outData, outSize, data);

        memcpy(mPassword, password, SIZE_PASSWORD);
    }
}

void CSecure::encryptFileAndUpload(CMainData* data)
{
    clear();

    CFile::collect(data, inData, inSize);

    // CCipher::encrypt(mPassword, inData, inSize, outData, outSize);

    // remove("../valid/KeyByAllTheDoor.txt");
    // CFile::write("../valid/KeyByAllTheDoor.txt", outData, outSize);
    remove("../valid/testall.txt");
    CFile::write("../valid/testall.txt", outData, outSize);

    mDataExchange.upload();
}