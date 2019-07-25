#include "OnexTreeZlibItem.h"

OnexTreeZlibItem::OnexTreeZlibItem(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                                   int creationDate, bool compressed)
    : OnexTreeItem(name, content), header(header), id(id), creationDate(creationDate), compressed(compressed),
      opener(opener) {
}

QWidget *OnexTreeZlibItem::getPreview() {
    return nullptr;
}

QWidget *OnexTreeZlibItem::getInfos() {
    if (!hasParent())
        return nullptr;

    QWidget *infos = new QWidget();
    QGridLayout *infoLayout = new QGridLayout();

    QLabel *idLabel = new QLabel("ID");
    infoLayout->addWidget(idLabel, 0, 0);
    QLineEdit *idIn = new QLineEdit(QString::number(getId()));
    connect(idIn, SIGNAL(textChanged(QString)), this, SLOT(setId(QString)));
    infoLayout->addWidget(idIn, 0, 1);

    QLabel *sizeLabel = new QLabel("Size");
    infoLayout->addWidget(sizeLabel, 1, 0);
    QLineEdit *sizeIn = new QLineEdit(QString::number(getContentSize()));
    sizeIn->setEnabled(false);
    infoLayout->addWidget(sizeIn, 1, 1);

    QLabel *headerLabel = new QLabel("Header");
    infoLayout->addWidget(headerLabel, 2, 0);
    QLineEdit *headerIn = new QLineEdit("NT Data");
    connect(headerIn, SIGNAL(textChanged(QString)), this, SLOT(setHeader(QString)));
    infoLayout->addWidget(headerIn, 2, 1);

    QLabel *dateLabel = new QLabel("Date");
    infoLayout->addWidget(dateLabel, 3, 0);
    int year = (creationDate & 0xFFFF0000) >> 0x10;
    int month = (creationDate & 0xFF00) >> 0x08;
    int day = creationDate & 0xFF;
    QString date =
        QString("%1/%2/%3").arg(day, 2, 16, QChar('0')).arg(month, 2, 16, QChar('0')).arg(year, 4, 16, QChar('0'));
    QLineEdit *dateIn = new QLineEdit(date);
    connect(dateIn, SIGNAL(textChanged(QString)), this, SLOT(setCreationDate(QString)));
    infoLayout->addWidget(dateIn, 3, 1);

    QCheckBox *compressCheckBox = new QCheckBox("isCompressed");
    compressCheckBox->setChecked(isCompressed());
    connect(compressCheckBox, SIGNAL(clicked(bool)), this, SLOT(setCompressed(bool)));
    infoLayout->addWidget(compressCheckBox, 4, 1);

    infos->setLayout(infoLayout);
    infos->setMinimumWidth(200);
    infos->setMaximumWidth(200);

    return infos;
}

int OnexTreeZlibItem::onExporAsOriginal() {
    QString fileName = getSaveDirectory(this->getName(), "NOS Archive (*.NOS)");

    if (fileName.isEmpty())
        return 0;

    if (!fileName.endsWith(".NOS"))
        fileName += ".NOS";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(NULL, "Woops", "Couldn't open this file for writing");
        return 0;
    }

    if (file.write(opener->encrypt(this)) == -1) {
        QMessageBox::critical(NULL, "Woops", "Couldn't write to this file");
        return 0;
    }

    file.close();
    QMessageBox::information(NULL, "Yeah", "File exported into .NOS");

    return 1;
}

int OnexTreeZlibItem::getId() {
    return id;
}

int OnexTreeZlibItem::getCreationDate() {
    return creationDate;
}

bool OnexTreeZlibItem::isCompressed() {
    return compressed;
}

QByteArray OnexTreeZlibItem::getHeader() {
    return header;
}

void OnexTreeZlibItem::setId(int id) {
    this->id = id;
}

void OnexTreeZlibItem::setId(QString id) {
    setId(id.toInt());
}

void OnexTreeZlibItem::setCreationDate(QString date) {
    QStringList parts = date.split("/", QString::SplitBehavior::SkipEmptyParts);
    if (parts.size() != 3)
        this->creationDate = 0;
    else {
        int year = parts[0].toInt() << 0x10;
        int month = parts[1].toInt() << 0x08;
        int day = parts[2].toInt();
        this->creationDate = year + month + day;
    }
}

void OnexTreeZlibItem::setCompressed(bool compressed) {
    this->compressed = compressed;
}

void OnexTreeZlibItem::setHeader(QString header) {
    this->header = header.toLocal8Bit();
}

OnexTreeZlibItem::~OnexTreeZlibItem() {
}
