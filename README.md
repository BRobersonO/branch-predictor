# Branch Predictor Simulator

This simulator models a GShare Branch Predictor, which features a shared global branch histroy register (GBHR) for making more accurate predictions for conditional jumps. Each entry in the table contains a 2-bit counter, initiatized to state `10`, decimal 2, or "weakly-taken".  

The program executes in the console and is named sim_gshare. It takes three inputs as arguments in the form `./sim_gshare <_PCB> <_GBHRB> <Trace_File>` 
where:
- <_PCB> is the number of address bits used in the index of the gshare table (traditionally labelled 'M')
- <_GBHRB> is the global branch history register bits used in the index of the gshare table (traditionally labelled 'N')
- <Trace_File> is the full path of the trace file

The trace file contains lines of information in two parts. Part one of each line is a hexadecimal address, followed by part two, a lower-case character, 't' or 'n', which designates whether the branch was actually taken or not taken, respectively. An example couple of lines:

  00A4C6FB t

  00A6D80D n
  
  The program outputs the number of address bits used (M), the number of GBRH bits used (N), and the misprediction ratio.
