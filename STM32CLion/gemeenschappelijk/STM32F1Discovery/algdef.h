
/************************************************************************/
/* Kiwanda                                                            */
/* Algemene definities voor operatie                                    */
/* Alle software in dit bestand is                                      */
/* (c) 2001-2012 Kiwanda Embedded Systemen                              */
/* $Id: algdef.h 2126 2016-03-09 16:59:56Z ewout $                                                                 */
/************************************************************************/

#ifndef ESE_ALGDEF_H
#define ESE_ALGDEF_H


#include <stdio.h>

#include <cmath>

/* standaard definities */

#ifndef PI
#define PI  3.141592654
#endif

#ifdef  FALSE
#define False		FALSE
#else
#define False		0
#endif

#ifdef  TRUE
#define True		TRUE
#else
#define True		1
#endif

#ifndef NULL
#define NULL            0
#endif

#ifndef Null
#define Null            NULL
#endif

#define forever         while(1==1)

/* Basic Types
 */
typedef char	 		Char;
typedef unsigned char		UChar;
typedef unsigned int		UInt;
typedef int			Int;
typedef float			Float;
typedef double			Double;

/* Sized Types
 */
typedef signed char		  Int8;
typedef unsigned char		UInt8;
typedef signed short		Int16;
typedef unsigned short  UInt16;
typedef signed int 		  Int32;
typedef unsigned int		UInt32;
typedef long long		    Int64; 
typedef unsigned long long	UInt64; 

/* Bool Type  */
typedef UInt32			Bool32;	
typedef UInt16			Bool16;
typedef UInt8			Bool8;


/* algemene funktie foutmelding */

typedef enum
{
    Ok=0,
    Waarschuwing,
    Fout,
    Ernstig,
    Rampzalig,
    ReserveFoutCode
} FoutCode;

typedef enum
{
    SchakelaarUit=0,
    SchakelaarAan,
	SchakelaarOm,
    SchakelaarEinde
} Schakelaar;


typedef UInt32 Teller;             /* standaard breedte van de ARM7 */ 
typedef Int32 STeller;             /* standaard breedte van de ARM7 */ 
typedef float WiskundeFloatType;   /* schakel tussen enkel en dubbel precisie */


/* funkties en inline funkties */

/* bitsgewijze operatie macros . Geef bits aan als nummer en niet als 2 tot de macht (nummer) */ 
#define SetBit(x,y)          x |= (1<<y) /* Set bit y in byte x*/ 
#define ClearBit(x,y)        x &= (~(1<<(y))) /* Clear bit y in byte x*/ 
#define CheckBit(x,y)        (x & (1<<y))  /* Check bit y in byte x*/
#define EnterBit(x,y,b)      if (b)  SetBit(x,y) else  ClearBit(x,y)  /* zet bit y in byte x gelijk aan b */
#define CopyBit(a,b,x,y)     EnterBit((x),(y), (CheckBit(a,b)) ) /* kopieer bit b in byte a naar bit y in byte x */  //    (x =  (x&(~(1<<(y))))  | ((a & (1<<b) ) << (y-b) ) )  /* kopieer bit b in byte a naar bit y in byte x */
#define ToggleBit(x,y)       x = (x ^ (1<<y));  /* verander bit y in byte x */

/* bitsgewijze operatie macros . Geef bits als 2 tot de macht (nummer) */ 
/* Werkt in samenhang met Atmel AT91 bitdefinities */
#define SetBitM(var,bit)          var |= (bit) /* Set bit bit in byte var*/ 
#define ClearBitM(var,bit)        var &= (~(bit))  /* Clear bit y in bbitte var*/ 
#define CheckBitM(var,bit)        (var & (bit))  /* Check bit y in byte var*/
#define EnterBitM(var,bit,b)      if (b) SetBitM(var,bit) else ClearBitM(var,bit)  /* zet bit bit in byte var gelijk aan b */
#define CoybitBitM(src,srcbit,dest,destbit)     EnterBitM((dest),(destbit), (CheckBitM(src,srcbit)) ) /* kopieer bit b in byte a naar bit bit in byte var */  //    (var =  (var&(~(1<<(bit))))  | ((a & (1<<b) ) << (bit-b) ) )  /* kopieer bit b in bbitte a naar bit bit in byte var */
#define ToggleBitM(var,bit)       var = (var ^ (bit))  /* flip bit y in byte var */


/* tel b bij a op binnen 'bits' aantal bits. Bij een overflow over 'bits' bits wordt slechts het restant doorgegeven */
/* Werkt alleen bij unsigned integers !!! */
#define kiwandaTelBinnenBits(a,b, bits)  ( ( (a+b) < (1<<bits) ) ? a+b : (a+b-1-(1<<bits)))  

    
/* Bit definities */
#define KBit0                0x01
#define KBit1                0x02
#define KBit2                0x04
#define KBit3                0x08
#define KBit4                0x10
#define KBit5                0x20
#define KBit6                0x40
#define KBit7                0x80


/* Serieele hulpjes */
#define HoogWoord(woord)    ((UInt16) 0xffff & (woord >> 16))  /* In een 32 bit geheugen, is dit het Most Significant Word (16 bit) */
#define LaagWoord(woord)    ((UInt16) 0xffff & woord)          /* In een 32 bit geheugen, is dit het Least Significant Word (16 bit) */
#define HoogByte(woord)    ((UInt8) 0xff & (woord >> 8))       /* In een 16 bit geheugen, is dit het Most Significant Byte (8 bit) */
#define LaagByte(woord)    ((UInt8) 0xff & woord)              /* In een 16 bit geheugen, is dit het Least Significant Byte (8 bit) */

#define Hoog              1
#define Laag              0
                              
#define ReturnIsOk(a)  ( ( (a) == Ok ) ? True : False )
#define ReturnIsNietOk(a)  ( ( (a) != Ok ) ? True : False )



typedef enum
{
    BreukRelNummers_0=0,
    BreukRelNummers_1,
    BreukRelNummers_2,
    BreukRelNummers_3,
    BreukRelNummers_4,
    BreukRelNummers_5,
    BreukRelNummers_6,
    BreukRelNummers_7,
    BreukRelNummers_8,
    BreukRelNummers_Einde
} BreukRelAantalRelevanteNummers;

class Breuk
{
public:
    Breuk(const Int16 t,     /* de teller */
          const Int16 n=1);  /* de noemer. standaard = 1 --> geheel getal */

    Breuk(const WiskundeFloatType,          /* input: floating point getal */
	  const BreukRelAantalRelevanteNummers );           /* aantal relevante cijfers in het drijvende komma getal */

    bool operator != (const Breuk &) const;
    bool operator == (const Breuk &) const;

    Breuk operator + (const Breuk &) const;

    Breuk operator + (const Int16 ) const;
  
    Breuk operator - (const Breuk &) const;

    Breuk operator - (const Int16 ) const;

    Breuk & operator += (const Breuk &);
    Breuk & operator -= (const Breuk &);
   
    Breuk & operator *= (const Int16);
    Breuk & operator *= (const Breuk &);

    Breuk  operator * (const Int16) const;
    Breuk  operator * (const Breuk &) const;

    Breuk & operator /= (const Int16);
    Breuk  operator / (const Int16) const;
    Breuk  operator / (const Breuk &) const;


    Int16 teller() const
        {
            return(tel);
        }

    Int16 noemer() const
        {
            return(noem);
        }
    
    Int16 vermenigvuldig(const Int16) const; /* vermenigvuldig breuk met Int16 en rond af naar dichtbijzijnde Int16*/

    float geefFloat() const;  /* geef breuk als float */
 
    Int16 rondAf() const;   /* rond breuk af naar integer */
    
protected:
    void normaliseer();

private:
    Int16 tel,noem;
    static const Int32 nulWerkers[];
};








#endif  /* ALGDEF_H */





