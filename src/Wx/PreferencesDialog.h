#pragma once

#include <wx/dialog.h>
#include <wx/wx.h>

#include <wx/choice.h>
#include <wx/config.h>
#include <wx/spinctrl.h>

enum class ButtonStyle
{
	TextUnderIcon = 0,
	TextBesidesIcon,
	IconOnly,
	TextOnly
};

class PreferencesDialog : public wxDialog
{
public:
	PreferencesDialog();

	ButtonStyle GetButtonStyle();
	int GetIconSize() const;

private:
	void OnChangeStyle(wxCommandEvent &evt);
	void OnChangeIconSize(wxSpinEvent &evt);

	void Read();
	void Write();

	wxConfig m_config;

	ButtonStyle m_style;
	int m_icon_size;

	wxChoice *mp_choice;
	wxSpinCtrl *mp_spinctl;
};
