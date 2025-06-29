#include "dataexchange.h"

CDataExchange::CDataExchange()
{
    // mJNIData.token = "<token>";
    // mJNIData.remoteFullPath = "/KeyByAllTheDoor.txt";
    // mJNIData.localFullPath = "../valid/KeyByAllTheDoor.txt";
    // mJNIData.localFullPath = "../valid/testall.txt";
}

CDataExchange::~CDataExchange()
{
}

void CDataExchange::load()
{
#ifdef JVM_RELEAS
    // CJNIDiskData data;
    // data.token = "<token>";
    mJNIData.remoteFullPath = "/testall.txt";
    // data.localFullPath = "../valid/KeyByAllTheDoor.txt";

    try {
        mJNIDisk.load(mJNIData);
    } catch(const char* message) {
        printf(message);
    }
#endif
}

void CDataExchange::upload()
{
#ifdef JVM_RELEAS
    // CJNIDiskData data;
    // data.token = "<token>";
    mJNIData.remoteFullPath = "/";
    // data.localFullPath = "../valid/KeyByAllTheDoor.txt";

    try {
        mJNIDisk.upload(mJNIData);
    } catch(const char* message) {
        printf(message);
    }
#endif
}