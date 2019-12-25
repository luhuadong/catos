;***********************************************************
;           Global variable
;***********************************************************
	IMPORT rt_thread_switch_interrupt_flag
	IMPORT rt_interrupt_from_thread
	IMPORT rt_interrupt_to_thread
	
;***********************************************************
;            Constant
;***********************************************************
SCB_VTOR        EQU    0xE000ED08    ; Vector table offset register
NVIC_INT_CTRL   EQU    0xE000ED04    ; NVIC control status register
NVIC_SYSPRI2    EQU    0xE000ED20    ; System priority register (2)
NVIC_PENDSV_PRI EQU    0x00FF0000    ; PendSV priority value (lowest)
NVIC_PENDSVSET  EQU    0x10000000    ; Toggle the value of PendSV exception

;***********************************************************
;           Code generate instructions
;***********************************************************
	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8

;/*
; *---------------------------------------------------------------
; * void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
; * r0 --> from
; * r1 --> to
; *---------------------------------------------------------------
; */
rt_hw_context_switch    PROC
	EXPORT rt_hw_context_switch
	
	; 设置中断标志位rt_thread_switch_interrupt_flag为1
	LDR r2, =rt_thread_switch_interrupt_flag
	LDR r3, [r2]
	CMP r3, #1
	BEQ _reswitch
	MOV r3, #1
	STR r3, [r2]
	
	; r0 --> rt_interrupt_from_thread
	LDR r2, =rt_interrupt_from_thread
	STR r0, [r2]
	
_reswitch
	; r1 --> rt_interrupt_to_thread
	LDR r2, =rt_interrupt_to_thread
	STR r1, [r2]
	
	LDR r0, =NVIC_INT_CTRL
	LDR r1, =NVIC_PENDSVSET
	STR r1, [r0]
	BX  LR
	
	ENDP

;/*
; *-----------------------------------------------------------------
; * void PendSV_Handler(void);
; * r0 --> switch from thread stack
; * r1 --> switch to thread stack
; * psr, pc, lr, r12, r3, r2, r1, r0 are pushed into [from] stack
; *-----------------------------------------------------------------
; */
PendSV_Handler    PROC
	EXPORT PendSV_Handler
	
	; 失能中断，为了保护上下文切换不被中断
	MRS r2, PRIMASK
	CPSID I
	
	; 获取中断标志位，检查是否为0，为0则跳转到pendsv_exit
	LDR r0, =rt_thread_switch_interrupt_flag
	LDR r1, [r0]
	CBZ r1, pendsv_exit
	
	; 将rt_thread_switch_interrupt_flag标志清0
	MOV r1, #0x00
	STR r1, [r0]
	
	; 判断rt_interrupt_from_thread是否为0，为0则跳转到switch_to_thread
	LDR r0, =rt_interrupt_from_thread
	LDR r1, [r0]
	CBZ r1, switch_to_thread
	
	; 上文保存
	MRS r1, psp
	STMFD r1!, {r4 - r11}
	LDR r0, [r0]
	STR r1, [r0]
	
	; 下文切换
switch_to_thread
	LDR     r1, =rt_interrupt_to_thread
	LDR     r1, [r1]
	LDR     r1, [r1]
	LDMFD   r1!, {r4 - r11}
	MSR     psp, r1

	; 恢复中断
pendsv_exit
	; restore interrupt
	MSR     PRIMASK, r2
	ORR     lr, lr, #0x04
	BX      lr
	ENDP

;/*
; *----------------------------------------------------
; * void rt_hw_context_switch_to(rt_uint32_t to);
; * r0 --> to
; * this function will be used to the first switch
; *----------------------------------------------------
; */
rt_hw_context_switch_to    PROC
	
	; 导出函数名，使其可以在C文件调用
	EXPORT rt_hw_context_switch_to
	
	; 设置rt_interrupt_to_thread的值
	LDR r1, =rt_interrupt_to_thread
	STR r0, [r1]
	
	; 设置rt_interrupt_from_thread的值为0，表示启动第一次线程切换
	LDR r1, =rt_interrupt_from_thread
	MOV r0, #0x0
	STR r0, [r1]
	
	; 设置中断标志位rt_thread_switch_interrupt_flag的值为1
	LDR r1, =rt_thread_switch_interrupt_flag
	MOV r0, #1
	STR r0, [r1]
	
	; 设置PendSV异常的优先级
	LDR r0, =NVIC_SYSPRI2
	LDR r1, =NVIC_PENDSV_PRI
	LDR.W r2, [r0, #0x00]    ; read
	ORR r1, r1, r2           ; modify
	STR r1, [r0]             ; write-back
	
	; 触发PendSV异常（产生上下文切换）
	LDR r0, =NVIC_INT_CTRL
	LDR r1, =NVIC_PENDSVSET
	STR r1, [r0]
	
	; 开中断
	CPSIE F
	CPSIE I
	
	ENDP

	ALIGN   4
		
	END
