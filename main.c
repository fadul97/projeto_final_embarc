#include <proc/pic18f4520.h>
#include "ds1307.h"
#include "keypad.h"
#include "lcd.h"
#include "ssd.h"
#include "timer.h"
#include "pwm.h"
#include "delay.h"

#define L_CLR 0x01
#define L_L1 0x80
#define L_L2 0xC0



// Variaveis
int novoUsuarioPodeEntrar = 1;
int tempoEstadia = 0;
int usuarioPediuParaSair = 0;
unsigned int valorAPagar = 0;
int tempoDeTolerancia = 10;
int valorPorHora = 2;



// Funcoes
void iniciarPerifericos();

void mostrarMenuInicial();

int verificarUsuarioPediuParaEntrar();

void atualizarHorarioEntrada();

void ativarBuzzer();

void atualizarTempoEstadia();

int verificarUsuarioPediuParaSair();

void atualizarHorarioSaida();

void resetarTempoEstadia();

int verificarUsuarioPagou(); 

void acenderLedNaSequencia();

void ledParaDireita();

void delay();

void ledParaEsquerda();


void main(void) {

    iniciarPerifericos();

    for (;;) {
        if (novoUsuarioPodeEntrar == 1) {
            mostrarMenuInicial();
            if (verificarUsuarioPediuParaEntrar()) {
                atualizarHorarioEntrada();
                ativarBuzzer();
                novoUsuarioPodeEntrar = 0;
            }
        } else if (novoUsuarioPodeEntrar == 0) {
            if(usuarioPediuParaSair == 0){
                atualizarTempoEstadia();

                if (verificarUsuarioPediuParaSair()) {
                    atualizarHorarioSaida();
                    ativarBuzzer();
                    resetarTempoEstadia();    
                    usuarioPediuParaSair = 1;
                }

            } else{
                if(verificarUsuarioPagou()){
                    lcdCommand(L_CLR);
                    lcdString("Pagando...");
                    acenderLedNaSequencia();
                    novoUsuarioPodeEntrar = 1;
                    usuarioPediuParaSair = 0;
                    tempoEstadia = 0;
                    valorAPagar = 0;
                    lcdCommand(L_CLR);
                }
            }
        }
    }
}



void iniciarPerifericos() {
    lcdInit();
    kpInit();
    timerInit();
    ssdInit();
    pwmInit();
}

void mostrarMenuInicial(){   

    lcdString("Bem vindo ao SE");
    lcdCommand(L_L2);
    lcdString("Hora: ");
    lcdChar(((getHours() / 10) % 10) + 48);
    lcdChar((getHours() % 10) + 48);
    lcdChar(':');
    lcdChar(((getMinutes() / 10) % 10) + 48);
    lcdChar((getMinutes() % 10) + 48);
    lcdChar(':');
    lcdChar(((getSeconds() / 10) % 10) + 48);
    lcdChar((getSeconds() % 10) + 48);
    lcdChar(' ');
    lcdChar(' ');
}

int verificarUsuarioPediuParaEntrar() {
    kpDebounce();
    return (kpReadKey() != 0) && (kpReadKey() == 'U');
}

void atualizarHorarioEntrada() {
    lcdCommand(L_CLR);
    lcdString("In:  ");
    lcdChar(((getHours() / 10) % 10) + 48);
    lcdChar((getHours() % 10) + 48);
    lcdChar(':');
    lcdChar(((getMinutes() / 10) % 10) + 48);
    lcdChar((getMinutes() % 10) + 48);
    lcdChar(':');
    lcdChar(((getSeconds() / 10) % 10) + 48);
    lcdChar((getSeconds() % 10) + 48);
}

void ativarBuzzer(){
    pwmSet(100);
    atrasar(15);
    pwmSet(0);
    atrasar(15);
    pwmSet(100);
    atrasar(15);
    pwmSet(0);
}

void atualizarTempoEstadia() {
    ssdDigit(((tempoEstadia / 60000) % 10), 0);
    ssdDigit(((tempoEstadia / 6000) % 10), 1);
    ssdDigit(((tempoEstadia / 1000) % 6), 2);
    ssdDigit(((tempoEstadia / 100) % 10), 3);
    ssdUpdate();
    timerReset(9000);
    timerWait();
    tempoEstadia++;
}

int verificarUsuarioPediuParaSair() {
    kpDebounce();
    return (kpReadKey() != 0) && (kpReadKey() == 'D');
}

void atualizarHorarioSaida() {
    lcdCommand(L_L2);
    lcdString("Out: ");
    lcdChar(((getHours() / 10) % 10) + 48);
    lcdChar((getHours() % 10) + 48);
    lcdChar(':');
    lcdChar(((getMinutes() / 10) % 10) + 48);
    lcdChar((getMinutes() % 10) + 48);
    lcdChar(':');
    lcdChar(((getSeconds() / 10) % 10) + 48);
    lcdChar((getSeconds() % 10) + 48);
    
    unsigned char t = 120;
    atrasar(t);

    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Valor a pagar");
    lcdCommand(L_L2);
    lcdString("R$: ");

    valorAPagar = tempoEstadia;
    lcdChar(((valorAPagar / 100000) % 10) + 48);
    lcdChar(((valorAPagar / 10000) % 10) + 48);
    lcdChar('.');
    lcdChar(((valorAPagar / 1000) % 10) + 48);
    lcdChar(((valorAPagar / 100) % 10) + 48);
}

void resetarTempoEstadia() {
    ssdDigit(0, 0);
    ssdDigit(0, 1);
    ssdDigit(0, 2);
    ssdDigit(0, 3);
    ssdUpdate();
}

int verificarUsuarioPagou() {
    kpDebounce();
    return (kpReadKey() != 0) && (kpReadKey() == 'R');
}

void acenderLedNaSequencia(){
    ledParaDireita();
    delay();
    ledParaEsquerda();
}



void ledParaDireita() {
    delay();
    unsigned char k, x;
    x = 0b10000001;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x << 1;
    }
}

void delay() {
    char i, j;
    for (i = 0; i < 255; i++) {
        for (j = 0; j < 255; j++);
        for (j = 0; j < 255; j++);
    }
}

void ledParaEsquerda() {
    delay();
    unsigned char k, x;
    x = 0b10000000;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x >> 1;
    }
}
