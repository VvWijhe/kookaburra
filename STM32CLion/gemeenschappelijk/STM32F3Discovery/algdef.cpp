/************************************************************************
Klassen om eerste en tweedegraadsfunkties, en breuken te bewerken

Alle software in dit bestand is                                     
(c) 2005-2012 Kiwanda Embedded Systemen                                               
$Id: algdef.cpp 2038 2016-01-05 16:20:54Z ewout $
************************************************************************/

#include "algdef.h"

#include <cassert>
#include <math.h>

const Int32 Breuk::nulWerkers[] = { 1,
				     10,
				     100,
				     1000,
				     10000,
				     100000,
				     1000000,
				     10000000 };


Breuk::Breuk(const STeller t,
             const STeller n) : tel(t),
                                noem(n)
{
    assert(n != 0);
    normaliseer();
}

Breuk::Breuk(const WiskundeFloatType fp, 
             const BreukRelAantalRelevanteNummers relCijfers)
{
    assert((relCijfers < 9));   /* meer heeft geen zin bij single point floating point */ 

    /* splits in  geheel getal en fractie */
    if (relCijfers==0)
    {

	tel = floor(fp);
	
			
	noem = 1;
    }
    else
    {
	const STeller gg =  floor(fp);
		
	STeller drijvend = floor((fp-gg)*nulWerkers[relCijfers-1]);
	STeller tNoemer = nulWerkers[relCijfers-1];

        /* delen door 2 */
	while ((drijvend % 2) == 0)
	{
	    drijvend >>= 1;
	    tNoemer >>=1;
	}

        /* delen door 3,5,7 */
	for (STeller noemerWaarde = 3;noemerWaarde<9;noemerWaarde+=2)
	    while ((drijvend % noemerWaarde) == 0)
            {
                drijvend/=noemerWaarde;
                tNoemer/=noemerWaarde;
            }


	tel= gg*tNoemer + drijvend;
	noem = tNoemer;
    }
}

void Breuk::normaliseer()
{
     /* Normaliseer naar zo klein mogelijk */

     /* delen door 2 */
    while (((tel % 2) == 0) && ((noem % 2) == 0) )
    {
	tel >>= 1;
	noem >>=1;
    }

     /* delen door 3,5,7 */
    for (STeller noemerWaarde = 3;noemerWaarde<9;noemerWaarde+=2)
	while (((tel % noemerWaarde) == 0) && ((noem % noemerWaarde) == 0) )
	{
	    tel/=noemerWaarde;
	    noem/=noemerWaarde;
	}
}

bool Breuk::operator != (const Breuk &rhs) const
{
    if (this == &rhs)
        return(false);
    else
    {
        if ((tel != rhs.teller()) || (noem != rhs.noemer()))
            return(true);
        else
            return(false);
    }
}
 
bool Breuk::operator == (const Breuk &rhs) const
{
    if (this == &rhs)
        return(true);
    else
    {
        if ((tel == rhs.teller()) && (noem == rhs.noemer()))
            return(true);
        else
            return(false);
    }
}

Breuk Breuk::operator + (const Breuk &rhs) const
{
    Breuk resultaat((STeller)((tel*rhs.noem)+(rhs.tel*noem)),
		    (STeller)(noem*rhs.noem));

    resultaat.normaliseer();
    return(resultaat);
}

Breuk Breuk::operator + (const STeller rhs) const
{
    Breuk resultaat((tel+(rhs*noem)),noem);

    resultaat.normaliseer();
    return(resultaat);
}

  
Breuk Breuk::operator - (const Breuk &rhs) const
{
    Breuk resultaat((STeller)((tel*rhs.noem)-(rhs.tel*noem)),(STeller)(noem*rhs.noem));

    resultaat.normaliseer();
    return(resultaat);
}

Breuk Breuk::operator - (const STeller rhs) const
{
    const Breuk resultaat((tel-(rhs*noem)),noem);

    return(resultaat);
}

Breuk & Breuk::operator += (const Breuk &rhs)
{
    tel = (tel*rhs.noem)+(rhs.tel*noem);
    noem = (noem*rhs.noem);

    normaliseer();

    return(*this);
}

Breuk & Breuk::operator -= (const Breuk &rhs)
{
    tel = (tel*rhs.noem)-(rhs.tel*noem);
    noem = (noem*rhs.noem);
    normaliseer();
    return(*this);
}

Breuk & Breuk::operator *= (const STeller getal)
{
    tel*=getal;
    return(*this);
}


Breuk Breuk::operator * (const STeller getal) const
{
    const Breuk resultaat(tel*getal,noem);
    return(resultaat);
}

Breuk Breuk::operator * (const Breuk &rhs) const
{
    Breuk resultaat((STeller)(tel*rhs.teller()),(STeller)(noem*rhs.noemer()));

     /* resultaat is waarschijnlijk erg groot geworden */
    resultaat.normaliseer();

    return(resultaat);
}

Breuk &Breuk::operator *= (const Breuk &rhs)
{
    tel*=rhs.teller();
    noem*=rhs.noemer();
    normaliseer();
    return(*this);
}


Breuk & Breuk::operator /= (const STeller getal)
{
    assert(getal != 0);
    noem*=getal;

    normaliseer();
    return(*this);

}

Breuk  Breuk::operator / (const STeller getal) const
{
    assert(getal != 0);
    Breuk resultaat(tel,noem*getal);

    resultaat.normaliseer();
    return(resultaat);
}

Breuk Breuk::operator / (const Breuk &rhs) const
{
    if (this == &rhs)
	return(Breuk(1));
    else
    {
	const Breuk omgekeerd(rhs.noemer(),rhs.teller());
	return(Breuk::operator *(omgekeerd));
    }
}


STeller Breuk::vermenigvuldig(const STeller getal) const
{
    const STeller uitkomst = (getal*tel)/noem;

    return(uitkomst);
}
 
WiskundeFloatType Breuk::geefFloat() const
{
    const float resultaat = (float)teller()/noemer();

    return(resultaat);
}

STeller Breuk::rondAf() const
{
    return(tel/noem);
}

