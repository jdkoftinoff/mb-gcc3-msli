/* crtn.s for __init, __fini
   This file supplies the epilogue for __init and __fini routines */

    .section .init, "ax"
    lw      r15, r0, r1
    rtsd    r15, 8 
    addik   r1, r1, 8

    .section .fini, "ax"
    lw      r15, r0, r1
    rtsd    r15, 8 
    addik   r1, r1, 8    
