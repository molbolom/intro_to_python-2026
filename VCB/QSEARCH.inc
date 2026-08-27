
CONST NO_SEARCH = 0

CONST SEARCH_PRESENT = 1
CONST SEARCH_ALL = 2
CONST SEARCH_DICTIONARY = 4

CONST BASIC_SEARCH = 8
CONST EXTENDED_SEARCH = 16

TYPE QSEARCH EXTENDS QFORM
	spnl AS QPANEL
	spresentRB AS QRADIOBUTTON
	sallRB AS QRADIOBUTTON
	sdicRB AS QRADIOBUTTON
   	sedit AS QRICHEDIT
    scB AS QBUTTON
   	sexpandB AS QBUTTON
	sB AS QBUTTON
	searchstate AS INTEGER

	SUB searchclick(sender AS QBUTTON)
		SELECT CASE sender.taborder
			CASE 3
				IF QSEARCH.spresentRB.checked = 1 THEN 
						QSEARCH.searchstate = SEARCH_PRESENT OR  BASIC_SEARCH
				ELSEIF QSEARCH.sallRB.checked = 1 THEN
						QSEARCH.searchstate = SEARCH_ALL OR BASIC_SEARCH
				ELSEIF QSEARCH.sdicRB.checked THEN 
						QSEARCH.searchstate = SEARCH_DICTIONARY OR BASIC_SEARCH
				END IF
			CASE 4
				IF QSEARCH.spresentRB.checked = 1 THEN 
						QSEARCH.searchstate = SEARCH_PRESENT OR EXTENDED_SEARCH
				ELSEIF QSEARCH.sallRB.checked = 1 THEN
						QSEARCH.searchstate = SEARCH_ALL OR EXTENDED_SEARCH
				ELSEIF QSEARCH.sdicRB.checked THEN 
						QSEARCH.searchstate = SEARCH_DICTIONARY OR EXTENDED_SEARCH
				END IF
			CASE 5: QSEARCH.searchstate = NO_SEARCH
		END SELECT
		QSEARCH.close	
	END SUB
			
	CONSTRUCTOR
		width = 520
		height = 245
		center
		borderstyle = 0
		
		spnl.parent = QSEARCH
				
		sB.parent = QSEARCH.spnl
		sexpandB.parent = QSEARCH.spnl
		scB.parent = QSEARCH.spnl
		sedit.parent = QSEARCH.spnl
		spresentRB.parent = QSEARCH.spnl
		sallRB.parent = QSEARCH.spnl
		sdicRB.parent = QSEARCH.spnl
		
        spresentRB.taborder = 0
        sallRB.TabOrder = 1
       	sdicRB.TabOrder = 2
		sB.TabOrder = 3
		sexpandB.TabOrder = 4	
		scB.TabOrder = 5   	
		sedit.TabOrder = 6
		spnl.taborder = 7		
		
		spnl.left = 2
		spnl.top = 2
		spnl.bevelouter = 1
		spnl.width = QSEARCH.clientwidth - 4
		spnl.height = QSEARCH.clientheight - 4	

		spresentRB.Caption = "Search Present Lesson"
        spresentRB.Left = 8
        spresentRB.Top = 136
        spresentRB.Width = 153
        spresentRB.checked = 1
		
        sallRB.Caption = "Search All Lessons"
        sallRB.Left = 8
        sallRB.Top = 176

	
	    sdicRB.Caption = "Search Dictionary"
      	sdicRB.Left = 8
       	sdicRB.Top = 216

   	
       	
		
		sB.Caption = "Search"
		sB.Left = 224
		sB.Top = 144
		sB.Width = 99
		sB.onclick = QSEARCH.searchclick
		
		sexpandB.Caption = "Expanded Search"
		sexpandB.Left = 376
		sexpandB.Top = 144
		sexpandB.Width = 99
		sexpandB.onclick = QSEARCH.searchclick

		scB.Caption = "Cancel"
		scB.Left = 400
		scB.Top = 208
		scB.Width = 99
		scB.onclick = QSEARCH.searchclick

		sedit.Left = 5
		sedit.Top = 5
		sedit.Width = QSEARCH.spnl.clientwidth - 10
		sedit.Height = 121
   	END CONSTRUCTOR
   	
   	FUNCTION stringcount AS INTEGER				'Returns the number of strings to search for.
   		DIM x AS INTEGER
   		DIM count AS INTEGER
   		FOR x = 0 TO LEN(QSEARCH.sedit.text)
   			IF mid$(QSEARCH.sedit.text, x, 1) = ";" THEN count = count + 1
   		NEXT x
   			IF RIGHT$(QSEARCH.sedit.text, 1) = ";" OR LEFT$(QSEARCH.sedit.text, 1) = ";" THEN RESULT = count ELSE RESULT = count + 1
   	END FUNCTION
   	
    FUNCTION execute AS INTEGER
    	QSEARCH.showmodal
    	RESULT = QSEARCH.searchstate
    END FUNCTION
END TYPE

'SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
'																		SEARCH AGAIN DIALOG
TYPE QSEARCHAGAIN EXTENDS QFORM
	research AS INTEGER
	sapnl AS QPANEL
	sal AS QLABEL
	
	SUB keydown(key AS WORD, shift AS INTEGER)
	   IF key = 78 THEN
		   QSEARCHAGAIN.research = 0
   		ELSE
	      	QSEARCHAGAIN.research = 1
	   	END IF
		QSEARCHAGAIN.close
	END SUB
	
	CONSTRUCTOR
		width = 112
		height = 22
		borderstyle = 0
		keypreview = 1
		onkeydown = QSEARCHAGAIN.keydown
		center
		
		sapnl.top = 2
		sapnl.left = 2
		sapnl.bevelinner = 1
		sapnl.width = QSEARCHAGAIN.clientwidth - 4
		sapnl.height = QSEARCHAGAIN.clientheight - 4
		sapnl.parent = QSEARCHAGAIN
		
		sal.caption = "Search Again? (Y/N)"
		sal.parent = QSEARCHAGAIN.sapnl
		sal.left = 4
		sal.top = 2
		sal.width = 104
		sal.transparent = 1		
	END CONSTRUCTOR
	
	FUNCTION execute AS INTEGER
		QSEARCHAGAIN.showmodal
		RESULT = QSEARCHAGAIN.research
	END FUNCTION
END TYPE



