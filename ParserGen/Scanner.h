// This file is fully generated by running scannergen v1.5.1

#ifndef ScannerH
#define ScannerH

#include <bux/ImplScanner.h>

class C_ParserScanner: public bux::C_ScannerImpl<uint8_t,int8_t,bux::C_LexUTF32>
{
public:
    C_ParserScanner(bux::I_Parser &parser);
};

#endif // ScannerH
