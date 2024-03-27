.code
CalcMulMatrix_asm_ proc frame
; Function prolog		
	push rsi					;save caller's rsi
		.pushreg rsi
	push rbx					;save caller's rbx
		.pushreg rbx	
	push rdi					;save caller's rdi
		.pushreg rdi
		.endprolog
; Make sure nrows and ncols are valid
	cmp r9d,0
	jle InvalidCount					;jump if rowcol <= 0
; Initialize pointers to source and destination arrays
	mov rsi,rdx							;rsi = mat1
	mov rbx,r8							;rbx = mat2
	mov rdi,rcx							;rdi = result
	xor rcx,rcx							;rcx = i
	movsxd r9,r9d						;r9 = rowcol sign extended

; Perform the required calculations
Loop1:
	xor rdx,rdx							;rdx = j
Loop2:
	xor r8,r8							;r8 = k			
	xor r13d,r13d						;r13d = sum
Loop3:
	mov rax,rcx							;rax = i
	imul rax,r9							;rax = i * rowcol
	add rax,r8							;rax = i * rowcol + k
	mov r11d ,dword ptr [rsi+rax*4]		;r11d = mat1[i][k]
	mov rax,r8							;rax = k
	imul rax,r9							;rax = k * rowcol
	add rax,rdx							;rax = k * rowcol + j
	mov r12d,dword ptr [rbx+rax*4]		;r12d = mat2[k][j]

	imul r11d,r12d						;r10d = mat1[i][k] * mat2[k][j]
	add r13d,r11d						;r13d = r11d + r13d
; inner Loop:
	inc r8								;k += 1
	cmp r8,r9
	jl Loop3							;jump if j < rowcol
; Insert the elements of the destination Matrix:
	mov rax,rcx							;rax = i
	imul rax,r9							;rax = i * rowcol
	add rax,rdx							;rax = i * rowcol + j;
	mov dword ptr [rdi+rax*4],r13d		;result[i][j] = r13d
; Middle Loop:
	inc rdx								;j += 1
	cmp rdx,r9
	jl Loop2							;jump if j < rowcol
;Outer Loop:
	inc rcx								;i += 1
	cmp rcx,r8
	jl Loop1							;jump if i < rowcol
InvalidCount:
; Function epilog
	 pop rdi							;restore caller's rdi
	 pop rbx							;restore caller's rbx
	 pop rsi							;restore caller's rsi
	 ret
CalcMulMatrix_asm_ endp
 end