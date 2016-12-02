//
//
#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
public:
    Timer(unsigned long futureMillis);
    bool expired();

private:
    unsigned long expirationTime;
};

#endif // _TIMER_H_