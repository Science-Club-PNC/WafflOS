.global pit_wrapper
.global keyboard_wrapper
.global double_fault_wrapper

.align 4
.intel_syntax noprefix

pit_wrapper:
    pushad
    call pit_handler
    popad
    iret

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
