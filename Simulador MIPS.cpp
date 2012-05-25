#include "Memory.h"
#include "Mips.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <locale.h>
#include <fstream>
#include <conio.h>

using namespace std;
using std::getline;
using std::string;

// Declarações de objetos.

Memory objMemory; 
Mips objIR;
Mips objOP;
Mips objRS;
Mips objRT;
Mips objRD;
Mips objSHAMT;
Mips objFUNCT;
Mips objPC;
Mips objLINHAS;

int pc = 0;

char line[] = "\n========================================================================= \n";

// enums.

enum opcode
{
    LW = 35, SW = 43
};

enum function
{
    ADD = 32, SUB = 34, MUL = 24, DIV = 26
};

// Método da classe Memory.

int Memory::read(int address)
{
    return m_array[address];
}

void Memory::write(int address, int data)
{
    m_array[address] = data;
}

// Método para mostrar os valores finais armazenados nos registradores.

void dump_regs()
{
    cout << "Registrador RS: " << objRS.rs << "\nRegistrador RT: " 
    << objRT.rt << "\nRegistrador RD: " << objRD.rd
    << "\nRegistrador funct: " << objFUNCT.funct << "\n" << endl;
}

// Método para mostrar o contúdo da memória.

void dump_mem(int start, int end, char format)
{
    int x;
    unsigned long int aux1;
    x = start;
    cout << "\n" << "Posição   Conteúdo \n\n";
    while(x <= end)
    {
        switch(format)
        {
            case 'd':
            case 'D':
                 aux1 = objMemory.read(start);
                 printf("   %.3ld -- %ld \n", start, aux1);
                 break;
            case 'h':
            case 'H':
                 aux1 = objMemory.read(start);
                 printf("   %.3ld -- %X \n", start, aux1);
                 break;
            default:
                 cout << line 
                 << "ATENÇÃO!!! \nVocê digitou uma opção não compatível. "
                 << "O programa sera encerrado! \n" << endl;
                 getch();
                 exit(0);
        }
        start++;
        x++;
    }
    cout << "\n";
}

// Métodos de execução das instruções (palavras).

void execute()
{
    unsigned long int funcao, opcode=0, s0=0, s1=0, s2=0, t0=0, t1=0, t2=0;
    unsigned long int i, HI, LO, C;
    
    opcode = strtoul(objOP.op, NULL, 2);
    funcao = strtoul(objFUNCT.funct, NULL, 2);
    
    switch(opcode)
    {
        case SW:
             t0 = strtoul(objRS.rs, NULL, 2);
             C = strtoul(objRD.rd, NULL, 2);
             objMemory.write(C, t0);
             itoa(t0, objRD.rd, 2);
             break;
             
        case 0:
             switch(funcao)
             {
                 case ADD: //Função de adição.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      s0 = s2 + s1;
                      itoa(s0, objRD.rd, 2);
                      break;
             
                 case SUB: //Função de subtração.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      s0 = s2 - s1;
                      itoa(s0, objRD.rd, 2); 
                      break;
             
                 case MUL: //Função de multiplicação.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      i = 1;
                      t0 = s2;
                      while(i < s1)
                      {
                          t0 = t0 + s2;
                          i++;
                      }
                      itoa(t0, objRD.rd, 2);
                      break;
             
                 case DIV: //Função de divisão.
                      s2 = strtoul(objRS.rs,NULL, 2);
                      s1 = strtoul(objRT.rt, NULL, 2);
                      t0 = s2;
                      HI=1;
                      while(1)
                      {
                          t0-=s1;
                          if(t0 <= 1)
                              break;
                          HI++;
                      }
                      s0 = HI;
                      itoa(s0, objRD.rd, 2);
                      break;
             }
    }
}

/* Método auxiliar para complementar as instruções convertidas de DEC para BIN 
com 'zeros' a esquerda. */

void complemento(int quantidade01)
{
     int k = 0;
     char zero[100];
     while(quantidade01 < 32)
     {
         zero[k] = '0';
         quantidade01++;
         k++;
     }
     zero[k] = '\0';
     strcat(zero, objIR.ir);
     strcpy(objIR.ir, zero);
}

// Métodos de decodificação das instruções para os registradores(palavras).

void decode()
{
    int quantidade;
    quantidade = strlen(objIR.ir);
    if(quantidade < 32);
        complemento(quantidade);

    string s = objIR.ir;
    unsigned long int aux;
    
    s.copy(objOP.op, 6, 0); 
    aux = strtoul(objOP.op, NULL, 2);
    
    switch(aux)
    {
        case LW:
             s.copy(objRS.rs, 5, 6); 
             s.copy(objRT.rt, 5, 11); 
             s.copy(objRD.rd, 16, 16);
             break;
        case SW:
             s.copy(objRS.rs, 5, 6); 
             s.copy(objRT.rt, 5, 11); 
             s.copy(objRD.rd, 16, 16);
             break;
        default:
                s.copy(objRS.rs, 5, 6); 
                s.copy(objRT.rt, 5, 11); 
                s.copy(objRD.rd, 5, 16); 
                s.copy(objSHAMT.shamt, 5, 21); 
                s.copy(objFUNCT.funct, 6, 26);
    }
}

// Método de busca das instruções.

void fetch()
{
    unsigned long int instrucao1;
    instrucao1 = objMemory.read(pc);
    itoa(instrucao1, objIR.ir, 2);
    pc++;
}

// Método para carregar as instruções (palavras) de um arquivo para a memória.

void load()
{
    setlocale(LC_ALL, "");
    char caminho[300], inst[32]; 
    unsigned long int instrucao;
    
    cout << line << "Digite o endereço (caminho) onde está seu documento "
    << "com as instruções."
    << "\nExemplo: \"C:\\Arquivos de programas\\MIPS\\mips.txt\" \n" << endl;
    cin >> caminho;
    
    ifstream arquivo;
    arquivo.open(caminho);
    if(arquivo.fail())
    {
        cout << "\n\nErro na abertura do arquivo!!! \n\n" << "Verifique se o "
        << "caminho digitado está correto. \n"
        << "Verifique se o nome do arquivo está correto. \nVerifique se a "
        << "extensão do arquivo "
        << "esta correta." << "\n\n\tPressione qualquer tecla para sair. ";
        getch();
        exit(0);
    }
    
    int i = 0; //a variável "i" fará a função do PC nesse método.
    while(arquivo.getline(inst, 33))
    {
        instrucao = strtoul(inst, NULL, 2);
        objMemory.write(i, instrucao);
        objLINHAS.linhas = i;
        i+=4;
    }
    arquivo.close();
}

// Métodos para realizar busca, decidificação e execução das instruções.

void step()
{
    fetch();
    decode();
    execute();
}

/* Método para realizar os passos do 'step' até que não tenha mais intruções 
em memória. */

void run()
{
    step();
}

// Método auxiliar para mostrar o menu na tela.

void dump()
{
    int valor_ini, valor_fin;
    char escolha, formato;
    system("cls");
    cout << line << "Escolha uma das opções abaixo: \n\nDigite (1) para ver o "
    << "conteúdo dos registradores."
    << "\nDigite (2) para ver o conteúdo da memória." 
    << "\nDigite (3) para sair do programa. \n\nDigite sua opção: ";
    cin >> escolha;
    
    switch(escolha)
    {
        case '1':
             system("cls");
             cout << line << "Opção escolhida: " << escolha << "\n" << endl;
             dump_regs();
             break;
        case '2':
             system("cls");
             cout << line << "Opção escolhida: " << escolha << "\n" << endl;
             cout << "Digite a posição inicial da memória a ser lida: ";
             cin >> valor_ini;
             cout << "\nDigite o valor final da memória a ser lida: ";
             cin >> valor_fin;
             cout << "\nDigite o tipo a ser mostrado \n(D) para decimal ou (H) "
             << "para hexadecimal: ";
             cin >> formato;
             cout << line;
             dump_mem(valor_ini, valor_fin, formato);
             break;
        case '3':
             exit(0);
        default:
                system("cls");
                dump();
    }
}

// Método principal.

int main()
{
    load();
    int z = 0;
    do
    {
        run();
        z++;
    }
    while(z <= objLINHAS.linhas);
    dump();
    system("pause");
    return 0;
}
