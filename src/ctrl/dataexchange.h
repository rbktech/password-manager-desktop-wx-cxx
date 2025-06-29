#ifndef DATAEXCHANGE_H
#define DATAEXCHANGE_H

// #include "tools/jnidisk.h"

class CDataExchange
{
private:
    // CJNIDisk mJNIDisk;
    // CJNIDiskData mJNIData;

public:
    CDataExchange();
    ~CDataExchange();

    void load();
    void upload();
};

#endif // DATAEXCHANGE_H