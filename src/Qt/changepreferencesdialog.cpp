#include "changepreferencesdialog.h"
#include "ui_changepreferencesdialog.h"

#include <QApplication>
#include <QSettings>

static QSettings settings("schorsch-tech.de", "AsciiFlowQT");

ChangePreferencesDialog::ChangePreferencesDialog(Preferences &prefs,
												 QWidget *parent)
	: QDialog(parent), m_prefs(prefs), ui(new Ui::ChangePreferencesDialog)
{
	ui->setupUi(this);

	// button style
	ui->comboBoxButtonStyle->addItem(tr("Icon Only"));
	ui->comboBoxButtonStyle->addItem(tr("Text Only"));
	ui->comboBoxButtonStyle->addItem(tr("Text Beside Icon"));
	ui->comboBoxButtonStyle->addItem(tr("Text Under Icon"));

	ui->comboBoxButtonStyle->setCurrentIndex(m_prefs.button_style);

	// icon size
	ui->spinBoxIconSize->setRange(16, 128);
	ui->spinBoxIconSize->setValue(m_prefs.icon_size);

	// connect
	connect(ui->comboBoxButtonStyle,
			QOverload<int>::of(&QComboBox::currentIndexChanged),
			[this](int index) { m_prefs.button_style = index; });
	connect(ui->spinBoxIconSize, QOverload<int>::of(&QSpinBox::valueChanged),
			[this](int value) { m_prefs.icon_size = value; });
}

ChangePreferencesDialog::~ChangePreferencesDialog() { delete ui; }

// static helper
Preferences ChangePreferencesDialog::LoadPreferences()
{
	Preferences ret;
	ret.icon_size = settings.value("icon_size", "50").toInt();
	ret.button_style = settings.value("button_style", "3").toInt();
	return ret;
}
void ChangePreferencesDialog::SavePreferences(Preferences prefs)
{
	settings.setValue("icon_size", prefs.icon_size);
	settings.setValue("button_style", prefs.button_style);
}
