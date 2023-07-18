#ifndef CHANGEPREFERENCESDIALOG_H
#define CHANGEPREFERENCESDIALOG_H

#include <QDialog>

namespace Ui
{
class ChangePreferencesDialog;
}

struct Preferences
{
	int icon_size = 50;
	int button_style = 0;
};

class ChangePreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ChangePreferencesDialog(Preferences &prefs,
									 QWidget *parent = nullptr);
	~ChangePreferencesDialog();

	// static helper
	static Preferences LoadPreferences();
	static void SavePreferences(Preferences prefs);

private:
	Preferences &m_prefs;
	Ui::ChangePreferencesDialog *ui;
};

#endif // CHANGEPREFERENCESDIALOG_H
