%macro ISR_NOERRORCODE 1
    [GLOBAl isr%1]
    isr%1:
        cli                     ;disable interrupts
        push byte 0             ;dummy error code
        push byte %1             ;interrupt number
        jmp isr_common_stub     ;common handler
%endmacro

%macro ISR_ERRORCODE 1
    [GLOBAL isr%1]
    isr%1:
        cli
        push byte %1
        jmp isr_common_stub
%endmacro

ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_ERRORCODE 8
ISR_ERRORCODE 9
ISR_ERRORCODE 10
ISR_ERRORCODE 11
ISR_ERRORCODE 12
ISR_ERRORCODE 13
ISR_ERRORCODE 14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_NOERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31

[EXTERN isr_handler]

isr_common_stub:
    pusha        
    mov ax, ds    
    push eax      ; save the data segment

    mov ax, 0x10  ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop ebx         ;restore the world
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa           
    add esp, 8     ; macros pushed error code & irupt #
    sti            ; game on again
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

%macro IRQ 2
    global irq%1
    irq%1:
        cli
        push byte 0
        push byte %2
        jmp irq_common_stub
%endmacro

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

extern irq_handler
irq_common_stub:
    pusha
    mov ax, ds 
    push eax
    
    mov ax, 0x10 ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler

    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8 ;clean up pushed error code & isr no.
    sti
    iret
    

