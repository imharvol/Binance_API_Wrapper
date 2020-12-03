#ifndef _BINANCE_API_WRAPPER_
#define _BINANCE_API_WRAPPER_

#include "Arduino.h"

class Binance_API {
    public:
       float getPrice(String pair); 
};

#endif // _BINANCE_API_WRAPPER_
