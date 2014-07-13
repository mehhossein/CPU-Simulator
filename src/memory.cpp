//created by Amir Mehrabian mehhossein@gmail.com
#include <fstream>
using namespace std ;

#include "memory.h"

Memory :: Memory( int size)
:memSize( size )
{
	segmentArray = new unsigned char*[ memSize ] ; 

	for( int i = 0 ; i < memSize ; i++ )
	{
		segmentArray[i] = 0;
	}

}



Memory::~Memory()
{

	for( int i = 0 ; i < memSize ; i++ )
	{
		delete[] segmentArray[i] ;
	}


	delete segmentArray ;

}
	

bool Memory::prepareSegment( int segmentNumber )
{
	segmentArray[ segmentNumber ] = new unsigned char[ 65536 ];

	if( segmentArray[ segmentNumber ] == 0 )
		return false ;
	else 
		return true ;
}


bool Memory::loadToSegment( int segmentNumber , int startAddress , string address )
{
	ifstream in( address.c_str() , ios::binary );

	unsigned char *segment = segmentArray[ segmentNumber ] ;

	if( !in )

		return false ;

	else

		if( segment != 0 )
		{

			while( !in.eof() )
			{
				segment[startAddress] = in.get() ;

				startAddress++;
			}

			in.close();

			return true ;
		}

		else

			return false;
}



void Memory::write( REGISTER segment , REGISTER offset , unsigned char value )
{
	int segmentNumber = segment & 983040 ;

	offset += segment * 16 ;

	segmentArray[ segmentNumber ][ offset ] = value ;
}



void Memory::write( REGISTER segment , REGISTER offset , REGISTER value )
{
	int segmentNumber = segment & 983040 ;
	
	int lowByte = value & 255 ;

	offset += segment * 16 ;

	segmentArray[ segmentNumber ][ offset ] = static_cast< unsigned char >( lowByte ) ;


	offset++ ;

	int highByte = value & 65280 ;
	highByte >>= 8 ;

	segmentArray[ segmentNumber ][ offset ] = static_cast< unsigned char >( highByte ) ;

}
	


unsigned char Memory::read( REGISTER segment , REGISTER offset )
{
	int segmentNumber = segment & 983040 ;
	offset += segment * 16 ;

	return segmentArray[ segmentNumber ] [ offset ] ;
}


REGISTER Memory::read( REGISTER segment , REGISTER offset , int ByteNumber )
{

	int segmentNumber = segment & 983040 ;

	REGISTER value  = 0;
	offset += segment * 16 ;
	
	value += segmentArray[ segmentNumber ] [ offset ] ;

	offset++;

	value += segmentArray[ segmentNumber ] [ offset ] * 256 ; 

	return value ;
}
