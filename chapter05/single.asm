;single.asm-----------------------------------
.386
codeSegment SEGMENT USE16
ASSUME CS:codeSegment, DS:codeSegment, SS:codeSegment, ES:codeSegment
ORG 100H
;instructions---------------------------------
startHere:
PUSH DS
MOV AH,0H
PUSH AX

MOV [stackAddress],SP
MOV SP,OFFSET newStack

MOV AX,OFFSET message1
PUSH AX
CALL printMessage

MOV AX,OFFSET message2
PUSH AX
CALL printMessage

MOV SP,[stackAddress]
RETF

;embedded storage-----------------------------
stackAddress 	DW ?
message1 	DB "Hey Moe! Hey larry! "
terminate1	DB '$'
message2  	DB "woo-woo-woo"
terminate2 	DB '$'

;embedded stack-------------------------------
stackStorage	DB 31 dup ('01')
newStack	DB 01H

;more instructions----------------------------
printMessage:
PUSH BP
MOV  BP,SP
MOV AH,09H
MOV DX,[BP+4]
INT 21H
POP BP
RET
codeSegment ENDS
END startHere
