//created by Amir Mehrabian mehhossein@gmail.com
#ifndef MEMORY_H
#define MEMORY_H

#include <string>
using namespace std ;


#define _32_BIT
#define LITTLE_ENDIAN_ARCHITECTURE



#ifdef _32_BIT
	
	typedef unsigned short int REGISTER ;

#else 

	typedef unsigned int REGISTER ;

#endif



class Memory
{
public :


	Memory( int size );
	~Memory();


	bool prepareSegment( int segmentNumber );
	bool loadToSegment( int segmentNumber , int startAddress , string address );


	void write( REGISTER segment , REGISTER offset , REGISTER value);
	void write( REGISTER segment , REGISTER offset , unsigned char value ) ;


	unsigned char read( REGISTER segment , REGISTER offset );
	REGISTER read( REGISTER segment , REGISTER offset , int ByteNumber );


private :

	const int memSize ;

	unsigned char** segmentArray ;	

};



#endif
