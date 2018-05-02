PIC_LD=ld

ARCHIVE_OBJS=
ARCHIVE_OBJS += _6811_archive_1.so
_6811_archive_1.so : archive.0/_6811_archive_1.a
	@$(AR) -s $<
	@$(PIC_LD) -shared  -o .//../work/ase_simv.daidir//_6811_archive_1.so --whole-archive $< --no-whole-archive
	@rm -f $@
	@ln -sf .//../work/ase_simv.daidir//_6811_archive_1.so $@


ARCHIVE_OBJS += _6818_archive_1.so
_6818_archive_1.so : archive.0/_6818_archive_1.a
	@$(AR) -s $<
	@$(PIC_LD) -shared  -o .//../work/ase_simv.daidir//_6818_archive_1.so --whole-archive $< --no-whole-archive
	@rm -f $@
	@ln -sf .//../work/ase_simv.daidir//_6818_archive_1.so $@


ARCHIVE_OBJS += _6819_archive_1.so
_6819_archive_1.so : archive.0/_6819_archive_1.a
	@$(AR) -s $<
	@$(PIC_LD) -shared  -o .//../work/ase_simv.daidir//_6819_archive_1.so --whole-archive $< --no-whole-archive
	@rm -f $@
	@ln -sf .//../work/ase_simv.daidir//_6819_archive_1.so $@


ARCHIVE_OBJS += _6820_archive_1.so
_6820_archive_1.so : archive.0/_6820_archive_1.a
	@$(AR) -s $<
	@$(PIC_LD) -shared  -o .//../work/ase_simv.daidir//_6820_archive_1.so --whole-archive $< --no-whole-archive
	@rm -f $@
	@ln -sf .//../work/ase_simv.daidir//_6820_archive_1.so $@






%.o: %.c
	$(CC_CG) $(CFLAGS_CG) -c -o $@ $<
CU_UDP_OBJS = \


CU_LVL_OBJS = \
SIM_l.o 

MAIN_OBJS = \
amcQwB.o objs/amcQw_d.o 

CU_OBJS = $(MAIN_OBJS) $(ARCHIVE_OBJS) $(CU_UDP_OBJS) $(CU_LVL_OBJS)

