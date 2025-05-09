# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\final project\Workspace01\Jazzy_Dude.cydsn\Jazzy_Dude.cyprj
# Date: Fri, 09 May 2025 19:58:41 GMT
#set_units -time ns
create_clock -name {sax_clk(routed)} -period 41666.666666666664 -waveform {0 20833.3333333333} [list [get_pins {ClockBlock/dclk_0}]]
create_clock -name {piano_clk(routed)} -period 83333.333333333328 -waveform {0 41666.6666666667} [list [get_pins {ClockBlock/dclk_1}]]
create_clock -name {Clock_1(routed)} -period 500000000 -waveform {0 250000000} [list [get_pins {ClockBlock/dclk_2}]]
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 12000001 24000001} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {sax_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 993 2001} -nominal_period 41666.666666666664 [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {piano_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2001 4001} -nominal_period 83333.333333333328 [list [get_pins {ClockBlock/dclk_glb_1}]]


# Component constraints for D:\final project\Workspace01\Jazzy_Dude.cydsn\TopDesign\TopDesign.cysch
# Project: D:\final project\Workspace01\Jazzy_Dude.cydsn\Jazzy_Dude.cyprj
# Date: Fri, 09 May 2025 19:58:39 GMT
