$TYPECHECK ON

CONST CONFIG_EDITORFONTA = 0
CONST CONFIG_EDITORFONTB = 2
CONST CONFIG_TREEVIEWFONT = 4
CONST EDITOR_OR_VIEWER = 6
CONST SCROLLBARS_ON_OFF = 7
CONST WORDWRAP_ON_OFF = 8
CONST TREEVIEW_BACKGROUND = 9
CONST TREEVIEW_FOREGROUND = 10
CONST EDITOR_BACKGROUND = 11
CONST EDITOR_FOREGROUND = 12


'USAGE:

'QTV.
'		insertitem(#, itemtext)			Insert item at #
'		additem(itemtext)				Add new item at end of items.
'		addchilditem(#, itemtext)		Insert a Child item at #.
'		delitem(#)							Delete Item at #.
'		itemtextfromstream(#, memorystream)			This copies text from a memory stream to an item at #.
'																				00This is an item and it's text.
'																				This is the string from the memorystream.
'		loadfromfile(filename)			Load a file and setup the treeview.

TYPE QTV EXTENDS QTREEVIEW
	slst AS QSTRINGLIST
	optioncount AS INTEGER
	filename AS STRING												'Remember the name of the file loaded.
	
	FUNCTION onlyfilename(filename AS STRING) AS STRING
						'Used to only return the file name without directories and drive numbers.
						
			DIM x AS INTEGER
			FOR x = LEN(filename) TO 1 STEP -1
				IF MID$(filename, x, 1) = "\" THEN EXIT FOR
			NEXT x
			IF x > 1 THEN RESULT = MID$(filename, x+1, LEN(filename) - x) ELSE RESULT = filename
	END FUNCTION

	
	FUNCTION gennumber(number AS INTEGER) AS STRING
		DIM zzz AS STRING
		zzz = STR$(number)
		IF LEN(zzz) = 1 THEN 
				RESULT = "0" + zzz 
		ELSEIF LEN(zzz) > 0 THEN
				RESULT = "01"
		ELSE
				RESULT = zzz
		END IF
	END FUNCTION
	
	SUB insertitem(position AS INTEGER, text AS STRING)
		QTV.slst.insertitem position * 2 + QTV.optioncount, QTV.gennumber(QTV.item(position).level) + text
		QTV.slst.insertitem position * 2 + 1 + QTV.optioncount, ""
		super.insertitem position, text
	END SUB
	
	SUB additem(text AS STRING)
		QTV.slst.additems "00" + text, ""
		QTV.additems text
	END SUB
	
	SUB addchilditem(position AS INTEGER, text AS STRING)
		DIM oldlevel AS BYTE
		DIM x AS BYTE
			oldlevel = QTV.item(position).level
			FOR x = position + 1 TO QTV.itemcount - 1
				IF QTV.item(x).level <> oldlevel + 1 THEN EXIT FOR								
			NEXT x
		QTV.addchilditems position, text
		QTV.slst.insertitem x * 2 + QTV.optioncount, QTV.gennumber(QTV.item(position).level + 1) + text
		QTV.slst.insertitem x * 2 + 1 + QTV.optioncount, ""
	END SUB

	SUB delitem(position AS INTEGER)
		IF QTV.item(position).haschildren = 0 THEN
			QTV.delitems position
			QTV.slst.delitems position * 2 + QTV.optioncount, position * 2 + 1 + QTV.optioncount
		END IF
	END SUB	
	
	FUNCTIONI optionitem(...) AS STRING
		IF (paramvalcount = 0) OR ((paramval(1) > (QTV.optioncount - 1))) THEN RESULT = "": EXIT FUNCTION
		IF paramstrcount = 0 THEN
			RESULT = QTV.slst.item(paramval(1))
		ELSE
			QTV.slst.item(paramval(1)) = paramstr$(1)
			RESULT = ""
		END IF		
	END FUNCTIONI
	
	FUNCTIONI itemtext(...) AS STRING
		IF paramvalcount = 0 THEN RESULT = "": EXIT FUNCTION
		IF paramstrcount = 0 THEN
			RESULT = QTV.slst.item(paramval(1) * 2 + 1 + QTV.optioncount)
		ELSE
			QTV.slst.item(paramval(1) * 2 + 1 + QTV.optioncount) = QTV.onlyfilename(paramstr$(1))
			RESULT = ""
		END IF
	END FUNCTIONI

	FUNCTION getnameofitem(position AS INTEGER) AS STRING
		DIM tstr AS STRING
			tstr = QTV.slst.item(position * 2 + QTV.optioncount)
			RESULT = RIGHT$(tstr, LEN(tstr) - 2)
	END FUNCTION
	
	SUB setupTV	
		IF QTV.slst.itemcount = 0 THEN EXIT SUB
		DIM x AS INTEGER
		DIM y AS INTEGER
		FOR x = 0 TO (QTV.slst.itemcount - QTV.optioncount) / 2 - 1
			IF VAL(LEFT$(QTV.slst.item(x * 2 + QTV.optioncount), 2)) = 0 THEN
				QTV.additems QTV.getnameofitem(x)
				QTV.item(x).text = QTV.getnameofitem(x)
			ELSEIF VAL(LEFT$(QTV.slst.item(x * 2 + QTV.optioncount), 2)) > 0 THEN
				FOR y = x TO 0 STEP -1
					IF VAL(LEFT$(QTV.slst.item(y * 2 + QTV.optioncount), 2)) = VAL(LEFT$(QTV.slst.item(x * 2 + QTV.optioncount), 2)) - 1 THEN 
							QTV.addchilditems y, QTV.getnameofitem(x)
							EXIT FOR
					END IF
				NEXT x
			END IF
		NEXT x
	END SUB

	SUB clean
		DIM x AS INTEGER
		QTV.slst.clear
		QTV.clear
		FOR x = 1 TO QTV.optioncount
			QTV.slst.additems ""
		NEXT x
	END SUB

	SUB loadfromfile(filename AS STRING)
		QTV.clean
		QTV.filename = filename
		QTV.slst.loadfromfile(filename)
		QTV.setupTV
	END SUB
	SUB savetofile(filename AS STRING)
		QTV.filename = filename
		QTV.slst.savetofile(filename)
	END SUB
	
	SUB edited(index%, BYREF s$)
		QTV.slst.item(index% * 2 + QTV.optioncount) = QTV.gennumber(QTV.item(index%).level) + s$
	END SUB
	
	CONSTRUCTOR
		onedited = QTV.edited
		clean
	END CONSTRUCTOR
END TYPE

