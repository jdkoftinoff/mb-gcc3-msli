# Target: Xilinx microblaze processor
TDEPFILES= microblaze-tdep.o  microblaze-rom.o monitor.o dsrec.o
TM_FILE= tm-microblaze.h
SIM_OBS = remote-sim.o
SIM = ../sim/microblaze/libsim.a
