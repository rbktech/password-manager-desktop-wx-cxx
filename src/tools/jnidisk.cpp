#include "jnidisk.h"

#include <windows.h>

typedef jint(JNICALL* pCreateJavaVM)(JavaVM**, void**, void*);

CJNIDisk::CJNIDisk()
{
#ifdef JVM_RELEAS
    /** Init variavle */

    JavaVMInitArgs vm_args;  /* JDK/JRE 6 VM initialization arguments */
    JavaVMOption options[1]; /* Option for JavaVM */

    const char* path = "-Djava.class.path="
                       "../libs/codemanager_java.jar;"
                       "../libs/commons-logging-1.2.jar;"
                       "../libs/httpclient-4.5.13.jar;"
                       "../libs/httpcore-4.4.15.jar;"
                       "../libs/xmlpull-1.1.3.4a.jar;";

    options[0].optionString = const_cast<char*>(path);

    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_TRUE;

    /** Load jvm.dll */
    HINSTANCE hInstance = LoadLibrary("../libs/jdk-18.0.1.1/bin/server/jvm.dll");

    /** If JNI_CreateJavaVM not work, get pCreateJavaVM */
    pCreateJavaVM CreateJavaVM = (pCreateJavaVM)GetProcAddress(hInstance, "JNI_CreateJavaVM");

    CreateJavaVM(&mJavaVM, (void**)&mJNIEnv, &vm_args);
#endif
    // resVal = CreateJavaVM(&mJavaVM, (void**)&mJNIEnv, &vm_args);
    // if(resVal != 0)
    // exception(mJNIEnv, "JavaVM is NOT create\n");
    // printf("JavaVM is create\n");
}

CJNIDisk::~CJNIDisk()
{
    if(mJavaVM != nullptr)
        mJavaVM->DestroyJavaVM();
}

bool CJNIDisk::load(CJNIDiskData& data)
{
    data.nameClass = "DiskLibJava";
    data.nameMethod = "load";
    data.signMethod = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z";

    return jvm(data);
}

bool CJNIDisk::upload(CJNIDiskData& data)
{
    data.nameClass = "DiskLibJava";
    data.nameMethod = "upload";
    data.signMethod = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z";

    return jvm(data);
}

bool CJNIDisk::jvm(const CJNIDiskData& data)
{
    /** Init variavle */

    jstring arg[3];                /* Array argument for call method */
    jboolean resVal = 0;           /* Result call methods */
    jclass objClass = nullptr;     /* Call java class */
    jmethodID objMethod = nullptr; /* Call java method */

    // --

    objClass = mJNIEnv->FindClass(data.nameClass);
    if(objClass == nullptr)
        exception(mJNIEnv, "Java class is NOT find\n");
    // printf("Java class is find\n");

    objMethod = mJNIEnv->GetStaticMethodID(objClass, data.nameMethod, data.signMethod);
    if(objMethod == nullptr)
        exception(mJNIEnv, "Java method is NOT find\n");
    // printf("Java method is find\n");

    arg[0] = mJNIEnv->NewStringUTF(data.token);
    arg[1] = mJNIEnv->NewStringUTF(data.remoteFullPath);
    arg[2] = mJNIEnv->NewStringUTF(data.localFullPath);

    resVal = mJNIEnv->CallBooleanMethod(objClass, objMethod, arg[0], arg[1], arg[2]);
    // printf("result: %d\n", resVal);

    return resVal;
}

void CJNIDisk::exception(void* jniEnv, const char* message)
{
    JNIEnv* env = static_cast<JNIEnv*>(jniEnv);

    if(env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }

    throw message;
}