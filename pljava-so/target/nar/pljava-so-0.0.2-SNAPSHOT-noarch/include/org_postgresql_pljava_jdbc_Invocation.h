/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_postgresql_pljava_jdbc_Invocation */

#ifndef _Included_org_postgresql_pljava_jdbc_Invocation
#define _Included_org_postgresql_pljava_jdbc_Invocation
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_postgresql_pljava_jdbc_Invocation
 * Method:    _register
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_postgresql_pljava_jdbc_Invocation__1register
  (JNIEnv *, jobject);

/*
 * Class:     org_postgresql_pljava_jdbc_Invocation
 * Method:    _getCurrent
 * Signature: ()Lorg/postgresql/pljava/jdbc/Invocation;
 */
JNIEXPORT jobject JNICALL Java_org_postgresql_pljava_jdbc_Invocation__1getCurrent
  (JNIEnv *, jclass);

/*
 * Class:     org_postgresql_pljava_jdbc_Invocation
 * Method:    _getNestingLevel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_postgresql_pljava_jdbc_Invocation__1getNestingLevel
  (JNIEnv *, jclass);

/*
 * Class:     org_postgresql_pljava_jdbc_Invocation
 * Method:    _clearErrorCondition
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_postgresql_pljava_jdbc_Invocation__1clearErrorCondition
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
