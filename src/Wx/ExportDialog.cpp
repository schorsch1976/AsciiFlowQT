#include "ExportDialog.h"

#include <wx/filedlg.h>
#include <wx/stattext.h>
#include <wx/wfstream.h>

ExportDialog::ExportDialog(const wxString &data)
	: wxDialog(nullptr, -1, "Export...", wxDefaultPosition, wxSize(250, 230)),
	  m_data(data)
{
	auto *mainsizer = new wxBoxSizer(wxVERTICAL);

	auto *hbox_just = new wxBoxSizer(wxHORIZONTAL);
	hbox_just->AddStretchSpacer(1);
	hbox_just->Add(new wxStaticText(this, 0, "Just copy to your clipboard..."));
	hbox_just->AddStretchSpacer(1);
	mainsizer->Add(hbox_just, 0, wxEXPAND | wxCENTRE | wxALL, 5);
	mainsizer->Add(new wxTextCtrl(this, 0, data, wxDefaultPosition,
								  wxSize(400, 300),
								  wxTE_MULTILINE | wxHSCROLL | wxVSCROLL),
				   1, wxEXPAND | wxALL, 5);

	auto *hbox_or = new wxBoxSizer(wxHORIZONTAL);
	hbox_or->AddStretchSpacer(1);
	hbox_or->Add(new wxStaticText(this, 0, "or"));
	hbox_or->AddStretchSpacer(1);
	mainsizer->Add(hbox_or, 0, wxEXPAND | wxCENTRE | wxALL, 5);

	auto *save_btn = new wxButton(this, 0, "Save to file ...");
	mainsizer->Add(save_btn, 0, wxEXPAND | wxCENTRE | wxALL, 5);
	SetSizerAndFit(mainsizer);

	save_btn->Bind(wxEVT_BUTTON,
				   [this](wxCommandEvent &)
				   {
					   wxFileDialog saveDlg(this, ("Save TXT1 file"), "", "",
											"TXT files (*.txt)|*.txt",
											wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

					   if (wxID_CANCEL == saveDlg.ShowModal())
					   {
						   return;
					   }

					   // save the current contents in the file;
					   // this can be done with e.g. wxWidgets output streams:
					   wxFileOutputStream output_stream(saveDlg.GetPath());
					   if (!output_stream.IsOk())
					   {
						   wxLogError(
							   "Cannot save current contents in file '%s'.",
							   saveDlg.GetPath());
						   return;
					   }
					   output_stream.GetFile()->Write(m_data);
				   });
}
