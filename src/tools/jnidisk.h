#ifndef JNIDISK_H
#define JNIDISK_H

#include <jni.h>

struct CJNIDiskData {

    /** INIT JVM */

    // const char* dllPath;
    // const char* jarPath;
    const char* nameClass;
    const char* nameMethod;
    const char* signMethod;

    /** ARGUMENT METHOD */

    const char* token;
    const char* remoteFullPath;
    const char* localFullPath;
};

class CJNIDisk
{
private:
    JavaVM* mJavaVM; /* Denotes a Java VM */
    JNIEnv* mJNIEnv; /* Pointer to native method interface */

    bool jvm(const CJNIDiskData& data);
    void exception(void* env, const char* message);

public:
    CJNIDisk();
    ~CJNIDisk();

    bool load(CJNIDiskData& data);
    bool upload(CJNIDiskData& data);
};

#endif // JNIDISK_H