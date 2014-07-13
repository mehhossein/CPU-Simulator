#ifndef FLAG_REGISTER_H
#define FLAG_REGISTER_H


#define _32_BIT
//#define LITTLE_ENDIAN

class CPU ;

#ifdef _32_BIT
	
	typedef unsigned short int REGISTER ;

#else 

	typedef unsigned int REGISTER ;

#endif


class FlagRegister
{
	friend void lahf( unsigned char secondNibble , CPU &cpu );
	
public :

	FlagRegister();

	int getCF() const;
	void setCF() ;
	void clearCF();
	void complementCF();

	int getPF() const ;
	void setPF();
	void clearPF();

	int getAF() const ;
	void setAF();
	void clearAF();

	int getZF() const ;
	void setZF() ;
	void clearZF();


	int getSF() const ;
	void setSF() ;
	void clearSF();

	int getTF() const ;
	void setTF();
	void clearTF();


	int getIF() const ;
	void setIF();
	void clearIF();

	int getDF() const ;
	void setDF();
	void clearDF();


	int getOF() const ;
	void setOF() ;
	void clearOF();


	operator REGISTER()  ;
	void operator =( REGISTER );
	unsigned char getfb() ;
	


private :
	
	void setfb( unsigned char );
	REGISTER flag ;
};



#endif
