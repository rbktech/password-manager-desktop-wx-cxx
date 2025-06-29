#pragma once

#include <wx/xml/xml.h>

#include "ctrl/maindata.h"

class CFile
{

private:
    static void getNode(wxXmlNode* node, CMainData* outData);
    static void getAttribute(wxXmlAttribute* attr, CMainData* outData);

public:
    CFile() = default;
    ~CFile() = default;

    static int load(const char* nameFile, std::vector<std::vector<std::string>>& value);

    static int read(const char* nameFile, unsigned char* data, int& size);
    static int write(const char* nameFile, unsigned char* data, int& size);

    static void parse(unsigned char* inData, const int& inSize, CMainData* outData);
    static void collect(CMainData* inData, unsigned char* outData, int& outSize);
};