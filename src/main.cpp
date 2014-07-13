//created by Amir Mehrabian mehhossein@gmail.com

#include<iostream>
using namespace std ;

#include "flag.h"
#include "memory.h"
#include "cpu.h"


int main()
{
	Memory mem( 10 );

	mem.prepareSegment( 0 );

	mem.loadToSegment( 0 , 256 , "f:\\n.com" );

	CPU cpu( mem ) ;

	cpu.start();


	return 0;

}
