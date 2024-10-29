GLOBAL cpuVendor
GLOBAL bringHours
GLOBAL bringMinutes
GLOBAL bringSeconds
GLOBAL keyPress
GLOBAL outb
GLOBAL inb
GLOBAL sound
GLOBAL nosound
GLOBAL call_timer_tick
GLOBAL initialize_stack
GLOBAL aquire
GLOBAL release

section .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

;Consigue la hora actual del Timer Clock y la retorna
bringHours:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x04
    out 70h, al
    in al, 71h
    sti
    ret

;Consigue los minutos actuales del Timer Clock y los retorna
bringMinutes:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x02
    out 70h, al
    in al, 71h
    sti

    ret
       
;Consigue los segundos actuales del Timer Clock y los retorna       
bringSeconds:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x00
    out 70h, al
    in al, 71h
    sti
    ret

keyPress:
    push rbp
    mov rbp, rsp

    mov rax, 0
    in al, 60h

    mov rbp, rsp
    pop rbp

    ret


sound:
	push rbp
    mov rbp, rsp

    mov rax, 1193180
    xor rdx, rdx
    div rdi

    mov rcx, rax

    mov dx, 0x43
    mov al, 0xB6
    out dx, al

    mov dx, 0x42
    mov al, cl
    out dx, al

    sar rcx, 8
    mov al, cl
    out dx, al

    in al, 0x61
    mov bl, al
    or bl, 3
    cmp al, bl
    je .end
    mov al, bl
    out 0x61, al
.end:
    mov rsp, rbp
	pop rbp
    ret


nosound:
    push rbp
    mov rbp, rsp

    in al, 0x61
    and al, 0xFC
    out 0x61, al

    mov rsp, rbp
    pop rbp
    ret

call_timer_tick:
    int 20h
    ret


initialize_stack:
	mov r9, rsp
	mov rsp, rdi	

	push 0	    ;ss

	push rdi	;rsp

	push 0x202  ;RFLAGS

	push 0x8    ;cs

	push r8	    ;rip -> main_wrapper

	mov rax, 0
	push rax    ;rax
	push rax	;rbx
	push rax	;rcx
	push rcx	;rdx

	push rdi	;rbp
	push rsi	;rdi -> argc 
	push rdx	;rsi -> argv

	push rax	;r8
	push rax	;r9
	push rax	;r10
	push rax	;r11
	push rax	;r12
	push rax	;r13
	push rax	;r14
	push rax	;r15
	mov rax, rsp
	mov rsp, r9 
	ret

aquire:
    mov al, 0
.retry
    xchg [rdi], al
    test al, al
    jz .retry
    ret

release:
    mov byte [rdi], 1
    ret