

'Treeview subs and functions...

$INCLUDE "color.inc"


DECLARE SUB createnewlesson
'DECLARE SUB setuptreeview
DECLARE SUB openfile
DECLARE SUB changenameoflesson(index%, BYREF s$)
DECLARE SUB savefile

DECLARE SUB insertlesson
DECLARE SUB addlesson
DECLARE SUB addchildlesson
DECLARE SUB deletelesson


DIM filename AS STRING			'Used more than once...

newmenu.onclick = createnewlesson
openmenu.onclick = openfile
saveasmenu.onclick = savefile

newlessonpm.onclick = addlesson
insertlessonpm.onclick = insertlesson
newsublessonpm.onclick = addchildlesson
deletelessonpm.onclick = deletelesson

SUB openfile
	filename = dlg.opendlg("Open a Language Lesson", "Lesson Files|*.vcb;")
	IF UCASE$(RIGHT$(filename, 4)) <> ".VCB" THEN filename = filename + ".vcb"
	IF filename <> "" THEN
		IF FILEEXISTS(filename) THEN
			'First ask if the user wants to save a copy of the file he created.
			IF vocabtv.filename = "" AND vocabtv.itemcount <> 0 THEN
				IF dlg.yesnodlg("Do you want to save a copy of the current file?") = 1 THEN
					vocabtv.filename = dlg.savedlg("Save file as", "Lesson Files|*.vcb;")
					IF UCASE$(RIGHT$(vocabtv.filename, 4)) <> ".VCB" THEN vocabtv.filename = vocabtv.filename + ".vcb"
					vocabtv.savetofile(vocabtv.filename)
				END IF
			END IF	
				vocabtv.loadfromfile(filename)
				IF vocabtv.itemtext(0) <> "" THEN 
					textfileloaded = vocabtv.itemtext(0)
					vocabedit.loadfromfile(vocabtv.itemtext(0))
				END IF
				dlg.dir = CURDIR$
				setupoptions
				setupallfonts
		ELSE
			SHOWMESSAGE "File does not exist."
		END IF
	END IF
END SUB


SUB savefile
	filename = dlg.savedlg("Save File As", "Lesson Files|*.vcb;")
	IF UCASE$(RIGHT$(filename, 4)) <> ".VCB" THEN filename = filename + ".vcb"
	
	IF FILEEXISTS(filename) THEN
		IF dlg.yesnodlg("Do you want to save over the old file?") = 1 THEN
			vocabtv.savetofile(filename)
			dlg.dir = CURDIR$
		ELSE 
			EXIT SUB
		END IF
	ELSE
		IF filename = ".vcb" THEN EXIT SUB
		vocabtv.savetofile(filename)
		dlg.dir = CURDIR$
	END IF
	
END SUB



SUB addlesson
	vocabtv.additem("New Lesson")
END SUB

SUB insertlesson
	vocabtv.insertitem vocabtv.itemindex, "New Lesson"
END SUB

SUB addchildlesson
	vocabtv.addchilditem vocabtv.itemindex, "New Lesson"
END SUB

SUB deletelesson
	vocabtv.delitem vocabtv.itemindex
END SUB

SUB createnewlesson
	IF vocabtv.filename <> "" THEN
				vocabtv.savetofile(filename)
				savefile
	ELSE 
		savefile
		IF vocabtv.filename = ".vcb" THEN EXIT SUB
	END IF
	vocabtv.clean
	vocabedit.clear
	IF vocabtv.filename <> ".vcb" OR vocabtv.filename <> "" THEN vocabtv.additem "New Lesson"
END SUB

