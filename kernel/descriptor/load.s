.global lgdt
.global lidt

lgdt:
    mov %eax, -4(%esp)
    lgdt (%eax)
    ret

lidt:
    mov %eax, -4(%esp)
    lidt (%eax)
    ret
