
TYPE QDIALOGS EXTENDS QOBJECT
	'This is for basic and general Dialogs.
	'Each file dialog returns the filename.
	dir AS STRING
	CONSTRUCTOR
		dir = CURDIR$
	END CONSTRUCTOR
	
	FUNCTION opendlg(caption AS STRING, filters AS STRING) AS STRING
		DIM opendlg AS QOPENDIALOG
		IF filters = "" THEN opendlg.filter = "All Files|*.*;" ELSE opendlg.filter = filters
		opendlg.caption = caption
		opendlg.initialdir = QDIALOGS.dir
		
		IF opendlg.execute THEN
			result = opendlg.filename
		ELSE
			RESULT = ""
		END IF		
	END FUNCTION
	
	FUNCTION savedlg(caption AS STRING, filters AS STRING) AS STRING
		DIM savedlg AS QSAVEDIALOG
		IF filters = "" THEN savedlg.filter = "All Files|*.*;" ELSE savedlg.filter = filters
		savedlg.caption = caption
		savedlg.initialdir = QDIALOGS.dir
		
		IF savedlg.execute THEN
			RESULT = savedlg.filename
		ELSE
			RESULT = ""
		END IF
	END FUNCTION
	
	FUNCTION YesNodlg(caption AS STRING) AS INTEGER
		IF MESSAGEDLG (caption, 3, 1 OR 2, 0) = 6 THEN RESULT = 1 ELSE RESULT = 0
	END FUNCTION

	FUNCTION YesNoCanceldlg(caption AS STRING) AS INTEGER
		SELECT CASE MESSAGEDLG(caption, 3, 1 OR 2 OR 8, 0)
			CASE 2: RESULT = 2			'Cancel
			CASE 6: RESULT = 1			'Yes
			CASE 7: RESULT = 0			'No
		END SELECT
	END FUNCTION
END TYPE

