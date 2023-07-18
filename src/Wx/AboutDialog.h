#pragma once

#include <wx/commandlinkbutton.h>
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/wx.h>

class AboutDialog : public wxDialog
{
public:
	AboutDialog();

private:
	wxNotebookPage *CreatePageAsciiFlowWx(wxNotebook *parent);
	wxNotebookPage *CreatePageWxWidgets(wxNotebook *parent);

	wxCommandLinkButton *AddLink(wxNotebookPage *page, const wxString &url);
};
