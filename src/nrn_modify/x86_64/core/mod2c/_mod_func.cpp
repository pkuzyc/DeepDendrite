#include <stdio.h>
namespace coreneuron {
extern int nrnmpi_myid;
extern int nrn_nobanner_;
extern int _Ca_reg(void),
  _CaDynamics_E2_reg(void),
  _Ca_HVA_reg(void),
  _Ca_LVAst_reg(void),
  _Im_reg(void),
  _K_Pst_reg(void),
  _K_Tst_reg(void),
  _NMDA_reg(void),
  _NaTg_reg(void),
  _Nap_Et2_reg(void),
  _SK_E2_reg(void),
  _SKv3_1_reg(void),
  _halfgap_reg(void),
  _vecevent_reg(void);

void modl_reg() {
    if (!nrn_nobanner_ && nrnmpi_myid < 1) {
        fprintf(stderr, " Additional mechanisms from files\n");
        fprintf(stderr, " Ca.mod");
        fprintf(stderr, " CaDynamics_E2.mod");
        fprintf(stderr, " Ca_HVA.mod");
        fprintf(stderr, " Ca_LVAst.mod");
        fprintf(stderr, " Im.mod");
        fprintf(stderr, " K_Pst.mod");
        fprintf(stderr, " K_Tst.mod");
        fprintf(stderr, " NMDA.mod");
        fprintf(stderr, " NaTg.mod");
        fprintf(stderr, " Nap_Et2.mod");
        fprintf(stderr, " SK_E2.mod");
        fprintf(stderr, " SKv3_1.mod");
        fprintf(stderr, " halfgap.mod");
        fprintf(stderr, " vecevent.mod");
        fprintf(stderr, "\n\n");
    }

    _Ca_reg();
    _CaDynamics_E2_reg();
    _Ca_HVA_reg();
    _Ca_LVAst_reg();
    _Im_reg();
    _K_Pst_reg();
    _K_Tst_reg();
    _NMDA_reg();
    _NaTg_reg();
    _Nap_Et2_reg();
    _SK_E2_reg();
    _SKv3_1_reg();
    _halfgap_reg();
    _vecevent_reg();
}
} //namespace coreneuron
