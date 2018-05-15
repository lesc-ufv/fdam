# Future-proofing against Python 3 syntax changes in 'print'
from __future__ import print_function

import os
import sys
import traceback

if sys.version_info < (2, 7):
    import commands
else:
    pass

try:
    FDAM_BASEDIR = os.environ['FDAM_BASEDIR']
    sys.path.insert(0, FDAM_BASEDIR + '/fdam-hw-generator')
except:
    print("\033[1;31m")
    print("Set FDAM_BASEDIR environment variable and rerun the script!")
    print("\033[1;m")
    exit(1)

from src.utils import *

### Supported file extensions
# USERs may modify this if needed

VLOG_EXTENSIONS = [".vh", ".v"]
SVLOG_EXTENSIONS = [".sv", ".vs"]
VHD_EXTENSIONS = [".vhd"]

SIM_SOURCES = "sources.txt"

begin_green_fontcolor()
print("#############################################################################")
print("#                                                                           #")
print("#                      FDAM Sources Files QuickAssist                       #")
print("#                                                                           #")
print("#############################################################################")
end_green_fontcolor()

dir_project = commands_getoutput("pwd")
dir_rtl = ""
if (len(dir_project.strip()) != 0):
    dir_rtl = dir_project[:-1] + "/../rtl"
else:
    begin_red_fontcolor()
    print("Error!")
    end_red_fontcolor()
    exit(1)

fd_sim = open(SIM_SOURCES, "w")

vincdir = commands_getoutput("ls ../rtl").strip().split('\n')
incdir = ""
for v in vincdir:
    incdir= incdir + "+incdir+../rtl/%s\n"%v
    
header = "../rtl/base/afu.json\n\n# For now we force MPF to a particular platform.  This will be fixed later.\n+define+MPF_PLATFORM_BDX\n\nQI:../rtl/base/static_base_PAR_files.qsf\n\nSI:$FPGA_BBB_CCI_SRC/BBB_cci_mpf/hw/sim/cci_mpf_sim_addenda.txt\nSI:$FPGA_BBB_CCI_SRC/BBB_ccip_async/hw/sim/ccip_async_sim_addenda.txt\n\n%s\n"%incdir
fd_sim.write(header)

# Check if VHDL files exist, populate if any
str_sim = find_files(dir_rtl,"VHDL",VHD_EXTENSIONS)
if len(str_sim) != 0:
    fd_sim.write(str_sim)
else:
    begin_red_fontcolor()
    print("No VHDL files were found !")
    end_red_fontcolor()

# Check if V files exist, populate if any
str_sim = find_files(dir_rtl,"Verilog",VLOG_EXTENSIONS)
if len(str_sim) != 0:
    fd_sim.write(str_sim)
else:
    begin_red_fontcolor()
    print("No Verilog files were found !")
    end_red_fontcolor()

# Check if SV files exist, populate if any
str_sim = find_files(dir_rtl,"System Verilog",SVLOG_EXTENSIONS)
if len(str_sim) != 0:
    fd_sim.write(str_sim)
else:
    begin_red_fontcolor()
    print("No SystemVerilog files were found !")
    end_red_fontcolor()

begin_green_fontcolor()
print("End of update!")
end_green_fontcolor()
