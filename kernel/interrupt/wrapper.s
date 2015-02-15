.global keyboard_wrapper
.global double_fault_wrapper

.align 4
.intel_syntax noprefix

keyboard_wrapper:
    pushad
    call keyboard_handler
    popad
    iret

double_fault_wrapper:
    pushad
    call double_fault_handler
    popad
    iret
