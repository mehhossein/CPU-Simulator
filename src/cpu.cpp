//created by Amir Mehrabian mehhossein@gmail.com
#include "cpu.h"
#include <iostream>
using namespace std;

CPU::CPU( Memory &memory )
:mem( memory ) 
{
	cs = ds = es = 0;
	ss = 65535 ;
	sp = 0 ;
	ip = 256;

	int i ;
	decoder.setCPU( this );

	for( i = 0 ; i< 6 ; i++ )
		instructions[0][i] = add ;
	for( i = 0 ; i < 6 ; i++ )
		instructions[1][i] = adc ;
/*
	for( i = 0 ; i < 6 ; i++ )
		instructions[2][i] = and ;
	for( i = 0 ; i < 6 ; i++ )
		instructions[3][i] = xor ;
	for( i = 8 ; i < 14 ; i++ )
		instructions[0][i] = or ;
*/
	for( i = 8 ; i < 14 ; i++ )
		instructions[1][i] = sbb ;
	for( i = 8 ; i < 14 ; i++ )
		instructions[2][i] = sub ;
	for( i = 8 ; i < 14 ; i++ )
		instructions[3][i] = cmp ;
	for( i = 0 ; i < 8 ; i++ )
		instructions[4][i] = inc ;
	for( i = 8 ; i < 16 ; i++ )
		instructions[4][i] = DEC ;
	for( i = 0 ; i < 8 ; i++ )
		instructions[5][i] = push ;
	for( i = 8 ; i < 16 ; i++ )
		instructions[5][i] = pop ;
	instructions[6][0] = pusha ;
	instructions[6][1] = popa ;
	instructions[6][8] = pushw ;
	instructions[6][10] = pushb ;
	for( i = 0 ; i < 16 ; i++ )
		instructions[7][i] = jumpShort ;
	for( i = 4 ; i < 6 ; i++ )
		instructions[8][i] = test ;
	for( i = 6 ; i < 8 ; i++ )
		instructions[8][i] = xchg ;
	for( i = 8 ; i < 12 ; i++ )
		instructions[8][i] = mov ;
	instructions[9][0] = nop ;
	for( i = 1 ; i < 8 ; i++ )
		instructions[9][i] = xchgAx ;
	instructions[9][8] = cbw ;
	instructions[9][9] = cwd ;
	instructions[9][12] = pushf ;
	instructions[9][13] = popf ;
	instructions[9][14] = sahf ;
	instructions[9][15] = lahf ;
	for( i = 0 ; i < 4 ; i++ )
		instructions[10][i] = movao ;
	for( i = 8 ; i < 10 ; i++ )
		instructions[10][i] = testal ;
	for( i = 0 ; i < 8 ; i++ )
		instructions[11][i] = movb ;
	for( i = 8 ; i < 16 ; i++ )
		instructions[11][i] = movw ;
	for( i = 2 ; i < 4 ; i++ )
		instructions[12][i] = retn ;
	for( i = 6 ; i < 8 ; i++ )
		instructions[12][i] = move ;
	instructions[12][9] = leave ;
	for( i = 10 ; i < 12 ; i++ )
		instructions[12][i] = retf ;
	instructions[13][7] = xlat ;
	instructions[15][5] = cmc ;
	instructions[15][8] = clc ;
	instructions[15][9] = stc ;
	instructions[15][10] = cli ;
	instructions[15][11] = sti ;
	instructions[15][12] = cld ;
	instructions[15][13] = setd ;
	instructions[6][11] = imul ;
	instructions[6][19] = imul ;
	instructions[2][7] = daa ;
	instructions[2][15] = das ;
	instructions[3][7] = aaa ;
	instructions[3][15] = aas ;
	instructions[6][2] = bound ;
	instructions[8][13] = lea ;
	instructions[8][15] = pope ;
	for( i = 4 ; i < 6 ; i++ )
		instructions[10][i] = movs ;
	for( i = 6 ; i < 8 ; i++ )
		instructions[10][i] = cmps ;
	instructions[12][8] = enter ;
	for( i = 10 ; i < 12 ; i++ )
		instructions[10][i] = stos ;
	for( i = 12 ; i < 14 ; i++ )
		instructions[10][i] = lods ;
	for( i = 14 ; i < 16 ; i++ )
		instructions[10][i] = scas ;
	instructions[12][4] = aam ;
	instructions[12][5] = aad ;
	for( i = 0 ; i < 4 ; i++ )
		instructions[14][i] = loop ;
	instructions[14][8] = call ;
	for( i = 10 ; i < 13 ; i++ )
		instructions[14][i] = jmp ;
	instructions[15][2] = repne ;	
	instructions[15][3] = rep ;
	instructions[15][4] = hlt ;
	instructions[8][0] = group1b ;
	instructions[15][1] = terminate ;
	instructions[8][1] = group1w ;
	instructions[8][3] = group1w ;
	instructions[15][14] = group4 ;

	finished = false;

	Gregs.ax=2;
	Gregs.bx=2;

}

void add( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 + oprand2 ;
		haveMode = false ;

		cpu.ip++ ;
		cpu.Gregs.al = result ;
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 + oprand2 ;

		cpu.ip += 2 ;
		cpu.Gregs.ax = result ;
		haveMode = false ;
	}


	result = oprand1 + oprand2 ;

	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb += *gb;
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev += *gv;
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb += *eb;
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv += *ev;
			break;

		}

		result = oprand1 + oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E );
					oprand1 = *gb ;
					*gb += cpu.mem.read( cpu.ds , E );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E );
					oprand1 = *gb ;
					*gb += cpu.mem.read( cpu.ss , E );
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = *gv ;
					*gv += cpu.mem.read( cpu.ds , E , 2 );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = *gv ;
					*gv += cpu.mem.read( cpu.ss , E  ,2 );
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte += *gb ;
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte += *gb ;
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word += *gv ;
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word += *gv ;
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 + oprand2 ;
		}
	}


	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rb )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();

		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();
			

		if( (o1 & 128 )== (o2 & 128 ) )
			if( (o1 & 128) == (rb & 128) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rw )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();
		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();

		if( (ow1 & 32768 ) == (ow2 & 32768 ) )
			if( (ow1 & 32768 ) == (rw & 32768) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	}
}



void CPU::printStatus()
{
	cout<<" AX = "<<Gregs.ax<<" AL = "<<static_cast<int>( Gregs.al )<<" AH = "<<static_cast<int>(Gregs.ah)<<endl; 
	cout<<" CX = "<<Gregs.cx<<" CL = "<<static_cast<int>(Gregs.cl)<<" CH = "<<static_cast<int>(Gregs.ch)<<endl;
	cout<<" DX = "<<Gregs.dx<<" DL = "<<static_cast<int>(Gregs.dl)<<" DH = "<<static_cast<int>(Gregs.dh)<<endl;
	cout<<" BX = "<<Gregs.bx<<" BL = "<<static_cast<int> (Gregs.bl)<<" BH = "<<static_cast<int>(Gregs.bh)<<endl;


	cout<<" CS = "<<cs<<endl; 
	cout<<" DS = "<<ds<<endl;
	cout<<" ES = "<<es<<endl;
	cout<<" SS = "<<ss<<endl;


	cout<<" SI = "<<si<<endl;
	cout<<" DI = "<<di<<endl;
	cout<<" BP = "<<bp<<endl;
	cout<<" SP = "<<sp<<endl;

	cout<<" IP = "<<ip<<endl;


	if( flag.getCF() )
		cout<< " CF = 1"<<endl;
	else
		cout<< " CF = 0"<<endl;

	if( flag.getPF() )
		cout<< " PF = 1"<<endl;
	else
		cout<< " PF = 0"<<endl;

	if( flag.getAF() )
		cout<< " AF = 1"<<endl;
	else
		cout<< " AF = 0"<<endl;

	if( flag.getZF() )
		cout<< " ZF = 1"<<endl;
	else
		cout<< " ZF = 0"<<endl;


	if( flag.getSF() )
		cout<< " SF = 1"<<endl;
	else
		cout<< " SF = 0"<<endl;


	if( flag.getTF() )
		cout<< " TF = 1"<<endl;
	else
		cout<< " TF = 0"<<endl;

	if( flag.getIF() )
		cout<< " IF = 1"<<endl;
	else
		cout<< " IF = 0"<<endl;


	if( flag.getDF() )
		cout<< " DF = 1"<<endl;
	else
		cout<< " DF = 0"<<endl;

	if( flag.getOF() )
		cout<< " OF = 1"<<endl;
	else
		cout<< " OF = 0"<<endl;

	cout<<"----------------------------------------------------------------"<<endl;


}

void terminate(unsigned char secondNibble , CPU &cpu)
{
	cpu.finished = true ;
}

void CPU ::start()
{
	sp -= 2;
	REGISTER temp = 0;
	unsigned char c = 241 ;
	mem.write( ss , sp , temp );
	mem.write( cs , 0 ,  c );
	
	while( !finished )
	{
		decoder.decodeOpcode( instructionRow , instructionColumn );
		instructions[ instructionRow ] [instructionColumn]( instructionColumn , *this );
		printStatus();
	}
}


	
bool CPU::checkParity( unsigned char byte )
{
	unsigned char mask = 1 ;
	int numberOfOnes = 0;

	for( int i = 0 ; i < 8 ; i++ )
	{
		if( mask & byte )
			numberOfOnes++;

		mask <<= 1;
	}

	if( ( numberOfOnes % 2 ) == 0 )
		return true ;
	else
		return false ;
}


bool CPU::checkParity( REGISTER word )
{
	REGISTER mask = 1 ;
	int numberOfOnes = 0;

	for( int i = 0 ; i < 16 ; i++ )
	{
		if( mask & word )
			numberOfOnes++;

		mask <<= 1;
	}

	if( ( numberOfOnes % 2 ) == 0 )
		return true ;
	else
		return false ;
}


bool CPU ::checkAf( REGISTER reg1 , REGISTER reg2 )
{

	unsigned char c1 , c2 ;
	c1 = reg1 & 15;
	c2 = reg2 & 15;
	
	if( c1 + c2 > 15 )
		return true ;
	else
		return false ;
}


void adc( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 + oprand2 + cpu.flag.getCF() ;
		haveMode = false ;

		cpu.ip++ ;
		cpu.Gregs.al = result ;
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 + oprand2 + cpu.flag.getCF();

		cpu.ip += 2 ;
		cpu.Gregs.ax = result ;
		haveMode = false ;
	}


	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb += *gb + cpu.flag.getCF();
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev += *gv + cpu.flag.getCF();
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb += *eb + cpu.flag.getCF();
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv += *ev + cpu.flag.getCF();
			break;

		}

		result = oprand1 + oprand2 + cpu.flag.getCF();

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E );
					oprand2 = *gb ;
					*gb += cpu.mem.read( cpu.ds , E ) + cpu.flag.getCF();
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E );
					oprand2 = *gb ;
					*gb += cpu.mem.read( cpu.ss , E ) + cpu.flag.getCF();
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E , 2 );
					oprand2 = *gv ;
					*gv += cpu.mem.read( cpu.ds , E , 2 ) + cpu.flag.getCF();
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E , 2 );
					oprand2 = *gv ;
					*gv += cpu.mem.read( cpu.ss , E  ,2 ) + cpu.flag.getCF();
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte += *gb + cpu.flag.getCF();
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte += *gb + cpu.flag.getCF();
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word += *gv + cpu.flag.getCF();
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word += *gv + cpu.flag.getCF();
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 + oprand2 + cpu.flag.getCF();
		}
	}


	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rb )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();

		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();
			

		if( (o1 & 128 )== (o2 & 128 ) )
			if( (o1 & 128) == (rb & 128) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rw )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();
		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();

		if( (ow1 & 32768 ) == (ow2 & 32768 ) )
			if( (ow1 & 32768 ) == (rw & 32768) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	}
}


void andd( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 & oprand2 ;
		haveMode = false ;

		cpu.ip++ ;
		cpu.Gregs.al = result ;
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 & oprand2 ;

		cpu.ip += 2 ;
		cpu.Gregs.ax = result ;
		haveMode = false ;
	}



	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb &= *gb;
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev &= *gv;
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb &= *eb;
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv &= *ev;
			break;

		}

		result = oprand1 & oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E );
					oprand2 = *gb ;
					*gb &= cpu.mem.read( cpu.ds , E );
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E );
					oprand2 = *gb ;
					*gb &= cpu.mem.read( cpu.ss , E );
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E , 2 );
					oprand2 = *gv ;
					*gv &= cpu.mem.read( cpu.ds , E , 2 );
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E , 2 );
					oprand2 = *gv ;
					*gv &= cpu.mem.read( cpu.ss , E  ,2 );
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte &= *gb ;
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte &= *gb ;
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word &= *gv ;
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word &= *gv ;
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 & oprand2 ;
		}
	}

	cpu.flag.clearCF();
	cpu.flag.clearOF();
	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

	

		break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		break;
	}
}



void xorr( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 ^ oprand2 ;
		haveMode = false ;

		cpu.ip++ ;
		cpu.Gregs.al = result ;
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 ^ oprand2 ;

		cpu.ip += 2 ;
		cpu.Gregs.ax = result ;
		haveMode = false ;
	}



	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb ^= *gb;
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev ^= *gv;
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb ^= *eb;
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv ^= *ev;
			break;

		}

		result = oprand1 ^ oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E );
					oprand2 = *gb ;
					*gb ^= cpu.mem.read( cpu.ds , E );
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E );
					oprand2 = *gb ;
					*gb ^= cpu.mem.read( cpu.ss , E );
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E , 2 );
					oprand2 = *gv ;
					*gv ^= cpu.mem.read( cpu.ds , E , 2 );
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E , 2 );
					oprand2 = *gv ;
					*gv ^= cpu.mem.read( cpu.ss , E  ,2 );
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte ^= *gb ;
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte ^= *gb ;
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word ^= *gv ;
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word ^= *gv ;
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 ^ oprand2 ;
		}
	}

	cpu.flag.clearCF();
	cpu.flag.clearOF();
	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

	

		break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		break;
	}
}



void orr( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;
	secondNibble -= 8 ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 | oprand2 ;
		haveMode = false ;

		cpu.ip++ ;
		cpu.Gregs.al = result ;
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 | oprand2 ;

		cpu.ip += 2 ;
		cpu.Gregs.ax = result ;
		haveMode = false ;
	}



	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb |= *gb;
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev |= *gv;
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb |= *eb;
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv |= *ev;
			break;

		}

		result = oprand1 | oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E );
					oprand2 = *gb ;
					*gb |= cpu.mem.read( cpu.ds , E );
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E );
					oprand2 = *gb ;
					*gb |= cpu.mem.read( cpu.ss , E );
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand1 = cpu.mem.read( cpu.ds , E , 2 );
					oprand2 = *gv ;
					*gv |= cpu.mem.read( cpu.ds , E , 2 );
				}
				else
				{
					oprand1 = cpu.mem.read( cpu.ss , E , 2 );
					oprand2 = *gv ;
					*gv |= cpu.mem.read( cpu.ss , E  ,2 );
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte |= *gb ;
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte |= *gb ;
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word |= *gv ;
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word |= *gv ;
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 | oprand2 ;
		}
	}

	cpu.flag.clearCF();
	cpu.flag.clearOF();
	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

	

		break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		break;
	}
}


void sub( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	secondNibble -= 8 ;
	bool haveMode = true ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 - oprand2 ;
		haveMode = false ;

		cpu.ip++ ;
		cpu.Gregs.al = result ;
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 - oprand2 ;

		cpu.ip += 2 ;
		cpu.Gregs.ax = result ;
		haveMode = false ;
	}


	

	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb -= *gb;
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev -= *gv;
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb -= *eb;
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv -= *ev;
			break;

		}

		result = oprand1 - oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E );
					oprand1 = *gb ;
					*gb -= cpu.mem.read( cpu.ds , E );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E );
					oprand1 = *gb ;
					*gb -= cpu.mem.read( cpu.ss , E );
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = *gv ;
					*gv -= cpu.mem.read( cpu.ds , E , 2 );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = *gv ;
					*gv -= cpu.mem.read( cpu.ss , E  ,2 );
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte -= *gb ;
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte -= *gb ;
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word -= *gv ;
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word -= *gv ;
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 - oprand2 ;
		}
	}


	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rb )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();

		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();
			

		if( (o1 & 128 ) != (o2 & 128 ) )
			if( (o1 & 128) == (rb & 128) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rw )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();
		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();

		if( (ow1 & 32768 ) != (ow2 & 32768 ) )
			if( (ow1 & 32768 ) == (rw & 32768) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	}
}


void sbb( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	secondNibble -= 8 ;
	bool haveMode = true ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 - oprand2 - cpu.flag.getCF();
		haveMode = false ;

		cpu.ip++ ;
		cpu.Gregs.al = result ;
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 - oprand2 - cpu.flag.getCF() ;

		cpu.ip += 2 ;
		cpu.Gregs.ax = result ;
		haveMode = false ;
	}




	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb -= *gb - cpu.flag.getCF();
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev -= *gv - cpu.flag.getCF();
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb -= *eb - cpu.flag.getCF();
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv -= *ev - cpu.flag.getCF();
			break;

		}

		result = oprand1 - oprand2 - cpu.flag.getCF();

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E );
					oprand1 = *gb ;
					*gb -= cpu.mem.read( cpu.ds , E ) - cpu.flag.getCF();
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E );
					oprand1 = *gb ;
					*gb -= cpu.mem.read( cpu.ss , E ) - cpu.flag.getCF();
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = *gv ;
					*gv -= cpu.mem.read( cpu.ds , E , 2 ) - cpu.flag.getCF();
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = *gv ;
					*gv -= cpu.mem.read( cpu.ss , E  ,2 ) - cpu.flag.getCF();
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte -= *gb - cpu.flag.getCF();
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte -= *gb - cpu.flag.getCF();
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word -= *gv - cpu.flag.getCF();
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word -= *gv - cpu.flag.getCF();
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 - oprand2 ;
		}
	}


	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rb )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();

		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();
			

		if( (o1 & 128 ) != (o2 & 128 ) )
			if( (o1 & 128) == (rb & 128) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rw )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();
		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();

		if( (ow1 & 32768 ) != (ow2 & 32768 ) )
			if( (ow1 & 32768 ) == (rw & 32768) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	}
}


void cmp( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	secondNibble -= 8 ;
	bool haveMode = true ;

	if( secondNibble == 4 )
	{
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		result = oprand1 - oprand2 ;
		haveMode = false ;
		cpu.ip++ ;
		
	}

	if( secondNibble == 5 )
	{
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		result = oprand1 - oprand2 ;

		cpu.ip += 2 ;
		haveMode = false ;
	}


	

	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
		
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
		
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			
			break;

		}

		result = oprand1 - oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E );
					oprand1 = *gb ;
					
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E );
					oprand1 = *gb ;
					
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = *gv ;
					
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = *gv ;
					
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
				
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 - oprand2 ;
		}
	}


	switch( secondNibble )
	{
	case 0:
	case 2:
	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rb )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();

		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();
			

		if( (o1 & 128 ) != (o2 & 128 ) )
			if( (o1 & 128) == (rb & 128) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	case 1 :
	case 3 :
	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		if( result - rw )
			cpu.flag.setCF();
		else
			cpu.flag.clearCF();
		if( cpu.checkAf( oprand1 , oprand2 ) )
			cpu.flag.setAF();
		else
			cpu.flag.clearAF();

		if( (ow1 & 32768 ) != (ow2 & 32768 ) )
			if( (ow1 & 32768 ) == (rw & 32768) )
				cpu.flag.clearOF();
			else
				cpu.flag.setOF();

			break;
	}
}


void inc( unsigned char secondNibble , CPU &cpu )
{
	int oprand1 , oprand2 , result ;
	oprand2 = 1 ;

	
	
	switch( secondNibble )
	{
	case 0:
		oprand1 = cpu.Gregs.ax ;
		cpu.Gregs.ax++;
		break;
	case 1:
		oprand1 = cpu.Gregs.cx ;
		cpu.Gregs.cx++;
		break;
	case 2:
		oprand1 = cpu.Gregs.dx ;
		cpu.Gregs.dx++;
		break;
	case 3:
		oprand1 = cpu.Gregs.bx ;
		cpu.Gregs.bx ++;
		break;
	case 4:
		oprand1 = cpu.sp ;
		cpu.sp ++;
		break;
	case 5:
		oprand1 = cpu.bp ;
		cpu.bp ++;
		break;
	case 6:
		oprand1 = cpu.si ;
		cpu.si ++;
		break;
	case 7:
		oprand1 = cpu.di ;
		cpu.di ++;
		break;
	}

		result = oprand1 + oprand2 ;

	

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	if( (ow1 & 32768 ) == (ow2 & 32768 ) )
		if( (ow1 & 32768 ) == (rw & 32768) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();
}

void DEC( unsigned char secondNibble , CPU &cpu )
{
	int oprand1 , oprand2 , result ;
	oprand2 = 1 ;
	
	secondNibble -= 8;
	
	
	switch( secondNibble )
	{
	case 0:
		oprand1 = cpu.Gregs.ax ;
		cpu.Gregs.ax--;
		break;
	case 1:
		oprand1 = cpu.Gregs.cx ;
		cpu.Gregs.cx--;
		break;
	case 2:
		oprand1 = cpu.Gregs.dx ;
		cpu.Gregs.dx--;
		break;
	case 3:
		oprand1 = cpu.Gregs.bx ;
		cpu.Gregs.bx --;
		break;
	case 4:
		oprand1 = cpu.sp ;
		cpu.sp --;
		break;
	case 5:
		oprand1 = cpu.bp ;
		cpu.bp --;
		break;
	case 6:
		oprand1 = cpu.si ;
		cpu.si --;
		break;
	case 7:
		oprand1 = cpu.di ;
		cpu.di --;
		break;
	}

		result = oprand1 - oprand2 ;

	

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	if( (ow1 & 32768 ) == (ow2 & 32768 ) )
		if( (ow1 & 32768 ) == (rw & 32768) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();
}



void push( unsigned char secondNibble , CPU &cpu )
{
	cpu.sp -= 2 ;

	switch( secondNibble )
	{

	case 0 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.Gregs.ax );
		break;
	case 1 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.Gregs.cx );
		break;
	case 2 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.Gregs.dx );
		break;
	case 3 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.Gregs.bx );
		break;
	case 4 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.sp );
		break;
	case 5 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.bp );
		break;
	case 6 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.si );
		break;
	case 7 :
		cpu.mem.write( cpu.ss , cpu.sp , cpu.di );
		break;
	}

	
}


void pop( unsigned char secondNibble , CPU &cpu )
{

	secondNibble -= 8 ;

	switch( secondNibble )
	{

	case 0 :
		cpu.Gregs.ax = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	case 1 :
		cpu.Gregs.cx = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	case 2 :
		cpu.Gregs.dx = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	case 3 :
		cpu.Gregs.bx = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	case 4 :
		cpu.sp = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	case 5 :
		cpu.bp = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	case 6 :
		cpu.si = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	case 7 :
		cpu.di = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		break;
	}

	cpu.sp += 2 ;
}

///need change for sp change done 
void pusha( unsigned char secondNibble , CPU &cpu )
{
	REGISTER tempSp = cpu.sp ;
	REGISTER temp ;
	for( unsigned char i = 0 ; i < 8 ; i++ )
	{
		if( i == 4 )
		{
			temp = cpu.si;
			cpu.si = tempSp ;
			push( 6 , cpu );
			cpu.si = temp ;
			continue ;
		}
			
		push( i , cpu );
	}
}
/////////////////////////////////////////

void popa( unsigned char secondNibble , CPU &cpu )
{
	REGISTER tempSp = cpu.sp ;
	REGISTER temp ;
	for( char i = 7 ; i >= 0 ; i-- )
	{
		if( i == 4 )
		{
			temp = cpu.si;
			pop( 6 , cpu );
			tempSp= cpu.si ;
			cpu.si = temp;
			continue ;
		}
			
		pop( i , cpu );
	}

	cpu.sp = tempSp ;
}


void pushw( unsigned char secondNibble , CPU &cpu )
{
	REGISTER temp ;
	REGISTER tempAx ;

	temp = cpu.mem.read( cpu.cs , cpu.ip , 2 );
	cpu.ip += 2;

	tempAx = cpu.Gregs.ax ;

	cpu.Gregs.ax = temp ;


	push(  0 , cpu );

	cpu.Gregs.ax = tempAx ;
}



void pushb( unsigned char secondNibble , CPU &cpu )
{
	REGISTER temp ;
	REGISTER tempAx ;

	temp = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;

	tempAx = cpu.Gregs.ax ;

	cpu.Gregs.ax = temp ;


	push(  0 , cpu );

	cpu.Gregs.ax = tempAx ;
}


void jumpShort( unsigned char secondNibble , CPU &cpu )
{
	char disp = 0  ;
	disp = static_cast< signed >( cpu.mem.read( cpu.cs , cpu.ip ) ) ;
	cpu.ip++;

	switch( secondNibble )
	{
	case 0:
		if( cpu.flag.getOF() )
			cpu.ip += disp ;
		break;
	case 1:
		if( ! cpu.flag.getOF() )
			cpu.ip += disp ;
		break;
	case 2:
		if( cpu.flag.getCF() )
			cpu.ip += disp ;
		break;
	case 3:
		if( !cpu.flag.getCF() )
			cpu.ip += disp ;
		break;
	case 4:
		if( cpu.flag.getZF() )
			cpu.ip += disp ;
		break;
	case 5:
		if( !cpu.flag.getZF() )
			cpu.ip += disp ;
		break;
	case 6:
		if( cpu.flag.getZF()  || cpu.flag.getCF() )
			cpu.ip += disp ;
		break;
	case 7:
		if( !cpu.flag.getCF()  && !cpu.flag.getZF() )
			cpu.ip += disp ;
		break;
	case 8:
		if( cpu.flag.getSF() )
			cpu.ip += disp ;
		break;
	case 9:
		if( !cpu.flag.getSF() )
			cpu.ip += disp ;
		break;
	case 10:
		if( cpu.flag.getPF() )
			cpu.ip += disp ;
		break;
	case 11:
		if( !cpu.flag.getPF() )
			cpu.ip += disp ;
		break;
	case 12:
		if( ( cpu.flag.getSF() / 4 ) !=  ( cpu.flag.getOF() / 4096 )  )
			cpu.ip += disp ;
		break;
	case 13:
		if( ( cpu.flag.getSF() / 4 ) ==  ( cpu.flag.getOF() / 4096 ))
			cpu.ip += disp ;
		break;
	case 14:
		if( ( ( cpu.flag.getSF() / 4 ) !=  ( cpu.flag.getOF() / 4096 )  ) || cpu.flag.getZF() )
			cpu.ip += disp ;
		break;
	case 15:
		if( ( ( cpu.flag.getSF() / 4 ) ==  ( cpu.flag.getOF() / 4096 ) ) && !cpu.flag.getZF()  )
			cpu.ip += disp ;
		break;
	}
}
	

void test( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;

	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );

	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;

	switch( secondNibble )
	{
	case 4:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 5:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}
	
		

	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 4:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
		case 5:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 4:
			oprand1 = *eb ;
			oprand2 = *gb ;
			
			break;
		case 5:
			oprand1 = *ev ;
			oprand2 = *gv ;
		
			break;

		}

		result = oprand1 & oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			
			case 4 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;

					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					
					oprand2 = *gb ;
				}
				break;
			case 5:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					
					oprand2 = *gv ;
				}
				break;
				
				
			}
			result = oprand1 & oprand2 ;
		}
	}


	cpu.flag.clearCF();
	cpu.flag.clearOF();
	switch( secondNibble )
	{

	case 4:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

	

		break;

	case 5 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		break;
	}
}




void xchg( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;

	secondNibble -= 2 ;

	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );

	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;

	switch( secondNibble )
	{
	case 4:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 5:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}
	
		

	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 4:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
		case 5:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 4:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*gb = oprand1 ;
			*eb = oprand2 ;
			
			break;
		case 5:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*gv = oprand1 ;
			*ev = oprand2 ;
		
			break;

		}

		

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			
			case 4 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					oprand2 = *gb ;
					byte = oprand2 ;
					*gb = oprand1 ;
					cpu.mem.write( cpu.ds , E , byte );
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					
					oprand2 = *gb ;
					byte = oprand2 ;
					*gb = oprand1 ;
					cpu.mem.write( cpu.ss , E , byte );
				}
				break;
			case 5:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					
					oprand2 = *gv ;
					word = oprand2 ;
					*gv = oprand1 ;
					cpu.mem.write( cpu.ds , E , word );
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					
					oprand2 = *gv ;
					word = oprand2 ;
					*gv = oprand1 ;
					cpu.mem.write( cpu.ss , E , word );
				}
				break;
				
				
			}
			
		}
	}

}



void mov( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;
	secondNibble -= 8 ;

	

	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;


	switch( secondNibble )
	{
	case 0:
	case 2:
		switch( G )
		{
		case 0 :
			gb = &( cpu.Gregs.al );
			break ;
		case 1:
			gb = &( cpu.Gregs.cl );
			break ;
		case 2:
			gb = &( cpu.Gregs.dl );
			break ;
		case 3:
			gb = &( cpu.Gregs.bl );
			break ;
		case 4:
			gb = &( cpu.Gregs.ah );
			break ;
		case 5:
			gb = &( cpu.Gregs.ch );
			break ;
		case 6:
			gb = &( cpu.Gregs.dh );
			break ;
		case 7:
			gb = &( cpu.Gregs.bh );
			break ;
		}

		break;
	case 1:
	case 3:
		switch( G )
		{
		case 0 :
			gv = &( cpu.Gregs.ax );
			break ;
		case 1:
			gv = &( cpu.Gregs.cx );
			break ;
		case 2:
			gv = &( cpu.Gregs.dx );
			break ;
		case 3:
			gv = &( cpu.Gregs.bx );
			break ;
		case 4:
			gv = &( cpu.sp );
			break ;
		case 5:
			gv = &( cpu.bp );
			break ;
		case 6:
			gv = &( cpu.si );
			break ;
		case 7:
			gv = &( cpu.di );
			break ;
		}
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 0:
		case 2:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
			case 1:
			case 3:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 0:
			oprand1 = *eb ;
			oprand2 = *gb ;
			*eb = *gb;
			break;
		case 1:
			oprand1 = *ev ;
			oprand2 = *gv ;
			*ev = *gv;
			break;
		case 2:
			oprand1 = *gb ;
			oprand2 = *eb ;
			*gb = *eb;
			break;
		case 3:
			oprand1 = *gv ;
			oprand2 = *ev ;
			*gv = *ev;
			break;

		}

		

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 2 :
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E );
					oprand1 = *gb ;
					*gb = cpu.mem.read( cpu.ds , E );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E );
					oprand1 = *gb ;
					*gb = cpu.mem.read( cpu.ss , E );
				}
				break;
			case 3:
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = *gv ;
					*gv = cpu.mem.read( cpu.ds , E , 2 );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = *gv ;
					*gv = cpu.mem.read( cpu.ss , E  ,2 );
				}
				break;
			case 0 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					oprand1 = byte ;
					byte = *gb ;
					cpu.mem.write( cpu.ds , E , byte );
					oprand2 = *gb ;
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					oprand1 = byte ;
					byte = *gb ;
					cpu.mem.write( cpu.ss , E , byte );
					oprand2 = *gb ;
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = word ;
					word = *gv ;
					cpu.mem.write( cpu.ds , E , word );
					oprand2 = *gv ;
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = word ;
					word = *gv ;
					cpu.mem.write( cpu.ss , E , word );
					oprand2 = *gv ;
				}
				break;
				
				
			}
			
		}
	}
}


void nop( unsigned char secondNibble , CPU &cpu )
{
	cout<<"NOP"<<endl;
}


void xchgAx( unsigned char secondNibble , CPU &cpu )
{
	REGISTER temp = cpu.Gregs.ax ;

	switch( secondNibble )
	{
	
	case 1:
		cpu.Gregs.ax = cpu.Gregs.cx ;
		cpu.Gregs.cx = temp ;
		
		break;
	case 2:
		cpu.Gregs.ax = cpu.Gregs.dx ;
		cpu.Gregs.dx = temp ;
		break;
	case 3:
		cpu.Gregs.ax = cpu.Gregs.bx ;
		cpu.Gregs.bx = temp ;
		break;
	case 4:
		cpu.Gregs.ax = cpu.sp ;
		cpu.sp = temp ;
		break;
	case 5:
		cpu.Gregs.ax = cpu.bp ;
		cpu.bp = temp ;
		break;
	case 6:
		cpu.Gregs.ax = cpu.si ;
		cpu.si = temp ;
		break;
	case 7:
		cpu.Gregs.ax = cpu.di ;
		cpu.di = temp ;
		break;
	
	}
}

void cbw( unsigned char secondNibble , CPU &cpu )
{
	unsigned char mask = 128;

	if( cpu.Gregs.al & mask )
		cpu.Gregs.ah = 255;
	else
		cpu.Gregs.ah = 0 ;
}

void cwd( unsigned char secondNibble , CPU &cpu )
{
	REGISTER mask = 32768;

	if( cpu.Gregs.ax & mask )
		cpu.Gregs.dx = 65535 ;
	else
		cpu.Gregs.dx = 0 ;
}

void pushf( unsigned char secondNibble , CPU &cpu )
{

	cpu.sp -= 2 ;

	cpu.mem.write( cpu.ss , cpu.sp , cpu.flag );
}

void popf( unsigned char secondNibble , CPU &cpu )
{
	REGISTER temp = cpu.mem.read( cpu.ss , cpu.sp , 2 );
	cpu.sp += 2 ;

	cpu.flag = temp ;
}


void sahf( unsigned char secondNibble , CPU &cpu )
{
	cpu.Gregs.ah = cpu.flag.getfb();
}


void lahf( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.setfb( cpu.Gregs.ah );
}


void movao( unsigned char secondNibble , CPU &cpu )
{
	unsigned char baddress;
	REGISTER waddress;
	switch( secondNibble )
	{
	case 0 :
	case 2:
		baddress = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip++;
		break;
	case 1:
	case 3:
		waddress = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip++;
		break;
	}

	switch( secondNibble )
	{
	case 0:
		cpu.Gregs.al = cpu.mem.read( cpu.ds , baddress );
		break;
	case 1:
		cpu.Gregs.ax = cpu.mem.read(  cpu.ds , waddress , 2 );
		break ;
	case 2:
		cpu.mem.write( cpu.ds , baddress , cpu.Gregs.al );
		break;
	case 3:
		cpu.mem.write( cpu.ds , waddress , cpu.Gregs.ax );
		break;
	}
}


void testal( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;

	switch( secondNibble )
	{
	case 8:
		oprand1 = cpu.Gregs.al ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
		break;
	case 9:
		oprand1 = cpu.Gregs.ax ;
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2 ;
		break;
	}


	result = oprand1 & oprand2 ;


	cpu.flag.clearCF();
	cpu.flag.clearOF();
	switch( secondNibble )
	{

	case 8:
	
		unsigned char o1 , o2 , rb ;
		o1 = oprand1;
		o2 = oprand2;
		rb = result ;

		if( rb )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();


		if( cpu.checkParity( rb ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();


		if( rb & 128 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

	

		break;

	case 9 :

		REGISTER rw , ow1 , ow2 ;
		ow1 = oprand1;
		ow2 = oprand2;
		rw = result ;
		
		if( rw )
			cpu.flag.clearZF();
		else
			cpu.flag.setZF();

		if( cpu.checkParity( rw ) )
			cpu.flag.setPF();
		else
			cpu.flag.clearPF();

		if( rw & 32768 )
			cpu.flag.setSF();
		else
			cpu.flag.clearSF();

		break;
	}
}

void movb( unsigned char secondNibble , CPU &cpu )
{
	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;

	switch( secondNibble )
	{
	case 0 :
		cpu.Gregs.al = byte ;
		break;
	case 1 :
		cpu.Gregs.cl = byte ;
		break;
	case 2 :
		cpu.Gregs.dl = byte ;
		break;
	case 3 :
		cpu.Gregs.bl = byte ;
		break;
	case 4 :
		cpu.Gregs.ah = byte ;
		break;
	case 5 :
		cpu.Gregs.ch = byte ;
		break;
	case 6 :
		cpu.Gregs.dh = byte ;
		break;
	case 7 :
		cpu.Gregs.bh = byte ;
		break;
	}
}



void movw( unsigned char secondNibble , CPU &cpu )
{
	REGISTER word = cpu.mem.read( cpu.cs , cpu.ip , 2 );
	cpu.ip += 2 ;

	switch( secondNibble )
	{
	case 0 :
		cpu.Gregs.ax = word ;
		break;
	case 1 :
		cpu.Gregs.cx = word ;
		break;
	case 2 :
		cpu.Gregs.dx = word ;
		break;
	case 3 :
		cpu.Gregs.bx = word ;
		break;
	case 4 :
		cpu.sp = word ;
		break;
	case 5 :
		cpu.bp = word ;
		break;
	case 6 :
		cpu.si = word ;
		break;
	case 7 :
		cpu.di = word ;
		break;
	}
}	


void retn( unsigned char secondNibble , CPU &cpu )
{
	REGISTER reg ;
	switch( secondNibble )
	{
	case 2:
		reg = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2 ;
		cpu.ip = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2 ;
		cpu.sp += reg ;
		break;
	case 3 :
		cpu.ip = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2 ;
		break ;
	}
}

		
void move( unsigned char secondNibble , CPU &cpu )
{
	bool haveMode = true ;
	int E , G ;
	int addressingMode ;

	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );

	if( addressingMode == 0 && haveMode)
	{
		switch( secondNibble )
		{
		case 6:
			switch( E )
			{
			case 0 :
				eb = &( cpu.Gregs.al );
				break ;
			case 1:
				eb = &( cpu.Gregs.cl );
				break ;
			case 2:
				eb = &( cpu.Gregs.dl );
				break ;
			case 3:
				eb = &( cpu.Gregs.bl );
				break ;
			case 4:
				eb = &( cpu.Gregs.ah );
				break ;
			case 5:
				eb = &( cpu.Gregs.ch );
				break ;
			case 6:
				eb = &( cpu.Gregs.dh );
				break ;
			case 7:
				eb = &( cpu.Gregs.bh );
				break ;
			}
			
			break;
		case 7:
				switch( E )
				{
				case 0 :
					ev = &( cpu.Gregs.ax );
					break ;
				case 1:
					ev = &( cpu.Gregs.cx );
					break ;
				case 2:
					ev = &( cpu.Gregs.dx );
					break ;
				case 3:
					ev = &( cpu.Gregs.bx );
					break ;
				case 4:
					ev = &( cpu.sp );
					break ;
				case 5:
					ev = &( cpu.bp );
					break ;
				case 6:
					ev = &( cpu.si );
					break ;
				case 7:
					ev = &( cpu.di );
					break ;
				}
				break ;
		}


		switch( secondNibble )
		{
		case 6:

			 *eb = cpu.mem.read( cpu.cs , cpu.ip );
			 cpu.ip ++;
			
			break;
		case 7:
			*ev = cpu.mem.read( cpu.cs , cpu.ip , 2 );
			cpu. ip += 2 ;
		
			break;
		

		}

		

	}
	else
	{
		if( haveMode )
		{
			switch( secondNibble )
			{
			case 6 :
				unsigned char byte ;
				if( segment == 0 )
				{
					byte = cpu.mem.read( cpu.ds , E );
					byte = cpu.mem.read( cpu.cs , cpu.ip ) ;
					cpu.ip ++ ;
					cpu.mem.write( cpu.ds , E , byte );
					
				}			
				else
				{
					byte = cpu.mem.read( cpu.ss , E );
					byte = cpu.mem.read( cpu.cs , cpu.ip ) ;
					cpu.ip ++ ;
					cpu.mem.write( cpu.ss , E , byte );
					
				}
				break;
			case 1:
				REGISTER word ;
				if( segment == 0 )
				{
					word = cpu.mem.read( cpu.ds , E , 2 );
					word = cpu.mem.read( cpu.cs , cpu.ip  , 2 );
					cpu.ip +=2 ;
					cpu.mem.write( cpu.ds , E , word );
					
				}			
				else
				{
					word = cpu.mem.read( cpu.ss , E , 2 );
					word = cpu.mem.read( cpu.cs , cpu.ip  , 2 );
					cpu.ip +=2 ;
					cpu.mem.write( cpu.ss , E , word );
				}
				break;
				
				
			}
		
		}
	}
}

	
void leave( unsigned char secondNibble , CPU &cpu )
{
	cpu.sp = cpu.bp ;
	cpu.ip = cpu.mem.read( cpu.ss , cpu.sp , 2 );
	cpu.sp += 2;
}

void retf( unsigned char secondNibble , CPU &cpu )
{
	REGISTER reg ;
	switch( secondNibble )
	{
	case 10:
		reg = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2 ;
		cpu.ip = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2 ;
		cpu.cs = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2 ;
		cpu.sp += reg ;
		break;
	case 3 :
		cpu.ip = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2 ;
		cpu.ip = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2 ;
		break ;
	}
}


void xlat( unsigned char secondNibble , CPU &cpu )
{
	REGISTER reg = cpu.Gregs.bx  + cpu.Gregs.al ;
	cpu.Gregs.al = cpu.mem.read( cpu.ds , reg );
}

void cmc( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.complementCF();
}

void clc( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.clearCF();
}

void stc( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.setCF();
}

void cli( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.clearIF();
}

void sti( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.setIF();
}


void cld( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.clearDF();
}

void setd( unsigned char secondNibble , CPU &cpu )
{
	cpu.flag.setDF();
}

void imul( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;
	bool haveMode = true ;
	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;

	switch( G )
	{
	case 0 :
		gv = &( cpu.Gregs.ax );
		break ;
	case 1:
		gv = &( cpu.Gregs.cx );
		break ;
	case 2:
		gv = &( cpu.Gregs.dx );
		break ;
	case 3:
		gv = &( cpu.Gregs.bx );
		break ;
	case 4:
		gv = &( cpu.sp );
		break ;
	case 5:
		gv = &( cpu.bp );
		break ;
	case 6:
		gv = &( cpu.si );
		break ;
	case 7:
		gv = &( cpu.di );
		break ;
	}


	if( addressingMode == 0 && haveMode)
	{	
		switch( E )
		{
		case 0 :
			ev = &( cpu.Gregs.ax );
			break ;
		case 1:
			ev = &( cpu.Gregs.cx );
			break ;
		case 2:
			ev = &( cpu.Gregs.dx );
			break ;
		case 3:
			ev = &( cpu.Gregs.bx );
			break ;
		case 4:
			ev = &( cpu.sp );
			break ;
		case 5:
			ev = &( cpu.bp );
			break ;
		case 6:
			ev = &( cpu.si );
			break ;
		case 7:
			ev = &( cpu.di );
			break ;
		}
	
		

		short word ;
		char byte ;
		switch( secondNibble )
		{
		case 9:
			word = cpu.mem.read( cpu.cs , cpu.ip , 2 );
			cpu.ip += 2;

			oprand1 = *ev ;
			oprand2 = word ;
			*gv = *ev * word ;
			break;
		case 11:
			byte = cpu.mem.read( cpu.cs , cpu.ip );
			cpu.ip ++;
			oprand1 = *ev ;
			oprand2 = byte ;
			*gv = *ev * byte ;
			break;
	
		}

		result = oprand1 * oprand2 ;

	}
	else
	{
		if( haveMode )
		{
			short word ;
			char byte ;
			switch( secondNibble )
			{
			case 9 :
				word = cpu.mem.read( cpu.cs , cpu.ip , 2 );
				cpu.ip += 2;

				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E );
					oprand1 = word ;
					*gv = word * cpu.mem.read( cpu.ds , E );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E );
					oprand1 = word ;
					*gv = word * cpu.mem.read( cpu.ss , E );
				}
				break;
			case 11:
				byte = cpu.mem.read( cpu.cs , cpu.ip );
				cpu.ip ++;
				if( segment == 0 )
				{
					oprand2 = cpu.mem.read( cpu.ds , E , 2 );
					oprand1 = byte ;
					*gv = byte * cpu.mem.read( cpu.ds , E , 2 );
				}
				else
				{
					oprand2 = cpu.mem.read( cpu.ss , E , 2 );
					oprand1 = byte  ;
					*gv = byte * cpu.mem.read( cpu.ss , E  ,2 );
				}
				break;
			
				
				
			}
			result = oprand1 * oprand2 ;
		}
	}

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;

	if( result - rw )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();

	if( result > 32767 || result < -32768 )
		cpu.flag.setOF();
	else
		cpu.flag.clearOF(); 

}
	
void daa( unsigned char secondNibble , CPU &cpu )
{
	unsigned char nibble1 = cpu.Gregs.al & 15 ;
	unsigned char nibble2 = cpu.Gregs.al & ( 15 * 16 ) ;
	if( nibble1 > 9 )
	{
		nibble1 += 6 ;
		nibble2++;
	}
	else
	{
		if(cpu.flag.getAF() )
			nibble1 += 6 ;
	}
	if( nibble2 > 9 )
	{
		nibble2 += 6 ;
		cpu.flag.setCF();
	}
	

	cpu.Gregs.al = nibble2 * 16 + nibble1 ;
}


void das( unsigned char secondNibble , CPU &cpu )
{
	unsigned char nibble1 = cpu.Gregs.al & 15 ;
	unsigned char nibble2 = cpu.Gregs.al & ( 15 * 16 ) ;
	if( nibble1 > 9 )
	{
		nibble1 -= 6 ;
		nibble2--;
	}
	else
	{
		if(cpu.flag.getAF() )
			nibble1 -= 6 ;
	}
	if( nibble2 > 9 )
	{
		nibble2 -= 6 ;
		cpu.flag.setCF();
	}

	cpu.Gregs.al = nibble2 * 16 + nibble1 ;
}


void aaa( unsigned char secondNibble , CPU &cpu )
{
	unsigned char nibble1 = cpu.Gregs.al & 15 ;
	if( nibble1 > 9 )
	{
		nibble1 += 6 ;
		cpu.flag.setCF();
	}
	else
	{
		if(cpu.flag.getAF() )
		{
			nibble1 += 6 ;
			cpu.flag.setCF();
		}
			
	}

	cpu.Gregs.al = nibble1 ;
}

void aas( unsigned char secondNibble , CPU &cpu )
{
	unsigned char nibble1 = cpu.Gregs.al & 15 ;
	if( nibble1 > 9 )
	{
		nibble1 -= 6 ;
		cpu.flag.setCF();
	}
	else
	{
		if(cpu.flag.getAF() )
		{
			nibble1 -= 6 ;
			cpu.flag.setCF();
		}
			
	}

	cpu.Gregs.al = nibble1 ;
}


void bound( unsigned char secondNibble , CPU &cpu )
{

	REGISTER low ;
	REGISTER high ;

	bool haveMode = true ;
	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;

	switch( G )
	{
	case 0 :
		gv = &( cpu.Gregs.ax );
		break ;
	case 1:
		gv = &( cpu.Gregs.cx );
		break ;
	case 2:
		gv = &( cpu.Gregs.dx );
		break ;
	case 3:
		gv = &( cpu.Gregs.bx );
		break ;
	case 4:
		gv = &( cpu.sp );
		break ;
	case 5:
		gv = &( cpu.bp );
		break ;
	case 6:
		gv = &( cpu.si );
		break ;
	case 7:
		gv = &( cpu.di );
		break ;
	}

	if( segment = 0 )
	{
		low = cpu.mem.read( cpu.ds , E , 2 );
		high = cpu.mem.read( cpu.ds , E+2 , 2 );
	}
	else
	{
		low = cpu.mem.read( cpu.ss , E , 2 );
		high = cpu.mem.read( cpu.ss , E+2 , 2 );
	}

	if( *gv  > high || *gv < low )
		cout<<" INT6 MUST BE CALLED"<<endl;
}

void lea( unsigned char secondNibble , CPU &cpu )
{


	bool haveMode = true ;
	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	unsigned char * gb = 0 ;
	unsigned char * eb = 0 ;

	REGISTER * ev = 0;
	REGISTER * gv = 0;

	switch( G )
	{
	case 0 :
		gv = &( cpu.Gregs.ax );
		break ;
	case 1:
		gv = &( cpu.Gregs.cx );
		break ;
	case 2:
		gv = &( cpu.Gregs.dx );
		break ;
	case 3:
		gv = &( cpu.Gregs.bx );
		break ;
	case 4:
		gv = &( cpu.sp );
		break ;
	case 5:
		gv = &( cpu.bp );
		break ;
	case 6:
		gv = &( cpu.si );
		break ;
	case 7:
		gv = &( cpu.di );
		break ;
	}

	*gv = E ;
}


void pope( unsigned char secondNibble , CPU &cpu )
{

	bool haveMode = true ;
	int E , G ;
	int addressingMode ;
	int segment ;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );
	REGISTER * ev = 0;

	if( addressingMode == 0 && haveMode)
	{
		switch( E )
		{
		case 0 :
			ev = &( cpu.Gregs.ax );
			break ;
		case 1:
			ev = &( cpu.Gregs.cx );
			break ;
		case 2:
			ev = &( cpu.Gregs.dx );
			break ;
		case 3:
			ev = &( cpu.Gregs.bx );
			break ;
		case 4:
			ev = &( cpu.sp );
			break ;
		case 5:
			ev = &( cpu.bp );
			break ;
		case 6:
			ev = &( cpu.si );
			break ;
		case 7:
			ev = &( cpu.di );
			break ;
		}

		*ev = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2 ;
	}
	else
	{
		REGISTER temp = cpu.mem.read( cpu.ss , cpu.sp , 2 );
		cpu.sp += 2;

		if( segment == 0 )
		{
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
			cpu.mem.write( cpu.ss , E , temp );
	}
}

void movs( unsigned char secondNibble , CPU &cpu )
{
	 unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip - 2 );
	 char advance = 0 ;
	 if( cpu.flag.getDF() )
		 advance = -1 ;
	 else
		 advance = 1 ;

	
	 REGISTER word;
	
	 if( byte == 243 )
	 {
		 switch( secondNibble )
		 {
		 case 4 :
			 while( cpu.Gregs.cx != 0)
			 {
				 byte = cpu.mem.read( cpu.es , cpu.di );
				 cpu.mem.write( cpu.ds , cpu.si , byte );
				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 5:
			 while( cpu.Gregs.cx != 0)
			 {
				 word = cpu.mem.read( cpu.es , cpu.di , 2 );
				 cpu.mem.write( cpu.ds , cpu.si , word );
				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
	 else
	 {
		 switch( secondNibble )
		 {
		 case 4 :
			 
			 {
				 byte = cpu.mem.read( cpu.es , cpu.di );
				 cpu.mem.write( cpu.ds , cpu.si , byte );
				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 5:
			 
			 {
				 word = cpu.mem.read( cpu.es , cpu.di , 2 );
				 cpu.mem.write( cpu.ds , cpu.si , word );
				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
}
//need some modification
void cmps( unsigned char secondNibble , CPU &cpu )
{
	 unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip - 2 );
	 char advance = 0 ;
	 if( cpu.flag.getDF() )
		 advance = -1 ;
	 else
		 advance = 1 ;
	 int oprand1;
	 int oprand2;
	 int result ;

	
	 REGISTER word;
	 REGISTER rw ;
	 char rb ;
	
	 if( byte == 243 )
	 {
		 switch( secondNibble )
		 {
		 case 6 :
			 while( cpu.Gregs.cx != 0 && cpu.flag.getZF() )
			 {
				 byte = cpu.mem.read( cpu.es , cpu.di );
				 oprand1 = byte ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si );
				 result = oprand1 - oprand2 ;
				 rb = result ;
				 if( result - rb )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 7:
			 while( cpu.Gregs.cx != 0 && cpu.flag.getZF() )
			 {
				 word = cpu.mem.read( cpu.es , cpu.di );
				 oprand1 = word ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si , 2 );
				 result = oprand1 - oprand2 ;
				 rw = result ;
				 if( result - rw )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
	 else
	 {
		switch( secondNibble )
		 {
		 case 6 :
			 while( cpu.Gregs.cx != 0 && !cpu.flag.getZF() )
			 {
				 byte = cpu.mem.read( cpu.es , cpu.di );
				 oprand1 = byte ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si );
				 result = oprand1 - oprand2 ;
				 rb = result ;
				 if( result - rb )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 7:
			 while( cpu.Gregs.cx != 0 && !cpu.flag.getZF() )
			 {
				 word = cpu.mem.read( cpu.es , cpu.di );
				 oprand1 = word ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si , 2 );
				 result = oprand1 - oprand2 ;
				 rw = result ;
				 if( result - rw )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 } 
	 }
}


void enter( unsigned char secondNibble , CPU &cpu )
{
	REGISTER word = cpu.mem.read( cpu.cs , cpu.ip , 2 );
	cpu.ip += 2 ;

	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;

	push( 5 , cpu );

	REGISTER bpContent ;

	REGISTER temp = cpu.sp;

	if( byte > 0 )
	{
		for( int i = 0 ; i < byte -1 ; i++ )
		{
			cpu.bp -= 2 ;
			bpContent = cpu.mem.read( cpu.ss , cpu.bp , 2 );
			cpu.sp -= 2 ;
			cpu.mem.write( cpu.ss , cpu.sp , bpContent ); 
		}

		cpu.sp -= 2 ;
		cpu.mem.write( cpu.ss , cpu.sp , temp );

	}

	cpu.bp = temp ;

	cpu.sp -= word ;
}



void stos( unsigned char secondNibble , CPU &cpu )
{
	 unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip - 2 );
	 char advance = 0 ;
	 if( cpu.flag.getDF() )
		 advance = -1 ;
	 else
		 advance = 1 ;

	
	 REGISTER word;
	
	 if( byte == 243 )
	 {
		 switch( secondNibble )
		 {
		 case 10 :
			 while( cpu.Gregs.cx != 0)
			 {
				 byte = cpu.Gregs.al ;
				 cpu.mem.write( cpu.es , cpu.di , byte );
				 cpu.di += advance ;
			
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 11:
			 while( cpu.Gregs.cx != 0)
			 {
				 word = cpu.Gregs.ax ;
				 cpu.mem.write( cpu.es , cpu.di , word );
				 cpu.di += advance ;
		
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
	 else
	 {
		 switch( secondNibble )
		 {
		 case 10 :
			 
			 {
				 byte = cpu.Gregs.al ;
				 cpu.mem.write( cpu.es , cpu.di , byte );
				 cpu.di += advance ;
			
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 11:
			 
			 {
				 word = cpu.Gregs.ax ;
				 cpu.mem.write( cpu.es , cpu.di , word );
				 cpu.di += advance ;
				 
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
}

void lods( unsigned char secondNibble , CPU &cpu )
{
	 unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip - 2 );
	 char advance = 0 ;
	 if( cpu.flag.getDF() )
		 advance = -1 ;
	 else
		 advance = 1 ;

	

	
	 if( byte == 243 )
	 {
		 switch( secondNibble )
		 {
		 case 12 :
			 while( cpu.Gregs.cx != 0)
			 {
				 cpu.Gregs.al = cpu.mem.read( cpu.ds , cpu.si );
				
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 13:
			 while( cpu.Gregs.cx != 0)
			 {
				 cpu.Gregs.ax = cpu.mem.read( cpu.ds , cpu.si , 2 );
				 
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
	 else
	 {
		 switch( secondNibble )
		 {
		 case 12 :
			 
			 {
				cpu.Gregs.al = cpu.mem.read( cpu.ds , cpu.si );
			
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 13:
			 
			 {
				 
				 cpu.Gregs.ax = cpu.mem.read( cpu.ds , cpu.si , 2 );
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
}


void scas( unsigned char secondNibble , CPU &cpu )
{
	 unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip - 2 );
	 char advance = 0 ;
	 if( cpu.flag.getDF() )
		 advance = -1 ;
	 else
		 advance = 1 ;
	 int oprand1;
	 int oprand2;
	 int result ;

	
	 REGISTER word;
	 REGISTER rw ;
	 char rb ;
	
	 if( byte == 243 )
	 {
		 switch( secondNibble )
		 {
		 case 14 :
			 while( cpu.Gregs.cx != 0 && cpu.flag.getZF() )
			 {
				 byte = cpu.Gregs.al ;
				 oprand1 = byte ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si );
				 result = oprand1 - oprand2 ;
				 rb = result ;
				 if( result - rb )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 15:
			 while( cpu.Gregs.cx != 0 && cpu.flag.getZF() )
			 {
				 word = cpu.Gregs.ax ;
				 oprand1 = word ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si , 2 );
				 result = oprand1 - oprand2 ;
				 rw = result ;
				 if( result - rw )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 }
	 }
	 else
	 {
		switch( secondNibble )
		 {
		 case 14 :
			 while( cpu.Gregs.cx != 0 && !cpu.flag.getZF() )
			 {
				 byte = cpu.Gregs.al ;
				 oprand1 = byte ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si );
				 result = oprand1 - oprand2 ;
				 rb = result ;
				 if( result - rb )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 case 15:
			 while( cpu.Gregs.cx != 0 && !cpu.flag.getZF() )
			 {
				 word = cpu.Gregs.ax ;
				 oprand1 = word ;
				 oprand2 = cpu.mem.read( cpu.ds , cpu.si , 2 );
				 result = oprand1 - oprand2 ;
				 rw = result ;
				 if( result - rw )
					 cpu.flag.clearZF() ;
				 else
					 cpu.flag.setZF();

				 cpu.di += advance ;
				 cpu.si += advance ;
				 cpu.Gregs.cx --;
			 }
			 break;
		 } 
	 }
}

void aam( unsigned char secondNibble , CPU &cpu )
{
	 cpu.ip ++;

	 cpu.Gregs.ah = cpu.Gregs.al / 10 ; 
	 cpu.Gregs.al = cpu.Gregs.al % 10 ;
}

void aad( unsigned char secondNibble , CPU &cpu )
{
	 cpu.ip ++;

	 cpu.Gregs.al = cpu.Gregs.ah * 10 + cpu.Gregs.al;
}

void loop( unsigned char secondNibble , CPU &cpu )
{
	char disp = 0;
	disp = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip++;

	switch( secondNibble )
	{
	case 0:
		if( cpu.Gregs.cx > 0 && !cpu.flag.getCF() )
		{
			cpu.ip += disp ;
			cpu.Gregs.cx --;
		}
		break;
	case 1:
		if( cpu.Gregs.cx > 0 && cpu.flag.getCF() )
		{
			cpu.ip += disp ;
			cpu.Gregs.cx --;
		}
		break ;
	case 2 :
		if( cpu.Gregs.cx > 0  )
		{
			cpu.ip += disp ;
			cpu.Gregs.cx --;
		}
		break ;
	case 3:
		if( cpu.Gregs.cx == 0  )
		{
			cpu.ip += disp ;
			cpu.Gregs.cx --;
		}
		break;
	}
}

void call( unsigned char secondNibble , CPU &cpu )
{
	REGISTER disp = cpu.mem.read( cpu.cs , cpu.ip , 2 );
	cpu.ip += 2 ;

	cpu.sp -= 2 ;
	cpu.mem.write( cpu.ss , cpu.sp , cpu.ip );

	cpu.ip = disp ;
}


void jmp( unsigned char secondNibble , CPU &cpu )
{
	int disp ;
	REGISTER temp ;
	switch( secondNibble )
	{
	case 10 :
		disp = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2 ;

		cpu.ip += disp;
		break;
	case 12 :
		disp = cpu.mem.read( cpu.cs, cpu.ip );
		cpu.ip ++;

		cpu.ip += disp ;
		break;
	case 11:
		disp = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip +=2 ;
		temp = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip +=2 ;

		cpu.ip = disp;
		cpu.cs = temp;
		break;
	}
}
void repne( unsigned char secondNibble , CPU &cpu )
{
}

void rep( unsigned char secondNibble , CPU &cpu )
{
}

void hlt( unsigned char secondNibble , CPU &cpu )
{
	cout<<" CPU halte press any key to conyinue "<<endl;

	cin.get();
}


void group1b( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;

	bool haveMode = true ;
	int E , G ;
	int addressingMode ;
	int segment ;
	unsigned char * eb = 0 ;
	REGISTER * ev = 0;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );

	switch( G )
	{
	case 0:
		addib( E , segment , addressingMode , cpu );
		break;
	case 1:
		orib( E , segment , addressingMode , cpu );
		break;
	case 2:
		adcib( E , segment , addressingMode , cpu );
		break;
	case 3:
		sbbib( E , segment , addressingMode , cpu );
		break;
	case 4:
		andib( E , segment , addressingMode , cpu );
		break;
	case 5:
		subib( E , segment , addressingMode , cpu );
		break;
	case 6:
		xorib( E , segment , addressingMode , cpu );
		break;
	case 7:
		cmpib( E , segment , addressingMode , cpu );
		break;

	}
}

void addib( int E , int segment , int addressingMode ,CPU &cpu )
{
	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand1 = byte ;
		oprand2 = *eb;
		result = oprand1 + oprand2 ;
		*eb += byte ;
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp += byte ;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp += byte ;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}
	result = oprand1 + oprand2 ;

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rb )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	
	if( (o1 & 128 )== (o2 & 128 ) )
		if( (o1 & 128) == (rb & 128) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();
}


void orib( int E , int segment , int addressingMode ,CPU &cpu )
{


	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand1 = byte ;
		oprand2 = *eb;
		result = oprand1 | oprand2 ;
		*eb |= byte ;
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp |= byte ;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp |= byte ;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}
	result = oprand1 | oprand2 ;

	cpu.flag.clearCF();
	cpu.flag.clearOF();

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
}

void adcib( int E , int segment , int addressingMode ,CPU &cpu )
{
	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand1 = byte ;
		oprand2 = *eb;
		result = oprand1 + oprand2 +cpu.flag.getCF() ;
		*eb += byte + cpu.flag.getCF();
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp += byte  + cpu.flag.getCF();
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp += byte  + cpu.flag.getCF();
			cpu.mem.write( cpu.ss , E , temp );
		}
	}
	result = oprand1 + oprand2  + cpu.flag.getCF() ;

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rb )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	
	if( (o1 & 128 )== (o2 & 128 ) )
		if( (o1 & 128) == (rb & 128) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();
}


void andib( int E , int segment , int addressingMode ,CPU &cpu )
{


	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand1 = byte ;
		oprand2 = *eb;
		result = oprand1 & oprand2 ;
		*eb &= byte ;
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp &= byte ;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp &= byte ;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}
	result = oprand1 & oprand2 ;

	cpu.flag.clearCF();
	cpu.flag.clearOF();

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
}

void subib( int E , int segment , int addressingMode ,CPU &cpu )
{
	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand2 = byte ;
		oprand1 = *eb;
		result = oprand1 - oprand2 ;
		*eb -= byte ;
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand2 = byte ;
			oprand1 = temp;
			temp -= byte ;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand2 = byte ;
			oprand1 = temp;
			temp -= byte ;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}
	result = oprand1 - oprand2 ;

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rb )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	
	if( (o1 & 128 )== (o2 & 128 ) )
		if( (o1 & 128) == (rb & 128) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();
}

void sbbib( int E , int segment , int addressingMode ,CPU &cpu )
{
	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand2 = byte ;
		oprand1 = *eb;
		result = oprand1 - oprand2 - cpu.flag.getCF();
		*eb -= byte - cpu.flag.getCF() ;
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand2 = byte ;
			oprand1 = temp;
			temp -= byte - cpu.flag.getCF();
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand2 = byte ;
			oprand1 = temp;
			temp -= byte - cpu.flag.getCF();
			cpu.mem.write( cpu.ss , E , temp );
		}
	}
	result = oprand1 - oprand2 - cpu.flag.getCF() ;

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rb )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	
	if( (o1 & 128 )== (o2 & 128 ) )
		if( (o1 & 128) == (rb & 128) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();
}


void xorib( int E , int segment , int addressingMode ,CPU &cpu )
{


	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand1 = byte ;
		oprand2 = *eb;
		result = oprand1 ^ oprand2 ;
		*eb ^= byte ;
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp ^= byte ;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand1 = byte ;
			oprand2 = temp;
			temp ^= byte ;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}
	result = oprand1 ^ oprand2 ;

	cpu.flag.clearCF();
	cpu.flag.clearOF();

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
}

void cmpib( int E , int segment , int addressingMode ,CPU &cpu )
{
	unsigned char byte = cpu.mem.read( cpu.cs , cpu.ip );
	cpu.ip ++;
	unsigned char *eb;
	int oprand1;
	int oprand2;
	int result ;

	if( addressingMode == 0 )
	{
		switch( E )
		{
		case 0 :
			eb = &( cpu.Gregs.al );
			break ;
		case 1:
			eb = &( cpu.Gregs.cl );
			break ;
		case 2:
			eb = &( cpu.Gregs.dl );
			break ;
		case 3:
			eb = &( cpu.Gregs.bl );
			break ;
		case 4:
			eb = &( cpu.Gregs.ah );
			break ;
		case 5:
			eb = &( cpu.Gregs.ch );
			break ;
		case 6:
			eb = &( cpu.Gregs.dh );
			break ;
		case 7:
			eb = &( cpu.Gregs.bh );
			break ;
		}

		oprand2 = byte ;
		oprand1 = *eb;
		result = oprand1 - oprand2 ;
		
	}
	else
	{
		unsigned char temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E );
			oprand2 = byte ;
			oprand1 = temp;
			
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E );
			oprand2 = byte ;
			oprand1 = temp;
			
		}
	}
	result = oprand1 - oprand2 ;

	unsigned char o1 , o2 , rb ;
	o1 = oprand1;
	o2 = oprand2;
	rb = result ;
	
	if( rb )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	
	if( cpu.checkParity( rb ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	
	if( rb & 128 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rb )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	
	if( (o1 & 128 )== (o2 & 128 ) )
		if( (o1 & 128) == (rb & 128) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();
}


void group1w( unsigned char secondNibble , CPU &cpu )
{
	unsigned int oprand1 = 0 ;
	unsigned int oprand2 = 0;
	unsigned int result = 0 ;

	bool haveMode = true ;
	int E , G ;
	int addressingMode ;
	int segment ;
	unsigned char * eb = 0 ;
	REGISTER * ev = 0;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );

	switch( G )
	{
	case 0:
		addiw( secondNibble , E , segment , addressingMode , cpu );
		break;
	case 1:
		oriw( secondNibble , E , segment , addressingMode , cpu );
		break;
	case 2:
		adciw( secondNibble , E , segment , addressingMode , cpu );
		break;
	case 3:
		sbbiw( secondNibble , E , segment , addressingMode , cpu );
		break;
	case 4:
		andiw( secondNibble , E , segment , addressingMode , cpu );
		break;
	case 5:
		subiw( secondNibble , E , segment , addressingMode , cpu );
		break;
	case 6:
		xoriw( secondNibble , E , segment , addressingMode , cpu );
		break;
	case 7:
		cmpiw( secondNibble , E , segment , addressingMode , cpu );
		break;

	}
}

void addiw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
		*ev += oprand2;
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			temp += oprand2;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			temp += oprand2;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}

	result = oprand1 + oprand2 ;

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rw )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	if( (ow1 & 32768 ) == (ow2 & 32768 ) )
		if( (ow1 & 32768 ) == (rw & 32768) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();

}

void oriw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
		*ev |= oprand2;
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			temp |= oprand2;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			temp |= oprand2;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}

	result = oprand1 | oprand2 ;
	cpu.flag.clearCF();
	cpu.flag.clearOF();

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	

}
void adciw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
		*ev += oprand2 + cpu.flag.getCF();
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			temp += oprand2 + cpu.flag.getCF();
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			temp += oprand2 + cpu.flag.getCF();
			cpu.mem.write( cpu.ss , E , temp );
		}
	}

	result = oprand1 + oprand2 + cpu.flag.getCF();

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rw )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	if( (ow1 & 32768 ) == (ow2 & 32768 ) )
		if( (ow1 & 32768 ) == (rw & 32768) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();

}

void subiw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
		*ev -= oprand2;
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			temp -= oprand2;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			temp -= oprand2;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}

	result = oprand1 - oprand2 ;

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rw )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	if( (ow1 & 32768 ) == (ow2 & 32768 ) )
		if( (ow1 & 32768 ) == (rw & 32768) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();

}

void sbbiw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
		*ev -= oprand2 - cpu.flag.getCF();
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			temp -= oprand2 - cpu.flag.getCF();
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			temp -= oprand2 - cpu.flag.getCF();
			cpu.mem.write( cpu.ss , E , temp );
		}
	}

	result = oprand1 - oprand2 - cpu.flag.getCF();

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rw )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	if( (ow1 & 32768 ) == (ow2 & 32768 ) )
		if( (ow1 & 32768 ) == (rw & 32768) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();

}

void andiw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
		*ev &= oprand2;
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			temp &= oprand2;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			temp &= oprand2;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}

	result = oprand1 & oprand2 ;
	cpu.flag.clearCF();
	cpu.flag.clearOF();

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	

}


void xoriw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
		*ev ^= oprand2;
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			temp ^= oprand2;
			cpu.mem.write( cpu.ds , E , temp );
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			temp ^= oprand2;
			cpu.mem.write( cpu.ss , E , temp );
		}
	}

	result = oprand1 ^ oprand2 ;
	cpu.flag.clearCF();
	cpu.flag.clearOF();

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	

}

void cmpiw( unsigned char secondNibble , int E , int segment , int addressingMode , CPU& cpu )
{
	int oprand2 ;
	int oprand1;
	int result ;
	if( secondNibble == 1 )
	{
		oprand2 = cpu.mem.read( cpu.cs , cpu.ip , 2 );
		cpu.ip += 2;
	}
	else
	{
		oprand1 = cpu.mem.read( cpu.cs , cpu.ip );
		cpu.ip ++;
	}

	REGISTER * ev;
	switch( E )
	{
	case 0 :
		ev = &( cpu.Gregs.ax );
		break ;
	case 1:
		ev = &( cpu.Gregs.cx );
		break ;
	case 2:
		ev = &( cpu.Gregs.dx );
		break ;
	case 3:
		ev = &( cpu.Gregs.bx );
		break ;
	case 4:
		ev = &( cpu.sp );
		break ;
	case 5:
		ev = &( cpu.bp );
		break ;
	case 6:
		ev = &( cpu.si );
		break ;
	case 7:
		ev = &( cpu.di );
		break ;
	}
	if( addressingMode == 0 )
	{
		oprand1 = *ev;
	
	}
	else
	{
		REGISTER temp ;
		if( segment == 0 )
		{
			temp = cpu.mem.read( cpu.ds , E , 2 );
			oprand1 = temp ;
			
		}
		else
		{
			temp = cpu.mem.read( cpu.ss , E , 2 );
			oprand1 = temp ;
			
		}
	}

	result = oprand1 - oprand2 ;

	REGISTER rw , ow1 , ow2 ;
	ow1 = oprand1;
	ow2 = oprand2;
	rw = result ;
	
	if( rw )
		cpu.flag.clearZF();
	else
		cpu.flag.setZF();
	
	if( cpu.checkParity( rw ) )
		cpu.flag.setPF();
	else
		cpu.flag.clearPF();
	
	if( rw & 32768 )
		cpu.flag.setSF();
	else
		cpu.flag.clearSF();
	
	if( result - rw )
		cpu.flag.setCF();
	else
		cpu.flag.clearCF();
	if( cpu.checkAf( oprand1 , oprand2 ) )
		cpu.flag.setAF();
	else
		cpu.flag.clearAF();
	
	if( (ow1 & 32768 ) == (ow2 & 32768 ) )
		if( (ow1 & 32768 ) == (rw & 32768) )
			cpu.flag.clearOF();
		else
			cpu.flag.setOF();

}

void group4( unsigned char secondNibble , CPU &cpu )
{
	bool haveMode = true ;
	int E , G ;
	int addressingMode ;
	int segment ;
	unsigned char * eb = 0 ;
	REGISTER * ev = 0;

	if( haveMode )
		cpu.decoder.decodeModeRM( G , E , addressingMode , segment );


	switch( E )
	{
	case 0 :
		eb = &( cpu.Gregs.al );
		break ;
	case 1:
		eb = &( cpu.Gregs.cl );
		break ;
	case 2:
		eb = &( cpu.Gregs.dl );
		break ;
	case 3:
		eb = &( cpu.Gregs.bl );
		break ;
	case 4:
		eb = &( cpu.Gregs.ah );
		break ;
	case 5:
		eb = &( cpu.Gregs.ch );
		break ;
	case 6:
		eb = &( cpu.Gregs.dh );
		break ;
	case 7:
		eb = &( cpu.Gregs.bh );
		break ;
	}

	



}
