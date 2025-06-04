#include "main.h"
#include "rc.h"
#include <filesystem>

using namespace Gdiplus;

main_window::main_window() {
	TCHAR file[MAX_PATH] = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = *this;
	ofn.lpstrFile = file;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = _T("Images\0*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.tif;*.tiff;*.emf\0All files\0*.*\0");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
};

void main_window::draw_filename(Gdiplus::Graphics* graphics, RECT rc) {
	Gdiplus::Font font(L"Times New Roman", 16);
	
	Gdiplus::SolidBrush wb(Gdiplus::Color::White);
	Gdiplus::SolidBrush bb(Gdiplus::Color::Black);

	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentFar);

	Gdiplus::RectF rcf(rc.left, rc.top, rc.right, rc.bottom);

	graphics->DrawString(file_name.c_str(), -1, &font, rcf, &format, &bb);

	rcf.Width = rc.right - 1;
	rcf.Height = rc.bottom - 1;

	graphics->DrawString(file_name.c_str(), -1, &font, rcf, &format, &wb);
}

void main_window::on_paint(HDC hdc) 
{
	if (image) {
		Gdiplus::Graphics g(hdc);
		
		RECT rc;
		GetClientRect(*this, &rc);
		
		auto h = rc.bottom - rc.top;
		auto w = rc.right - rc.left;

		g.DrawImage(image.get(), 0, 0, w, h);
		draw_filename(&g, rc);
	}
}

void main_window::on_command(int id) 
{
	switch (id) 
	{
		case ID_OPEN:
			if (GetOpenFileName(&ofn)) {
				image = std::make_unique<Gdiplus::Image>(ofn.lpstrFile);
				file_name = std::filesystem::path(ofn.lpstrFile).filename();
				InvalidateRect(*this, nullptr, TRUE);
			}
			break;
		case ID_EXIT:
			DestroyWindow(*this);
			break;
	}
}

void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
