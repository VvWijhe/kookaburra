//
// Created by Jamie on 8-12-2016.
//

#ifndef _AIRPLANE_H
#define _AIRPLANE_H

#ifdef __cplusplus
extern "C" {
#endif

class Airplane {
public:
    Airplane();

    void start();

private:
    void manual();

    void autopilot();
};

#ifdef __cplusplus
}
#endif

#endif //MAINPROJECT_AIRPLANE_H
