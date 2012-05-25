#include <iostream>
#include <stdlib.h>
#include <string>


using namespace std;

class Mips
{
    public:
    //Métodos principais.
           //Busca a instrução e incrementa o contador.
           void fecth(); 
           //Extrai os campos da instrução, OP, RS, RD, RT, SHAMT e FUNCT.
           void decode(); 
           //Executa as intruções.
           void execute(); 
           //Executa uma-a-uma instrução.
           void step(); 
           //Executa o código até o final.
           void run(); 
           
    //Métodos auxiliares.
           //Mostra o conteúdo dos registradores.
           void dump_regs(); 
           //Mostra o conteúdo da memória incluindo os endereços.
           void dump_mem(int start, int end, char format); 
           //Carrega as instruções de um arquivo de texto para a memória.
           void load(); 
           
    //Registradores.
           /*Contador de Programa, Opcode, Registrador fonte 1,
           Registrador fonte 2, Registrador destino, Shift Amont, Função. */
           int pc; 
           char ir[32], op[5], rs[4], rt[4], rd[15], shamt[4], funct[5]; 
           //Registrador de instrução.
           
    //Variável auxilar para contagem de quantas instruções tem o arquivo.
           int linhas;
};

