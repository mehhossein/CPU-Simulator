//created by Amir Mehrabian mehhossein@gmail.com
#ifndef CPU_H
#define CPU_H

#include "flag.h"
#include "decoder.h"

#include "memory.h"


#define _32_BIT
#define LITTLE_ENDIAN_ARCHITECTURE

#ifdef _32_BIT
	
	typedef unsigned short int REGISTER ;

#else 

	typedef unsigned int REGISTER ;

#endif


	



class CPU
{
	friend class Decoder ;
	friend void add( unsigned char secondNibble , CPU &cpu );
	friend void adc( unsigned char secondNibble , CPU &cpu );
	friend void andd( unsigned char secondNibble , CPU &cpu );
	friend void xorr( unsigned char secondNibble , CPU &cpu );
	friend void orr( unsigned char secondNibble , CPU &cpu );
	friend void sub( unsigned char secondNibble , CPU &cpu );
	friend void sbb( unsigned char secondNibble , CPU &cpu );
	friend void cmp( unsigned char secondNibble , CPU &cpu );
	friend void inc( unsigned char secondNibble , CPU &cpu );
	friend void DEC( unsigned char secondNibble , CPU &cpu );
	friend void push( unsigned char secondNibble , CPU &cpu );
	friend void pop( unsigned char secondNibble , CPU &cpu );
	friend void pusha( unsigned char secondNibble , CPU &cpu );
	friend void popa( unsigned char secondNibble , CPU &cpu );
	friend void pushw( unsigned char secondNibble , CPU &cpu );
	friend void pushb( unsigned char secondNibble , CPU &cpu );
	friend void jumpShort( unsigned char secondNibble , CPU &cpu );
	friend void test( unsigned char secondNibble , CPU &cpu );
	friend void xchg( unsigned char secondNibble , CPU &cpu );
	friend void mov( unsigned char secondNibble , CPU &cpu );
	friend void nop( unsigned char secondNibble , CPU &cpu );
	friend void xchgAx( unsigned char secondNibble , CPU &cpu );
	friend void cbw( unsigned char secondNibble , CPU &cpu );
	friend void cwd( unsigned char secondNibble , CPU &cpu );
	friend void pushf( unsigned char secondNibble , CPU &cpu );
	friend void popf( unsigned char secondNibble , CPU &cpu );
	friend void sahf( unsigned char secondNibble , CPU &cpu );
	friend void lahf( unsigned char secondNibble , CPU &cpu );
	friend void movao( unsigned char secondNibble , CPU &cpu );
	friend void testal( unsigned char secondNibble , CPU &cpu );
	friend void movb( unsigned char secondNibble , CPU &cpu );
	friend void movw( unsigned char secondNibble , CPU &cpu );
	friend void retn( unsigned char secondNibble , CPU &cpu );
	friend void move( unsigned char secondNibble , CPU &cpu );
	friend void leave( unsigned char secondNibble , CPU &cpu );
	friend void retf( unsigned char secondNibble , CPU &cpu );
	friend void xlat( unsigned char secondNibble , CPU &cpu );
	friend void cmc( unsigned char secondNibble , CPU &cpu );
	friend void clc( unsigned char secondNibble , CPU &cpu );
	friend void stc( unsigned char secondNibble , CPU &cpu );
	friend void cli( unsigned char secondNibble , CPU &cpu );
	friend void sti( unsigned char secondNibble , CPU &cpu );
	friend void cld( unsigned char secondNibble , CPU &cpu );
	friend void setd( unsigned char secondNibble , CPU &cpu );
	friend void imul( unsigned char secondNibble , CPU &cpu );
	friend void daa( unsigned char secondNibble , CPU &cpu );
	friend void das( unsigned char secondNibble , CPU &cpu );
	friend void aaa( unsigned char secondNibble , CPU &cpu );
	friend void aas( unsigned char secondNibble , CPU &cpu );
	friend void bound( unsigned char secondNibble , CPU &cpu );
	friend void lea( unsigned char secondNibble , CPU &cpu );
	friend void pope( unsigned char secondNibble , CPU &cpu );
	friend void movs( unsigned char secondNibble , CPU &cpu );
	friend void cmps( unsigned char secondNibble , CPU &cpu );
	friend void enter( unsigned char secondNibble , CPU &cpu );
	friend void stos( unsigned char secondNibble , CPU &cpu );
	friend void lods( unsigned char secondNibble , CPU &cpu );
	friend void scas( unsigned char secondNibble , CPU &cpu );
	friend void aam( unsigned char secondNibble , CPU &cpu );
	friend void aad( unsigned char secondNibble , CPU &cpu );
	friend void loop( unsigned char secondNibble , CPU &cpu );
	friend void call( unsigned char secondNibble , CPU &cpu );
	friend void jmp( unsigned char secondNibble , CPU &cpu );
	friend void repne( unsigned char secondNibble , CPU &cpu );
	friend void rep( unsigned char secondNibble , CPU &cpu );
	friend void hlt( unsigned char secondNibble , CPU &cpu );
	friend void group1b( unsigned char secondNibble , CPU &cpu );
	friend void terminate(unsigned char secondNibble , CPU &cpu);
	friend void addib(int , int , int , CPU &cpu);
	friend void orib(int , int , int , CPU &cpu);
	friend void adcib(int , int , int , CPU &cpu);
	friend void andib(int , int , int , CPU &cpu);
	friend void subib(int , int , int , CPU &cpu);
	friend void sbbib(int , int , int , CPU &cpu);
	friend void xorib(int , int , int , CPU &cpu);
	friend void cmpib(int , int , int , CPU &cpu);
	friend void group1w( unsigned char secondNibble , CPU &cpu );
	friend void addiw(unsigned char , int , int , int , CPU &cpu);
	friend void oriw(unsigned char , int , int , int , CPU &cpu);
	friend void adciw(unsigned char , int , int , int , CPU &cpu);
	friend void subiw(unsigned char , int , int , int , CPU &cpu);
	friend void sbbiw(unsigned char , int , int , int , CPU &cpu);
	friend void andiw(unsigned char , int , int , int , CPU &cpu);
	friend void xoriw(unsigned char , int , int , int , CPU &cpu);
	friend void cmpiw(unsigned char , int , int , int , CPU &cpu);
	friend void group4( unsigned char secondNibble , CPU &cpu );

public :

		
	CPU( Memory &memory);
	void printStatus();
	void start();
	
		
	

private :


	union GeneralRegisters
	{
		struct
		{
			REGISTER ax;
			REGISTER cx;
			REGISTER dx;
			REGISTER bx;
		};

#ifdef LITTLE_ENDIAN_ARCHITECTURE

		struct
		{
			unsigned char al;
			unsigned char ah;
			unsigned char cl;
			unsigned char ch;
			unsigned char dl;
			unsigned char dh;
			unsigned char bl;
			unsigned char bh;

		};

#else
		struct
		{
			unsigned char ah;
			unsigned char al;
			unsigned char ch;
			unsigned char cl;
			unsigned char dh;
			unsigned char dl;
			unsigned char bh;
			unsigned char bl;

		};

#endif


	}Gregs;



	REGISTER sp ;
	REGISTER bp ;
	REGISTER si ;
	REGISTER di ;
	

	REGISTER cs ;
	REGISTER ds ;
	REGISTER ss ;
	REGISTER es ;



	REGISTER ip ;
	FlagRegister flag;


	Memory &mem ;
	Decoder decoder ;
	
	void (* instructions [16][16]) (unsigned char , CPU & ) ;
	int instructionRow;
	int instructionColumn ;

	bool checkParity( unsigned char );
	bool checkParity( REGISTER );
	bool checkAf( REGISTER , REGISTER ); 
	bool finished ;

	
};


void add( unsigned char secondNibble , CPU &cpu );
void adc( unsigned char secondNibble , CPU &cpu );
void andd( unsigned char secondNibble , CPU &cpu );
void xorr( unsigned char secondNibble , CPU &cpu );
void orr( unsigned char secondNibble , CPU &cpu );
void sub( unsigned char secondNibble , CPU &cpu );
void sbb( unsigned char secondNibble , CPU &cpu );
void cmp( unsigned char secondNibble , CPU &cpu );
void inc( unsigned char secondNibble , CPU &cpu );
void DEC( unsigned char secondNibble , CPU &cpu );
void push( unsigned char secondNibble , CPU &cpu );
void pop( unsigned char secondNibble , CPU &cpu );
void pusha( unsigned char secondNibble , CPU &cpu );
void popa( unsigned char secondNibble , CPU &cpu );
void pushw( unsigned char secondNibble , CPU &cpu );
void pushb( unsigned char secondNibble , CPU &cpu );
void jumpShort( unsigned char secondNibble , CPU &cpu );
void test( unsigned char secondNibble , CPU &cpu );
void xchg( unsigned char secondNibble , CPU &cpu );
void mov( unsigned char secondNibble , CPU &cpu );
void nop( unsigned char secondNibble , CPU &cpu );
void xchgAx( unsigned char secondNibble , CPU &cpu );
void cbw( unsigned char secondNibble , CPU &cpu );
void cwd( unsigned char secondNibble , CPU &cpu );
void pushf( unsigned char secondNibble , CPU &cpu );
void popf( unsigned char secondNibble , CPU &cpu );
void sahf( unsigned char secondNibble , CPU &cpu );
void lahf( unsigned char secondNibble , CPU &cpu );
void movao( unsigned char secondNibble , CPU &cpu );
void testal( unsigned char secondNibble , CPU &cpu );
void movb( unsigned char secondNibble , CPU &cpu );
void movw( unsigned char secondNibble , CPU &cpu );
void retn( unsigned char secondNibble , CPU &cpu );
void move( unsigned char secondNibble , CPU &cpu );
void leave( unsigned char secondNibble , CPU &cpu );
void retf( unsigned char secondNibble , CPU &cpu );
void xlat( unsigned char secondNibble , CPU &cpu );
void cmc( unsigned char secondNibble , CPU &cpu );
void clc( unsigned char secondNibble , CPU &cpu );
void stc( unsigned char secondNibble , CPU &cpu );
void cli( unsigned char secondNibble , CPU &cpu );
void sti( unsigned char secondNibble , CPU &cpu );
void cld( unsigned char secondNibble , CPU &cpu );
void setd( unsigned char secondNibble , CPU &cpu );
void imul( unsigned char secondNibble , CPU &cpu );
void daa( unsigned char secondNibble , CPU &cpu );
void das( unsigned char secondNibble , CPU &cpu );
void aaa( unsigned char secondNibble , CPU &cpu );
void aas( unsigned char secondNibble , CPU &cpu );
void bound( unsigned char secondNibble , CPU &cpu );
void lea( unsigned char secondNibble , CPU &cpu );
void pope( unsigned char secondNibble , CPU &cpu );
void movs( unsigned char secondNibble , CPU &cpu );
void cmps( unsigned char secondNibble , CPU &cpu );
void enter( unsigned char secondNibble , CPU &cpu );
void stos( unsigned char secondNibble , CPU &cpu );
void lods( unsigned char secondNibble , CPU &cpu );
void scas( unsigned char secondNibble , CPU &cpu );
void aam( unsigned char secondNibble , CPU &cpu );
void aad( unsigned char secondNibble , CPU &cpu );
void loop( unsigned char secondNibble , CPU &cpu );
void call( unsigned char secondNibble , CPU &cpu );
void jmp( unsigned char secondNibble , CPU &cpu );
void repne( unsigned char secondNibble , CPU &cpu );
void rep( unsigned char secondNibble , CPU &cpu );
void hlt( unsigned char secondNibble , CPU &cpu );
void group1b( unsigned char secondNibble , CPU &cpu );
void terminate(unsigned char secondNibble , CPU &cpu);
void addib(int , int , int , CPU &cpu);
void orib(int , int , int , CPU &cpu);
void adcib(int , int , int , CPU &cpu);
void andib(int , int , int , CPU &cpu);
void subib(int , int , int , CPU &cpu);
void sbbib(int , int , int , CPU &cpu);
void xorib(int , int , int , CPU &cpu);
void cmpib(int , int , int , CPU &cpu);
void group1w( unsigned char secondNibble , CPU &cpu );
void addiw(unsigned char , int , int , int , CPU &cpu);
void oriw(unsigned char , int , int , int , CPU &cpu);
void adciw(unsigned char , int , int , int , CPU &cpu);
void subiw(unsigned char , int , int , int , CPU &cpu);
void sbbiw(unsigned char , int , int , int , CPU &cpu);
void andiw(unsigned char , int , int , int , CPU &cpu);
void xoriw(unsigned char , int , int , int , CPU &cpu);
void cmpiw(unsigned char , int , int , int , CPU &cpu);
void group4( unsigned char secondNibble , CPU &cpu );




#endif
