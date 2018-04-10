#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_MEM 16777216
/*
UFC - Ciï¿½ncias da Computaï¿½ï¿½o
Trabalho de Arquitetura de Computadores
Baseado no Processador Teï¿½rico mostrado no livro "Organizaï¿½ï¿½o Estruturada de Computadores - Tanenbaum"
Autor: WandsonSantos
*/


//Definindo uma word(palavra) de 32bits, utilizando unsigned int para ter acesso ao 32ï¿½ bit;
typedef unsigned int word;


//Definindo um byte utilizando char, pois o char possui 8bits, e unsigned para ter acesso ao 8ï¿½ bit;
typedef unsigned char byte;

typedef unsigned long int microinstruction;

//Definindo os Registradores;
word mar,mdr,pc,sp,lv,cpp,tos,opc,h,bA,bB,bC;
byte mbr,z,n;

microinstruction firmware[512];

byte ram[MAX_MEM]; // 16 MBytes de RAM

/*Mï¿½todo de leitura dos Registradores de maneira que somente 1 resgistrador
  pode ser lido por vez.*/
void ler_registrador(byte ender)
{

	//Ender ï¿½ um byte logo ï¿½ suficiente para endereï¿½ar os 8 registradores
	switch(ender)
	{
		case 0: bB = mdr; break;
		case 1: bB = pc ; break;
		case 2: bB = mbr; break;
		case 3:
		{
			byte sinal = ender >> 7;
			bB = (sinal) ? 0xFFFFFF00 : 0;
			bB = bB|mbr;
		}                 break;
		case 4: bB = sp ; break;
		case 5: bB = lv ; break;
		case 6: bB = cpp; break;
		case 7: bB = tos; break;
		case 8: bB = opc; break;
		default:          break;
	}

}

void debug()
{
	system("clear");
	printf("---Registradores---\n");
	printf("MAR: %d\n",mar);
	printf("MDR: %d\n",mdr);
	printf("PC: %d\n" ,pc );
	printf("MBR: %d\n",mbr);
	printf("SP: %d\n" ,sp );
	printf("LV: %d\n" ,lv );
	printf("CPP: %d\n",cpp);
	printf("TOS: %d\n",tos);
	printf("OPC: %d\n",opc);
	printf("H: %d\n"  ,h  );
}

void gravar_registrador(word ender)
{

	if(ender & 1)   mar = bC;
	if(ender & 2)   mdr = bC;
	if(ender & 4)   pc  = bC;
	if(ender & 8)   sp  = bC;
	if(ender & 16)  lv  = bC;
	if(ender & 32)  cpp = bC;
	if(ender & 64)  tos = bC;
	if(ender & 128) opc = bC;
	if(ender & 256)   h = bC;
}

void alu(byte operacao)
{
	bA = h;
	byte op_alu = (operacao << 2) >> 2;

	switch(op_alu)
	{
		case 12: bC = bA & bB    ; break;
		case 16: bC = 0          ; break;
		case 20: bC = bB         ; break;
		case 24: bC = bA         ; break;
		case 26: bC = ~bA        ; break;
		case 28: bC = bA | bB    ; break;
		case 44: bC = ~bB        ; break;
		case 49: bC = 1          ; break;
		case 50: bC = -1         ; break;
		case 53: bC = bB + 1     ; break;
		case 54: bC = bB - 1     ; break;
		case 57: bC = bA + 1     ; break;
		case 59: bC = -bA        ; break;
		case 60: bC = bA + bB    ; break;
		case 61: bC = bA + bB + 1; break;
		case 63: bC = bB - bA    ; break;
		default:		   break;
	}

	if(bC == 0){
		z == 1;
		n == 0;
	}else{
		n == 1;
		z == 0;
	}

	byte op_desloc = operacao >> 6;
	switch(op_desloc)
	{
		case 0:                break;
		case 1:  bC = bC << 8; break;
		case 2:  bC = bC >> 1; break;
		default: bC = bC << 7; break;

	}

}

word next_address(word next,byte jmp)
{
	if(jmp == 0) return next           ;
	if(jmp & 1)  return next | (z << 8);
	if(jmp & 2)  return next | (n << 8);
	if(jmp & 4)  return next | mbr	   ;
}

void memory(byte mem)
{

	if(mem & 1) mbr = ram[pc]	      ;
	if(mem & 2) memcpy(&mdr,&ram[mar*4],4);
	if(mem & 4) memcpy(&ram[mar*4],&mdr,4);
}

int main()
{
	byte mpc = 0,reg_end_lei,op_alu,jmp,mem;
	word reg_end_grav,addr;
	microinstruction mi;
	

	firmware[0] = 0b000000001000001100010000010000000000;
	firmware[1] = 0b000000010000001101010000010000000100;
	firmware[2] = 0b000000011000001101010000010000000100;
	firmware[3] = 0b000000100000001101010000010000000100;
	firmware[4] = 0b000000000000001101010000010000000100;

	op_alu = 0;
	while(true)
	{
		debug();

		mi = firmware[mpc];
		reg_end_lei = (mi << 60) >> 60;
		ler_registrador(reg_end_lei);

		op_alu = (mi << 40) >> 56;
		alu(op_alu);
		
		reg_end_grav = (mi << 48) >> 55;
		gravar_registrador(reg_end_grav);
		
		mem = (mi << 57) >> 61;
		memory(mem);

		addr = mi >> 27;
		jmp = ((mi << 37) >> 61);
		mpc = next_address(addr, jmp);
	
		getchar();
	}

}
