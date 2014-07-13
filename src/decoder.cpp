//created by Amir Mehrabia mehhossein@gmail.com
#include "decoder.h"
#include "memory.h"
#include "cpu.h"

void Decoder::setCPU( CPU* cpu )
{
	_cpu = cpu ;
}


void Decoder::decodeOpcode( int &row , int &column )
{
	opcode = _cpu->mem.read( _cpu->cs , _cpu->ip );
	column = opcode & 15 ;
	row = opcode & 240 ;
	row = row >> 4 ;
	_cpu->ip++;

}


void Decoder::decodeModeRM( int &G , int &E , int &addressingMode , int &segment )
{
	mode = _cpu->mem.read( _cpu->cs , _cpu->ip ) ;
	
	_cpu->ip++ ;

	int address = mode & 192 ;

	address = address >> 6 ; 

	G = mode & 56 ;

	G = G >> 3 ;


	if( address == 3 )
	{
		E = mode & 7 ;
		addressingMode = 0 ;//means register addressing 
		return ;
	}
	else 
		addressingMode = 1 ;
	


	E = mode & 7 ;
	segment = 0 ;//0 means ds

	int disp = 0 ;


	if( address == 0 )
		disp = 0 ;
	else
		if( address == 1 )
		{
			disp =  _cpu->mem.read( _cpu->cs , _cpu->ip ) ;
			_cpu->ip++ ;
		}
		else
		{
			disp =  _cpu->mem.read( _cpu->cs , _cpu->ip , 2) ;
			_cpu->ip +=2 ;
		}



		switch ( E )
		{
		case 0 :
			E = _cpu->Gregs.bx  + _cpu->si + disp;
			break ;
		case 1 :
			E = _cpu->Gregs.bx  + _cpu->di + disp ;
			break ;
		case 2 :
			E = _cpu->bp  + _cpu->si + disp;
			segment = 1;//means ss
			break ;
		case 3:
			E = _cpu->bp  + _cpu->di + disp;
			segment = 1;
			break ;
		case 4:
			E = _cpu->si + disp;
			break ;
		case 5 :
			E = _cpu->di + disp;
			break ;
		case 6:
			if( address == 0 )
			{
				E = _cpu->mem.read( _cpu->cs , _cpu->ip , 2) ;
				_cpu->ip += 2 ;
			}
			else
				E = _cpu->bp + disp;

			break ;
		case 7:
			E = _cpu->Gregs.bx + disp;
			break ;
		}
}




