#include "NosTextOpener.h"

NosTextOpener::NosTextOpener()
{

}

bool NosTextOpener::decrypt(QFile &file)
{
    file.seek(0);

    int fileAmount = readNextInt(file);

    for (int i = 0; i < fileAmount; ++i)
    {
        int fileNumber = readNextInt(file);
        int stringNameSize = readNextInt(file);
        QString stringName = file.read(stringNameSize);
        int isDat = readNextInt(file);
        int fileSize = readNextInt(file);
        QByteArray fileContent = file.read(fileSize);

        QByteArray decryptedArray = datDecryptor.decrypt(fileContent);

        qDebug() << stringName << " : " << QString(decryptedArray);
    }

    return true;
}
