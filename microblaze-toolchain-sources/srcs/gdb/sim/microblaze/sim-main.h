#ifndef MICROBLAZE_SIM_MAIN
#define MICROBLAZE_SIM_MAIN

/*
 * Copyright (c) 2001 Xilinx, Inc.  All rights reserved. 
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Xilinx, Inc.  The name of the Company may not be used to endorse 
 * or promote products derived from this software without specific prior 
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	Xilinx, Inc.
 * $Header:
 */

#include "microblaze.h"
#include "sim-basics.h"
typedef address_word sim_cia;
#include "sim-base.h"

/* The machine state.
   This state is maintained in host byte order.  The 
   fetch/store register functions must translate between host
   byte order and the target processor byte order.  
   Keeping this data in target byte order simplifies the register
   read/write functions.  Keeping this data in native order improves
   the performance of the simulator.  Simulation speed is deemed more
   important.  */

/* The ordering of the microblaze_regset structure is matched in the
   gdb/config/microblaze/tm-microblaze.h file in the REGISTER_NAMES macro.  */
 struct microblaze_regset
{
  word	          regs [32];		/* primary registers */
  word	          spregs [2];		/* pc + msr */
  int		  cycles;
  int		  insts;
  int		  exception;
  unsigned long   msize;
  unsigned char * memory;
  ubyte           imm_enable;
  half            imm_high;
};

struct _sim_cpu {
  struct microblaze_regset microblaze_cpu;
  sim_cpu_base base;
};

#define MAX_NR_PROCESSORS 1
struct sim_state {
  sim_cpu cpu[MAX_NR_PROCESSORS];
#define STATE_CPU(sd,n) (&(sd)->cpu[0])
  sim_state_base base;
};
#endif /* MICROBLAZE_SIM_MAIN */

