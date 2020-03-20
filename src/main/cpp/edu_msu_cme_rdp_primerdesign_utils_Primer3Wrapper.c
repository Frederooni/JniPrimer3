#include "edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper.h"
#include "oligotm.h"
#include "thal.h"
#include "thal_parameters.h"
#include <stdio.h>

static __thread int init_thermo_done = 0;
static __thread int init_params_done = 0;
static __thread thal_args a;

static void init_thermo() {
    if (init_thermo_done) return;
    thal_results o;
    thal_parameters thermodynamic_parameters;
    thal_set_null_parameters(&thermodynamic_parameters);
    set_default_thal_parameters(&thermodynamic_parameters);
    get_thermodynamic_values(&thermodynamic_parameters, &o);
    init_thermo_done = 1;
}

/** If parameters have not been set then set some reasonable defaults. */
static void init_params() {
    init_thermo();
    if (init_params_done) return;
    a.type = thal_any;
    a.maxLoop = 30;
    a.mv = 50;
    a.dv = 1.5;
    a.dntp = 0.6;
    a.dna_conc = 50;
    a.temp = 37 + 273.15;
    a.dimer = 1; // this value is ignored
    init_params_done = 1;
}

static void set_params(int maxloop, double mv,
                       double dv, double dntp, double dna_conc, double temp,
                       int aligntype) {
    init_thermo();
    a.type = (thal_alignment_type) aligntype;
    a.maxLoop = maxloop;
    a.mv = mv;
    a.dv = dv;
    a.dntp = dntp;
    a.dna_conc = dna_conc;
    a.temp = temp + 273.15;
    a.dimer = 1; // this value is ignored
    init_params_done = 1;
}

static void check_init_params() {
    if (!init_params_done || !init_thermo_done) {
        fprintf(stderr, "\nJavaPrimer3: Parameters have not been set!\n\n");
        exit(1);
    }
}

JNIEXPORT jdouble JNICALL
Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_calcTm(JNIEnv *env,jobject obj, jstring seq) {
  const char *sequence = (*env)->GetStringUTFChars(env, seq, 0);
  check_init_params();
  double ret = oligotm(sequence, a.dna_conc, a.mv, a.dv, a.dntp, 1, 1);
  (*env)->ReleaseStringUTFChars(env, seq, sequence);
  return ret; 
}

JNIEXPORT void JNICALL
Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_setParamsNative(JNIEnv *env, jobject obj,
        jint maxloop, jdouble mv, jdouble dv, jdouble dntp, jdouble dna_conc, jdouble temp,
        jint aligntype) {
    // fprintf(stderr, "setParamsNative calling set_params\n");
    set_params(maxloop, mv, dv, dntp, dna_conc, temp, aligntype);
    // fprintf(stderr, "setParamsNative done calling set_params\n");
}

JNIEXPORT jdouble JNICALL
Java_edu_msu_cme_rdp_primerdesign_utils_Primer3Wrapper_calcThermo(JNIEnv *env, jobject obj,
        jstring seq1, jstring seq2) {

    const char *sequence1 = (*env)->GetStringUTFChars(env, seq1, 0);
    const char *sequence2 = (*env)->GetStringUTFChars(env, seq2, 0);

    check_init_params();

    thal_results o;

    // printf("thal(%s,%s,mv %g dv %g dntp %g dna %g temp %g dimer %d type %d,0,&o)\n", sequence1, sequence2, mv, dv, dntp, dna_conc, temp, dimer, aligntype);
    thal(sequence1, sequence2, &a, THL_FAST, &o);

    (*env)->ReleaseStringUTFChars(env, seq1, sequence1);
    (*env)->ReleaseStringUTFChars(env, seq2, sequence2);

    // printf("result: msg %s temp %g end1 %d end2 %d struct %s\n", o.msg, o.temp, o.align_end_1, o.align_end_2, o.sec_struct);

    return o.temp;
}
