#include <stdio.h>
#include "thal.h"

int main(int argc, char **argv) {
  thal_args a;

  a.type = (thal_alignment_type) 1;
  a.maxLoop = 30;
  a.mv = 50;
  a.dv = 1.5;
  a.dntp = 0.6;
  a.dna_conc = 50;
  a.temp = 37 + 273.15;
  a.dimer = 0;

  thal_results o;

  thal_parameters thermodynamic_parameters;
  thal_set_null_parameters(&thermodynamic_parameters);
  set_default_thal_parameters(&thermodynamic_parameters);
  get_thermodynamic_values(&thermodynamic_parameters, &o);

  char *sequence1 = "CATTCCACCCTCACGTAAATGGA";
  char *sequence2 = "TCCATTTACGTGAGGGTGGAATG";
  printf("thal(%s,%s,mv %g dv %g dntp %g dna %g temp %g dimer %d type %d,0,&o)\n", sequence1, sequence2,
      a.mv, a.dv, a.dntp, a.dna_conc, a.temp, a.dimer, a.type);
  thal(sequence1, sequence2, &a, THL_GENERAL, &o);
  printf("result: msg %s temp %g end1 %d end2 %d struct %s\n", o.msg, o.temp, o.align_end_1, o.align_end_2, o.sec_struct);
}
