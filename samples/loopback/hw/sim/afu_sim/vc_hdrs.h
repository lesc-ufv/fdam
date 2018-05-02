#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <dlfcn.h>
#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VC_TYPES_
#define _VC_TYPES_
/* common definitions shared with DirectC.h */

typedef unsigned int U;
typedef unsigned char UB;
typedef unsigned char scalar;
typedef struct { U c; U d;} vec32;

#define scalar_0 0
#define scalar_1 1
#define scalar_z 2
#define scalar_x 3

extern long long int ConvUP2LLI(U* a);
extern void ConvLLI2UP(long long int a1, U* a2);
extern long long int GetLLIresult();
extern void StoreLLIresult(const unsigned int* data);
typedef struct VeriC_Descriptor *vc_handle;

#ifndef SV_3_COMPATIBILITY
#define SV_STRING const char*
#else
#define SV_STRING char*
#endif

#endif /* _VC_TYPES_ */

typedef struct _vcs_dpi_cci_pkt	cci_pkt;
typedef struct _vcs_dpi_mmio_t	mmio_t;
typedef struct _vcs_dpi_umsgcmd_t	umsgcmd_t;
typedef struct _vcs_dpi_ase_cfg_t	ase_cfg_t;

struct	_vcs_dpi_cci_pkt	{
	int	mode;
	int	qw_start;
	int	mdata;
	long long	cl_addr;
	long long	qword[8];
	int	resp_channel;
	int	intr_id;
	int	success;
};

struct	_vcs_dpi_mmio_t	{
	int	tid;
	int	write_en;
	int	width;
	int	addr;
	long long	qword[8];
	int	resp_en;
};

struct	_vcs_dpi_umsgcmd_t	{
	int	id;
	int	hint;
	long long	qword[8];
};

struct	_vcs_dpi_ase_cfg_t	{
	int	ase_mode;
	int	ase_timeout;
	int	ase_num_tests;
	int	enable_reuse_seed;
	int	ase_seed;
	int	enable_cl_view;
	int	usr_tps;
	int	phys_memory_available_gb;
};



 extern void scope_function();

 extern void ase_init();

 extern void ase_ready();

 extern void ase_listener();

 extern void start_simkill_countdown();

 extern void count_error_flag_pong(/* INPUT */int flag);

 extern int ase_instance_running();

 extern void update_glbl_dealloc(/* INPUT */int flag);

 extern void sv2c_config_dex(/* INPUT */const char* str);

 extern void sv2c_script_dex(/* INPUT */const char* str);

 extern void rd_memline_dex(/* INOUT */cci_pkt *foo);

 extern void wr_memline_dex(/* INOUT */cci_pkt *foo);

 extern int get_ase_seed();

 extern void mmio_response(/* INOUT */mmio_t *mmio_pkt);

 extern void sw_reset_response();

 extern void ase_reset_trig();

 extern void afu_softreset_trig(/* INPUT */int init, /* INPUT */int value);

 extern void run_clocks(/* INPUT */int num_clks);

 extern void buffer_msg_inject(/* INPUT */int timestamp_en, /* INPUT */const char* logstr);

 extern void mmio_dispatch(/* INPUT */int initialize, /* INPUT */mmio_t *mmio_pkt);

 extern void umsg_dispatch(/* INPUT */int init, /* INPUT */umsgcmd_t *umsg_pkt);

 extern void ase_config_dex(/* INPUT */ase_cfg_t *cfg_in);

 extern void count_error_flag_ping();

 extern void simkill();
void SdisableFork();

#ifdef __cplusplus
}
#endif

