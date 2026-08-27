$INCLUDE "ui.inc"
$INCLUDE "editpop.inc"
$INCLUDE "c:\rqlibs\getcolor.inc"

DECLARE SUB changecolor
DECLARE SUB setupoptions
DECLARE SUB setcolors

DECLARE SUB OKBUTTONCLICK
DECLARE SUB treeviewbgbtnclick
DECLARE SUB treeviewfgbtnclick
DECLARE SUB editorbgbtnclick
DECLARE SUB editorfgbtnclick

colormenu.onclick = changecolor
CREATE colorform AS QFORM
    Caption = "Form1"
    Width = 399
    Height = 175
    Center
    CREATE colorOKbutton AS QBUTTON
        Caption = "OK"
        Left = 160
        Top = 112
        onclick = OKBUTTONCLICK
    END CREATE
    CREATE GroupBox1 AS QGROUPBOX
        Caption = "TreeView"
        Left = 8
        Top = 8
        Height = 97
        TabOrder = 1
        CREATE treeviewBACKGROUNDbtn AS QBUTTON
            Caption = "Back Ground"
            Left = 40
            Top = 16
            Width = 99
            onclick = treeviewbgbtnclick
        END CREATE
        CREATE treeviewFOREGROUNDbtn AS QBUTTON
            Caption = "Font"
            Left = 40
            Top = 64
            Width = 99
            onclick = treeviewfgbtnclick
        END CREATE
    END CREATE
    CREATE GroupBox2 AS QGROUPBOX
        Caption = "Editor"
        Left = 200
        Top = 8
        Height = 97
        TabOrder = 2
        CREATE editorBACKGROUNDbtn AS QBUTTON
            Caption = "Back Ground"
            Left = 40
            Top = 16
            Width = 99
            onclick = editorbgbtnclick
        END CREATE
        CREATE editorFOREGROUNDbtn AS QBUTTON
            Caption = "Font"
            Left = 40
            Top = 64
            Width = 99
            onclick = editorfgbtnclick
        END CREATE
    END CREATE
END CREATE


SUB changecolor
	colorform.showmodal
END SUB


SUB OKBUTTONCLICK
	colorform.close
END SUB

SUB setcolors

	IF vocabtv.optionitem(TREEVIEW_BACKGROUND) <> "" THEN
		vocabtv.color = VAL(vocabtv.optionitem(TREEVIEW_BACKGROUND))
	END IF
	IF vocabtv.optionitem(TREEVIEW_FOREGROUND) <> "" THEN
		treeviewfont.color = VAL(vocabtv.optionitem(TREEVIEW_BACKGROUND))
		vocabtv.font = treeviewfont
	END IF
	IF vocabtv.optionitem(EDITOR_BACKGROUND) <> "" THEN
		vocabedit.color = VAL(vocabtv.optionitem(EDITOR_BACKGROUND))
	END IF
	IF vocabtv.optionitem(EDITOR_FOREGROUND) <> "" THEN
		richeditfont.color = VAL(vocabtv.optionitem(EDITOR_FOREGROUND))
		vocabedit.selattributes = richeditfont
	END IF

END SUB


SUB treeviewbgbtnclick
	IF vocabtv.optionitem(TREEVIEW_BACKGROUND) = "" THEN
		vocabtv.optionitem TREEVIEW_BACKGROUND, STR$(getcolor(vocabform.handle, &HFFFFFF))
	ELSE
		vocabtv.optionitem TREEVIEW_BACKGROUND,  STR$(getcolor(vocabform.handle, VAL(vocabtv.optionitem(TREEVIEW_BACKGROUND))))
	END IF
		vocabtv.color = VAL(vocabtv.optionitem(TREEVIEW_BACKGROUND))
		colorform.close
END SUB
SUB treeviewfgbtnclick
	IF vocabtv.optionitem(TREEVIEW_FOREGROUND) = "" THEN
		vocabtv.optionitem TREEVIEW_FOREGROUND, STR$(getcolor(vocabform.handle, &H000000))
	ELSE
		vocabtv.optionitem TREEVIEW_FOREGROUND, STR$(getcolor(vocabform.handle, VAL(vocabtv.optionitem(TREEVIEW_FOREGROUND))))
	END IF
		treeviewfont.color = VAL(vocabtv.optionitem(TREEVIEW_FOREGROUND))
		vocabtv.font = treeviewfont
		colorform.close

END SUB
SUB editorbgbtnclick
	IF vocabtv.optionitem(EDITOR_BACKGROUND) = "" THEN
		vocabtv.optionitem EDITOR_BACKGROUND, STR$(getcolor(vocabform.handle, &HFFFFFF))
	ELSE
		vocabtv.optionitem EDITOR_BACKGROUND, STR$(getcolor(vocabform.handle, VAL(vocabtv.optionitem(EDITOR_BACKGROUND))))
	END IF
		vocabedit.color = VAL(vocabtv.optionitem(EDITOR_BACKGROUND))
	colorform.close
END SUB
SUB editorfgbtnclick
	DIM temp AS INTEGER
	IF vocabtv.optionitem(EDITOR_FOREGROUND) = "" THEN
		temp = getcolor(vocabform.handle, &H000000)
		vocabtv.optionitem EDITOR_FOREGROUND, STR$(temp)
	ELSE
		vocabtv.optionitem EDITOR_FOREGROUND, STR$(getcolor(vocabform.handle, VAL(vocabtv.optionitem(EDITOR_FOREGROUND))))
	END IF
		richeditfont.color = VAL(vocabtv.optionitem(EDITOR_FOREGROUND))
		vocabedit.selattributes = richeditfont
		colorform.close


END SUB


SUB setupoptions
	
	IF vocabtv.optionitem(EDITOR_OR_VIEWER) = "1" THEN
		editoreditororviewermenu.caption = "Editor Mode Off"
		vocabedit.readonly = 1
	ELSE
		editoreditororviewermenu.caption = "Editor Mode On"
		vocabedit.readonly = 0
	END IF
	IF vocabtv.optionitem(SCROLLBARS_ON_OFF) = "1" THEN
		editorscrollbarsonoffmenu.caption = "Scroll Bars On"
		vocabedit.scrollbars = 3

	ELSE
		editorscrollbarsonoffmenu.caption = "Scroll Bars Off"
		vocabedit.scrollbars = 0

	END IF
	IF vocabtv.optionitem(WORDWRAP_ON_OFF) = "1" THEN
		editorwordwraponoffmenu.caption = "Word Wrap On"
		vocabedit.wordwrap = 1
	ELSE
		editorwordwraponoffmenu.caption = "Word Wrap Off"
		vocabedit.wordwrap = 0
	END IF
	setcolors
END SUB
