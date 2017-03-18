#include "INosFileOpener.h"

int INosFileOpener::readNextInt(QFile &file)
{
    return qFromLittleEndian<int>(reinterpret_cast<const uchar *>(file.read(4).data()));
}
