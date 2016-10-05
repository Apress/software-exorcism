/* bytecode -------------------------------------------------------*/

#include<stdio.h>

#define R0			0
#define R1			1
#define R2			2
#define R3			3
#define R4			4

#define PUT		0	//PUT	constant,R*
#define STORE	1	//STORE R*,address
#define LOAD	2	//LOAD  address,R*
#define MOV		3	//MOV	R*,R*
#define ADD		4	//ADD	R*,R*,R*
#define SUB		5	//SUB	R*,R*,R*
#define STOP	6	//STOP
#define PRINT	7	//PRINT R*
#define BREAK	8	//BREAK

#define write(arg)	fputc(arg,filePointer)

void main()
{
	FILE *filePointer;
	filePointer = fopen("program.run","wb");
	
	//address 0		encoding:(0x8)
	write(BREAK); 
	
	//address 1		(0x0 0xDDCCBBAA 0x0)
	write(PUT); 
	write(0xAA); write(0xBB); write(0xCC); write(0xDD);
	write(R0);	

	//address 7		encoding:(0x3 0x0 0x2)
	write(MOV);
	write(R0);
	write(R2);

	//address 10	encoding:(0x1 0x2 0x00000025) 
	//(store at 0x25 = 37 )
	write(STORE);
	write(R2);
	write(0x25); write(0x00); write(0x00); write(0x00);

	//address 16	encoding:(0x2 0x00000025 0x4) 
	write(LOAD);
	write(0x25); write(0x00); write(0x00); write(0x00);
	write(R4);

	//address 22	encoding:(0x4 0x2 0x4 0x1 )
	write(ADD); write(R2); write(R4); write(R1);

	//address 26	encoding:(0x5 0x2 0x4 0x1)
	write(SUB); write(R2); write(R4); write(R1);

	//address 30	encoding:(0x7 0x1)
	write(PRINT); write(R1);

	//address 32	encoding:(0x6)
	write(STOP);

	//address 33, data storage at end of file

	write(0x00);write(0x00);write(0x00);write(0x00);
	write(0x00);write(0x00);write(0x00);write(0x00);
	write(0x00);write(0x00);write(0x00);write(0x00);
	write(0x00);write(0x00);write(0x00);write(0x00);

	fclose(filePointer);
}