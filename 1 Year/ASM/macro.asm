CR		EQU		13
LF		EQU		10
Space	EQU		20h
;===================================================
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
;=====================================================
get_vector	macro	vector, DD_for_save_vector  
	mov AX,35&vector                      	;   получить вектор
    int 21h
    mov word ptr DD_for_save_vector,BX    	;   ES:BX - вектор
	mov word ptr DD_for_save_vector+2,ES  	;
endm
;=====================================================	
set_vector	macro	vector,	handler
    mov DX,offset handler           		;   получить смещение точки входа в новый обработчик на DX
    mov AX,25&vector                    	;   функция установки прерывания
	int 21h  								; 	AL - номер прерыв. DS:DX - указатель программы обработки прер.
endm
;=====================================================	
recover_vector	macro	vector,	handler
	lds DX, handler        					;   получить смещение точки входа в новый обработчик на DX
    mov AX,25&vector                    	;   функция установки прерывания
	int 21h  								; 	AL - номер прерыв. DS:DX - указатель программы обработки прер.
endm
;=====================================================
print_word_dec	macro	src	, mes	;	выводит на экран источник src в десятичном виде
local	l1, l2, ex
push	AX
push	BX
push	CX
push	DX
print_mes	mes
	mov		AX,	src					;	Выводимое число в регисте EAX
	push		-1					;	Сохраним признак конца числа
	mov		cx,	10					;	Делим на 10
l1:	
	xor		dx,	dx					;	Очистим регистр dx 
	div		cx						;	Делим 
	push		DX						;	Сохраним цифру
	or 			AX,	AX				;	Остался 0? (это оптимальнее, чем  cmp	ax,	0 )
	jne		l1						;	нет -> продолжим
	mov		ah,	2h
l2:	
	pop		DX						;	Восстановим цифру
	cmp		dx,	-1					;	Дошли до конца -> выход {оптимальнее: or EDX,dx jl ex}
	je			ex
	add		dl,	'0'					;	Преобразуем число в цифру
	int		21h						;	Выведем цифру на экран
	jmp	l2							;	И продолжим
ex:	
pop		DX
pop		CX
pop		BX
pop		AX
endm
;=====================================================
print_byte	macro	src
	local	nxt, prn_hex,prnt

	push	AX
	push	DX
	;
	mov		BL,	src
    mov     DL,		BL
	shr		DL,		4
	call 	prn_hex
    mov 	DL,		BL
	call	prn_hex
;
	jmp short nxt
;
prn_hex	proc	near
	and		DL,		0Fh
	add		DL,		30h
	cmp		DL,		3Ah
	jl		prnt
	add		DL,		07h
prnt:		
	mov 	AH,		02
	int		21h
	ret
prn_hex	endp	
nxt:
pop		DX
pop		AX
	endm
;=====================================================
print_word	macro	src
;
push	AX
push	DX

	mov		DX,		src
	print_byte		DH
	mov		DX,		src
	print_byte		DL
;
pop		DX
pop		AX
	endm
;=====================================================