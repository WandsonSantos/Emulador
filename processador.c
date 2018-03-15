#include <stdlib.h>
#include <stddio.h>
#include <math.h>
/*
UFC - Ciências da Computação
Trabalho de Arquitetura de Computadores
Baseado no Processador Teórico mostrado no livro "Organização Estruturada de Computadores - Tanenbaum" 
Autor: WandsonSantos
*/


//Definindo uma word(palavra) de 32bits, utilizando unsigned para ter acesso ao 32º bit;
typedef unsigned int word;


//Definindo um byte utilizando char, pois o char possui 8bits, e unsigned para ter acesso ao 8º bit;
typedef unsigned char byte;

//Definindo os Registradores;
word mar,mdr,pc,sp,lv,cpp,tos,opc,h,bB,bC;
byte mbr;


/*Método de leitura dos Registradores de maneira que somente 1 resgistrador
  pode ser lido por vez.*/
void ler_registrador(byte ender)
{
	//Ender é um byte logo é suficiente para endereçar os 8 registradores
	switch(ender)
	{
		case 1: bB = mdr; break;
		case 2: bB = pc;  break;
		case 3: bB = mbr; break;
		case 4: 
		{
			sinal = ender >> 7;
			bB = (sinal) ? 0xFFFFFF00 : 0;
			bB = bB|mbr;
		}                 break;
		case 5: bB = sp;  break;
		case 6: bB = lv;  break;
		case 7: bB = cpp; break;
		case 8: bB = tos; break;
		case 9: bB = opc; break;
		default:          break;
	}

}

void gravar_registrador(word ender)
{
	if(ender & 1)   mar = Bc;
	if(ender & 2)   mdr = Bc;
	if(ender & 4)   pc = Bc;
	if(ender & 8)   sp = Bc;
	if(ender & 16)  lv = Bc;
	if(ender & 32)  cpp = Bc;
	if(ender & 64)  tos = Bc;
	if(ender & 128) opc = Bc;
	if(ender & 256) h = Bc;
}


int main()
{

}
