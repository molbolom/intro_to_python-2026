

$INCLUDE "vocabtv.inc"
$INCLUDE "c:\rqlibs\rapidq.inc"
$INCLUDE "QSEARCH.inc"


DECLARE SUB exitprogram
DECLARE SUB lessonselected
DECLARE SUB changetextfile
DECLARE SUB edittextlesson

DECLARE SUB startsearch
DECLARE FUNCTION getstringfromsearch(searchstring AS STRING, stringindex AS INTEGER) AS STRING
DECLARE FUNCTION searchcount(searchstring AS STRING) AS INTEGER

DIM searchstringlist AS QSTRINGLIST			'A string list of all the search cryteria.

vocabtv.ondblclick = lessonselected
edittextfilepm.onclick = edittextlesson
changetextfilepm.onclick = changetextfile

vocabform.onclose = exitprogram
exitmenu.onclick = exitprogram
searchmenu.onclick = startsearch

SUB changetextfile
	DIM nfn AS STRING
	IF vocabtv.itemtext(vocabtv.itemindex) <> "" THEN
		IF dlg.yesnodlg("Do you wish to change the file?") = 2 THEN EXIT SUB
    END IF
    	nfn = dlg.opendlg("Open a Lesson File", "Rich Text Documents|*.rtf")
    	dlg.dir = CURDIR$
    	IF FILEEXISTS(nfn) THEN 
    		IF textfileloaded <> "" THEN vocabedit.savetofile(textfileloaded)
    		vocabtv.itemtext vocabtv.itemindex, nfn
    		vocabedit.loadfromfile(nfn)
    		textfileloaded = vocabtv.itemtext(vocabtv.itemindex)
    		vocabedit.savetofile(dlg.dir + "\" + vocabtv.itemtext(vocabtv.itemindex))
    		initfont
    		
    	ELSE
    		showmessage "File does not exist."
    	END IF
END SUB

SUB exitprogram
	IF textfileloaded <> "" THEN vocabedit.savetofile(textfileloaded)
	IF vocabtv.filename <> "" THEN vocabtv.savetofile(vocabtv.filename)
	vocabform.close
END SUB

SUB edittextlesson
	filename = vocabtv.itemtext(vocabtv.itemindex)
	IF textfileloaded <> "" THEN vocabedit.savetofile(textfileloaded)
	IF filename = "" THEN
		
		filename = dlg.opendlg("Create a file for " + vocabtv.item(vocabtv.itemindex).text, "Lesson Text Files|*.rtf;")
			IF UCASE$(RIGHT$(filename, 4)) <> ".RTF" THEN filename = filename + ".rtf"		
		IF UCASE$(filename) = ".RTF" THEN 
			EXIT SUB 
		ELSE
				textfileloaded = filename
				vocabedit.clear
				
				vocabtv.itemtext(vocabtv.itemindex, filename)
			IF FILEEXISTS(filename) THEN
				vocabedit.loadfromfile(filename)
				IF CURDIR$ <> dlg.dir THEN vocabedit.savetofile(dlg.dir + "\" + vocabtv.itemtext(vocabtv.itemindex))
			ELSE
				vocabedit.savetofile(vocabtv.itemindex)
			END IF
			initfont
		END IF
	ELSE
		vocabedit.clear
		vocabedit.loadfromfile(filename)
		textfileloaded = filename
		initfont
	END IF
END SUB

SUB lessonselected
	DIM filename AS STRING
	filename = vocabtv.itemtext(vocabtv.itemindex)
	IF vocabtv.item(vocabtv.itemindex).haschildren > 0 THEN vocabtv.expand(vocabtv.itemindex, 0)
	IF LTRIM$(filename) <> "" THEN
		IF textfileloaded <> "" THEN vocabedit.savetofile(textfileloaded)
		vocabedit.clear
		vocabedit.loadfromfile(filename)
		textfileloaded = filename
		initfont
	END IF
END SUB

'SEARCH ROUTINES...................................................................................................................................
FUNCTION searchcount(searchstring AS STRING) AS INTEGER
	DIM x AS INTEGER
	DIM count AS INTEGER
	count = 0
	FOR x = 1 TO LEN(searchstring)
		IF MID$(searchstring, x, 1) = ";" THEN count = count + 1
	NEXT x
	searchcount = count + 1
END FUNCTION

FUNCTION getstringfromsearch(searchstring AS STRING, stringindex AS INTEGER) AS STRING
	DIM x AS INTEGER
	DIM count AS INTEGER
	DIM newstr AS STRING
	newstr = ""
	count = 0
	x = 0
	IF stringindex > 1 THEN
		DO
			x = x + 1
						'If search item doesn't exist then exit function.
			IF x > LEN(searchstring) THEN getstringfromsearch = "":EXIT FUNCTION
			IF MID$(searchstring, x, 1) = ";" THEN count = count + 1
			IF count = stringindex-1 THEN EXIT DO
		LOOP
	END IF
	
	DO
		x = x + 1
		IF MID$(searchstring, x, 1) = ";" THEN EXIT DO
		IF x > LEN(searchstring) THEN EXIT DO
		newstr = newstr + MID$(searchstring, x, 1)
	LOOP
	getstringfromsearch = newstr
END FUNCTION

SUB startsearch
	DIM search AS QSEARCH

	IF vocabtv.filename = "" THEN EXIT SUB
	
    
	vocabedit.savetofile(vocabtv.filename)								'Save the RTF file before doing a search.
		DIM linepos AS INTEGER											'Line position in RichEdit.
		DIM searchpos AS INTEGER										'Search position.
		DIM REpos AS INTEGER											'Position within RichEdit.
		DIM test AS INTEGER
			test = search.execute
	SELECT CASE test
		CASE SEARCH_PRESENT OR BASIC_SEARCH
			searchstringlist.parse(search.sedit, ";")
			FOR linepos = 0 TO vocabedit.linecount - 1
				FOR searchpos = 0 TO searchstringlist.itemcount - 1
					IF INSTR(UCASE$(vocabedit.line(linepos)), UCASE$(searchstringlist.item(searchpos))) THEN
							vocabedit.selstart = REpos + INSTR(UCASE$(vocabedit.line(linepos)), UCASE$(searchstringlist.item(searchpos)))
							vocabedit.sellength = LEN(searchstringlist.item(searchpos))
					ELSE
							REpos = REpos + LEN(vocabedit.line(linepos))
				    END IF
				NEXT searchpos
			NEXT linepos
SHOWMESSAGE "This would search the presently opened file, and stop at the first word found."
		CASE SEARCH_PRESENT OR EXTENDED_SEARCH
SHOWMESSAGE "This would search the presently opened file, and continue the search until the user presses 'n' on the keyboard."
		CASE SEARCH_ALL OR BASIC_SEARCH
SHOWMESSAGE "This would search all RTF files and stop at the first find."
		CASE SEARCH_ALL OR EXTENDED_SEARCH
SHOWMESSAGE "This would search all RTF files until the user presses 'n' on the keyboard."
		CASE SEARCH_DICTIONARY OR BASIC_SEARCH
SHOWMESSAGE "This would search ONLY in the dictionary file and stop at the first find."
		CASE SEARCH_DICTIONARY OR EXTENDED_SEARCH
SHOWMESSAGE "This would search ONLY the dictionary file and stop only when the user presses 'n' on the keyboard."
	END SELECT		
END SUB


IF command$(1) <> "" THEN
	IF UCASE$(RIGHT$(command$(1), 4)) = ".VCB" THEN
		vocabtv.loadfromfile(command$(1))
		IF vocabtv.itemtext(0) <> "" THEN 
			textfileloaded = vocabtv.itemtext(0)
			vocabedit.loadfromfile(vocabtv.itemtext(0))
		END IF
		setupallfonts
		setupoptions
	END IF
END IF
	vocabform.showmodal
	
	
