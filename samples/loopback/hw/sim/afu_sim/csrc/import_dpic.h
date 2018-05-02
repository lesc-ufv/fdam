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
