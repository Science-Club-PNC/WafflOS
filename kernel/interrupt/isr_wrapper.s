.intel_syntax noprefix
.globl   keyboard_wrapper
.align   4

keyboard_wrapper:
    pushad
    call keyboard_handler
    popad
    iret
