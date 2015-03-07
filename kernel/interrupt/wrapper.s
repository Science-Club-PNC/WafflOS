.global divide_by_zero_wrapper
.global debug_wrapper
.global non_maskable_interrupt_wrapper
.global breakpoint_wrapper
.global overflow_wrapper
.global bound_range_exceeded_wrapper
.global invalid_opcode_wrapper
.global device_not_available_wrapper
.global double_fault_wrapper
.global invalid_TSS_wrapper
.global segment_not_present_wrapper
.global stack_segment_fault_wrapper
.global general_protection_fault_wrapper
.global page_fault_wrapper
.global x87_floating_point_wrapper
.global alignment_check_wrapper
.global machine_check_wrapper
.global SIMD_floating_point_wrapper
.global virtualization_wrapper
.global pit_wrapper
.global keyboard_wrapper

.align 4
.intel_syntax noprefix

divide_by_zero_wrapper:
    pushad
    call divide_by_zero_handler
    popad
    iret

debug_wrapper:
    pushad
    call debug_handler
    popad
    iret

non_maskable_interrupt_wrapper:
    pushad
    call non_maskable_interrupt_handler
    popad
    iret

breakpoint_wrapper:
    pushad
    call breakpoint_handler
    popad
    iret

overflow_wrapper:
    pushad
    call overflow_handler
    popad
    iret

bound_range_exceeded_wrapper:
    pushad
    call bound_range_exceeded_handler
    popad
    iret

invalid_opcode_wrapper:
    pushad
    call invalid_opcode_handler
    popad
    iret

device_not_available_wrapper:
    pushad
    call device_not_available_handler
    popad
    iret

double_fault_wrapper:
    pushad
    call double_fault_handler
    popad
    iret

invalid_TSS_wrapper:
    pushad
    call invalid_TSS_handler
    popad
    iret

segment_not_present_wrapper:
    pushad
    call segment_not_present_handler
    popad
    iret

stack_segment_fault_wrapper:
    pushad
    call stack_segment_fault_handler
    popad
    iret

general_protection_fault_wrapper:
    pushad
    call general_protection_fault_handler
    popad
    iret

page_fault_wrapper:
    pushad
    call page_fault_handler
    popad
    iret

x87_floating_point_wrapper:
    pushad
    call x87_floating_point_handler
    popad
    iret

alignment_check_wrapper:
    pushad
    call alignment_check_handler
    popad
    iret

machine_check_wrapper:
    pushad
    call machine_check_handler
    popad
    iret

SIMD_floating_point_wrapper:
    pushad
    call SIMD_floating_point_handler
    popad
    iret

virtualization_wrapper:
    pushad
    call virtualization_handler
    popad
    iret

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
