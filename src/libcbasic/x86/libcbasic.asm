global _start

; String routines
global _sprint

; Int16 routines

; Float routines
global _f2s_scientific

;
; BSS and DATA sections
;
section .bss
  ; Floating point number printing buffer
  fsbuffer:  resb 256

section .data
  ; debug stuff
  ftest:     db 0x3D,0x00,0x00,0x00,0x04,0x00,0x00,0x71
  stest:     db 0x0C,'Ana are mere'

section .text

;
;  STRING OPERATIONS
;

; Print a CBASIC string
; Structure of string:
;   str[0] = length (0..255)
;   str[1..str[0]] = actual string
; Param1:  pointer to string (stack)
; Returns: always zero
_sprint:
  ; Get string address
  mov ecx, dword [esp+4]
  ; Get string size
  mov eax, 0
  mov al, byte [ecx]
  cmp eax, 0
  jz .exit
  ; Kernel write
  mov edx, eax  ; character count
  mov eax, 4    ; sys_write
  mov ebx, 1    ; stdout
  inc ecx       ; pointer+1
  int 0x80
.exit:
  ; All ok
  mov eax, 0
  ret

;
; 16BIT INTEGER NUMBER OPERATIONS
;

; Inline convert a 16bit integer into string
; Word is in AX
; String pointer is in ECX; function alters ECX accordingly
; Function will mess up AX, BX, DX and DI
_inline_w2s:
  ; Print sign
  mov bx, ax
  and bx, 0x8000
  jz .print_digits
  mov bl, '-'
  mov [ecx], byte bl
  inc ecx
  neg ax
.print_digits:
  ; Initialize context for digits
  mov bx, 10000    ; upper range divisor
  mov di, 0        ; flag for first non-zero encountered
.loop:
  mov dx, 0
  ; ax = ax / bx; dx = ax % bx
  idiv bx
  ; check if first non-zero encountered
  or di, ax
  jnz .write_digit
  ; Skip over digit
  ; bx = bx / 10
  mov ax, bx
  mov bx, dx
  mov dx, 0
  mov di, 10
  idiv di
  mov dx, bx
  mov bx, ax
  mov ax, dx
  mov di, 0
  cmp bx, 0
  jnz .loop
  ; This falls trough if all digits are zero and prints just one zero
.write_digit:
  ; Write digit (assume ax < 10)
  add al, 0x30
  mov [ecx], byte al
  inc ecx
  ; bx = bx / 10
  mov ax, bx
  mov bx, dx
  mov dx, 0
  mov di, 10        ; from now on it will always jump to .write_digit
  idiv di
  mov dx, bx
  mov bx, ax
  mov ax, dx
  ; Loop
  cmp bx, 0
  jnz .loop
  ret

;
; FLOATING POINT NUMBERS OPERATIONS
;

; Print a CBASIC floating point number
; Param1:  8byte floating point number (stack)
; Returns: error code of _f2s
_fprint:
   ; Fetch float
  mov eax, dword [esp+4]
  mov ebx, dword [esp+8]
  ; Convert to string
  push dword fsbuffer
  push ebx
  push eax
  call _f2s
  add esp, 12
  cmp eax, 0
  jnz .end
  ; Print string
  push dword fsbuffer
  call _sprint
  add esp, 4
  mov eax, 0
.end:
  ret

; Print a CBASIC floating point number (scientific format)
; Param1:  8byte floating point number (stack)
; Returns: error code of _f2s_scientific
_fprint_scientific:
  ; Fetch float
  mov eax, dword [esp+4]
  mov ebx, dword [esp+8]
  ; Convert to string
  push dword fsbuffer
  push ebx
  push eax
  call _f2s_scientific
  add esp, 12
  cmp eax, 0
  jnz .end
  ; Print string
  push dword fsbuffer
  call _sprint
  add esp, 4
  mov eax, 0
.end:
  ret

; Convert a CBASIC floating point number to a string
; Param1:  8byte floating point number (stack)
; Param2:  address to string buffer (stack)
; Returns: 0 on success, 1 on failure
_f2s:
  ; Clear EAX
  ; AL = buffer for digits
  mov eax, 0
  ; String iterator
  mov ecx, dword [esp+12]    ; 4 (eip) + 8 (param1)
  mov [ecx], byte 0          ; initialize size to zero (in case of error)
  inc ecx                    ; skip size byte
  ; Float reverse iterator
  mov edx, esp
  add edx, 11                ; last byte of param1
  ; EDI will hold pointer in string to last non-zero digit after first two digits
  ; Initialize in case of shitstorm
  mov edi, ecx
  ; Print sign
  mov al, byte [esp+4]
  test al, 0x80
  jz .fetch_exp
  mov al, '-'
  mov [ecx], al
  inc ecx
.fetch_exp:
  and al, 0x7F
  mov ebx, eax
  sub ebx, 0x40
.leading_zeros:
  ; E-? case, print leading zeros
  test ebx, ebx
  jns .loop
  mov al, 0x30
  mov [ecx], byte al
  inc ecx
  inc ebx
  jmp .leading_zeros
.loop:
  ; Fetch byte
  mov al, byte [edx]
  test ebx, 0x1
  jz .check_range
  shr al, 4
  dec edx
.check_range:
  and al, 0xF
  ; Check 0-9 range
  cmp al, 9
  ;jg .error
  ; Print digit
  add al, 0x30               ; '0' + al
  mov [ecx], byte al
  inc ecx
  ; Print dot after first digit
  cmp ebx, 0
  jnz .fractional_part
  mov [ecx], byte '.'
  inc ecx
  mov edi, ecx
  jmp .loop_next
.fractional_part:
  cmp ebx, 1
  jz .save_digit_as_nonzero
  ; If digit is non-zero, save ECX as last true non-zero digit
  sub al, 0x30
  jz .loop_next
.save_digit_as_nonzero:
  mov edi, ecx
.loop_next:
  ; Advance and loop
  inc ebx
  cmp ebx, 14
  jnz .loop
  ; Re-initialize ECX
  ;mov ecx, edi
  ; Put string size
  mov eax, ecx
  mov ecx, dword [esp+12]
  sub eax, ecx
  dec eax
  mov [ecx], al
  ; All ok
  mov eax, 0
  ret
.error:
  ; An error occured
  mov eax, 1
  ret

; Convert a CBASIC floating point number to a string (scientific format)
; Param1:  8byte floating point number (stack)
; Param2:  address to string buffer (stack)
; Returns: 0 on success, 1 on failure
_f2s_scientific:
  ; Clear EAX
  ; AL = buffer for digits
  mov eax, 0
  ; Current digit
  mov ebx, 0
  ; String iterator
  mov ecx, dword [esp+12]    ; 4 (eip) + 8 (param1)
  mov [ecx], byte 0          ; initialize size to zero (in case of error)
  inc ecx                    ; skip size byte
  ; Float reverse iterator
  mov edx, esp
  add edx, 11                ; last byte of param1
  ; EDI will hold pointer in string to last non-zero digit after first two digits
  ; Initialize in case of shitstorm
  mov edi, ecx
  ; Print sign
  mov al, byte [esp+4]
  and al, 0x80
  jz .loop
  mov al, '-'
  mov [ecx], al
  inc ecx
.loop:
  ; Fetch byte
  mov al, byte [edx]
  test ebx, 0x1
  jz .check_range
  shr al, 4
  dec edx
.check_range:
  and al, 0xF
  ; Check 0-9 range
  cmp al, 9
  jg .error
  ; Print digit
  add al, 0x30               ; '0' + al
  mov [ecx], byte al
  inc ecx
  ; Print dot after first digit
  cmp ebx, 0
  jnz .fractional_part
  mov [ecx], byte '.'
  inc ecx
  mov edi, ecx
  jmp .loop_next
.fractional_part:
  cmp ebx, 1
  jz .save_digit_as_nonzero
  ; If digit is non-zero, save ECX as last true non-zero digit
  sub al, 0x30
  jz .loop_next
.save_digit_as_nonzero:
  mov edi, ecx
.loop_next:
  ; Advance and loop
  inc ebx
  cmp ebx, 14
  jnz .loop
  ; Re-initialize ECX
  mov ecx, edi
  ; Print E
  mov [ecx], byte 'E'
  inc ecx
  ; Extract exponent
  mov eax, 0
  mov al, [esp+4]            ; MSB of float
  and al, 0x7F	             ; ignore sign
  sub ax, 0x40               ; normalize excess64
  ; Print exponent
  call _inline_w2s
  ; Put string size
  mov eax, ecx
  mov ecx, dword [esp+12]
  sub eax, ecx
  dec eax
  mov [ecx], al
  ; All ok
  mov eax, 0
  ret
.error:
  ; An error occured
  mov eax, 1
  ret

_start:
  ; Wrint float
  push dword [ftest+4]
  push dword [ftest]
  call _fprint
  add esp, 8

  mov ebx, 0
  mov eax, 1
  int 0x80
