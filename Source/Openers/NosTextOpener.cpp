#include "NosTextOpener.h"

NosTextOpener::NosTextOpener()
{

}

bool NosTextOpener::decrypt(QFile &file)
{
    file.seek(0);

    int fileAmount = file.read(4).toInt();

    for (int i = 0; i < fileAmount; ++i)
    {
        int fileNumber = file.read(4).toInt();
        int stringNameSize = file.read(4).toInt();
        QString stringName = file.read(stringNameSize);
        int isDat = file.read(4).toInt();
    }

    return true;
}
