/******************************************************************************
 * Project        : HAN ESE Project 3 - Regeltechniek
 * File           : Source file for Fuzzy library
 * Copyright      : 2012 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:
  
    Version 1.0 - November 2012
    > Initial revision

******************************************************************************/
#include <cstdint>
#include "fuzzy.h"
#include "math.h"

/******************************************************************************
  Fuzzy function implemenations
******************************************************************************/
Fuzzy::Fuzzy() {}

Fuzzy::Fuzzy(t_cv control, t_er error) {
    _controlValue = control;
    _error = error;
}

void Fuzzy::setMBF(t_cv control) {
    _controlValue = control;
}

void Fuzzy::setMBF(t_er error) {
    _error = error;
}

// ----------------------------------------------------------------------------
// Membershipfunction implementation
// ----------------------------------------------------------------------------
t_dom Fuzzy::MBF_Trapezoid(t_mbf p[4], t_crisp crisp) {
    t_dom ret;

    if (crisp < p[0]) {
        return 0;
    } else if (crisp <= p[1]) {
        if (p[0] == p[1]) {
            ret = 100;
        } else {
            ret = ((crisp - p[0]) * 100) / (p[1] - p[0]);
        }
    } else if (crisp <= p[2]) {
        ret = 100;
    } else if (crisp <= p[3]) {
        if (p[2] == p[3]) {
            ret = 100;
        } else {
            ret = 100 - ((crisp - p[2]) * 100) / (p[3] - p[2]);
        }
    } else {
        ret = 0;
    }

    return (ret);
}

// ----------------------------------------------------------------------------
// Membershipfunction implementation
// ----------------------------------------------------------------------------
t_dom Fuzzy::MBF_Triangle(t_mbf p[3], t_crisp crisp) {
    t_dom ret;

    if (crisp < p[0]) {
        ret = 0;
    } else if (crisp <= p[1]) {
        if (p[0] == p[1]) {
            ret = 100;
        } else {
            ret = ((crisp - p[0]) * 100) / (p[1] - p[0]);
        }
    } else if (crisp <= p[2]) {
        if (p[1] == p[2]) {
            ret = 100;
        } else {
            ret = 100 - ((crisp - p[1]) * 100) / (p[2] - p[1]);
        }
    } else {
        ret = 0;
    }

    return (ret);
}

// ----------------------------------------------------------------------------
// Membershipfunction implementation
// ----------------------------------------------------------------------------
t_dom Fuzzy::MBF_Singleton(t_mbf p, t_crisp crisp) {
    t_dom ret;

    if (crisp == p) {
        ret = 100;
    } else {
        ret = 0;
    }

    return (ret);

}

// ----------------------------------------------------------------------------
// Membershipfunction implementation
// ----------------------------------------------------------------------------
t_dom Fuzzy::MBF_Gaussian(t_mbf p[3], t_crisp crisp) {
    return (0);
}

int32_t Fuzzy::process(int32_t setPoint, int32_t measurementValue) {
    int32_t errorValue = setPoint - measurementValue;

    // ********************************************************************
    // Fuzzification
    _error.dom_negL = MBF_Trapezoid(_error.mbf_negL, (t_crisp) errorValue);
    _error.dom_neg = MBF_Trapezoid(_error.mbf_neg, (t_crisp) errorValue);
    _error.dom_no = MBF_Trapezoid(_error.mbf_no, (t_crisp) errorValue);
    _error.dom_pos = MBF_Trapezoid(_error.mbf_pos, (t_crisp) errorValue);
    _error.dom_posL = MBF_Trapezoid(_error.mbf_posL, (t_crisp) errorValue);

    // ********************************************************************
    // Decision making
    _controlValue.dom_negL = _error.dom_negL;
    _controlValue.dom_neg = _error.dom_neg;
    _controlValue.dom_no = _error.dom_no;
    _controlValue.dom_pos = _error.dom_pos;
    _controlValue.dom_posL = _error.dom_posL;

    // ********************************************************************
    // Defuzzification
    // Centroid method
    return (int32_t) (((_controlValue.dom_negL * _controlValue.mbf_negL[1]) +
                       (_controlValue.dom_neg * _controlValue.mbf_neg[1]) +
                       (_controlValue.dom_no * _controlValue.mbf_no[1]) +
                       (_controlValue.dom_pos * _controlValue.mbf_pos[1]) +
                       (_controlValue.dom_posL * _controlValue.mbf_posL[1])) /
                      (_controlValue.dom_negL +
                       _controlValue.dom_neg +
                       _controlValue.dom_no +
                       _controlValue.dom_pos +
                       _controlValue.dom_posL));
}

/******************************************************************************
  EOF
******************************************************************************/
