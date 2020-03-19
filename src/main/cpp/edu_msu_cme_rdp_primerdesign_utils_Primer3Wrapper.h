#include <jni.h>
/* Header for class edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper */

#ifndef _Included_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper
#define _Included_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper
 * Method:    calcTm
 */
JNIEXPORT jdouble JNICALL Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_calcTm(JNIEnv *, jobject,
        jstring);

/** Calculate melting temperature for two primers. */
JNIEXPORT void JNICALL Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_setParams(JNIEnv *, jobject,
        jint maxloop, jdouble mv, jdouble dv, jdouble dntp, jdouble dna_conc, jdouble temp, jint aligntype);

/** Calculate melting temperature for two primers. */
JNIEXPORT jdouble JNICALL Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_calcThermo
        (JNIEnv *, jobject, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif
