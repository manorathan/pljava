/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_postgresql_pljava_internal_PgSavepoint */

#ifndef _Included_org_postgresql_pljava_internal_PgSavepoint
#define _Included_org_postgresql_pljava_internal_PgSavepoint
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_postgresql_pljava_internal_PgSavepoint
 * Method:    _set
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_postgresql_pljava_internal_PgSavepoint__1set
  (JNIEnv *, jclass, jstring);

/*
 * Class:     org_postgresql_pljava_internal_PgSavepoint
 * Method:    _release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_postgresql_pljava_internal_PgSavepoint__1release
  (JNIEnv *, jclass, jlong);

/*
 * Class:     org_postgresql_pljava_internal_PgSavepoint
 * Method:    _rollback
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_org_postgresql_pljava_internal_PgSavepoint__1rollback
  (JNIEnv *, jclass, jlong);

/*
 * Class:     org_postgresql_pljava_internal_PgSavepoint
 * Method:    _getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_postgresql_pljava_internal_PgSavepoint__1getName
  (JNIEnv *, jclass, jlong);

/*
 * Class:     org_postgresql_pljava_internal_PgSavepoint
 * Method:    _getId
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_org_postgresql_pljava_internal_PgSavepoint__1getId
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
