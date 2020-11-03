code_seg segment
    ASSUME  CS:CODE_SEG,DS:code_seg,ES:code_seg
    org 100h

CR		equ		13
LF		equ		10
    
start:
jmp begin
;================================MACRO=================================
;	выводит на экран источник src в десятичном виде
print_word_dec	macro   src, mes	
local	l1, l2, ex
push	AX
push	BX
push	CX
push	DX
print_mes	mes
	mov		AX,	src					
	push		-1					
	mov		cx,	10					
l1:	
	xor		dx,	dx					
	div		cx						
	push		DX					
	or 			AX,	AX				
	jne		l1						
	mov		ah,	2h
l2:	
	pop		DX						
	cmp		dx,	-1					
	je			ex
	add		dl,	'0'					
	int		21h						
	jmp	l2							
ex:	
pop		DX
pop		CX
pop		BX
pop		AX
endm
;====================================================
print_letter	macro	letter
	push	AX
	push	DX
	mov	DL, letter
	mov	AH,	02
	int	21h
	pop	DX
	pop	AX
endm
;====================================================
print_mes	macro	message
	local	msg, nxt
	push	AX
	push	DX
	mov	DX, offset msg
	mov	AH,	09h
	int	21h
	pop	DX
	pop	AX
	jmp nxt
		msg	DB message,'$'
	nxt:
	endm
;======================================================================



;==================================DATA================================
all_emails          dw  0
end_addr            dw  ?
start_addr          dw  ?
dog_addr            dw  ?
Inputptr 	        dw	?
Outputptr 	        db  'output.txt',0
Buf 		        db 	30000 dup (0)
correct_emails_buf  db  30000 dup (0)
cntr 		        dw	0
OutHandler	        dw 	?
InpHandler 	        dw  ?
email_cntr          dw  0
dot_flag            db  0
;======================================================================


;======================================================================
; Функция принимает на фход символ в AL
; Возвращает 1 или 0 в AH в зависимости от корректности
check_char proc
    cmp al, 'z'
    ja bad_char
    cmp al, 'a'
    jae good_char
    cmp al, 'Z'
    ja check_spec
    cmp al, 'A'
    jae good_char
    cmp al, '9'
    ja bad_char
    cmp al, '0'
    jae good_char

check_spec:
    cmp al, '_'
    je good_char    
    cmp al, 39d ; '
    je good_char  
    cmp al, '-'
    je good_char  
    cmp al, '+'
    je good_char  
    cmp al, '.'
    je good_char  
    jmp bad_char

good_char:
    cmp al, '.'
    je y
    mov dot_flag, 0                         ; Проверка на две идущие подряд точки
    mov ah, 1
    ret

y:
    cmp dot_flag, 0
    jne bad_char 
    mov dot_flag, 1
    mov ah, 1
    ret

bad_char:
    mov ah, 0
    ret
check_char endp
;=============================================================


;=============================================================
; функция принимает адрес '@' в SI
; возвращает в AH 1 или 0 в зависимости от корректности e-mail
is_email proc

    push si

    dec si
    mov al, [si]
    cmp al, '.'                             ; Проверка на неправильное расположение точки
    jne p1
        mov ah, 0
        jmp incorrect_email
    p1:
    inc si
    inc si
    mov al, [si]
    cmp al, '.'
    jne p2
        mov ah, 0
        jmp incorrect_email
    p2:
    dec si

check_local:                                ; Проверка локальной части
    dec si
    mov al, [si]

    cmp al, ' '
    je start_finded
    cmp al, ','
    je start_finded
    cmp al, 0Ah ; LF
    je start_finded
    cmp al, ';'
    je start_finded

    call check_char
    cmp ah, 0
    je incorrect_email

jmp check_local
return:

    mov al, [si]
    cmp al, '.'                             ; Проверка на неправильное расположение точки
    jne p3
        mov ah, 0
        jmp incorrect_email
    p3:

    mov si, dog_addr

check_domen:                              ; Проверка доменной части       
    push cx   
    mov ax, offset Buf
    add ax, cntr
    cmp si, ax                              ; Не выйдем ли мы за пределы буфера

    jne next
        mov ax, si
        sub ax, start_addr
        inc ax
        mov dx, ax
        neg dx
        mov ax, 4201h
        mov cx, 0FFFFh
        mov bx, InpHandler
        int 21h                             ; Если буфер "разрезал" e-mail, то сдвинем
        mov ah, 0
        pop cx
        dec all_emails
        jmp incorrect_email                 ;   указатель в файле
    next:    
    pop cx
    inc si
    mov al, [si]

    cmp al, ' '
    je end_finded
    cmp al, ','
    je end_finded
    cmp al, 0Dh ; CR
    je end_finded
    cmp al, ';'
    je end_finded

    call check_char
    cmp ah, 0
    je incorrect_email

jmp check_domen
return2:

    dec si
    mov al, [si]
    cmp al, '.'                             ; Проверка на неправильное расположение точки
    jne p4
        mov ah, 0
        jmp incorrect_email
    p4:

incorrect_email:
pop si
    ret

start_finded:
    inc si
    mov start_addr, si
jmp return

end_finded:
    mov end_addr, si
jmp return2

is_email endp
;=============================================================


;=============================================================
begin:
;==========ПРОВЕРКА_СТРОКИ_ПАРАМЕТРОВ===============
	mov 	cl,		es:[80h]
    cmp 	cl,		0
jne 		cont        		; no param
;---------------------------------------------------
    print_mes 'No parameters' 	; program was run without parameters
    mov ax, 4c00h
    int 21h
;========СЧИТЫВАНИЕ_ИЗ_СТРОКИ_ПАРАМЕТРОВ============
cont:
	xor	bh,	bh
	mov	bl, es:[80h]			  
	mov	byte ptr [bx+81h],	0
    mov 	cl,	es:80h
    xor 	ch,	ch
    cld
	mov 	di, 81h
;=============ПОПЫТКА_ОТКРЫТЬ_ФАЙЛ==================	
OpenFile:	
    mov 	al, ' '        		; Уберем пробелы из начала хвоста
repe    scasb   				; Сканируем хвост пока пробелы, повторять пока элементы равны
	dec di        				; DI-> на первый символ после пробелов (на первую букву имени первого файла)
	mov Inputptr, di 

	mov	ax,	3D02h				; Open file for read/write
	mov	dx, Inputptr
	int	21h
    mov InpHandler, ax
jc openERR
	jmp openOK
openERR:
print_letter	CR
print_letter	LF
print_mes	'Wrong file name'
	int	20h

openOK:
;================СОЗДАНИЕ_фАЙЛА======================
    mov ah, 3Ch					; Create file
    mov cx, 0					; Атрибут файла (0 - обычный)
    lea dx, Outputptr	 		; ASCIIZ - строка
    int 21h 
    mov OutHandler, ax
;==================СЧИТЫВАНИЕ=======================
    mov bx, InpHandler
	xor ax, ax
    mov cx, 30000
	mov dx, offset Buf

read:
	mov ah, 3Fh
	int 21h
	add cntr, ax
	add dx, ax
	cmp ax, 0
je smthg
	cmp dx, offset Buf + 30000
jne read
smthg:
push dx
;====================ЗАМЕНА==========================
	mov cx, cntr
    lea si, Buf
    lea di, correct_emails_buf
compare:
    mov al, [si]
    
	cmp al, '@'
jne not_email    				            ; Если не собака прыгаем в конец

    inc all_emails

    mov dog_addr, si

    call is_email

    cmp ah, 0
    je not_email                            ; что вернула функция?
    
    mov ax, dog_addr                        ; проверка на длину
    sub ax, start_addr
    cmp ax, 64
    ja not_email
    cmp ax, 0
    je not_email
    mov ax, end_addr
    sub ax, dog_addr
    cmp ax, 255
    ja not_email
    cmp ax, 1
    je not_email
    inc email_cntr

    push cx                                 ; запись в выходной буфер
    push si
    mov ax, end_addr
    sub ax, start_addr
    mov cx, ax
    mov si, start_addr
    rep movsb
    mov [di], CR
    mov [di+1], LF
    add di, 2
    pop si
    pop cx

not_email:
    inc si
loop compare

;====================ЗАПИСЬ==========================
    mov ax, di
    sub ax, offset correct_emails_buf
	mov bx, OutHandler			; BX - Handler 			
	mov cx, ax				    ; Сколько байт записать
	lea dx, correct_emails_buf
	mov ah, 40h
	int 21h 
;====================================================
pop dx
	cmp dx, offset Buf + 30000	; Если буфер заполнен
jne endd
	mov bx, InpHandler 
	mov cntr, 0
	mov dx, offset Buf
	mov cx, 30000
jmp read
;======================END===========================
endd:
    print_mes 'Number of correct e-mails:'
    print_word_dec  email_cntr
    print_letter	CR
    print_letter	LF
    print_mes 'Overall number of e-mails:'
    print_word_dec  all_emails
    print_letter	CR
    print_letter	LF
	mov	ax,	4C00h
	int	21h

code_seg ends
     end start