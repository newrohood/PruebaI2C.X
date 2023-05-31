#include<xc.h>
#define _XTAL_FREQ 1000000
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF

void Start(void);
void Stop(void);
void Rstart(void);
void EnvioDato(unsigned char,unsigned char,unsigned char);
unsigned char LecturaDato(unsigned char,unsigned char);

void main(void){
  unsigned char i=0;
  TRISD=0;
  LATD=0;
  SSPCON1=0b00101000;
  SSPCON2=0b00000000;
  SSPSTAT=0b11000000;
  SSPADD=2;
  PIR1=0;  
  while(1){
    for(i=0;i<10;i++){
      EnvioDato(0xA0,i,10+i);
      __delay_ms(1000);
    }
    for(i=0;i<10;i++){
      LATD=LecturaDato(0xA0,i);
      __delay_ms(1000);
    }
        
  }  
}
void Start(void){    
  SEN=1;
  while(SEN==1);  
}
void Stop(void){
  PEN=1;
  while(PEN==1);  
}
void Rstart(void){
  RSEN=1;
  while(RSEN==1);  
}
void EnvioDato(unsigned char Direccion, unsigned char Registro, unsigned char Dato){
  Start();
  SSPBUF=Direccion & 0b11111110;
  SSPIF=0;
  while(SSPIF==0);
  SSPBUF=Registro;
  SSPIF=0;
  while(SSPIF==0);
  SSPBUF=Dato;
  SSPIF=0;
  while(SSPIF==0);
  SSPIF=0;
  Stop();  
}
unsigned char LecturaDato(unsigned char Direccion, unsigned char Registro){
  Start();
  SSPBUF=Direccion & 0b11111110;
  SSPIF=0;
  while(SSPIF==0);
  SSPBUF=Registro;
  SSPIF=0;
  while(SSPIF==0);
  Rstart();
  SSPBUF=Direccion | 0b00000001;
  SSPIF=0;
  while(SSPIF==0);
  RCEN=1;
  SSPIF=0;
  while(SSPIF==0);
  Stop();
  return SSPBUF;  
}