#pragma once

#include <wx/tglbtn.h>
#include <wx/toolbar.h>
#include <wx/wx.h>

#include "wxAsciiArt.h"

#include <unordered_map>

enum
{
	ID_ButtonNew = 1,
	ID_ButtonImport,
	ID_ButtonExport,

	ID_ButtonUndo,
	ID_ButtonRedo,

	ID_ButtonPreferences,
	ID_ButtonAbout,

	ID_ButtonToolMove,
	ID_ButtonToolRectangle,
	ID_ButtonToolResize,
	ID_ButtonToolClass,
	ID_ButtonToolArrow,
	ID_ButtonToolLine,
	ID_ButtonToolText,
	ID_ButtonToolErase,
	ID_ButtonToolFreehand
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
	void OnClose(wxCloseEvent &event);
	void LoadBitmaps();
	void ResizeBitmaps(int size);
	void ApplyPrefs();

	void CreateTopToolbar();
	void CreateLeftToolbar();
	void AddTopTools();
	void AddLeftTools();

	//////////////////////////////////////////////////////////////////////////////
	// File stuff
	//////////////////////////////////////////////////////////////////////////////
	void OnFileNew(wxCommandEvent &event);
	void OnImport(wxCommandEvent &event);
	void OnExport(wxCommandEvent &event);

	void OnUndo(wxCommandEvent &event);
	void OnRedo(wxCommandEvent &event);

	void OnPreferences(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);

	//////////////////////////////////////////////////////////////////////////////
	// Tools
	//////////////////////////////////////////////////////////////////////////////
	void OnToolMove(wxCommandEvent &event);
	void OnToolRectangle(wxCommandEvent &event);
	void OnToolResize(wxCommandEvent &event);
	void OnToolClass(wxCommandEvent &event);
	void OnToolArrow(wxCommandEvent &event);
	void OnToolLine(wxCommandEvent &event);
	void OnToolText(wxCommandEvent &event);
	void OnToolErase(wxCommandEvent &event);
	void OnToolFreehand(wxCommandEvent &event);

	void ActivateToolHelper(Common::Tool tool);

	wxAsciiArt *mp_asciiart;
	// std::unordered_map<Tool, wxToggleButton *> m_tool2btn;

	enum class Btn
	{
		New = 0,
		Import,
		Export,
		Undo,
		Redo,
		Prefs,
		About,

		Move,
		Rectangle,
		Resize,
		Class,
		Arrow,
		Line,
		Text,
		Freehand,
		Erase
	};
	// std::unordered_map<Btn, wxAnyButton *> m_btns;
	std::unordered_map<Btn, wxBitmap> m_raw_bitmap;
	std::unordered_map<Btn, wxBitmap> m_resized_bitmap;

	wxToolBar *mp_top_toolbar;
	wxToolBar *mp_left_toolbar;
	bool m_undo_avail;
	bool m_redo_avail;

	wxDECLARE_EVENT_TABLE();
};
