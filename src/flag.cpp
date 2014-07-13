#include "flag.h"



FlagRegister :: FlagRegister()
{
	flag = 0;
	setIF();
}





int FlagRegister::getCF() const
{
	return flag & 1 ;
}


void FlagRegister::setCF()
{
	flag = flag | 1 ;
}



void FlagRegister::clearCF()
{
	flag = flag & 65534 ;
}


void FlagRegister::complementCF()
{
	if( getCF() )
		clearCF();
	else
		setCF();
}





int FlagRegister::getPF() const
{
	return flag & 4 ;
};



void FlagRegister::setPF()
{
	flag = flag | 4 ;
}


void FlagRegister::clearPF()
{
	flag = flag & 65531 ;
}






int FlagRegister::getAF() const 
{
	return flag & 16 ;
}


void FlagRegister::setAF()
{
	flag = flag | 16 ;
}


void FlagRegister::clearAF()
{
	flag = flag & 65519 ;
}






int FlagRegister::getZF() const
{
	return flag & 64;
}


void FlagRegister::setZF()
{
	flag = flag | 64 ;
}

void FlagRegister::clearZF()
{
	flag = flag & 65471;
}






int FlagRegister::getSF() const 
{
	return flag & 128 ;
}


void FlagRegister::setSF()
{
	flag = flag | 128 ;
}

void FlagRegister::clearSF()
{
	flag = flag & 65407 ;
}






int FlagRegister::getTF() const
{
	return flag & 256 ;
}

void FlagRegister::setTF()
{
	flag = flag | 256 ;
}


void FlagRegister::clearTF()
{
	flag = flag & 65279;
}





int FlagRegister::getIF() const 
{
	return flag & 512 ;
}

void FlagRegister::setIF()
{
	flag = flag | 512 ;

}

void FlagRegister::clearIF()
{
	flag = flag & 65203;
}




int FlagRegister::getDF() const
{
	return flag & 1024 ;
}

void FlagRegister::setDF()
{
	flag = flag | 1024 ;
}

void FlagRegister::clearDF()
{
	flag = flag & 64511;
}



int FlagRegister::getOF() const
{
	return flag & 2048 ;
}

void FlagRegister::setOF()
{
	flag = flag | 2048;
}

void FlagRegister::clearOF()
{
	flag = flag & 63487 ;
}



FlagRegister::operator REGISTER() 
{
	return flag;
}

void FlagRegister::operator =( REGISTER reg) 
{
	 flag = reg ;
}


unsigned char FlagRegister::getfb()
{
	return static_cast< unsigned char >( flag );
}


void FlagRegister::setfb( unsigned char byte )
{
	flag &= 65280;
	flag |= byte ;
}
