#pragma once

#include <wx/dialog.h>
#include <wx/wx.h>

#include <functional>

class ImportDialog : public wxDialog
{
public:
	explicit ImportDialog(std::function<void(wxString)> imported_cb);

private:
	void OnFileImport();
	void OnOkButton();
	std::function<void(wxString)> m_imported_cb;
	wxTextCtrl *mp_text_ctrl;
};
