# Legal Notice: (C)2017 Altera Corporation. All rights reserved.  Your
# use of Altera Corporation's design tools, logic functions and other
# software and tools, and its AMPP partner logic functions, and any
# output files any of the foregoing (including device programming or
# simulation files), and any associated documentation or information are
# expressly subject to the terms and conditions of the Altera Program
# License Subscription Agreement or other applicable license agreement,
# including, without limitation, that your use is for the sole purpose
# of programming logic devices manufactured by Altera and sold by Altera
# or its authorized distributors.  Please refer to the applicable
# agreement for further details.

#**************************************************************
# Timequest JTAG clock definition
#   Uncommenting the following lines will define the JTAG
#   clock in TimeQuest Timing Analyzer
#**************************************************************

#create_clock -period 10MHz {altera_reserved_tck}
#set_clock_groups -asynchronous -group {altera_reserved_tck}

#**************************************************************
# Set TCL Path Variables 
#**************************************************************

set 	olive_std_core_nios2_fast_cpu 	olive_std_core_nios2_fast_cpu:*
set 	olive_std_core_nios2_fast_cpu_oci 	olive_std_core_nios2_fast_cpu_nios2_oci:the_olive_std_core_nios2_fast_cpu_nios2_oci
set 	olive_std_core_nios2_fast_cpu_oci_break 	olive_std_core_nios2_fast_cpu_nios2_oci_break:the_olive_std_core_nios2_fast_cpu_nios2_oci_break
set 	olive_std_core_nios2_fast_cpu_ocimem 	olive_std_core_nios2_fast_cpu_nios2_ocimem:the_olive_std_core_nios2_fast_cpu_nios2_ocimem
set 	olive_std_core_nios2_fast_cpu_oci_debug 	olive_std_core_nios2_fast_cpu_nios2_oci_debug:the_olive_std_core_nios2_fast_cpu_nios2_oci_debug
set 	olive_std_core_nios2_fast_cpu_wrapper 	olive_std_core_nios2_fast_cpu_debug_slave_wrapper:the_olive_std_core_nios2_fast_cpu_debug_slave_wrapper
set 	olive_std_core_nios2_fast_cpu_jtag_tck 	olive_std_core_nios2_fast_cpu_debug_slave_tck:the_olive_std_core_nios2_fast_cpu_debug_slave_tck
set 	olive_std_core_nios2_fast_cpu_jtag_sysclk 	olive_std_core_nios2_fast_cpu_debug_slave_sysclk:the_olive_std_core_nios2_fast_cpu_debug_slave_sysclk
set 	olive_std_core_nios2_fast_cpu_oci_path 	 [format "%s|%s" $olive_std_core_nios2_fast_cpu $olive_std_core_nios2_fast_cpu_oci]
set 	olive_std_core_nios2_fast_cpu_oci_break_path 	 [format "%s|%s" $olive_std_core_nios2_fast_cpu_oci_path $olive_std_core_nios2_fast_cpu_oci_break]
set 	olive_std_core_nios2_fast_cpu_ocimem_path 	 [format "%s|%s" $olive_std_core_nios2_fast_cpu_oci_path $olive_std_core_nios2_fast_cpu_ocimem]
set 	olive_std_core_nios2_fast_cpu_oci_debug_path 	 [format "%s|%s" $olive_std_core_nios2_fast_cpu_oci_path $olive_std_core_nios2_fast_cpu_oci_debug]
set 	olive_std_core_nios2_fast_cpu_jtag_tck_path 	 [format "%s|%s|%s" $olive_std_core_nios2_fast_cpu_oci_path $olive_std_core_nios2_fast_cpu_wrapper $olive_std_core_nios2_fast_cpu_jtag_tck]
set 	olive_std_core_nios2_fast_cpu_jtag_sysclk_path 	 [format "%s|%s|%s" $olive_std_core_nios2_fast_cpu_oci_path $olive_std_core_nios2_fast_cpu_wrapper $olive_std_core_nios2_fast_cpu_jtag_sysclk]
set 	olive_std_core_nios2_fast_cpu_jtag_sr 	 [format "%s|*sr" $olive_std_core_nios2_fast_cpu_jtag_tck_path]

#**************************************************************
# Set False Paths
#**************************************************************

set_false_path -from [get_keepers *$olive_std_core_nios2_fast_cpu_oci_break_path|break_readreg*] -to [get_keepers *$olive_std_core_nios2_fast_cpu_jtag_sr*]
set_false_path -from [get_keepers *$olive_std_core_nios2_fast_cpu_oci_debug_path|*resetlatch]     -to [get_keepers *$olive_std_core_nios2_fast_cpu_jtag_sr[33]]
set_false_path -from [get_keepers *$olive_std_core_nios2_fast_cpu_oci_debug_path|monitor_ready]  -to [get_keepers *$olive_std_core_nios2_fast_cpu_jtag_sr[0]]
set_false_path -from [get_keepers *$olive_std_core_nios2_fast_cpu_oci_debug_path|monitor_error]  -to [get_keepers *$olive_std_core_nios2_fast_cpu_jtag_sr[34]]
set_false_path -from [get_keepers *$olive_std_core_nios2_fast_cpu_ocimem_path|*MonDReg*] -to [get_keepers *$olive_std_core_nios2_fast_cpu_jtag_sr*]
set_false_path -from *$olive_std_core_nios2_fast_cpu_jtag_sr*    -to *$olive_std_core_nios2_fast_cpu_jtag_sysclk_path|*jdo*
set_false_path -from sld_hub:*|irf_reg* -to *$olive_std_core_nios2_fast_cpu_jtag_sysclk_path|ir*
set_false_path -from sld_hub:*|sld_shadow_jsm:shadow_jsm|state[1] -to *$olive_std_core_nios2_fast_cpu_oci_debug_path|monitor_go
