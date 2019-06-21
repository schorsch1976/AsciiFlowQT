#include "about.h"
#include "ui_about.h"

#include <QDebug>
#include <QDesktopServices>

#define STRINGIFY(string) LOCALSTRING(string)
#define LOCALSTRING(string) #string

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About)
{
	ui->setupUi(this);

	connect(ui->btnOk, &QPushButton::clicked, [this]() { this->close(); });

	// Ascii flow
	ui->labelAsciiFlowVersion->setText(STRINGIFY(GIT_COMMIT_HASH));
	ui->labelAsciiFlowBranch->setText(STRINGIFY(GIT_BRANCH));
	ui->labelAsciiFlowDescription->setText(STRINGIFY(GIT_DESCRIPTION));

	connect(ui->commandLinkButtonHomeAsciiFlow, &QCommandLinkButton::pressed,
			[this]() {
				QString link = ui->commandLinkButtonHomeAsciiFlow->text();
				link.remove(0, 1);
				bool res = QDesktopServices::openUrl(QUrl(link));
				if (!res)
				{
					qDebug() << "Failed to open url: " << link;
				}
			});

	// QT
	ui->labelRunningQTVersion->setText(qVersion());
	ui->labelCompiledQTVersion->setText(QT_VERSION_STR);
	connect(ui->commandLinkButtonHomeQT, &QCommandLinkButton::pressed,
			[this]() {
				QString link = ui->commandLinkButtonHomeQT->text();
				link.remove(0, 1);
				bool res = QDesktopServices::openUrl(QUrl(link));
				if (!res)
				{
					qDebug() << "Failed to open url: " << link;
				}
			});
}

About::~About() { delete ui; }
