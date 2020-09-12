;=====================================================================
; Выводит на экран заставку через заданное в командной строке время
; При нажатии на любую клавишу экран возвращается в изначальное состояние
; Выгружается из памяти командой '-uninstall'
;=====================================================================
code_seg segment
assume  cs:code_seg, ds:code_seg, es:code_seg
org 100h
include macro.asm
start:
.286
jmp begin
;=====================DATA============================================
command_uninstall   db '-uninstall'
old_vector_1Ch      dd ?
old_vector_2Fh      dd ?
old_vector_09h      dd ?
time                dw 0
seed                dw 95
attr                db ?
max_x               db 24d
max_y               db 79d
max_color           db 07h
d_16                db 16d
screen_saver_active db 0
seconds             dw 0
;=====================================================================


;=====================================================================
; BL - номер строки
; DL - номер столбца
;   return  ES:BX -> адрес в видеобуфере
set_pos_video_buf proc
    mov     ax, 0B900h                      ; адрес видеобуфера второго дисплея
    mov     es, ax                          ; В ES сегментный адрес видеобуфера

    mov     al, 80*2                        ; Кол-во байт в строке (символ+атрибут)
    mul     bl                              ; 80*2*номер стр - смещение от начала видеобуфера

    mov     bx, ax                          ; Теперь в BX адрес первого байта нашей строки                                  
    mov     al, dl                          ; В AL номер столбца
    xor     ah, ah                          ; AH=0
    shl     ax, 1                           ; Каждая стр. в два раза длиннее т.к. содержит символ+атрибут                                  
    add     bx, ax                          ; return  ES:BX -> адрес в видеобуфере
    ret
set_pos_video_buf endp
;=====================================================================


;=====================================================================
; Выводит один прямоугольник с заданными координатами
;----------------------------------------------------------------
;            CH <---- high_Y     ; Координаты окна
;            CL <---- left_X     ;
;            DH <---- low_Y      ;
;            DL <---- right_X    ;
;----------------------------------------------------------------
window proc
    push    es
    push    di

    mov     cs:_le_X, cl                    ;  Запомним координаты
    mov     cs:_ri_X, dl

    mov     bl, ch                  
    mov     dl, cl 

call    set_pos_video_buf                   ; Получим адрес в видеобуфере

    mov     cl, dh                          ; Количество
    sub     cl, ch                          ; строк, занимаемых
    inc     cl                              ; окном (lo_Y-hi_Y+1)-->CL
    xor     ch, ch                          ; CH=0
    mov     ah, byte ptr cs:[attr]          ; Атрибут - рандомный
    mov     al, 20h         

    mov     dl, cs:_ri_X                    ; Количество
    sub     dl, cs:_le_X                    ; столбцов, занимаемых
    inc     dl                              ; окном в DL
    xor     dh, dh
    cld

next_row:
    push    cx
    mov     di, bx                          ; ES:DI --> на начало окна
    mov     cx, dx                          ; CX - счетчик по колонкам
                                            ; Цикл по колонкам
    rep     stosw                           ; stosw записывает AX по адресу в ES:DI

    add     bx, 80*2                        ; корректируем указатель -> на следующую строку окна                                   
    pop     cx
loop    next_row                            ; Цикл по строкам

    pop    di
    pop    es


    ret
_le_X       db  0
_ri_X       db  0
window endp
;=====================================================================


;=====================================================================
; Возращает случайное число от 0 до 256 в AL
rand8	proc near
push cx
push bx

        mov	ax,		word ptr cs:[seed]      ; инициализируется временем, указанным в командной строке
		mov	cx,		8	

newbit:	
        mov	bx,		ax
		and	bx,		002Dh
		xor	bh,	    bl
		clc
		jpe	shift
		stc
shift:	
        rcr	ax,	    1
        loop	newbit
        mov	word ptr cs:[seed],	ax
        mov	ah,	    0

pop bx
pop cx    
	ret
rand8 endp
;=====================================================================


;=====================================================================
; Корректно задает случайные координаты для процедуры window
screensaver proc

    pusha

        call rand8

        call rand8
        div cs:max_y
        add ah, 1
        mov dl, ah                          ; Правая сторона

        call rand8
        div cs:max_x
        add ah, 1
        mov dh, ah                          ; Нижняя сторона

        call rand8
        div dl
        mov cl, ah                          ; Левая сторона

        call rand8
        div dh
        mov ch, ah                          ; Верхняя сторона

        call rand8
        div cs:max_color
        mov al, ah
        mul cs:d_16
        add al, 0fh
        mov byte ptr cs:[attr], al          ; Случайный цвет

        call window

    popa

    ret
screensaver endp
;=====================================================================


;=====================================================================
; Обработчик прерывания клавиатуры до системного.
; По нажатию любой клавиши убирает заставку и восстанавливает экран,
; также не дает запустить заставку пока мы что-то печатаем.
new_09h proc far
    mov seconds, 0                          ; Обнуляем таймер, что бы заставка не запустилась
    cmp screen_saver_active, 0              ; Заставка активна?
    je end_int
    pusha

        mov ax, 0600h
        mov cx, 0
        mov dh, 24
        mov dl, 79
        int 10h                             ; Очищает дисплей, на котором выводится заставка

        mov ax, 0500h
        int 10h                             ; Возвращает на исходный дисплей
        mov screen_saver_active, 0

    popa
    end_int:
    jmp dword ptr cs:[old_vector_09h]
new_09h endp
;=====================================================================


;=====================================================================
; Обработчик прерывания по таймеру
new_1Ch proc far
push ax
    
    inc     bios_ticks
    cmp     bios_ticks,   18                ; Каждые 18 BIOS-тиков считаем за секунду
    jne g

        mov     bios_ticks,     0
        inc     seconds                     ; Счетчик секунд
        mov     ax, seconds
        cmp     ax, cs:time                 ; Дошел ли таймер до заданного времени?
        jne y

        mov ax, 0501h
        int 10h                             ; Переключить дисплей

        mov ah, 02h
        mov bh, 01h
        mov dh, 25
        mov dl, 0
        int 10h                             ; Скрыть курсор

        mov screen_saver_active, 1          ; Заставка активна

        y:
        jbe short cont
        call screensaver                    ; Вызов процедуры каждую секунду

        cont:
        inc     five_seconds                ; Так как так как в секунде не 18, а 18.2 BIOS-тика,
        cmp     five_seconds,   5           ; для того, что бы таймер не спешил 
        jne u                               ; каждые 5 секунд добавляем один 'лишний' тик.
            mov     bios_ticks,    -1       ; Далее таймеру понадобится на 1 итерацию больше, что бы
            mov     five_seconds,   0       ; 'засчитать' секунду. Это выравнивает погрешность по времени каждые 5 сек.
        u:                                  
    g:

pop ax
    jmp dword ptr cs:[old_vector_1Ch]

    bios_ticks      dw 0
    five_seconds    dw 0

new_1Ch endp
;=====================================================================


;=====================================================================
; Мультиплексное прерывание.
; Используем для проверки на установку и для выгрузки из памяти
new_2Fh proc far
    cmp     ah, 0C8h                        ; Наш номер?
    jne     Pass_2Fh                        ; Нет, на выход
    cmp     al, 00h                         ; Подфункция проверки на повторную установку?
    je      inst                            ; Программа уже установлена
    cmp     al, 01h                         ; Подфункция выгрузки?
    je      unins                           ; Да, на выгрузку
    jmp     short Pass_2Fh                  ; Неизвестная подфункция - на выход
    inst:
    mov     al,0FFh                         ; Сообщим о невозможности повторной установки
    iret
    Pass_2Fh:
    jmp dword ptr cs:[old_vector_2Fh]

    ; Проверка - возможна ли выгрузка программы из памяти ?
    unins:
push    bx
push    cx
push    dx
push    es

    mov     cx,cs                           ; Пригодится для сравнения, т.к. с CS сравнивать нельзя
    mov     ax,351Ch                        ; Проверить вектор 1Ch
    int     21h                             ; Функция 35h в AL - номер прерывания. Возврат-вектор в ES:BX

    mov     dx,es
    cmp     cx,dx
    jne     Not_remove

    cmp     bx, offset cs:[new_1Ch]
    jne     Not_remove

    mov     ax,352Fh                        ; Проверить вектор 2Fh
    int     21h                             ; Функция 35h в AL - номер прерывания. Возврат-вектор в ES:BX

    mov     dx,es
    cmp     cx,dx
    jne     Not_remove

    cmp     bx, offset cs:[new_2Fh]
    jne     Not_remove

    mov     ax,3509h                        ; Проверить вектор 09h
    int     21h                             ; Функция 35h в AL - номер прерывания. Возврат-вектор в ES:BX

    mov     dx,es
    cmp     cx,dx
    jne     Not_remove

    cmp     bx, offset cs:[new_09h]
    jne     Not_remove

    ; Выгрузка программы из памяти 
    push    ds
    recover_vector 1Ch, cs:[old_vector_1Ch]       ; Заполнение вектора 1Ch старым содержимым
    recover_vector 09h, cs:[old_vector_09h]       ; Заполнение вектора 09h старым содержимым    
    recover_vector 2Fh, cs:[old_vector_2Fh]       ; Заполнение вектора 2Fh старым содержимым
    pop     ds

    mov     es, cs:2Ch                      ; ES -> окружение
    mov     ah, 49h                         ; Функция освобождения блока памяти
    int     21h

    mov     ax, cs
    mov     es, ax                          ; ES -> PSP выгрузим саму программу
    mov     ah, 49h                         ; Функция освобождения блока памяти
    int     21h

    mov     al, 0Fh                         ; Признак успешной выгрузки
    jmp     short pop_ret

    Not_remove:
    mov     al, 0F0h                        ; Признак - выгружать нельзя

    pop_ret:
pop     es
pop     dx
pop     cx
pop     bx

    iret
new_2Fh endp
;=====================================================================


;=====================================================================
begin:
    xor     ax,     ax
    xor     cx,     cx
    xor     bx,     bx
    xor     dx,     dx
;=============ПРОВЕРКА_СТРОКИ_ПАРАМЕТРОВ===============
    mov 	cl,		es:[80h]
    cmp     cl,     0
    jne short e
        jmp no_param
    e:

    cld
    mov     di,     81h
    mov 	al, ' '
    repe    scasb
        dec di
    push di

    lea     si,     command_uninstall
    mov cx, 4
    repe    cmpsb                           ; Сравниваем хвост с '-uninstall'
    pop di                      
    jne cn
        jmp uninstall
    cn:


    mov     ax,     di
    sub     ax,     81h
    mov     cl,     es:[80h]
    sub     cl,     al
    mov     dx,     10d                     ; DX - система счисления
;=============ПЕРЕВОД_ИЗ_СТРОКИ_В_ЧИСЛО================
conversion:
    mov     al,     byte ptr [di]
    sub     ax,     30h

    push 	ax
    mov 	ax,		1
    push    cx
    dec     cx  
    cmp     cx,     0
    je short f
    mult:
        mul		dx
    loop mult   
f: 
    pop     cx
    mov 	bx, 	ax
    pop		ax
    mul		bx

    add     time,   ax
    inc     di
    xor     ah,     ah
loop conversion
    mov     ax, time
    mov     seed, ax
;========================================================
    mov     ax, 0c800h
    int 2fh
    cmp     al, 0ffh
    jne short t
        jmp alrd_inst
    t:
    get_vector 09h, old_vector_09h
    set_vector 09h, new_09h
    get_vector 2Fh, old_vector_2Fh
    set_vector 2Fh, new_2Fh
    get_vector 1Ch, old_vector_1Ch
    set_vector 1Ch, new_1Ch

    print_mes 'Screensaver timer set on'
    print_word_dec time
    print_mes ' seconds.'

    mov     dx,     offset begin
    int 27h

uninstall:
    mov     ah, 0c8h
    mov     al, 00h
    int 2fh
    cmp     al, 0ffh
    jne alrd_uninst
    mov     al, 01h
    int 2fh
    cmp     al, 0f0h
    je error
    cmp     al, 0fh
    jne error
    print_mes 'Screensaver uninstalled'
    int 20h

alrd_uninst:
    print_mes 'Timer not installed!'
    int 20h

alrd_inst:
    print_mes 'Timer alredy installed!'
    int 20h

error:
    print_mes 'An error occured while uninstalling'
    int 20h

no_param:
    print_mes 'No parameters'
    int 20h

code_seg ends
 end start