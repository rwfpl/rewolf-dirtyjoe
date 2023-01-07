/*
* dirtyJOE - Java Overall Editor
* Copyright 2011 ReWolf
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

#include "../common/PyJOEWrapper.h"
#include "../common/common_macros.h"

#define DEFINE_BOOL_SETTING(_name, _def) \
	private: \
	bool bool_setting_##_name; \
	static const bool bool_setting_default_##_name = _def; \
	public: \
	bool get_##_name() const { return bool_setting_##_name; } \
	void set_##_name(bool v) { bool_setting_##_name = v; }

#define READ_BOOL_SETTING(_name) \
	if (readSettingInt(L#_name, tmp)) \
		bool_setting_##_name = tmp ? true : false; \
	else \
		bool_setting_##_name = bool_setting_default_##_name;

#define WRITE_BOOL_SETTING(_name) \
	if (!writeSetting(L#_name, bool_setting_##_name ? L"1" : L"0")) \
		return false;

#define SET_BOOL_DEFAULT(_name) set_##_name(bool_setting_default_##_name)

class CDJSettings
{
	public:
		enum PyVer
		{
			PyNone = 0, 
			Python25 = 25, 
			Python26 = 26,
			Python27 = 27
		};

		enum BoolSettings
		{
			CopyAddr,
			CopyHex,
			CopyDisasm,
			CPHexIdx,
			CPCaseSensitive,
			StartupUpdateCheck,
			ShellExtension,
			LBCaseSensitive
		};

		DEFINE_BOOL_SETTING(CopyAddr, true);
		DEFINE_BOOL_SETTING(CopyHex, true);
		DEFINE_BOOL_SETTING(CopyDisasm, true);
		DEFINE_BOOL_SETTING(CPHexIdx, false);
		DEFINE_BOOL_SETTING(CPCaseSensitive, false);
		DEFINE_BOOL_SETTING(StartupUpdateCheck, true);
		DEFINE_BOOL_SETTING(ShellExtension, false);
		DEFINE_BOOL_SETTING(LBCaseSensitive, false);

	private:
		static const int pycnt = 3;
		static const struct __pylib
		{
			const wchar_t* pyJoeLibName;
			const wchar_t* pythonDllName;
			PyVer pyVer;
		} pylibs[pycnt];

		PyVer pythonVersion;

		PyVer loadDefaultPython_internal();
		bool loadDefaultFont();
		bool writeSetting(const wchar_t* key, const wchar_t* value);
		bool readSettingStr(const wchar_t* key, wchar_t* value, size_t valueSize);
		malloc_ptr<wchar_t> readSettingStr(const wchar_t* key);
		bool readSettingInt(const wchar_t* key, int& value);

		PyJOEWrapper pyjoewrapper;
		HMODULE hPyJOE;
		HMODULE hPython;
		HFONT hMonoFont;

		bool error;

		static const int maxRexentFiles = 10;
		wchar_t* recentFiles[maxRexentFiles];

		malloc_ptr<wchar_t> iniFileName;

	public:
		CDJSettings();
		~CDJSettings();
		bool getError() const;

		HFONT getFontHandle() const;
		void setFontHandle(HFONT hFont);

		bool saveSettings();
		bool loadSettings();
		bool loadDefault();

		PyVer getPythonVersion() const;
		bool loadReqPython(PyVer v);
		void loadDefaultPython();
		PyJOEWrapper& getPyModule();

		void removeRecentFile(wchar_t* rf);
		void addRecentFile(const wchar_t* rf);
		wchar_t* getRecentFile(int idx) const;
		int getMaxRecentFiles() const;

		bool shellExtAsked;
};
