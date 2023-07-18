#include "MainFrame.h"

#include "AboutDialog.h"
#include "ExportDialog.h"
#include "ImportDialog.h"
#include "PreferencesDialog.h"

#include <wx/gdicmn.h>
#include <wx/msgdlg.h>

namespace XPM
{
#include "AsciiFlowCommon/ico/About.png.xpm"
#include "AsciiFlowCommon/ico/Arrow.png.xpm"
#include "AsciiFlowCommon/ico/Class.png.xpm"
#include "AsciiFlowCommon/ico/Erase.png.xpm"
#include "AsciiFlowCommon/ico/Export.png.xpm"
#include "AsciiFlowCommon/ico/Freehand.png.xpm"
#include "AsciiFlowCommon/ico/Import.png.xpm"
#include "AsciiFlowCommon/ico/Line.png.xpm"
#include "AsciiFlowCommon/ico/Move.png.xpm"
#include "AsciiFlowCommon/ico/New.png.xpm"
#include "AsciiFlowCommon/ico/Prefs.png.xpm"
#include "AsciiFlowCommon/ico/Rectangle.png.xpm"
#include "AsciiFlowCommon/ico/Redo.png.xpm"
#include "AsciiFlowCommon/ico/Resize.png.xpm"
#include "AsciiFlowCommon/ico/Text.png.xpm"
#include "AsciiFlowCommon/ico/Undo.png.xpm"
} // namespace XPM

MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
					 const wxSize &size)
	: wxFrame(nullptr, wxID_ANY, title, pos, size), mp_top_toolbar(nullptr),
	  mp_left_toolbar(nullptr), m_undo_avail(false), m_redo_avail(false)
{
	CreateTopToolbar();
	CreateLeftToolbar();

	//////////////////////////////////////////////////////////////////////////////
	// Create the layout
	//////////////////////////////////////////////////////////////////////////////
	auto *mainsizer = new wxBoxSizer(wxVERTICAL);
	mainsizer->Add(mp_top_toolbar, 0, wxEXPAND | wxALL);

	//////////////////////////////////////////////////////////////////////////////
	// Mid Line (including drawing area)
	//////////////////////////////////////////////////////////////////////////////
	auto *midsizer = new wxBoxSizer(wxHORIZONTAL);
	mainsizer->Add(midsizer, 1, wxEXPAND | wxALL);
	midsizer->Add(mp_left_toolbar, 0, wxEXPAND | wxALL);

	// also adds the tool buttons
	LoadBitmaps();
	ApplyPrefs();

	//////////////////////////////////////////////////////////////////////////////
	// Mid: Drawing area
	//////////////////////////////////////////////////////////////////////////////
	mp_asciiart = new wxAsciiArt(this);
	wxASSERT(mp_asciiart);
	midsizer->Add(mp_asciiart, 1, wxEXPAND | wxALL);

	SetSizerAndFit(mainsizer); // use the sizer for layout and size window
							   // accordingly and prevent it from being resized
							   // to smaller size

	Maximize();

	ActivateToolHelper(Common::Tool::Rectangle);

	//////////////////////////////////////////////////////////////////////////////
	// Connect the callbacks (undo/redo avail)
	//////////////////////////////////////////////////////////////////////////////
	wxASSERT(mp_asciiart);
	mp_asciiart->OnUndoAvailable(
		[this](bool avail)
		{
			m_undo_avail = avail;
			mp_top_toolbar->EnableTool(ID_ButtonUndo, m_undo_avail);
		});
	mp_asciiart->OnRedoAvailable(
		[this](bool avail)
		{
			m_redo_avail = avail;
			mp_top_toolbar->EnableTool(ID_ButtonRedo, m_redo_avail);
		});
}

void MainFrame::OnClose(wxCloseEvent &event)
{
	wxASSERT(mp_asciiart);
	wxString exported = mp_asciiart->ExportData();
	if (not exported.empty())
	{
		if (wxYES !=
			wxMessageBox("Data area contains data", "Are you sure?",
						 wxYES_NO | wxCENTRE | wxICON_QUESTION | wxNO_DEFAULT))
		{
			event.Veto();
			return;
		}
	}

	Destroy();
}

void MainFrame::LoadBitmaps()
{
	// we always use XPMs on all platforms
	m_raw_bitmap[Btn::About] =
		wxBitmap(wxImage(XPM::About_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Arrow] =
		wxBitmap(wxImage(XPM::Arrow_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Class] =
		wxBitmap(wxImage(XPM::Class_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Erase] =
		wxBitmap(wxImage(XPM::Erase_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Export] =
		wxBitmap(wxImage(XPM::Export_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Freehand] =
		wxBitmap(wxImage(XPM::Freehand_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Import] =
		wxBitmap(wxImage(XPM::Import_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Line] =
		wxBitmap(wxImage(XPM::Line_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Move] =
		wxBitmap(wxImage(XPM::Move_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::New] = wxBitmap(wxImage(XPM::New_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Prefs] =
		wxBitmap(wxImage(XPM::Prefs_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Rectangle] =
		wxBitmap(wxImage(XPM::Rectangle_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Redo] =
		wxBitmap(wxImage(XPM::Redo_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Resize] =
		wxBitmap(wxImage(XPM::Resize_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Text] =
		wxBitmap(wxImage(XPM::Text_xpm), wxBITMAP_TYPE_XPM);
	m_raw_bitmap[Btn::Undo] =
		wxBitmap(wxImage(XPM::Undo_xpm), wxBITMAP_TYPE_XPM);
}
void MainFrame::ResizeBitmaps(int size)
{
	wxSize target_size(size, size);
	for (auto &c : m_raw_bitmap)
	{
		Btn btn = c.first;

		// calculate the new one
		const wxBitmap &original_bmp = c.second;
		wxImage original_image = original_bmp.ConvertToImage();

		wxImage scaled_image =
			original_image.Scale(target_size.GetWidth(),
								 target_size.GetHeight(), wxIMAGE_QUALITY_HIGH);

		m_resized_bitmap[btn] = wxBitmap(scaled_image);
	}
}

void MainFrame::ApplyPrefs()
{
	PreferencesDialog prefs;
	int icon_size = prefs.GetIconSize();
	wxLogDebug("IconSize: %d", icon_size);
	ButtonStyle style = prefs.GetButtonStyle();

	wxASSERT(mp_top_toolbar && mp_left_toolbar);
	mp_top_toolbar->ClearTools();
	mp_left_toolbar->ClearTools();

	ResizeBitmaps(icon_size);
	wxSize target_size(icon_size, icon_size);
	mp_top_toolbar->SetToolBitmapSize(target_size);
	mp_left_toolbar->SetToolBitmapSize(target_size);

	long top_style = mp_top_toolbar->GetWindowStyle();
	long left_style = mp_left_toolbar->GetWindowStyle();

	switch (style)
	{
		case ButtonStyle::IconOnly:
			top_style &= ~wxTB_TEXT;
			top_style &= ~wxTB_NOICONS;
			top_style &= ~wxTB_HORZ_LAYOUT;

			left_style &= ~wxTB_TEXT;
			left_style &= ~wxTB_NOICONS;
			left_style &= ~wxTB_HORZ_LAYOUT;
			break;

		case ButtonStyle::TextOnly:
			top_style |= wxTB_TEXT;
			top_style |= wxTB_NOICONS;
			top_style &= ~wxTB_HORZ_LAYOUT;

			left_style |= wxTB_TEXT;
			left_style |= wxTB_NOICONS;
			left_style &= ~wxTB_HORZ_LAYOUT;
			break;

		case ButtonStyle::TextBesidesIcon:
			top_style |= wxTB_TEXT;
			top_style &= ~wxTB_NOICONS;
			top_style |= wxTB_HORZ_LAYOUT;

			left_style |= wxTB_TEXT;
			left_style &= ~wxTB_NOICONS;
			left_style |= wxTB_HORZ_LAYOUT;
			break;

		default:
		case ButtonStyle::TextUnderIcon:
			top_style |= wxTB_TEXT;
			top_style &= ~wxTB_NOICONS;
			top_style &= ~wxTB_HORZ_LAYOUT;

			left_style |= wxTB_TEXT;
			left_style &= ~wxTB_NOICONS;
			left_style &= ~wxTB_HORZ_LAYOUT;
			break;
	}
	mp_top_toolbar->SetWindowStyle(top_style);
	mp_left_toolbar->SetWindowStyle(left_style);

	AddTopTools();
	AddLeftTools();

	mp_top_toolbar->Realize();
	mp_left_toolbar->Realize();

	// force a new Layout
	Refresh();
	Layout();
}

void MainFrame::CreateTopToolbar()
{
	long style = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;
	style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL | wxTB_BOTTOM | wxTB_RIGHT |
			   wxTB_HORZ_LAYOUT);
	style |= wxTB_TOP;

	delete mp_top_toolbar;
	mp_top_toolbar =
		new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxASSERT(mp_top_toolbar);
}

void MainFrame::CreateLeftToolbar()
{
	long style = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;
	style &= ~(wxTB_HORIZONTAL | wxTB_VERTICAL | wxTB_BOTTOM | wxTB_RIGHT |
			   wxTB_HORZ_LAYOUT);
	style |= wxTB_LEFT;

	if (mp_left_toolbar != nullptr)
	{
		delete mp_left_toolbar;
	}
	mp_left_toolbar =
		new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, style);
	wxASSERT(mp_left_toolbar);
}

void MainFrame::AddTopTools()
{
	wxASSERT(mp_top_toolbar);

	mp_top_toolbar->AddTool(ID_ButtonNew, "New", m_resized_bitmap[Btn::New],
							wxNullBitmap, wxITEM_NORMAL, "Create new file");

	mp_top_toolbar->AddTool(ID_ButtonImport, "Import",
							m_resized_bitmap[Btn::Import], wxNullBitmap,
							wxITEM_NORMAL, "Import file or clipboard");

	mp_top_toolbar->AddTool(ID_ButtonExport, "Export",
							m_resized_bitmap[Btn::Export], wxNullBitmap,
							wxITEM_NORMAL, "Export to file or clipboard");

	mp_top_toolbar->AddTool(ID_ButtonUndo, "Undo", m_resized_bitmap[Btn::Undo],
							wxNullBitmap, wxITEM_NORMAL, "Revert a change");

	mp_top_toolbar->AddTool(ID_ButtonRedo, "Redo", m_resized_bitmap[Btn::Redo],
							wxNullBitmap, wxITEM_NORMAL, "Redo a change");

	mp_top_toolbar->AddStretchableSpace();

	mp_top_toolbar->AddTool(ID_ButtonPreferences, "Preferences",
							m_resized_bitmap[Btn::Prefs], wxNullBitmap,
							wxITEM_NORMAL, "Open Preference dialog");

	mp_top_toolbar->AddTool(ID_ButtonAbout, "About",
							m_resized_bitmap[Btn::About], wxNullBitmap,
							wxITEM_NORMAL, "Open the About dialog");

	mp_top_toolbar->Realize();

	wxASSERT(mp_top_toolbar);
	mp_top_toolbar->EnableTool(ID_ButtonUndo, m_undo_avail);
	mp_top_toolbar->EnableTool(ID_ButtonRedo, m_redo_avail);
}

void MainFrame::AddLeftTools()
{
	wxASSERT(mp_left_toolbar);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolRectangle, "Rectangle",
								  m_resized_bitmap[Btn::Rectangle]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolResize, "Resize",
								  m_resized_bitmap[Btn::Resize]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolClass, "Class",
								  m_resized_bitmap[Btn::Class]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolArrow, "Arrow",
								  m_resized_bitmap[Btn::Arrow]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolLine, "Line",
								  m_resized_bitmap[Btn::Line]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolText, "Text",
								  m_resized_bitmap[Btn::Text]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolErase, "Erase",
								  m_resized_bitmap[Btn::Erase]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolFreehand, "Freehand",
								  m_resized_bitmap[Btn::Freehand]);
	mp_left_toolbar->AddRadioTool(ID_ButtonToolMove, "Move",
								  m_resized_bitmap[Btn::Move]);

	mp_left_toolbar->Realize();
}

////////////////////////////////////////////////////////////////////////////////
// File stuff
////////////////////////////////////////////////////////////////////////////////
void MainFrame::OnFileNew(wxCommandEvent &event)
{
	wxASSERT(mp_asciiart);
	wxString exported = mp_asciiart->ExportData();
	if (not exported.empty())
	{
		if (wxYES ==
			wxMessageBox("Data area contains data", "Are you sure?",
						 wxYES_NO | wxCENTRE | wxICON_QUESTION | wxNO_DEFAULT))
		{
			mp_asciiart->Clear();
		}
	}
}
void MainFrame::OnImport(wxCommandEvent &event)
{
	auto cb = [this](const wxString &data)
	{
		if (not data.empty())
		{
			mp_asciiart->ImportData(data);
		}
	};

	ImportDialog dlg(cb);
	dlg.ShowModal();
}
void MainFrame::OnExport(wxCommandEvent &event)
{
	ExportDialog dlg(mp_asciiart->ExportData());
	dlg.ShowModal();
}

void MainFrame::OnUndo(wxCommandEvent &event)
{
	wxASSERT(mp_asciiart);
	mp_asciiart->Undo();
}
void MainFrame::OnRedo(wxCommandEvent &event)
{
	wxASSERT(mp_asciiart);
	mp_asciiart->Redo();
}

void MainFrame::OnPreferences(wxCommandEvent &event)
{
	{
		PreferencesDialog dlg;
		dlg.ShowModal();
	}

	ApplyPrefs();
}
void MainFrame::OnAbout(wxCommandEvent &event)
{
	AboutDialog dlg;
	dlg.ShowModal();
}

////////////////////////////////////////////////////////////////////////////////
// Tools
////////////////////////////////////////////////////////////////////////////////
void MainFrame::OnToolMove(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Move);
}
void MainFrame::OnToolRectangle(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Rectangle);
}
void MainFrame::OnToolResize(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Resize);
}
void MainFrame::OnToolClass(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Class);
}
void MainFrame::OnToolArrow(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Arrow);
}
void MainFrame::OnToolLine(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Line);
}
void MainFrame::OnToolText(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Text);
}
void MainFrame::OnToolErase(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Erase);
}
void MainFrame::OnToolFreehand(wxCommandEvent &event)
{
	ActivateToolHelper(Common::Tool::Freehand);
}

void MainFrame::ActivateToolHelper(Common::Tool tool)
{
	wxASSERT(mp_asciiart);
	mp_asciiart->ActivateToolHelper(tool);
}

// clang-format off
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)

	EVT_TOOL(ID_ButtonNew, 			MainFrame::OnFileNew)
	EVT_TOOL(ID_ButtonImport, 		MainFrame::OnImport)
	EVT_TOOL(ID_ButtonExport, 		MainFrame::OnExport)

	EVT_TOOL(ID_ButtonUndo,			MainFrame::OnUndo)
	EVT_TOOL(ID_ButtonRedo,			MainFrame::OnRedo)

	EVT_TOOL(ID_ButtonPreferences,	MainFrame::OnPreferences)
	EVT_TOOL(ID_ButtonAbout,		MainFrame::OnAbout)

	EVT_TOOL(ID_ButtonToolMove,		MainFrame::OnToolMove)
	EVT_TOOL(ID_ButtonToolRectangle,MainFrame::OnToolRectangle)
	EVT_TOOL(ID_ButtonToolResize, 	MainFrame::OnToolResize)
	EVT_TOOL(ID_ButtonToolClass,	MainFrame::OnToolClass)
	EVT_TOOL(ID_ButtonToolArrow,	MainFrame::OnToolArrow)
	EVT_TOOL(ID_ButtonToolLine,		MainFrame::OnToolLine)
	EVT_TOOL(ID_ButtonToolText,		MainFrame::OnToolText)
	EVT_TOOL(ID_ButtonToolErase,	MainFrame::OnToolErase)
	EVT_TOOL(ID_ButtonToolFreehand,	MainFrame::OnToolFreehand)

wxEND_EVENT_TABLE()
	// clang-format on
