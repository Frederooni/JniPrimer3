#include "edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper.h"
#include "oligotm.h"
#include "thal.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

JNIEXPORT jdouble JNICALL
Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_calcTm(JNIEnv *env,
        jobject obj, jstring seq, jdouble d, jdouble mv, jdouble dv,
        jdouble dntp, jint tm_method, jint salt_method) {
  
  const char *sequence = (*env)->GetStringUTFChars(env, seq, 0);
  tm_method_type tm = tm_method;
  salt_correction_type salt = salt_method;

  double ret = oligotm(sequence, d, mv, dv, dntp, tm, salt);
  
  (*env)->ReleaseStringUTFChars(env, seq, sequence);

  return ret; 
}

JNIEXPORT jdouble JNICALL
Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_calcThermo(JNIEnv *env,
        jobject obj, jstring seq1, jstring seq2, jint maxloop, jdouble mv,
        jdouble dv, jdouble dntp, jdouble dna_conc, jdouble temp,
        jint temponly, jint dimer, jint aligntype) {

  const char *sequence1 = (*env)->GetStringUTFChars(env,seq1,0);
  const char *sequence2 = (*env)->GetStringUTFChars(env,seq2,0);
  
  thal_args a;
  
  a.type = (thal_alignment_type) aligntype;
  a.maxLoop = maxloop;
  a.mv = mv;
  a.dv = dv;
  a.dntp = dntp;
  a.dna_conc = dna_conc;
  a.temp = temp + 273.15;
  a.dimer = dimer;

  thal_mode mode = THL_FAST;
  
  thal_results o;

  thal_parameters thermodynamic_parameters;
  thal_set_null_parameters(&thermodynamic_parameters);
  set_default_thal_parameters(&thermodynamic_parameters);
  get_thermodynamic_values(&thermodynamic_parameters, &o);

  // printf("thal(%s,%s,mv %g dv %g dntp %g dna %g temp %g dimer %d type %d,0,&o)\n", sequence1, sequence2, mv, dv, dntp, dna_conc, temp, dimer, aligntype);
  thal(sequence1, sequence2, &a, mode, &o);
  
  (*env)->ReleaseStringUTFChars(env, seq1, sequence1);
  (*env)->ReleaseStringUTFChars(env, seq2, sequence2);

  // printf("result: msg %s temp %g end1 %d end2 %d struct %s\n", o.msg, o.temp, o.align_end_1, o.align_end_2, o.sec_struct);

  return o.temp;
}
