#ifndef NOSZLIBOPENER_H
#define NOSZLIBOPENER_H
#include "INosFileOpener.h"

class NosZlibOpener : public INosFileOpener
{
private:
    static const int NOS_HEADER_SIZE = 0x10;
public:
    NosZlibOpener();
    virtual OnexTreeItem* decrypt(QFile& file);
};

#endif // NOSZLIBOPENER_H
