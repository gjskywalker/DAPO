# Vitis hls 2022.2
set top_file_name "./crs.cc"
set top_func_name "dut"
open_project "unoptimized_crs"
set_top $top_func_name

add_files "./$top_file_name" 
open_solution solution -flow_target vivado

# u250
# set_part xcu250-figd2104-2L-e
# u280
set_part xcu280-fsvh2892-2L-e

create_clock -period 100MHz -name default

config_compile -pipeline_loops 0
config_dataflow -strict_mode warning
set_clock_uncertainty 27.000000%
config_rtl -enable_maxiConservative=1

# csim_design
csynth_design
# cosim_design -rtl verilog -setup

export_design -rtl verilog -format ip_catalog -flow syn

close_project
puts "HLS completed successfully"
exit
