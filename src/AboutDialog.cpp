#include "AboutDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

AboutDialog::AboutDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("About Aurora Visualizer");

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Aurora Visualizer", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QLabel* versionLabel = new QLabel("Version: " + QString(APP_VERSION), this);
    versionLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(versionLabel);

    QLabel* githubLabel = new QLabel("<a href=\"https://github.com/projectM-visualizer/projectm-qt\">https://github.com/projectM-visualizer/projectm-qt</a>", this);
    githubLabel->setOpenExternalLinks(true);
    githubLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(githubLabel);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AboutDialog::accept);
    layout->addWidget(buttonBox);
}

AboutDialog::~AboutDialog()
{
}
