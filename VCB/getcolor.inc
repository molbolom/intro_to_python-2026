' Color chooser dialog example

$TYPECHECK ON


CONST CC_RGBINIT = &H1
CONST CC_FULLOPEN = &H2
CONST CC_PREVENTFULLOPEN = &H4
CONST CC_SHOWHELP = &H8
CONST CC_ENABLEHOOK = &H10
CONST CC_ENABLETEMPLATE = &H20
CONST CC_ENABLETEMPLATEHANDLE = &H40
CONST CC_SOLIDCOLOR = &H80
CONST CC_ANYCOLOR = &H100

TYPE TCHOOSECOLOR
    lStructSize AS LONG
    hWndOwner AS LONG
    hInstance AS LONG
    rgbResult AS LONG
    CustColors(1 TO 16) AS LONG
    Flags AS DWORD
    lCustData AS LONG
    lpfnHook AS LONG
    lpTemplateName AS LONG
END TYPE

DECLARE FUNCTION ChooseColorDlg LIB "COMDLG32" ALIAS "ChooseColorA" _
                 (CC AS TCHOOSECOLOR) AS LONG
DECLARE FUNCTION getcolor(parent AS LONG, curcolor AS LONG) AS LONG
DIM CC AS TCHOOSECOLOR


FUNCTION getcolor(parent AS LONG, curcolor AS LONG) AS LONG
   DIM retval AS LONG
    CC.CustColors(1) = &HFF0000
    CC.CustColors(2) = &H00FF00
    CC.CustColors(3) = &H0000FF
    CC.CustColors(4) = &HFF00FF
    CC.CustColors(5) = &H00FFFF
    CC.CustColors(6) = &HFFFFFF
    CC.CustColors(7) = &H559911
    CC.CustColors(8) = &HEE44BB
    CC.CustColors(9) = &HBB44EE
    CC.CustColors(10) = &H115599
    CC.CustColors(11) = &H333333
    CC.CustColors(12) = &H666666
    CC.CustColors(13) = &H999999
    CC.CustColors(14) = &HABABAB
    CC.CustColors(15) = &HDDDDDD
    CC.CustColors(16) = &H550000

   CC.lstructsize = SIZEOF(CC)
   CC.hWndOwner = parent
   CC.Flags = CC_RGBINIT + CC_FULLOPEN
   CC.rgbresult = curcolor
   retval = ChooseColorDlg(CC)
   IF retval = 0 THEN getcolor = -1 ELSE getcolor = CC.rgbresult
END FUNCTION

