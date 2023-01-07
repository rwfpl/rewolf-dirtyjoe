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
#include "StickyWindow.h"

StickyWindow::StickyWindow(HWND& hWnd) : 
	hWnd(hWnd),
	stickedWnd(0),
	stickedDiff(0),
	stickedBorder(border_none) {}

void StickyWindow::stickTry(HWND wnd, RECT& rt)
{
	if (0 == hWnd)
		return;

	RECT curRect;
	GetWindowRect(hWnd, &curRect);

	if ((rt.left > curRect.right - stickySize) &&
		(rt.left < curRect.right + stickySize) &&
		(((rt.top >= curRect.top) && (rt.top < curRect.bottom)) ||
		((rt.bottom >= curRect.top) && (rt.bottom < curRect.bottom))))
	{
		rt.right = curRect.right + (rt.right - rt.left);
		rt.left = curRect.right;
		stickedDiff = rt.top - curRect.top;
		stickedWnd = wnd;
		stickedBorder = border_right;
	}
	else if ((rt.right > curRect.left - stickySize) &&
		(rt.right < curRect.left + stickySize) &&
		(((rt.top >= curRect.top) && (rt.top < curRect.bottom)) ||
		((rt.bottom >= curRect.top) && (rt.bottom < curRect.bottom))))
	{
		rt.left = curRect.left - (rt.right - rt.left);
		rt.right = curRect.left;
		stickedDiff = rt.top - curRect.top;
		stickedWnd = wnd;
		stickedBorder = border_left;
	}
	else if ((rt.top > curRect.bottom - stickySize) &&
		(rt.top < curRect.bottom + stickySize) &&
		(((rt.left >= curRect.left) && (rt.left < curRect.right)) ||
		((rt.right >= curRect.left) && (rt.right < curRect.right))))
	{
		rt.bottom = curRect.bottom + (rt.bottom - rt.top);
		rt.top = curRect.bottom;
		stickedDiff = rt.left - curRect.left;
		stickedWnd = wnd;
		stickedBorder = border_bottom;
	}
	else if ((rt.bottom > curRect.top - stickySize) &&
		(rt.bottom < curRect.top + stickySize) &&
		(((rt.left >= curRect.left) && (rt.left < curRect.right)) ||
		((rt.right >= curRect.left) && (rt.right < curRect.right))))
	{
		rt.top = curRect.top - (rt.bottom - rt.top);
		rt.bottom = curRect.top;
		stickedDiff = rt.left - curRect.left;
		stickedWnd = wnd;
		stickedBorder = border_top;
	}
	else
	{
		stickedWnd = 0;
		stickedBorder = border_none;
	}
}

void StickyWindow::stickyUpdate(RECT& curRect)
{
	if ((0 != stickedWnd) && (border_none != stickedBorder))
	{
		RECT rt;
		GetWindowRect(stickedWnd, &rt);
		switch (stickedBorder)
		{
		case border_right:
			{
				int temp = rt.bottom - rt.top;
				rt.top = curRect.top + stickedDiff;
				rt.bottom = rt.top + temp;
				rt.right = curRect.right + (rt.right - rt.left);
				rt.left = curRect.right;
			}
			break;
		case border_left:
			{
				int temp = rt.bottom - rt.top;
				rt.top = curRect.top + stickedDiff;
				rt.bottom = rt.top + temp;
				rt.left = curRect.left - (rt.right - rt.left);
				rt.right = curRect.left;
			}
			break;
		case border_bottom:
			{
				int temp = rt.right - rt.left;
				rt.left = curRect.left + stickedDiff;
				rt.right = rt.left + temp;
				rt.bottom = curRect.bottom + (rt.bottom - rt.top);
				rt.top = curRect.bottom;
			}
			break;
		case border_top:
			{
				int temp = rt.right - rt.left;
				rt.left = curRect.left + stickedDiff;
				rt.right = rt.left + temp;
				rt.top = curRect.top - (rt.bottom - rt.top);
				rt.bottom = curRect.top;
			}
			break;
		}
		SetWindowPos(stickedWnd, 0, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
