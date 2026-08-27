DECLARE SUB aboutpop
DECLARE SUB changefontmenuclick
'	RichEdit onmousedown events and popupmenu items...

DIM popmenu AS QPOPUPMENU
popmenu.onpopup = aboutpop

DIM playwavpop AS QMENUITEM
DIM recordwavpop AS QMENUITEM
playwavpop.caption = "Play Sound File"
recordwavpop.caption = "Record Sound File"

DIM editpopspacerA AS QMENUITEM
editpopspacerA.caption = "-"
DIM editpopspacerB AS QMENUITEM
editpopspacerB.caption = "-"

DIM addnotepop AS QMENUITEM
DIM readnotepop AS QMENUITEM
addnotepop.caption = "Add a Note"
readnotepop.caption = "Read Note"

DIM fontpop AS QMENUITEM
fontpop.caption = "Change Selected Font"
fontpop.onclick = changefontmenuclick
popmenu.additems fontpop, editpopspacerA,_
								playwavpop, recordwavpop,_
								editpopspacerB,_
								addnotepop, readnotepop
vocabedit.popupmenu = popmenu

SUB aboutpop
	IF vocabedit.seltext <> "" THEN
		fontpop.enabled = 1
		recordwavpop.enabled = 1
		playwavpop.enabled = 1
		readnotepop.enabled = 1
		addnotepop.enabled = 1
	ELSE		
		fontpop.enabled = 0
		recordwavpop.enabled = 0
		playwavpop.enabled = 0
		readnotepop.enabled = 0
		addnotepop.enabled = 0
	END IF
END SUB


SUB changefontmenuclick
	DIM fdlg AS QFONTDIALOG
	IF fdlg.execute THEN
		fdlg.setfont(richeditfont)
		vocabedit.selattributes = richeditfont
	END IF
END SUB