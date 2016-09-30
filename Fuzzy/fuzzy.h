/******************************************************************************
 * Project        : HAN ESE Project 3 - Regeltechniek
 * File           : Header file for Fuzzy library
 * Copyright      : 2012 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:
  
    Version 1.0 - November 2012
    > Initial revision

******************************************************************************/
#ifndef _FUZZY_H_
#define _FUZZY_H_

/******************************************************************************
  Defines
******************************************************************************/
#define f_and(a, b) (((a) < (b)) ? (a) : (b))
#define f_or(a, b)  (((a) > (b)) ? (a) : (b))

/******************************************************************************
  Type definitions
******************************************************************************/
typedef double t_crisp;
typedef double t_dom;   // Degree Of Membership
typedef double t_mbf;   // MemBership Function

// Linguistic ERROR
typedef struct {
    // Cisp value
    t_crisp crisp;

    // Member NEG_LARGE
    t_mbf mbf_negL[4];
    t_dom dom_negL;

    // Member NEG
    t_mbf mbf_neg[4];
    t_dom dom_neg;

    // Member NO
    t_mbf mbf_no[4];
    t_dom dom_no;

    // Member POS
    t_mbf mbf_pos[4];
    t_dom dom_pos;

    // Member POS_LARGE
    t_mbf mbf_posL[4];
    t_dom dom_posL;

} t_er;

// Consequence CONTROLVALUE
typedef struct {
    // Member NEG_LARGE
    t_mbf mbf_negL[3];
    t_dom dom_negL;

    // Member NEG
    t_mbf mbf_neg[3];
    t_dom dom_neg;

    // Member NO
    t_mbf mbf_no[3];
    t_dom dom_no;

    // Member POS
    t_mbf mbf_pos[3];
    t_dom dom_pos;

    // Member POS_LARGE
    t_mbf mbf_posL[3];
    t_dom dom_posL;

} t_cv;

class Fuzzy {
public:
    Fuzzy();
    Fuzzy(t_cv control, t_er error);

    int32_t process(int32_t setPoint, int32_t measurementValue);

    void setMBF(t_cv control);
    void setMBF(t_er error);

private:
    // Fuzzy variables
    t_er _error;  // Error
    t_cv _controlValue;  // Control Value
    t_dom dom; // Degree of membership (for MOM)

    t_dom MBF_Trapezoid(t_mbf p[4], t_crisp crisp);

    t_dom MBF_Triangle(t_mbf p[3], t_crisp crisp);

    t_dom MBF_Singleton(t_mbf p, t_crisp crisp);

    t_dom MBF_Gaussian(t_mbf p[2], t_crisp crisp);
};

#endif // _FUZZY_H_
