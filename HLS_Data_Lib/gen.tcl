# Vitis hls 2022.2
set top_file_name "./filename.cc"
set top_func_name "filename"
open_project conv
set_top $top_func_name

add_files "./$top_file_name" 
open_solution solution -flow_target vivado

# u250
# set_part xcu250-figd2104-2L-e
u280
set_part xcu280-fsvh2892-2L-e

create_clock -period 100MHz -name default

config_compile -pipeline_loops 0
config_dataflow -strict_mode warning
set_clock_uncertainty 27.000000%
# config_rtl -enable_maxiConservative=1
# config_interface -m_axi_addr64


# Here are some example about config_op (Bind_op)
# config_op add -impl fabric
# config_op sub -impl fabric
# config_op mul -impl dsp
# config_op fadd -impl fulldsp
# config_op fsub -impl fulldsp
# config_op fmul -impl fulldsp
# config_op fdiv -impl fabric
# config_op dadd -impl fulldsp
# config_op dsub -impl fulldsp
# config_op dmul -impl fulldsp
# config_op ddiv -impl fabric

# csim_design
csynth_design
# cosim_design -rtl verilog -setup

export_design -rtl verilog -format ip_catalog -flow syn

close_project
puts "HLS completed successfully"
exit
