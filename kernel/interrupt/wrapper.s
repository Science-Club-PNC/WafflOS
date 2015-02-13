.global keyboard_wrapper
.align 4
.intel_syntax noprefix

keyboard_wrapper:
    pushad
    call keyboard_handler
    popad
    iret
