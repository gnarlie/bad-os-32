[GLOBAL gdt_flush]

gdt_flush:
    mov eax, [esp+4]    ;eax is pointer to gdt, 1st parameter
    lgdt [eax]          ;load the gdt
    mov ax, 0x10        ;offset in the GDT to data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ;8 is he offset to our code segment, load with far jump
.flush:
    ret

[GLOBAL idt_flush]
idt_flush:
    mov eax, [esp+4]    ;eax is pointer to idt, 1st parameter
    lidt [eax]  
    ret;
