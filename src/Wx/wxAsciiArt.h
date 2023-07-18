#pragma once

#include "AsciiFlowCommon/AsciiArtData.h"
#include "AsciiFlowCommon/ITool.h"

#include <wx/wx.h>

#include <wx/scrolwin.h>
#include <wx/sharedptr.h>

#include <functional>

class wxAsciiArt : public wxScrolledWindow
{
public:
	static constexpr int CELLSIZE = 12;
	static constexpr int FONTSIZE = CELLSIZE - 4;

	wxAsciiArt();
	wxAsciiArt(wxWindow *parent);
	bool Create(wxWindow *parent);

protected:
	void Init();
	virtual wxSize DoGetBestSize() const override;
	void OnPaint(wxPaintEvent &);
	void OnResize(wxSizeEvent &event);

	void OnKeyPressed(wxKeyEvent &event);
	void OnChar(wxKeyEvent &event);

	void OnMouseBtnPressed(wxMouseEvent &event);
	void OnMouseMoved(wxMouseEvent &event);
	void OnMouseBtnReleased(wxMouseEvent &event);

public:
	void ActivateToolHelper(Common::Tool tool);
	Common::Tool CurrentTool() const;

	void Clear();
	wxString ExportData() const;
	void ImportData(const wxString &import);

	void Undo();
	void Redo();

	void OnUndoAvailable(std::function<void(bool)> cb);
	void OnRedoAvailable(std::function<void(bool)> cb);

private:
	wxPoint ScreenToText(wxPoint point);
	wxPoint TextToScreen(wxPoint point);

	void EnsureVisibility(wxPoint p);

	wxSharedPtr<Common::ITool> mp_current_tool;
	Common::Tool m_current_tool;

	// sliding
	wxPoint m_slide_start;

	// The data area
	wxPoint m_start;
	Common::AsciiArtData m_data;
	Common::AsciiArtData m_data_backup;

	wxVector<wxString> m_undo;
	wxVector<wxString> m_redo;

	// for drawing
	wxFont m_font;
	bool m_btn_pressed;

	std::function<void(bool)> m_undo_avail_cb;
	std::function<void(bool)> m_redo_avail_cb;

	wxDECLARE_DYNAMIC_CLASS(wxAsciiArt);
	wxDECLARE_EVENT_TABLE();
};
