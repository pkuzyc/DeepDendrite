bbsmpibuf* nrnmpi_newbuf(int size) {
  return (*p_nrnmpi_newbuf)(size);
}
void nrnmpi_copy(bbsmpibuf* dest, bbsmpibuf* src) {
  (*p_nrnmpi_copy)(dest, src);
}
void nrnmpi_ref(bbsmpibuf* buf) {
  (*p_nrnmpi_ref)(buf);
}
void nrnmpi_unref(bbsmpibuf* buf) {
  (*p_nrnmpi_unref)(buf);
}
void nrnmpi_upkbegin(bbsmpibuf* buf) {
  (*p_nrnmpi_upkbegin)(buf);
}
char* nrnmpi_getkey(bbsmpibuf* buf) {
  return (*p_nrnmpi_getkey)(buf);
}
int nrnmpi_getid(bbsmpibuf* buf) {
  return (*p_nrnmpi_getid)(buf);
}
int nrnmpi_upkint(bbsmpibuf* buf) {
  return (*p_nrnmpi_upkint)(buf);
}
double nrnmpi_upkdouble(bbsmpibuf* buf) {
  return (*p_nrnmpi_upkdouble)(buf);
}
void nrnmpi_upkvec(int n, double* x, bbsmpibuf* buf) {
  (*p_nrnmpi_upkvec)(n, x, buf);
}
char* nrnmpi_upkstr(bbsmpibuf* buf) {
  return (*p_nrnmpi_upkstr)(buf);
}
char* nrnmpi_upkpickle(size_t* size, bbsmpibuf* buf) {
  return (*p_nrnmpi_upkpickle)(size, buf);
}
void nrnmpi_pkbegin(bbsmpibuf* buf) {
  (*p_nrnmpi_pkbegin)(buf);
}
void nrnmpi_enddata(bbsmpibuf* buf) {
  (*p_nrnmpi_enddata)(buf);
}
void nrnmpi_pkint(int i, bbsmpibuf* buf) {
  (*p_nrnmpi_pkint)(i, buf);
}
void nrnmpi_pkdouble(double x, bbsmpibuf* buf) {
  (*p_nrnmpi_pkdouble)(x, buf);
}
void nrnmpi_pkvec(int n, double* x, bbsmpibuf* buf) {
  (*p_nrnmpi_pkvec)(n, x, buf);
}
void nrnmpi_pkstr(const char* s, bbsmpibuf* buf) {
  (*p_nrnmpi_pkstr)(s, buf);
}
void nrnmpi_pkpickle(const char* s, size_t size, bbsmpibuf* buf) {
  (*p_nrnmpi_pkpickle)(s, size, buf);
}
int nrnmpi_iprobe(int* size, int* tag, int* source) {
  return (*p_nrnmpi_iprobe)(size, tag, source);
}
void nrnmpi_probe(int* size, int* tag, int* source) {
  (*p_nrnmpi_probe)(size, tag, source);
}
void nrnmpi_bbssend(int dest, int tag, bbsmpibuf* r) {
  (*p_nrnmpi_bbssend)(dest, tag, r);
}
int nrnmpi_bbsrecv(int source, bbsmpibuf* r) {
  return (*p_nrnmpi_bbsrecv)(source, r);
}
int nrnmpi_bbssendrecv(int dest, int tag, bbsmpibuf* s, bbsmpibuf* r) {
  return (*p_nrnmpi_bbssendrecv)(dest, tag, s, r);
}
void nrnmpi_init(int nrnmpi_under_nrncontrol, int* pargc, char*** pargv) {
  (*p_nrnmpi_init)(nrnmpi_under_nrncontrol, pargc, pargv);
}
int nrnmpi_wrap_mpi_init(int* flag) {
  return (*p_nrnmpi_wrap_mpi_init)(flag);
}
double nrnmpi_wtime() {
  return (*p_nrnmpi_wtime)();
}
void nrnmpi_terminate() {
  (*p_nrnmpi_terminate)();
}
void nrnmpi_abort(int errcode) {
  (*p_nrnmpi_abort)(errcode);
}
void nrnmpi_subworld_size(int n) {
  (*p_nrnmpi_subworld_size)(n);
}
void nrnmpi_spike_initialize() {
  (*p_nrnmpi_spike_initialize)();
}
int nrnmpi_spike_exchange() {
  return (*p_nrnmpi_spike_exchange)();
}
int nrnmpi_spike_exchange_compressed() {
  return (*p_nrnmpi_spike_exchange_compressed)();
}
double nrnmpi_mindelay(double maxdel) {
  return (*p_nrnmpi_mindelay)(maxdel);
}
int nrnmpi_int_allmax(int i) {
  return (*p_nrnmpi_int_allmax)(i);
}
void nrnmpi_int_gather(int* s, int* r, int cnt, int root) {
  (*p_nrnmpi_int_gather)(s, r, cnt, root);
}
void nrnmpi_int_gatherv(int* s, int scnt, int* r, int* rcnt, int* rdispl, int root) {
  (*p_nrnmpi_int_gatherv)(s, scnt, r, rcnt, rdispl, root);
}
void nrnmpi_char_gatherv(char* s, int scnt, char* r, int* rcnt, int* rdispl, int root) {
  (*p_nrnmpi_char_gatherv)(s, scnt, r, rcnt, rdispl, root);
}
void nrnmpi_int_scatter(int* s, int* r, int cnt, int root) {
  (*p_nrnmpi_int_scatter)(s, r, cnt, root);
}
void nrnmpi_char_scatterv(char* s, int* scnt, int* sdispl, char* r, int rcnt, int root) {
  (*p_nrnmpi_char_scatterv)(s, scnt, sdispl, r, rcnt, root);
}
void nrnmpi_int_allgather(int* s, int* r, int n) {
  (*p_nrnmpi_int_allgather)(s, r, n);
}
void nrnmpi_int_allgather_inplace(int* srcdest, int n) {
  (*p_nrnmpi_int_allgather_inplace)(srcdest, n);
}
void nrnmpi_int_allgatherv(int* s, int* r, int* n, int* dspl) {
  (*p_nrnmpi_int_allgatherv)(s, r, n, dspl);
}
void nrnmpi_char_allgatherv(char* s, char* r, int* n, int* dspl) {
  (*p_nrnmpi_char_allgatherv)(s, r, n, dspl);
}
void nrnmpi_int_alltoall(int* s, int* r, int n) {
  (*p_nrnmpi_int_alltoall)(s, r, n);
}
void nrnmpi_int_alltoallv(int* s, int* scnt, int* sdispl, int* r, int* rcnt, int* rdispl) {
  (*p_nrnmpi_int_alltoallv)(s, scnt, sdispl, r, rcnt, rdispl);
}
void nrnmpi_long_allgatherv(int64_t* s, int64_t* r, int* n, int* dspl) {
  (*p_nrnmpi_long_allgatherv)(s, r, n, dspl);
}
void nrnmpi_long_allgatherv_inplace(long* srcdest, int* n, int* dspl) {
  (*p_nrnmpi_long_allgatherv_inplace)(srcdest, n, dspl);
}
void nrnmpi_long_alltoallv(int64_t* s, int* scnt, int* sdispl, int64_t* r, int* rcnt, int* rdispl) {
  (*p_nrnmpi_long_alltoallv)(s, scnt, sdispl, r, rcnt, rdispl);
}
void nrnmpi_dbl_allgatherv(double* s, double* r, int* n, int* dspl) {
  (*p_nrnmpi_dbl_allgatherv)(s, r, n, dspl);
}
void nrnmpi_dbl_allgatherv_inplace(double* srcdest, int* n, int* dspl) {
  (*p_nrnmpi_dbl_allgatherv_inplace)(srcdest, n, dspl);
}
void nrnmpi_dbl_alltoallv(double* s, int* scnt, int* sdispl, double* r, int* rcnt, int* rdispl) {
  (*p_nrnmpi_dbl_alltoallv)(s, scnt, sdispl, r, rcnt, rdispl);
}
void nrnmpi_char_alltoallv(char* s, int* scnt, int* sdispl, char* r, int* rcnt, int* rdispl) {
  (*p_nrnmpi_char_alltoallv)(s, scnt, sdispl, r, rcnt, rdispl);
}
void nrnmpi_dbl_broadcast(double* buf, int cnt, int root) {
  (*p_nrnmpi_dbl_broadcast)(buf, cnt, root);
}
void nrnmpi_int_broadcast(int* buf, int cnt, int root) {
  (*p_nrnmpi_int_broadcast)(buf, cnt, root);
}
void nrnmpi_char_broadcast(char* buf, int cnt, int root) {
  (*p_nrnmpi_char_broadcast)(buf, cnt, root);
}
void nrnmpi_char_broadcast_world(char** pstr, int root) {
  (*p_nrnmpi_char_broadcast_world)(pstr, root);
}
int nrnmpi_int_sum_reduce(int in) {
  return (*p_nrnmpi_int_sum_reduce)(in);
}
void nrnmpi_assert_opstep(int opstep, double t) {
  (*p_nrnmpi_assert_opstep)(opstep, t);
}
double nrnmpi_dbl_allmin(double x) {
  return (*p_nrnmpi_dbl_allmin)(x);
}
int nrnmpi_pgvts_least(double* t, int* op, int* init) {
  return (*p_nrnmpi_pgvts_least)(t, op, init);
}
void nrnmpi_send_doubles(double* pd, int cnt, int dest, int tag) {
  (*p_nrnmpi_send_doubles)(pd, cnt, dest, tag);
}
void nrnmpi_recv_doubles(double* pd, int cnt, int src, int tag) {
  (*p_nrnmpi_recv_doubles)(pd, cnt, src, tag);
}
void nrnmpi_postrecv_doubles(double* pd, int cnt, int src, int tag, void** request) {
  (*p_nrnmpi_postrecv_doubles)(pd, cnt, src, tag, request);
}
void nrnmpi_wait(void** request) {
  (*p_nrnmpi_wait)(request);
}
void nrnmpi_barrier() {
  (*p_nrnmpi_barrier)();
}
double nrnmpi_dbl_allreduce(double x, int type) {
  return (*p_nrnmpi_dbl_allreduce)(x, type);
}
void nrnmpi_dbl_allreduce_vec(double* src, double* dest, int cnt, int type) {
  (*p_nrnmpi_dbl_allreduce_vec)(src, dest, cnt, type);
}
void nrnmpi_longdbl_allreduce_vec(longdbl* src, longdbl* dest, int cnt, int type) {
  (*p_nrnmpi_longdbl_allreduce_vec)(src, dest, cnt, type);
}
void nrnmpi_long_allreduce_vec(long* src, long* dest, int cnt, int type) {
  (*p_nrnmpi_long_allreduce_vec)(src, dest, cnt, type);
}
void nrnmpi_dbl_allgather(double* s, double* r, int n) {
  (*p_nrnmpi_dbl_allgather)(s, r, n);
}
#if BGPDMA
void nrnmpi_bgp_comm() {
  (*p_nrnmpi_bgp_comm)();
}
void nrnmpi_bgp_multisend(NRNMPI_Spike* spk, int n, int* hosts) {
  (*p_nrnmpi_bgp_multisend)(spk, n, hosts);
}
int nrnmpi_bgp_single_advance(NRNMPI_Spike* spk) {
  return (*p_nrnmpi_bgp_single_advance)(spk);
}
int nrnmpi_bgp_conserve(int nsend, int nrecv) {
  return (*p_nrnmpi_bgp_conserve)(nsend, nrecv);
}
#endif
