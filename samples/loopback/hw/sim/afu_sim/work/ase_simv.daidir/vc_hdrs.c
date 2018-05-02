#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <dlfcn.h>
#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* VCS error reporting routine */
extern void vcsMsgReport1(const char *, const char *, int, void *, void*, const char *);

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

#ifndef _VC_STRUCT_TYPE_cci_pkt_
#define _VC_STRUCT_TYPE_cci_pkt_
typedef struct _vcs_dpi_cci_pkt	cci_pkt;
#endif
#ifndef _VC_STRUCT_TYPE_mmio_t_
#define _VC_STRUCT_TYPE_mmio_t_
typedef struct _vcs_dpi_mmio_t	mmio_t;
#endif
#ifndef _VC_STRUCT_TYPE_umsgcmd_t_
#define _VC_STRUCT_TYPE_umsgcmd_t_
typedef struct _vcs_dpi_umsgcmd_t	umsgcmd_t;
#endif
#ifndef _VC_STRUCT_TYPE_ase_cfg_t_
#define _VC_STRUCT_TYPE_ase_cfg_t_
typedef struct _vcs_dpi_ase_cfg_t	ase_cfg_t;
#endif

#ifndef _VC_STRUCT_TYPE_cci_pkt_
#define _VC_STRUCT_TYPE_cci_pkt_
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

#endif

#ifndef _VC_STRUCT_TYPE_mmio_t_
#define _VC_STRUCT_TYPE_mmio_t_
struct	_vcs_dpi_mmio_t	{
	int	tid;
	int	write_en;
	int	width;
	int	addr;
	long long	qword[8];
	int	resp_en;
};

#endif

#ifndef _VC_STRUCT_TYPE_umsgcmd_t_
#define _VC_STRUCT_TYPE_umsgcmd_t_
struct	_vcs_dpi_umsgcmd_t	{
	int	id;
	int	hint;
	long long	qword[8];
};

#endif

#ifndef _VC_STRUCT_TYPE_ase_cfg_t_
#define _VC_STRUCT_TYPE_ase_cfg_t_
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

#endif


#ifndef __VCS_IMPORT_DPI_STUB_scope_function
#define __VCS_IMPORT_DPI_STUB_scope_function
__attribute__((weak)) void scope_function()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)()) dlsym(RTLD_NEXT, "scope_function");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "scope_function");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_scope_function */

#ifndef __VCS_IMPORT_DPI_STUB_ase_init
#define __VCS_IMPORT_DPI_STUB_ase_init
__attribute__((weak)) void ase_init()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)()) dlsym(RTLD_NEXT, "ase_init");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "ase_init");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_ase_init */

#ifndef __VCS_IMPORT_DPI_STUB_ase_ready
#define __VCS_IMPORT_DPI_STUB_ase_ready
__attribute__((weak)) void ase_ready()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)()) dlsym(RTLD_NEXT, "ase_ready");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "ase_ready");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_ase_ready */

#ifndef __VCS_IMPORT_DPI_STUB_ase_listener
#define __VCS_IMPORT_DPI_STUB_ase_listener
__attribute__((weak)) void ase_listener()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)()) dlsym(RTLD_NEXT, "ase_listener");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "ase_listener");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_ase_listener */

#ifndef __VCS_IMPORT_DPI_STUB_start_simkill_countdown
#define __VCS_IMPORT_DPI_STUB_start_simkill_countdown
__attribute__((weak)) void start_simkill_countdown()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)()) dlsym(RTLD_NEXT, "start_simkill_countdown");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "start_simkill_countdown");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_start_simkill_countdown */

#ifndef __VCS_IMPORT_DPI_STUB_count_error_flag_pong
#define __VCS_IMPORT_DPI_STUB_count_error_flag_pong
__attribute__((weak)) void count_error_flag_pong(/* INPUT */int A_1)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* INPUT */int A_1) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)(int A_1)) dlsym(RTLD_NEXT, "count_error_flag_pong");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "count_error_flag_pong");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_count_error_flag_pong */

#ifndef __VCS_IMPORT_DPI_STUB_ase_instance_running
#define __VCS_IMPORT_DPI_STUB_ase_instance_running
__attribute__((weak)) int ase_instance_running()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static int (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (int (*)()) dlsym(RTLD_NEXT, "ase_instance_running");
    }
    if (_vcs_dpi_fp_) {
        return _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "ase_instance_running");
        return 0;
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_ase_instance_running */

#ifndef __VCS_IMPORT_DPI_STUB_update_glbl_dealloc
#define __VCS_IMPORT_DPI_STUB_update_glbl_dealloc
__attribute__((weak)) void update_glbl_dealloc(/* INPUT */int A_1)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* INPUT */int A_1) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)(int A_1)) dlsym(RTLD_NEXT, "update_glbl_dealloc");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "update_glbl_dealloc");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_update_glbl_dealloc */

#ifndef __VCS_IMPORT_DPI_STUB_sv2c_config_dex
#define __VCS_IMPORT_DPI_STUB_sv2c_config_dex
__attribute__((weak)) void sv2c_config_dex(/* INPUT */const char* A_1)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* INPUT */const char* A_1) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)(const char* A_1)) dlsym(RTLD_NEXT, "sv2c_config_dex");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "sv2c_config_dex");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_sv2c_config_dex */

#ifndef __VCS_IMPORT_DPI_STUB_sv2c_script_dex
#define __VCS_IMPORT_DPI_STUB_sv2c_script_dex
__attribute__((weak)) void sv2c_script_dex(/* INPUT */const char* A_1)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* INPUT */const char* A_1) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)(const char* A_1)) dlsym(RTLD_NEXT, "sv2c_script_dex");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "sv2c_script_dex");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_sv2c_script_dex */

#ifndef __VCS_IMPORT_DPI_STUB_rd_memline_dex
#define __VCS_IMPORT_DPI_STUB_rd_memline_dex
__attribute__((weak)) void rd_memline_dex(/* INOUT */cci_pkt *A_1)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* INOUT */cci_pkt *A_1) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)(cci_pkt* A_1)) dlsym(RTLD_NEXT, "rd_memline_dex");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "rd_memline_dex");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_rd_memline_dex */

#ifndef __VCS_IMPORT_DPI_STUB_wr_memline_dex
#define __VCS_IMPORT_DPI_STUB_wr_memline_dex
__attribute__((weak)) void wr_memline_dex(/* INOUT */cci_pkt *A_1)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* INOUT */cci_pkt *A_1) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)(cci_pkt* A_1)) dlsym(RTLD_NEXT, "wr_memline_dex");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "wr_memline_dex");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_wr_memline_dex */

#ifndef __VCS_IMPORT_DPI_STUB_get_ase_seed
#define __VCS_IMPORT_DPI_STUB_get_ase_seed
__attribute__((weak)) int get_ase_seed()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static int (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (int (*)()) dlsym(RTLD_NEXT, "get_ase_seed");
    }
    if (_vcs_dpi_fp_) {
        return _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "get_ase_seed");
        return 0;
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_get_ase_seed */

#ifndef __VCS_IMPORT_DPI_STUB_mmio_response
#define __VCS_IMPORT_DPI_STUB_mmio_response
__attribute__((weak)) void mmio_response(/* INOUT */mmio_t *A_1)
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)(/* INOUT */mmio_t *A_1) = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)(mmio_t* A_1)) dlsym(RTLD_NEXT, "mmio_response");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_(A_1);
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "mmio_response");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_mmio_response */

#ifndef __VCS_IMPORT_DPI_STUB_sw_reset_response
#define __VCS_IMPORT_DPI_STUB_sw_reset_response
__attribute__((weak)) void sw_reset_response()
{
    static int _vcs_dpi_stub_initialized_ = 0;
    static void (*_vcs_dpi_fp_)() = NULL;
    if (!_vcs_dpi_stub_initialized_) {
        _vcs_dpi_stub_initialized_ = 1;
        _vcs_dpi_fp_ = (void (*)()) dlsym(RTLD_NEXT, "sw_reset_response");
    }
    if (_vcs_dpi_fp_) {
        _vcs_dpi_fp_();
    } else {
        const char *fileName;
        int lineNumber;
        svGetCallerInfo(&fileName, &lineNumber);
        vcsMsgReport1("DPI-DIFNF", fileName, lineNumber, 0, 0, "sw_reset_response");
    }
}
#endif /* __VCS_IMPORT_DPI_STUB_sw_reset_response */

#ifndef __VCS_EXPORT_DPI_DUMMY_REFERENCES__
#define __VCS_EXPORT_DPI_DUMMY_REFERENCES__
/* Dummy references to those export DPI routines.
 * The symbols will be then exported, so the
 * import DPI routines in another shared
 * libraries can call.
 */
void __vcs_export_dpi_dummy_references__();
void __vcs_export_dpi_dummy_references__()
{
    extern void ase_reset_trig(void);
    void (*fp0)(void) = (void (*)(void)) ase_reset_trig;
    fp0 = fp0;
    extern void afu_softreset_trig(void);
    void (*fp1)(void) = (void (*)(void)) afu_softreset_trig;
    fp1 = fp1;
    extern void run_clocks(void);
    void (*fp2)(void) = (void (*)(void)) run_clocks;
    fp2 = fp2;
    extern void buffer_msg_inject(void);
    void (*fp3)(void) = (void (*)(void)) buffer_msg_inject;
    fp3 = fp3;
    extern void mmio_dispatch(void);
    void (*fp4)(void) = (void (*)(void)) mmio_dispatch;
    fp4 = fp4;
    extern void umsg_dispatch(void);
    void (*fp5)(void) = (void (*)(void)) umsg_dispatch;
    fp5 = fp5;
    extern void ase_config_dex(void);
    void (*fp6)(void) = (void (*)(void)) ase_config_dex;
    fp6 = fp6;
    extern void count_error_flag_ping(void);
    void (*fp7)(void) = (void (*)(void)) count_error_flag_ping;
    fp7 = fp7;
    extern void simkill(void);
    void (*fp8)(void) = (void (*)(void)) simkill;
    fp8 = fp8;
}
#endif /* __VCS_EXPORT_DPI_DUMMY_REFERENCES_ */

#ifdef __cplusplus
}
#endif

