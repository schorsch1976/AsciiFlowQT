#include "AboutDialog.h"

#include <wx/commandlinkbutton.h>
#include <wx/mimetype.h>
#include <wx/notebook.h>
#include <wx/utils.h>
#include <wx/version.h>

static const char *ascii = R"(
     _                   _   _   _____   _                       __        __
    / \     ___    ___  (_) (_) |  ___| | |   ___   __      __   \ \      / / __  __
   / _ \   / __|  / __| | | | | | |_    | |  / _ \  \ \ /\ / /    \ \ /\ / /  \ \/ /
  / ___ \  \__ \ | (__  | | | | |  _|   | | | (_) |  \ V  V /      \ V  V /    >  <
 /_/   \_\ |___/  \___| |_| |_| |_|     |_|  \___/    \_/\_/        \_/\_/    /_/\_\)";

AboutDialog::AboutDialog()
	: wxDialog(nullptr, -1, "About", wxDefaultPosition, wxSize(250, 230))
{
	auto *mainsizer = new wxBoxSizer(wxVERTICAL);

	// the logo box
	auto *logo_box = new wxBoxSizer(wxHORIZONTAL);
	mainsizer->Add(logo_box, 0, wxALIGN_CENTER);
	auto *logo = new wxStaticText(this, -1, ascii);
	logo->SetFont(wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
						 wxFontWeight::wxFONTWEIGHT_NORMAL));
	logo_box->Add(logo);

	// the notebook
	auto *book =
		new wxNotebook(this, 0, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
	mainsizer->Add(book, 1, wxEXPAND | wxALL, 5);

	book->AddPage(CreatePageAsciiFlowWx(book), "AsciiFlowWx");
	book->AddPage(CreatePageWxWidgets(book), "wxWidgets");
	book->SetSelection(0);

	SetSizerAndFit(mainsizer);
}

wxNotebookPage *AboutDialog::CreatePageAsciiFlowWx(wxNotebook *parent)
{
	auto *page = new wxNotebookPage(parent, 0, wxDefaultPosition, wxDefaultSize,
									wxEXPAND | wxALL);

	auto *padder = new wxBoxSizer(wxVERTICAL);
	auto *sizer = new wxGridSizer(5, 2, 5, 5);
	padder->Add(sizer, 1, wxEXPAND | wxALL, 5);

	sizer->Add(new wxStaticText(page, -1, "Homepage"), 0, wxEXPAND);
	sizer->Add(AddLink(page, "https://github.com/schorsch1976/AsciiFlowQT"), 0,
			   wxEXPAND);

	sizer->Add(new wxStaticText(page, -1, "Version"), 0, wxEXPAND);
	sizer->Add(new wxStaticText(page, -1, wxSTRINGIZE(GIT_COMMIT_HASH)), 0,
			   wxEXPAND);

	sizer->Add(new wxStaticText(page, -1, "Description"), 0, wxEXPAND);
	sizer->Add(new wxStaticText(page, -1, wxSTRINGIZE(GIT_DESCRIPTION)), 0,
			   wxEXPAND);

	sizer->Add(new wxStaticText(page, -1, "Branch"), 0, wxEXPAND);
	sizer->Add(new wxStaticText(page, -1, wxSTRINGIZE(GIT_BRANCH)), 0,
			   wxEXPAND);

	sizer->Add(new wxStaticText(page, -1, "Licence (GPL-3)"), 0, wxEXPAND);
	sizer->Add(AddLink(page, "https://github.com/schorsch1976/AsciiFlowQT/blob/"
							 "master/LICENCE.txt"),
			   0, wxEXPAND);

	page->SetSizerAndFit(padder);
	return page;
}
wxNotebookPage *AboutDialog::CreatePageWxWidgets(wxNotebook *parent)
{
	auto *page = new wxNotebookPage(parent, 0, wxDefaultPosition, wxDefaultSize,
									wxEXPAND | wxALL);

	auto *padder = new wxBoxSizer(wxVERTICAL);
	auto *sizer = new wxGridSizer(4, 2, 5, 5);
	padder->Add(sizer, 1, wxEXPAND | wxALL, 5);

	sizer->Add(new wxStaticText(page, -1, "Homepage"), 0, wxEXPAND);
	sizer->Add(AddLink(page, "http://www.wxwidgets.org/"), 0, wxEXPAND);

	sizer->Add(new wxStaticText(page, -1, "Compiled Version"), 0, wxEXPAND);
	sizer->Add(new wxStaticText(page, -1, wxVERSION_STRING), 0, wxEXPAND);

	sizer->Add(new wxStaticText(page, -1, "Running Version"), 0, wxEXPAND);
	sizer->Add(new wxStaticText(page, -1,
								wxGetLibraryVersionInfo().GetVersionString()),
			   0, wxEXPAND);

	sizer->Add(
		new wxStaticText(page, -1,
						 "Licence (wxWindows Library Licence, Version 3.1)"),
		0, wxEXPAND);
	sizer->Add(AddLink(page, "https://www.wxwidgets.org/about/licence/"), 0,
			   wxEXPAND);

	page->SetSizerAndFit(padder);
	return page;
}

wxCommandLinkButton *AboutDialog::AddLink(wxNotebookPage *page,
										  const wxString &url)
{
	auto *link = new wxCommandLinkButton(page, -1, url);
	link->Bind(wxEVT_BUTTON,
			   [this, url](wxCommandEvent &)
			   {
				   wxLogDebug("Link clicked: %s", url);

				   wxLaunchDefaultBrowser(url);
			   });

	return link;
}
