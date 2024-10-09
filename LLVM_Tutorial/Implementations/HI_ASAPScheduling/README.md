HI_ASAPScheduling pass is tested by LLVM_exp17_HI_ASAPScheduling

HI_ASAPScheduling will access the database to get the accurate infomation of the instruction, including timing and resource.
But in this pass we will only consider the timing constrains.

And, in this pass, the HLS directives will still not work, which will be enable in the next step.

In this implementation, the high-level idea is similiar to HI_SimpleTimingEvaluation. However, the class inst_timing_info and timingBase are
used to handle the detailed information for timing and resource of instructions in a nicer way.

Related operators are overloaded, e.g. +, * and >, for the caluation of critical path and the latency of loop/block/function. For each
instruction, HI_ASAPScheduling will inquiry the map variables to get the result of timing.

The result of the HI_ASAPScheduling will be used to directive the behavoir of HI_Verilog.
