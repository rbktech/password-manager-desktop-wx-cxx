#include "file.h"

#include <windows.h>

#include <wx/mstream.h>

#include <fstream>
#include <sstream>

#include <istream>
#include <ostream>

// #include <wx/txtstrm.h>
// #include <wx/wfstream.h>

size_t parseStream(std::basic_istream<char>& stream, std::vector<std::string>& tokens)
{
    std::string line;
    std::string value;

    std::getline(stream, line);
    std::stringstream ss(line);

    tokens.clear();

    while(ss.eof() == false) {

        std::getline(ss, value, ';');

        tokens.emplace_back(value);
    }

    return tokens.size();
}

int CFile::load(const char* nameFile, std::vector<std::vector<std::string>>& rowTokens)
{
    std::fstream file;
    size_t sizeTokens = 0;
    std::vector<std::string> colTokens;

    file.open(nameFile, std::ios_base::in | std::ios_base::binary);
    if(file.is_open() == true) {

        while(file.eof() == false) {

            sizeTokens = parseStream(file, colTokens);
            if(sizeTokens == 3) {
                rowTokens.emplace_back(colTokens);
            } else
                rowTokens.emplace_back(std::vector<std::string>({ "error" }));
        }

        file.close();

        return 0;
    }

    return 1;
}

int CFile::read(const char* nameFile, unsigned char* data, int& size)
{
    // wxFileInputStream input(nameFile);
    // input.ReadAll(out, size);

    HANDLE handle = 0;

    DWORD error = 0;
    DWORD bytesReader = 0;
    bool result = false;

    try {

        handle = CreateFileA(nameFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(handle == INVALID_HANDLE_VALUE)
            throw GetLastError();

        result = ReadFile(handle, data, size, &bytesReader, nullptr);
        size = bytesReader;
        if(result == false) // || size != bytesReader)
            throw GetLastError();

    } catch(DWORD exc) {
        error = exc;
    }

    CloseHandle(handle);
    return error;
}

int CFile::write(const char* nameFile, unsigned char* data, int& size)
{
    // wxFileOutputStream output(nameFile);
    // output.WriteAll(out, size);

    HANDLE handle = 0;

    DWORD error = 0;
    DWORD bytesWriter = 0;
    WINBOOL result = false;

    try {

        handle = CreateFileA(nameFile, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if(handle == INVALID_HANDLE_VALUE)
            throw GetLastError();

        result = WriteFile(handle, data, size, &bytesWriter, nullptr);
        if(result == false || (size_t)size != bytesWriter)
            throw GetLastError();

    } catch(DWORD error) {
        error = error;
    }

    CloseHandle(handle);
    return error;
}

void CFile::collect(CMainData* inData, unsigned char* outData, int& outSize)
{
    wxXmlDocument xmlDoc;
    wxMemoryOutputStream stream;

    wxXmlNode* root = nullptr;
    wxXmlNode* nodePage = nullptr;
    wxXmlNode* nodeWebSite = nullptr;

    root = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, wxT("root"));

    ListPage& listPage = inData->mList;
    const int sizeListPage = listPage.size();
    for(int nItemPage = 0; nItemPage < sizeListPage; nItemPage++) {

        TItemPage& itemPage = listPage.at(nItemPage);

        nodePage = new wxXmlNode(root, wxXML_ELEMENT_NODE, wxString::Format("page%d", nItemPage));
        nodePage->AddAttribute(wxT("NamePage"), itemPage.name);

        ListWebSite& listWebSite = itemPage.listWebSize;
        const int sizeListWebSite = listWebSite.size();
        for(int nItemWebSite = 0; nItemWebSite < sizeListWebSite; nItemWebSite++) {

            TItemWebSite& itemWebSite = listWebSite.at(nItemWebSite);

            nodeWebSite = new wxXmlNode(nodePage, wxXML_ELEMENT_NODE, wxString::Format("web%d", nItemWebSite));
            nodeWebSite->AddAttribute(wxT("WebSite"), itemWebSite.webSize);
            nodeWebSite->AddAttribute(wxT("Login"), itemWebSite.login);
            nodeWebSite->AddAttribute(wxT("Password"), itemWebSite.password);
        }
    }

    xmlDoc.SetRoot(root);
    xmlDoc.Save(stream);

    outSize = stream.GetSize();

    stream.CopyTo(outData, outSize);
}

void CFile::parse(unsigned char* inData, const int& inSize, CMainData* outData)
{
    wxXmlDocument xmlDoc;
    wxMemoryInputStream stream(inData, inSize);

    if(xmlDoc.Load(stream) == true)
        getNode(xmlDoc.GetRoot()->GetChildren(), outData);
}

void CFile::getNode(wxXmlNode* node, CMainData* outData)
{
    if(node != nullptr) {

        getNode(node->GetNext(), outData);

        getAttribute(node->GetAttributes(), outData);

        getNode(node->GetChildren(), outData);
    }
}

void CFile::getAttribute(wxXmlAttribute* attr, CMainData* outData)
{
    if(attr != nullptr) {

        std::string name = attr->GetName().ToStdString();
        std::string value = attr->GetValue().ToStdString();

        if(attr->GetName() == wxT("NamePage")) {

            TItemPage itemPage;
            itemPage.name = value;
            outData->mList.push_back(itemPage);
        }

        if(attr->GetName() == wxT("WebSite")) {

            TItemPage& itemPage = outData->mList.back();

            TItemWebSite itemWebSite;
            itemWebSite.webSize = value;
            itemPage.listWebSize.push_back(itemWebSite);
        }

        if(attr->GetName() == wxT("Login")) {

            TItemPage& itemPage = outData->mList.back();
            TItemWebSite& itemWebSite = itemPage.listWebSize.back();
            itemWebSite.login = value;
        }

        if(attr->GetName() == wxT("Password")) {

            TItemPage& itemPage = outData->mList.back();
            TItemWebSite& itemWebSite = itemPage.listWebSize.back();
            itemWebSite.password = value;
        }

        getAttribute(attr->GetNext(), outData);
    }
}