#include "about.h"
#include "ui_about.h"

#include <QDebug>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QUrl>

#define STRINGIFY(string) LOCALSTRING(string)
#define LOCALSTRING(string) #string

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About)
{
	ui->setupUi(this);

	connect(ui->btnOk, &QPushButton::clicked, [this]() { this->close(); });

	auto open_link = [](QString link)
	{
		QRegularExpression ex("&");
		link.remove(ex);
		bool res = QDesktopServices::openUrl(QUrl(link));
		if (!res)
		{
			qDebug() << "Failed to open url: " << link;
		}
	};

	// Ascii flow
	ui->labelAsciiFlowVersion->setText(STRINGIFY(GIT_COMMIT_HASH));
	ui->labelAsciiFlowBranch->setText(STRINGIFY(GIT_BRANCH));
	ui->labelAsciiFlowDescription->setText(STRINGIFY(GIT_DESCRIPTION));

	connect(ui->commandLinkButtonHomeAsciiFlow, &QCommandLinkButton::pressed,
			[this, open_link]()
			{ open_link(ui->commandLinkButtonHomeAsciiFlow->text()); });
	connect(ui->commandLinkButtonAsciiFlowLicence, &QCommandLinkButton::pressed,
			[this, open_link]()
			{ open_link(ui->commandLinkButtonAsciiFlowLicence->text()); });

	// QT
	ui->labelRunningQTVersion->setText(qVersion());
	ui->labelCompiledQTVersion->setText(QT_VERSION_STR);

	connect(ui->commandLinkButtonHomeQT, &QCommandLinkButton::pressed,
			[this, open_link]()
			{ open_link(ui->commandLinkButtonHomeQT->text()); });
	connect(ui->commandLinkButtonQTLicence, &QCommandLinkButton::pressed,
			[this, open_link]()
			{ open_link(ui->commandLinkButtonQTLicence->text()); });
}

About::~About() { delete ui; }
