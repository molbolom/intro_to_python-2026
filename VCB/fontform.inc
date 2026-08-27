' Copy and paste into your program

$TYPECHECK ON
DECLARE SUB setupfontform(fontnameA AS STRING, fontattrA AS STRING, fontnameB AS STRING, fontattrB AS STRING, fontnameC AS STRING, fontattrC AS STRING)
DECLARE FUNCTION getint(spos AS INTEGER, s2r AS STRING) AS INTEGER
DECLARE FUNCTION genhexstr$(num AS INTEGER) AS STRING
DECLARE FUNCTION genfontattrstr$(size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strike AS INTEGER) AS STRING
DECLARE SUB setfont(qfnt AS QFONT, setfontname AS STRING, setfontattr AS STRING)

CONST FONT_BOLD = 0
CONST FONT_ITALIC = 1
CONST FONT_UNDERLINE = 2
CONST FONT_STRIKE = 3

CONST NO_STYLE = &HF

DECLARE SUB qfontbox_onchange(fontname AS STRING, size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strikethrough AS INTEGER)

TYPE QFONTBOX EXTENDS QGROUPBOX
	onchange AS EVENT(qfontbox_onchange)

	namebox AS QCOMBOBOX
	stylebox AS QCOMBOBOX
	sizebox AS QCOMBOBOX
	example AS QPANEL

	tfnt AS QFONT
	
	
	name AS STRING
	size AS INTEGER
	bold AS INTEGER
	italic AS INTEGER
	underline AS INTEGER
	strike AS INTEGER
	
	SUB showsample
		QFONTBOX.tfnt.name = QFONTBOX.name
		QFONTBOX.tfnt.size = QFONTBOX.size
		QFONTBOX.tfnt.delstyles 0, 1, 2, 3
		IF QFONTBOX.bold = 1 THEN QFONTBOX.tfnt.addstyles 0
		IF QFONTBOX.italic = 1 THEN QFONTBOX.tfnt.addstyles FONT_ITALIC
		IF QFONTBOX.underline = 1 THEN QFONTBOX.tfnt.addstyles FONT_UNDERLINE
		IF QFONTBOX.strike = 1 THEN QFONTBOX.tfnt.addstyles FONT_STRIKE
		QFONTBOX.example.font = QFONTBOX.tfnt
	END SUB		
	
	CONSTRUCTOR
        Width = 385
			namebox.Left = 8
            namebox.Top = 16
            namebox.Width = 185
                    					  
            stylebox.Left = 200
            stylebox.Top = 16
            stylebox.Width = 113

            sizebox.Left = 320
            sizebox.Top = 16
            sizebox.Width = 57

            example.Left = 56
            example.Top = 48
            example.Caption = "Hello World!"
            example.Width = 273
            example.BevelInner = 1

			namebox.parent = QFONTBOX
			stylebox.parent = QFONTBOX
			sizebox.parent = QFONTBOX
			example.parent = QFONTBOX
			
			tfnt.charset = 4
	END CONSTRUCTOR
	EVENT namebox.onchange
		WITH QFONTBOX
			QFONTBOX.name = QFONTBOX.namebox.text
			QFONTBOX.showsample
			CALLFUNC(.onchange, QFONTBOX.name, QFONTBOX.size, QFONTBOX.bold, QFONTBOX.italic, QFONTBOX.underline, QFONTBOX.strike)
		END WITH
	END EVENT
	EVENT stylebox.onchange
		WITH QFONTBOX
			SELECT CASE QFONTBOX.stylebox.text
                CASE "Bold":
                		QFONTBOX.bold = 1: QFONTBOX.italic = 0
                		QFONTBOX.underline = 0: QFONTBOX.strike = 0
                CASE "Italic":
                		QFONTBOX.bold = 0: QFONTBOX.italic = 1
                		QFONTBOX.underline = 0: QFONTBOX.strike = 0
                CASE "Underline":
                		QFONTBOX.bold = 0: QFONTBOX.italic = 0
                		QFONTBOX.underline = 1: QFONTBOX.strike = 0
                CASE "Strike Through":
                		QFONTBOX.bold = 0: QFONTBOX.italic = 0
                		QFONTBOX.underline = 0: QFONTBOX.strike = 1
                CASE "Bold Italic":
                		QFONTBOX.bold = 1: QFONTBOX.italic = 1
                		QFONTBOX.underline = 0: QFONTBOX.strike = 0
                CASE "Italic Underline":
                		QFONTBOX.bold = 0: QFONTBOX.italic = 1
                		QFONTBOX.underline = 1: QFONTBOX.strike = 0
                CASE "Bold Strike Through":
                		QFONTBOX.bold = 1: QFONTBOX.italic = 0
                		QFONTBOX.underline = 0: QFONTBOX.strike = 1
                CASE ELSE
						QFONTBOX.bold = 0: QFONTBOX.italic = 0: QFONTBOX.underline = 0: QFONTBOX.strike = 0
             END SELECT
				QFONTBOX.showsample
						CALLFUNC(.onchange, QFONTBOX.name, QFONTBOX.size, QFONTBOX.bold, QFONTBOX.italic, QFONTBOX.underline, QFONTBOX.strike)
        END WITH
	END EVENT
	EVENT sizebox.onchange
		WITH QFONTBOX
			QFONTBOX.size = VAL(QFONTBOX.sizebox.item(QFONTBOX.sizebox.itemindex))
			QFONTBOX.showsample
			CALLFUNC(.onchange, QFONTBOX.name, QFONTBOX.size, QFONTBOX.bold, QFONTBOX.italic, QFONTBOX.underline, QFONTBOX.strike)
		END WITH
	END EVENT	

	SUB init
			DIM x AS INTEGER
				FOR x = 0 TO QFONTBOX.tfnt.fontcount - 1
					QFONTBOX.namebox.additems QFONTBOX.tfnt.fontname(x)
					IF QFONTBOX.name <> "" AND QFONTBOX.name = QFONTBOX.tfnt.fontname(x) THEN 
						QFONTBOX.namebox.itemindex = x
					ELSEIF QFONTBOX.name = "" AND x = 0 THEN
						QFONTBOX.name = QFONTBOX.tfnt.fontname(x)
						QFONTBOX.namebox.itemindex = x
					END IF
				NEXT x
            QFONTBOX.stylebox.AddItems "Regular", _
                			     		  "Bold", _
                     					  "Italic", _
                     					  "Underline",_
                     					  "Strike Through",_
                     					  "Bold Italic",_
                     					  "Italic Underline",_
                     					  "Bold Strike Through"
			IF QFONTBOX.bold = 1 AND QFONTBOX.italic = 1 THEN
				QFONTBOX.stylebox.itemindex = 5
			ELSEIF QFONTBOX.bold = 1 AND QFONTBOX.strike = 1 THEN
				QFONTBOX.stylebox.itemindex = 7
			ELSEIF QFONTBOX.italic = 1 AND QFONTBOX.underline = 1 THEN
				QFONTBOX.stylebox.itemindex = 6
			ELSEIF QFONTBOX.strike = 1 THEN
				QFONTBOX.stylebox.itemindex = 4
			ELSEIF QFONTBOX.underline = 1 THEN
				QFONTBOX.stylebox.itemindex = 3
			ELSEIF QFONTBOX.italic = 1 THEN
				QFONTBOX.stylebox.itemindex = 2
			ELSEIF QFONTBOX.bold = 1 THEN
				QFONTBOX.stylebox.itemindex = 1
			ELSE
				QFONTBOX.stylebox.itemindex = 0
			END IF

            QFONTBOX.sizebox.AddItems "8", _
					                     "9", _
                    					 "10", _
					                    "11", _
                    					"12", _
					                    "14", _
                    					"16", _
					                    "18", _
                    					"20", _
					                    "22", _
                    					"24", _
										"26", _
					                    "28", _
                     					"36", _
                     					"48", _
                     					"72"

		FOR x = 0 TO QFONTBOX.sizebox.itemcount - 1
			IF STR$(QFONTBOX.size) = QFONTBOX.sizebox.item(x) THEN 
				QFONTBOX.sizebox.itemindex = x: EXIT FOR
			ELSEIF QFONTBOX.size = 0 THEN
				QFONTBOX.sizebox.itemindex = 0
			END IF
		NEXT x                     						
		QFONTBOX.showsample
	END SUB

	SUB initfromdata(ftname AS STRING, ftsize AS INTEGER, ftbold AS INTEGER, ftitalic AS INTEGER, ftunderline AS INTEGER, ftstrike AS INTEGER)
		QFONTBOX.name = ftname
		QFONTBOX.size = ftsize
		QFONTBOX.bold = ftbold
		QFONTBOX.italic = ftitalic
		QFONTBOX.underline = ftunderline
		QFONTBOX.strike = ftstrike
		QFONTBOX.init
	END SUB

END TYPE

DIM fnt AS QFONT

CREATE fontform AS QFORM

	width = 410
	height = 400
	borderstyle = 3
	center
	CREATE fontbox AS QFONTBOX
        Left = 8
        Top = 8
		caption = "Editor Font A"
	END CREATE
	CREATE fontboxb AS QFONTBOX
		left = 8
		top = fontbox.height + 8 + 5
		caption = "Editor Font B"
	END CREATE
	CREATE treeviewfontbox AS QFONTBOX
		left = 8
		top = fontboxb.top + fontboxb.height + 5
		caption = "Tree View Font"
	END CREATE
	
	CREATE FONTOKBUTTON AS QBUTTON
		caption = "OK"
		left = FIX(fontform.clientwidth / 2) - FIX(FONTOKBUTTON.width / 2)
		top = treeviewfontbox.top + treeviewfontbox.height + 6
	END CREATE
END CREATE


FUNCTION getint(spos AS INTEGER, s2r AS STRING) AS INTEGER
	IF spos = 1 THEN 
			getint = VAL(LEFT$(s2r, 3))
	ELSE
		getint = VAL(MID$(s2r, spos + 2, 1))
	END IF
END FUNCTION

FUNCTION genhexstr$(num AS INTEGER) AS STRING
	DIM tnum AS STRING
	tnum = STR$(num)
	IF LEN(tnum) = 1 THEN 
			genhexstr$ = "00" + tnum
	ELSEIF LEN(tnum) = 2 THEN 
			genhexstr$ = "0" + tnum
	ELSE
			genhexstr$ = tnum
	END IF
END FUNCTION

FUNCTION genfontattrstr$(size AS INTEGER, bold AS INTEGER, italic AS INTEGER, underline AS INTEGER, strike AS INTEGER) AS STRING
	genfontattrstr$ = genhexstr$(size) + STR$(bold) + STR$(italic) + STR$(underline) + STR$(strike)
END FUNCTION


SUB setfont(qfnt AS QFONT, setfontname AS STRING, setfontattr AS STRING)
	qfnt.delstyles 0, 1, 2, 3, NO_STYLE
	qfnt.name = setfontname
	qfnt.size = getint(1, setfontattr)
	IF getint(2, setfontattr) = 1 THEN qfnt.addstyles 0
	IF getint(3, setfontattr) = 1 THEN qfnt.addstyles 1
	IF getint(4, setfontattr) = 1 THEN qfnt.addstyles 2
	IF getint(5, setfontattr) = 1 THEN qfnt.addstyles 3
END SUB

SUB setupfontform(fontnameA AS STRING, fontattrA AS STRING, fontnameB AS STRING, fontattrB AS STRING, fontnameC AS STRING, fontattrC AS STRING)
	fontbox.initfromdata fontnameA, getint(1, fontattrA), getint(2, fontattrA), getint(3, fontattrA), getint(4, fontattrA), getint(5, fontattrA)
	fontboxb.initfromdata fontnameB, getint(1, fontattrB), getint(2, fontattrB), getint(3, fontattrB), getint(4, fontattrB), getint(5, fontattrB)
	treeviewfontbox.initfromdata fontnameC, getint(1, fontattrC), getint(2, fontattrC), getint(3, fontattrC), getint(4, fontattrC), getint(5, fontattrC)
END SUB