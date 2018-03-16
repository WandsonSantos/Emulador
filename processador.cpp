#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
UFC - Ci�ncias da Computa��o
Trabalho de Arquitetura de Computadores
Baseado no Processador Te�rico mostrado no livro "Organiza��o Estruturada de Computadores - Tanenbaum"
Autor: WandsonSantos
*/


//Definindo uma word(palavra) de 32bits, utilizando unsigned int para ter acesso ao 32� bit;
typedef unsigned int word;


//Definindo um byte utilizando char, pois o char possui 8bits, e unsigned para ter acesso ao 8� bit;
typedef unsigned char byte;

//Definindo os Registradores;
word mar,mdr,pc,sp,lv,cpp,tos,opc,h,bA,bB,bC;
byte mbr,Z,N;
word firmware[10];

/*M�todo de leitura dos Registradores de maneira que somente 1 resgistrador
  pode ser lido por vez.*/
void ler_registrador(byte ender)
{

	//Ender � um byte logo � suficiente para endere�ar os 8 registradores
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
	system("cls");
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
		default:				   break;
	}

	if(bC == 0){
		Z == 1;
		N == 0;
	}else{
		N == 1;
		Z == 0;
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


int main()
{
	byte mpc = 0,reg_end_lei,op_alu;
	word mi,reg_end_grav;


	firmware[0] = 0b001100010000010000000;
	firmware[1] = 0b001101010000010000100;
	firmware[2] = 0b001101010000010000100;
	firmware[3] = 0b001101010000010000100;
	firmware[4] = 0b001101010000010000100;


	while(true)
	{
		reg_end_lei = (firmware[mpc] << 28) >> 28;
		ler_registrador(reg_end_lei);


		op_alu = firmware[mpc]>>13;
		alu(op_alu);

		reg_end_grav =(firmware[mpc] << 19) >> 23;
		gravar_registrador(reg_end_grav);

		debug();
		getchar();
		mpc++;
		if(mpc == 10) mpc = 0;
	}

}
