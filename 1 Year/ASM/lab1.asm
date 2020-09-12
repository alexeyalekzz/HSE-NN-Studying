code_seg segment
    ASSUME  CS:CODE_SEG,DS:code_seg,ES:code_seg
	org 100h
	include macro.asm
start:
print_letter	CR
print_letter	LF
;==========ПРОВЕРКА_СТРОКИ_ПАРАМЕТРОВ===============
	mov 	cl,		es:[80h] 	; addr. of length parameter in psp 
    cmp 	cl,		0   		; is it 0 in buffer?
jne 		cont        		; no param
;---------------------------------------------------
    print_mes 'No parameters' 	; program was run without parameters
    jmp	Parameter_Input
;========СЧИТЫВАНИЕ_ИЗ_СТРОКИ_ПАРАМЕТРОВ============
cont:
	xor	bh,	bh
	mov	bl, es:[80h]			  
	mov	byte ptr [bx+81h],	0
    mov 	cl,	es:80h    		; Длина хвоста в PSP
    xor 	ch,	ch      		; CX = CL = длина хвоста
    cld             			; DF = 0 - флаг направления вперед
	mov 	di, 81h     		; ES:DI-> начало хвоста в PSP
;=============ПОПЫТКА_ОТКРЫТЬ_ФАЙЛ==================	
								; Создание ASCIIZ строк, запоминание указателей на имена файлов
OpenFile:	
    mov 	al, ' '        		; Уберем пробелы из начала хвоста
repe    scasb   				; Сканируем хвост пока пробелы, повторять пока элементы равны
	dec di        				; DI-> на первый символ после пробелов (на первую букву имени первого файла)
	mov Inputptr, di 
	inc di  
	
repne 	scasb 
	cmp [di], 0
je s
	dec di
	mov byte ptr [di], 0		; DI -> на первый символ после имени первого файла
s:
	inc di
	
repe	scasb
	dec di						; DI -> на первую букву имени второго файла
	mov Outputptr, di			

	mov bx, [Outputptr]
	mov bx, [bx]
	cmp bx, 0
jne e
    mov OneFileFlag, 1          ; Если указан только один файл
e:
;----------------------------------------------------
	mov	ax,	3D02h				; Open file for read/write
	mov	dx, Inputptr
	int	21h
    mov InpHandler, ax
    mov OutHandler, ax
jc openERR
	jmp openOK
openERR:
print_letter	CR
print_letter	LF
print_mes	'Wrong file name'
	int	20h
;================ВВОД_ПАРАМЕТРОВ=====================
Parameter_Input:
print_letter	CR
print_letter	LF
print_mes	'Write input and output file names > '	
	mov		ah,	0Ah
	mov		dx,	offset	InputFile
	int		21h
	mov di, offset InputFile[2]
	mov cl, InputFile[1]  
	xor ch, ch
	mov bx, cx      
	mov InputFile[bx + 2], 0   
jmp 	OpenFile    			; Пытаемся открыть файл еще раз

openOK:
;================СОЗДАНИЕ_фАЙЛА======================
    cmp OneFileFlag, 0
jne j
    mov ah, 3Ch					; Create file
    mov cx, 0					; Атрибут файла (0 - обычный)
    mov dx, Outputptr	 		; ASCIIZ - строка
    int 21h 
    mov OutHandler, ax
j:
;==================СЧИТЫВАНИЕ=======================
    mov bx, InpHandler
	xor ax, ax
    mov cx, 10000
	mov dx, offset Buf

read:
	mov ah, 3Fh
	int 21h
	add cntr, ax
	add dx, ax
	cmp ax, 0
je smthg
	cmp dx, offset Buf + 40000
jne read
smthg:
push dx
;====================ЗАМЕНА==========================
	xor dx, dx
	mov cx, cntr
	mov bp, offset Buf
	xor bx, bx	
compare:
	mov di, offset Numbers
	mov al, [bp]

	cmp al, '9'
ja Not_a_Number    				; Если не цифра прыгаем в конец
	cmp al, '0' 
jb Not_a_Number

	push cx
	mov cx, 0Ah
	repne scasb					; Определяем что за цифра (DI -> на цифру массиве Numbers)
	dec di
	pop cx
								; Заменяем цифру				
	mov bl, [di]
	sub bl, 30h
	mov dl, Symbols[bx]
	mov [bp], dl

Not_a_Number:
	inc bp

loop compare
;=====================================================
cmp OneFileFlag, 0              ; Если файл один - отступаем от текущей позиции на количество считанных байт
je h
    mov ah, 42h
    mov al, 1
    mov cx, 0FFFFh
    neg cntr
    mov dx, cntr
    neg cntr
    mov bx, InpHandler
    int 21h
h:
;====================ЗАПИСЬ==========================
	mov bx, OutHandler			; BX - Handler 			
	mov cx, cntr				; Сколько байт записать
	mov dx, offset Buf
	mov ah, 40h
	int 21h 
;====================================================
pop dx
	cmp dx, offset Buf + 40000	; Если буфер заполнен
jne endd
	mov bx, InpHandler 
	mov cntr, 0
	mov dx, offset Buf
	mov cx, 200
jmp read
;======================END===========================
endd:
print_letter	CR
print_letter	LF
print_mes	'Success!'
	mov	ax,	4C00h
	int	21h

InputFile	DB	30, 0, 30 dup (0)
Inputptr 	DW	?
Outputptr 	DW	?
Buf 		DB 	40000 dup (0)
Numbers 	DB	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
Symbols 	DB	')', '!', '@', '#', '$', '%', '^', '&', '*', '('
cntr 		DW	0
OutHandler	DW 	?
InpHandler 	DW  ?
OneFileFlag DB  0

code_seg ends
     end start
