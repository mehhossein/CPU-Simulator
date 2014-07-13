//created by Amir Mehrabia mehhossein@gmail.com
#ifndef DECODER_H
#define DECODER_H





#define _32_BIT
//#define LITTLE_ENDIAN

class CPU ;

#ifdef _32_BIT
	
	typedef unsigned short int REGISTER ;

#else 

	typedef unsigned int REGISTER ;

#endif




class Decoder 
{
public:

	/*Decoder();
	~Decoder();*/
	void decodeOpcode( int &row , int &column);
	void setCPU( CPU* );
	void decodeModeRM( int &G , int &E , int &adressingMode , int &segment );  
	
private :

	
	


	CPU *_cpu ;
	unsigned char opcode ;
	unsigned char mode ;

};








#endif





