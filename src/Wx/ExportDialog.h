#pragma once

#include <wx/dialog.h>
#include <wx/wx.h>

class ExportDialog : public wxDialog
{
public:
	explicit ExportDialog(const wxString &data);

private:
	wxString m_data;
};
