'
$INCLUDE "c:\rqlibs\QDIALOGS.inc"
$INCLUDE "qtv.inc"
$INCLUDE "fontform.inc"

'This is the User Interface for the Vocabulary builder.


DECLARE SUB vocabmousemove(x%, y%, shift%)
DECLARE SUB vocabmouseup(button%, x%, y%, shift%)
DECLARE SUB vocabmousedown(button%, x%, y%, shift%)
DECLARE SUB vocabresize

DECLARE SUB fontboxachange(fontname AS STRING, size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strikethrough AS INTEGER)
DECLARE SUB fontboxbchange(fontname AS STRING, size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strikethrough AS INTEGER)
DECLARE SUB treeviewboxchange(fontname AS STRING, size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strikethrough AS INTEGER)
DECLARE SUB okbuttonclick

DECLARE SUB keyup(key AS WORD, shift AS INTEGER)				'Rich Edit Declarations.
DECLARE SUB copytoclipboard
DECLARE SUB pastefromclipboard
DECLARE SUB cuttoclipboard
DECLARE SUB setdelfontstyle(styles AS INTEGER)
DECLARE SUB rightjustify
DECLARE SUB centerjustify
DECLARE SUB leftjustify
DECLARE SUB initfont

DECLARE SUB editororviewer
DECLARE SUB scrollbarsonoff
DECLARE SUB wordwraponoff
DECLARE SUB keypress

DECLARE SUB changefont(wchfnt AS BYTE, fntnm AS STRING, fntsz AS INTEGER)
'DECLARE SUB setupoptions

DECLARE SUB setupallfonts

TYPE myform EXTENDS QFORM
	button AS INTEGER
END TYPE
'QFONT setup.

DIM treeviewfont AS QFONT
DIM richeditfont AS QFONT
DIM dlg AS QDIALOGS
treeviewfont.charset = 4
richeditfont.charset = 4

DIM whichfontisused AS BYTE

DIM textfileloaded AS STRING

fontbox.onchange = fontboxachange
fontboxb.onchange = fontboxbchange
treeviewfontbox.onchange = treeviewboxchange
FONTOKBUTTON.onclick = okbuttonclick
'SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
CREATE vocabform AS myform
	caption = "Vocabulary Builder 6.00.00"
	width = 610
	height = 470
	center
	cursor = -9
	onresize = vocabresize
	onmousedown = vocabmousedown
	onmousemove = vocabmousemove
	onmouseup = vocabmouseup

	
	CREATE mainmenu AS QMAINMENU
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'				File
		CREATE filemenu AS QMENUITEM
			Caption = "&Lesson"
			CREATE openmenu AS QMENUITEM
				caption = "Open"
			END CREATE
			CREATE newmenu AS QMENUITEM
				caption = "New"
			END CREATE
			CREATE saveasmenu AS QMENUITEM
				caption = "Save As"
			END CREATE
			CREATE filespacermenu AS QMENUITEM
				caption = "-"
			END CREATE
			CREATE exitmenu AS QMENUITEM
				caption = "Exit"
			END CREATE
		END CREATE
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'		Edit
		CREATE editmenu AS QMENUITEM
			caption = "&Edit"
			CREATE copymenu AS QMENUITEM
				caption = "Copy"
				onclick = copytoclipboard
			END CREATE
			CREATE pastemenu AS QMENUITEM
				caption = "Paste"
				onclick = pastefromclipboard
			END CREATE
			CREATE cutmenu AS QMENUITEM
				caption = "Cut"
				onclick = cuttoclipboard
			END CREATE
			CREATE editspacermenu AS QMENUITEM
				caption = "-"
			END CREATE
			CREATE searchmenu AS QMENUITEM
				caption = "Search"
			END CREATE
		END CREATE
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'		Rich Edit Format

	CREATE formatmenu AS QMENUITEM
		caption = "&Format"
			CREATE fontmenu AS QMENUITEM
				caption = "Fonts"
				onclick = changefont
			END CREATE
			CREATE colormenu AS QMENUITEM
				caption = "Colors"
			END CREATE
			CREATE spacermenuab AS QMENUITEM
				caption = "-"
			END CREATE
			CREATE justifymenu AS QMENUITEM
				caption = "Text Allignment"
				CREATE leftallignmenu AS QMENUITEM
					caption = "Left"
					onclick = leftjustify
				END CREATE
				CREATE centerallignmenu AS QMENUITEM
					caption = "Center"
					onclick = centerjustify
				END CREATE
				CREATE rightallignmenu AS QMENUITEM
					caption = "Right"
					onclick = rightjustify
				END CREATE
			END CREATE
			CREATE spacermenubb AS QMENUITEM
				caption = "-"
			END CREATE
			CREATE editorformatmenu AS QMENUITEM
				caption = "Editor Options"
				CREATE editorwordwraponoffmenu AS QMENUITEM
					caption = "Word Wrap On"
					onclick = wordwraponoff
				END CREATE
				CREATE editorscrollbarsonoffmenu AS QMENUITEM
					caption = "Scroll Bars On"
					onclick = scrollbarsonoff
				END CREATE
				CREATE editoreditororviewermenu AS QMENUITEM
					caption = "Editor Mode On"
					onclick = editororviewer
				END CREATE
			END CREATE
		END CREATE

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'		Tools
		CREATE toolsmenu AS QMENUITEM
			caption = "&Tools"
			CREATE dictionarymenu AS QMENUITEM
				caption = "Dictionary"
			END CREATE
			CREATE testmenu AS QMENUITEM
				caption = "Test"
			END CREATE
'			CREATE toolsspacermenu AS QMENUITEM
'				caption = "-"
'			END CREATE
'			CREATE detachwindowmenu AS QMENUITEM
'				caption = "Detach Window"
'			END CREATE
		END CREATE
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'		Options	
		CREATE optionsmenu AS QMENUITEM
			caption = "&Options"
			CREATE programsetupmenu AS QMENUITEM
				caption = "Program Setup"
			END CREATE
		END CREATE
	END CREATE
	CREATE vocabtv AS QTV
		optioncount = 14
		showhint = 0
		font = treeviewfont
		left = 0
		top = 0
		width = 150
		CREATE pm AS QPOPUPMENU
			CREATE newlessonpm AS QMENUITEM
				caption = "New Lesson"
			END CREATE
			CREATE insertlessonpm AS QMENUITEM
				caption = "Insert Lesson"
			END CREATE
			CREATE newsublessonpm AS QMENUITEM
				caption = "Add a Sub"
			END CREATE
			CREATE pmspacer1 AS QMENUITEM
				caption = "-"
			END CREATE
			CREATE deletelessonpm AS QMENUITEM
				caption = "Delete Lesson"
			END CREATE
			CREATE pmspacer2 AS QMENUITEM
				caption = "-"
			END CREATE
			CREATE edittextfilepm AS QMENUITEM
				caption = "Edit Text File"
			END CREATE
			CREATE changetextfilepm AS QMENUITEM
				caption = "Change Text File"
			END CREATE
		END CREATE
	END CREATE
	CREATE vocabedit AS QRICHEDIT
		font = richeditfont
		left = 152
		top = 0
		onkeyup = keyup
		wanttabs = 0
		scrollbars = 3
		onkeypress = keypress
		wanttabs = 1
	END CREATE
	
END CREATE


'SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
SUB vocabresize
	
	IF vocabform.width < 300 THEN vocabform.width = 300: EXIT SUB
	IF vocabform.height < 200 THEN vocabform.height = 200: EXIT SUB

	IF vocabform.clientwidth - vocabtv.width < 150 THEN vocabtv.width = vocabform.clientwidth - 150	
	vocabtv.height = vocabform.clientheight
	vocabedit.left = vocabtv.width + 2
	vocabedit.height = vocabform.clientheight
	vocabedit.width = vocabform.clientwidth - 2 - vocabtv.width

END SUB

SUB vocabmousedown(button%, x%, y%, shift%)
	vocabform.button = button% + 1
END SUB

SUB vocabmouseup(button%, x%, y%, shift%)
	vocabform.button = 0
END SUB

SUB vocabmousemove(x%, y%, shift%)
	IF vocabform.button = 1 THEN 
		IF (x% < 20)  OR (x% > vocabform.clientwidth - 150) THEN EXIT SUB
		vocabtv.width = x%
		vocabresize
	END IF
END SUB

'	EDITOR SUBS AND FUNCTIONS...

SUB copytoclipboard
	vocabedit.copytoclipboard
END SUB

SUB pastefromclipboard
	vocabedit.pastefromclipboard
END SUB

SUB cuttoclipboard
	vocabedit.cuttoclipboard
END SUB


SUB fontboxachange(fontname AS STRING, size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strikethrough AS INTEGER)
	vocabtv.optionitem CONFIG_EDITORFONTA, fontname
	vocabtv.optionitem CONFIG_EDITORFONTA + 1, genfontattrstr$(size, bold, italic, underline, strikethrough)
	setfont richeditfont, vocabtv.optionitem(CONFIG_EDITORFONTA), vocabtv.optionitem(CONFIG_EDITORFONTA + 1)
	vocabedit.selattributes = richeditfont
END SUB

SUB fontboxbchange(fontname AS STRING, size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strikethrough AS INTEGER)

	vocabtv.optionitem CONFIG_EDITORFONTB, fontname
	vocabtv.optionitem CONFIG_EDITORFONTB + 1, genfontattrstr$(size, bold, italic, underline, strikethrough)
	setfont richeditfont, vocabtv.optionitem(CONFIG_EDITORFONTA), vocabtv.optionitem(CONFIG_EDITORFONTA + 1)
	vocabedit.selattributes = richeditfont
END SUB

SUB treeviewboxchange(fontname AS STRING, size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strikethrough AS INTEGER)
	vocabtv.optionitem CONFIG_TREEVIEWFONT,  fontname
	vocabtv.optionitem CONFIG_TREEVIEWFONT + 1, genfontattrstr$(size, bold, italic, underline, strikethrough)
	setfont treeviewfont, vocabtv.optionitem(CONFIG_TREEVIEWFONT), vocabtv.optionitem(CONFIG_TREEVIEWFONT + 1)
	vocabtv.font = treeviewfont
END SUB

SUB changefont
	setupfontform vocabtv.optionitem(CONFIG_EDITORFONTA), vocabtv.optionitem(CONFIG_EDITORFONTA + 1), vocabtv.optionitem(CONFIG_EDITORFONTB), vocabtv.optionitem(CONFIG_EDITORFONTB + 1), vocabtv.optionitem(CONFIG_TREEVIEWFONT), vocabtv.optionitem(CONFIG_TREEVIEWFONT + 1)
	fontform.showmodal
END SUB

SUB setdelfontstyle(styles AS INTEGER)
	DIM bold AS BYTE
	DIM underline AS BYTE
	DIM strike AS BYTE
	DIM italic AS BYTE

	SELECT CASE styles
		CASE 0
			IF bold = 0 THEN 
				bold = 1
				richeditfont.addstyles(0) 
			ELSE 
				richeditfont.delstyles 0
				bold = 0
			END IF
		CASE 1
			IF underline = 0 THEN 
				underline = 1
				richeditfont.addstyles(1) 
			ELSE 
				richeditfont.delstyles 1
				underline = 0
			END IF
		CASE 2
			IF strike = 0 THEN 
				strike = 1
				richeditfont.addstyles(2) 
			ELSE 
				richeditfont.delstyles 2
				strike = 0
			END IF
		CASE 3
			IF italic = 0 THEN 
				italic = 1
				richeditfont.addstyles(3) 
			ELSE 
				richeditfont.delstyles 3
				italic = 0
			END IF
		CASE 5													'Reset all font styles.
			richeditfont.addstyles 0, 1, 2, 3
			richeditfont.delstyles 0, 1, 2, 3
					
	END SELECT
		vocabedit.selattributes = richeditfont
END SUB

SUB keyup(key AS WORD, shift AS INTEGER)
	
	IF shift = 256 THEN					'SHIFT
	
	ELSEIF shift = 1 THEN				'CTRL
		SELECT CASE key
			CASE 66							'BOLD
				setdelfontstyle 0
			CASE 85							'UNDERLINE
				setdelfontstyle 1
			CASE 83							'STRIKE
				setdelfontstyle 2
			CASE 73							'ITALIC
				setdelfontstyle 3
		END SELECT
	ELSEIF shift = 16 THEN			'ALT
	
	ELSEIF shift = 257 THEN			'Ctrl + Shift
	
	ELSEIF shift = 272 THEN			'Alt + Shift
	
	ELSEIF shift = 17 THEN			'Alt + Ctrl
	
	ELSEIF shift = 273 THEN			'Alt + Ctrl + Shift
	
	ELSE										'No shift key of any kind.
		SELECT CASE key
			CASE 113
				IF whichfontisused = 0 THEN whichfontisused = 1 ELSE whichfontisused = 0
				IF vocabtv.optionitem(EDITOR_OR_VIEWER) = "1" THEN EXIT SUB
				initfont
		END SELECT
	END IF
END SUB



SUB rightjustify
		vocabedit.alignment =1
END SUB
SUB centerjustify
		vocabedit.alignment =2
END SUB
SUB leftjustify
		vocabedit.alignment = 0
END SUB


SUB initfont
	IF whichfontisused = 0 THEN
		setfont richeditfont, vocabtv.optionitem(CONFIG_EDITORFONTA), vocabtv.optionitem(CONFIG_EDITORFONTA + 1)
	ELSEIF whichfontisused = 1 THEN
		setfont richeditfont, vocabtv.optionitem(CONFIG_EDITORFONTB), vocabtv.optionitem(CONFIG_EDITORFONTB + 1)
	END IF
	vocabedit.selattributes = richeditfont
END SUB


SUB wordwraponoff
	IF editorwordwraponoffmenu.caption = "Word Wrap On" THEN 
			editorwordwraponoffmenu.caption = "Word Wrap Off"
			vocabedit.wordwrap = 0
			vocabtv.optionitem(WORDWRAP_ON_OFF, "0")
	ELSE
			editorwordwraponoffmenu.caption = "Word Wrap On"
			vocabedit.wordwrap = 1
			vocabtv.optionitem(WORDWRAP_ON_OFF, "1")
	END IF
END SUB

SUB scrollbarsonoff
	IF editorscrollbarsonoffmenu.caption = "Scroll Bars On" THEN
		editorscrollbarsonoffmenu.caption = "Scroll Bars Off"

		vocabedit.scrollbars = 0
		vocabtv.optionitem(SCROLLBARS_ON_OFF, "0")
	ELSE

		editorscrollbarsonoffmenu.caption = "Scroll Bars On"
		vocabedit.scrollbars = 3
		vocabtv.optionitem(SCROLLBARS_ON_OFF, "1")
	END IF
END SUB

SUB editororviewer
	IF editoreditororviewermenu.caption = "Editor Mode On" THEN
		editoreditororviewermenu.caption = "Editor Mode Off"
		vocabedit.readonly = 1
		vocabtv.optionitem(EDITOR_OR_VIEWER, "1")
	ELSE
		editoreditororviewermenu.caption = "Editor Mode On"
		vocabedit.readonly = 0
		vocabtv.optionitem(EDITOR_OR_VIEWER, "0")
	END IF

END SUB

SUB keypress
	IF vocabtv.optionitem(EDITOR_OR_VIEWER) = "1" THEN EXIT SUB
	initfont																					'NO MATTER WHERE THE MOUSE CLICKS
																								'THE PREDEFINED FONT IS USED.
END SUB

SUB setupallfonts
	setfont richeditfont, vocabtv.optionitem(CONFIG_EDITORFONTA), vocabtv.optionitem(CONFIG_EDITORFONTA + 1)
	setfont treeviewfont, vocabtv.optionitem(CONFIG_TREEVIEWFONT), vocabtv.optionitem(CONFIG_TREEVIEWFONT + 1)
	vocabedit.selattributes = richeditfont
	vocabtv.font = treeviewfont
END SUB

SUB okbuttonclick
	fontform.close
END SUB