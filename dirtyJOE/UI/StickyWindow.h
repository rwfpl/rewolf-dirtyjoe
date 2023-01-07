/*
 * dirtyJOE - Java Overall Editor
 * Copyright 2014 ReWolf
 * Contact:
 * rewolf@dirty-joe.com
 * http://dirty-joe.com
 *
 * --
 *
 * see license.txt for more info
 *
 */
#pragma once
#include <Windows.h>

class StickyWindow
{
	public:
		enum stickyBorder
		{
			border_none,
			border_left,
			border_right,
			border_top,
			border_bottom
		};

		static const int stickySize = 6;

		StickyWindow::StickyWindow(HWND& hWnd);
		void stickyUpdate(RECT& curRect);
		void stickTry(HWND wnd, RECT& rt);

	private:
		const HWND& hWnd;
		HWND stickedWnd;
		stickyBorder stickedBorder;
		int stickedDiff;

		StickyWindow::StickyWindow();
		StickyWindow& StickyWindow::operator=(const StickyWindow&);
};