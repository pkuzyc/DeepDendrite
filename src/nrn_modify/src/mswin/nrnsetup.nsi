; nrnsetup.nsi
;
; This script is based on example2.nsi
;
; It will install the contents of c:/nrn into a directory that the user selects,
;

;Configuration

!define Version "7.7"

; The name of the installer
Name "NEURON ${Version}"

; The file to write
OutFile "nrn${Version}setup.exe"

RequestExecutionLevel user

SetCompressor bzip2

; Declaration of user variables
var "myCygVer1"
var "myCygVer2"
var "myCygVer3"
var "myCygVer"
var "cpCygwin"
var "uINSTDIR" ; posix version of $INSTDIR

; The basic program group name
!define NEURON  "NEURON ${Version}"
; The default directory
!define NRN "nrn"
; where to get the files
!define MARSHAL "c:\marshalnrn"
; where is cygwin on the host	
!define CYGWIN "c:\cygwin"

; The default installation directory
;InstallDir $PROGRAMFILES\${NRN}
; Do not want spaces in the installation path
InstallDir c:\${NRN}

;HKCU file association requires a subkey of Software\Classes\...
!define ASSOCPREFIX "Software\Classes\"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKCU "Software\NEURON_Simulator\${NRN}" "Install_Dir"

Function .onInit ; called when installer is nearly finished initializing.
  ReadRegStr $R1 HKCU "Software\NEURON_Simulator\${NRN}" "Install_Dir"
  StrCmp "$R1" "" ok +1
    MessageBox MB_OKCANCEL "${NEURON} already installed at $R1. Press OK to install again." IDOK ok
    Abort
ok:
  GetDLLVersionLocal "${MARSHAL}\nrn\bin\cygwin1.dll" $R0 $R1
  IntOp $R2 $R0 / 0x00010000
  IntOp $myCygVer1 $R2 / 1000
  IntOp $myCygVer2 $R2 % 1000
  IntOp $myCygVer3 $R0 & 0x000FFFF
  StrCpy $myCygVer "$myCygVer1.$myCygVer2.$myCygVer3"
  StrCpy $cpCygwin "no"
  IfFileExists "${CYGWIN}\bin\cygwin1.dll" +1 one
	GetDllVersion "${CYGWIN}\bin\cygwin1.dll" $R0 $R1
	IntOp $R2 $R0 / 0x00010000
	IntOp $R3 $R2 / 1000
	IntOp $R4 $R2 % 1000
	IntOp $R5 $R0 & 0x000FFFF
	StrCpy $0 "$R3.$R4.$R5"
	IntCmp $myCygVer1 $R3 +1 meold menew
	IntCmp $myCygVer2 $R4 +1 meold menew
	IntCmp $myCygVer3 $R5 one meold menew
	Goto one
meold:
	StrCpy $cpCygwin "yes"
	MessageBox MB_OKCANCEL "Already installed cygwin1.dll version $0 is NEWER and will be cused in place of this installer distributed version $myCygVer . If you have trouble running NEURON after installation you will need to upgrade to a more recent version of NEURON. Press OK to continue installing." IDOK one
	Abort
menew:
	MessageBox MB_OKCANCEL|MB_DEFBUTTON2 "Already installed cygwin1.dll version $0 is OLDER and may be incompatible with this installer distributed version $myCygVer . It is recommended that you upgrade to the most recent version from http://www.cygwin.org. Press OK to continue installing." IDOK one
	Abort
one:
FunctionEnd

; The text to prompt the user to enter a directory
ComponentText "This will install ${NEURON} on your computer. Select which optional things you want installed."

Page components
Page directory "" "" stayInDirectory
Page instfiles

; Get the install path. It cannot have spaces
; The text to prompt the user to enter a directory
DirText "Choose a directory without spaces to install in to (cannot have spaces in the path):"

Function stayInDirectory
	Push "$INSTDIR"
	Push " "
	Call StrStr
	Pop $R0
	StrCmp $R0 "" +3 ; if no spaces then go on
	MessageBox MB_OK "The installation path is not allowed to have spaces."
	Abort
	IfFileExists "$INSTDIR\*.*" +1 +3
	MessageBox MB_OK "$INSTDIR already exists. Remove it first or choose another path."
	Abort
FunctionEnd

; The primary neuron  install
Section "${NEURON} (required)"
  SectionIn 1 RO
  Push $INSTDIR
  Call d2u
  Pop $uINSTDIR
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File /r "${MARSHAL}\nrn\*.*"
  ; otherwise cannot figure out how to put the installation folder in the file
  FileOpen $r1 "$INSTDIR\lib\nrnstart.bsh" "w"
  FileWrite $r1 "#!/bin/bash$\n"
  FileWrite $r1 "export N=$uINSTDIR$\n"
  FileWrite $r1 "export PATH=$$N/bin:$$PATH$\n"
  FileWrite $r1 "export PYTHONHOME=$$N$\n"
  FileClose $r1
  Call cyginst
  ; Write the installation path into the registry
  WriteRegStr HKCU "Software\NEURON_Simulator\${NRN}" "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NEURON}" "DisplayName" "${NEURON} (remove only)"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NEURON}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
SectionEnd

Function cyginst
  StrCmp $cpCygwin "no" one +1
    Rename "$INSTDIR\bin\cygwin1.dll" "$INSTDIR\bin\cygwin1.dist"
    CopyFiles /SILENT /FILESONLY "${CYGWIN}\bin\cygwin1.dll" "$INSTDIR\bin\cygwin1.dll" 1000
one:
FunctionEnd

; optional section
Section "Associate .hoc and .nrnzip"
  ; I am trying to follow the makensis.nsi
  ; I do not know why we should back up old values
  WriteRegStr HKCU "${ASSOCPREFIX}.hoc" "" "NEURONFile"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONFile" "" "NEURON model file"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONFile\shell" "" "open"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONFile\DefaultIcon" "" "$INSTDIR\bin\nrniv10.ico"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONFile\shell\open\command" "" '"$INSTDIR\bin\neuron.exe" "%1"'
  WriteRegStr HKCU "${ASSOCPREFIX}.nrnzip" "" "NEURONArchive"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONArchive" "" "NEURON model archive"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONArchive\shell" "" "open"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONArchive\DefaultIcon" "" "$INSTDIR\bin\nrniv.ico"
  WriteRegStr HKCU "${ASSOCPREFIX}NEURONArchive\shell\open\command" "" '"$INSTDIR\bin\mos2nrn.exe" "%1"'
  FileOpen $r1 "$INSTDIR\lib\associate.dat" "w"
  FileWrite $r1 "If $INSTDIR\lib\associate.dat exists then, on uninstall, .hoc and .nrnzip associations will be removed."
  FileClose $r1
SectionEnd

; optional section
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\${NEURON}"
  SetOutPath "C:\" ; does not work, ends up as C
  StrCpy $OUTDIR "C:\"
  CreateShortCut "$SMPROGRAMS\${NEURON}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${NEURON}\nrngui.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i -c 'nrniv $uINSTDIR/lib/hoc/nrngui.hoc'" "$INSTDIR\bin\nrniv10.ico" 0
  CreateShortCut "$SMPROGRAMS\${NEURON}\nrngui_python.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i -c 'nrniv -python $uINSTDIR/lib/hoc/nrngui.hoc'" "$INSTDIR\bin\nrniv10.ico" 0
  CreateShortCut "$SMPROGRAMS\${NEURON}\mknrndll.lnk" "$INSTDIR\bin\nrniv.exe" "$uINSTDIR/lib/hoc/mknrndll.hoc" "$INSTDIR\bin\nmodl2a.ico" 0
  CreateShortCut "$SMPROGRAMS\${NEURON}\modlunit.lnk" "$INSTDIR\bin\nrniv.exe" "$uINSTDIR/lib/hoc/modlunit.hoc" "$INSTDIR\bin\nmodl2a.ico" 0
  CreateShortCut "$SMPROGRAMS\${NEURON}\Notes.lnk" "notepad.exe" "$INSTDIR\notes.txt"
  WriteINIStr "$SMPROGRAMS\${NEURON}\NEURON Home Page.url" "InternetShortcut" "URL" "http://neuron.yale.edu/"
  SetOutPath "$INSTDIR\demo"
  CreateShortCut "$SMPROGRAMS\${NEURON}\NEURON Demo.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i -c 'cd $uINSTDIR/demo ; nrniv -dll $uINSTDIR/demo/release/nrnmech.dll demo.hoc'" "$INSTDIR\bin\nrniv10.ico" 0
  SetOutPath "C:\"
  CreateShortCut "$SMPROGRAMS\${NEURON}\mintty bash.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i" "$INSTDIR\bin\mintty.exe" 0
  FileOpen $r1 "$INSTDIR\lib\smsinst.dat" "w"
  FileWrite $r1 "If $INSTDIR\lib\smsinst.dat exists then, on uninstall, the ${NEURON} Start Menu Shortcuts will be removed."
  FileClose $r1
SectionEnd

; optional section
Section "Desktop ${NEURON} folder with Shortcuts"
  CreateDirectory "$DESKTOP\${NEURON}"
  SetOutPath "C:\" ; does not work, ends up as C
  StrCpy $OUTDIR "C:\"
  CreateShortCut "$DESKTOP\${NEURON}\nrngui.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i -c 'nrniv $uINSTDIR/lib/hoc/nrngui.hoc'" "$INSTDIR\bin\nrniv10.ico" 0
  CreateShortCut "$DESKTOP\${NEURON}\nrngui_python.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i -c 'nrniv -python $uINSTDIR/lib/hoc/nrngui.hoc'" "$INSTDIR\bin\nrniv10.ico" 0
  CreateShortCut "$DESKTOP\${NEURON}\mknrndll.lnk" "$INSTDIR\bin\nrniv.exe" "$uINSTDIR/lib/hoc/mknrndll.hoc" "$INSTDIR\bin\nmodl2a.ico" 0
  CreateShortCut "$DESKTOP\${NEURON}\modlunit.lnk" "$INSTDIR\bin\nrniv.exe" "$uINSTDIR/lib/hoc/modlunit.hoc" "$INSTDIR\bin\nmodl2a.ico" 0
  CreateShortCut "$DESKTOP\${NEURON}\Notes.lnk" "notepad.exe" "$INSTDIR\notes.txt"
  WriteINIStr "$DESKTOP\${NEURON}\NEURON Home Page.url" "InternetShortcut" "URL" "http://neuron.yale.edu/"
  SetOutPath "$INSTDIR\demo"
  CreateShortCut "$DESKTOP\${NEURON}\NEURON Demo.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i -c 'cd $uINSTDIR/demo ; nrniv -dll $uINSTDIR/demo/release/nrnmech.dll demo.hoc'" "$INSTDIR\bin\nrniv10.ico" 0
  SetOutPath "C:\"
  CreateShortCut "$DESKTOP\${NEURON}\mintty bash.lnk" "$INSTDIR\bin\mintty.exe" "-c $uINSTDIR/lib/minttyrc $uINSTDIR/bin/bash --rcfile $uINSTDIR/lib/nrnstart.bsh -i" "$INSTDIR\bin\mintty.exe" 0
  FileOpen $r1 "$INSTDIR\lib\deskinst.dat" "w"
  FileWrite $r1 "If $INSTDIR\lib\deskinst.dat exists then, on uninstall, the ${NEURON} Desktop folder will be removed."
  FileClose $r1
SectionEnd

; optional section
Section "Documentation"
  SetOutPath "$INSTDIR"
  File /nonfatal /r "${MARSHAL}\html"
  IfFileExists "$INSTDIR\lib\smsinst.dat" +1 next
    CreateShortCut "$SMPROGRAMS\${NEURON}\Documentation.lnk" "$INSTDIR\html\helpfils.html"
next:
  IfFileExists "$INSTDIR\lib\deskinst.dat" +1 next2
    CreateShortCut "$DESKTOP\${NEURON}\Documentation.lnk" "$INSTDIR\html\helpfils.html"
next2:
SectionEnd

; uninstall stuff

UninstallText "This will uninstall ${NEURON}. Hit next to continue."

; special uninstall section.
Section "Uninstall"
  IfFileExists $INSTDIR\bin\neuron.exe skip_confirmation
    MessageBox MB_YESNO "It does not appear that ${NEURON} is installed in the directory '$INSTDIR'.$\r$\nContinue anyway (not recommended)" IDYES skip_confirmation
    Abort "Uninstall aborted by user"
  skip_confirmation:

  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NEURON}"
  DeleteRegKey HKCU "Software\NEURON_Simulator"
  ; remove registry keys if we own them
  IfFileExists "$INSTDIR\lib\associate.dat" +1 asoc
    DeleteRegKey HKCU "${ASSOCPREFIX}NEURONFile"
    DeleteRegKey HKCU "${ASSOCPREFIX}NEURONArchive"
    DeleteRegKey HKCU "${ASSOCPREFIX}.hoc"
    DeleteRegKey HKCU "${ASSOCPREFIX}.nrnzip"
asoc:
  ; remove directories used.
  IfFileExists "$INSTDIR\lib\smsinst.dat" +1 one
    ClearErrors
    IfFileExists "$SMPROGRAMS\${NEURON}\*.*" +1 one
    RMDir /r "$SMPROGRAMS\${NEURON}"
    IfErrors +1 +2
      MessageBox MB_OK "$SMPROGRAMS\${NEURON} could not be removed. Perhaps it is in use."
one:
    IfFileExists "$INSTDIR\lib\deskinst.dat" +1 two
    ClearErrors
    IfFileExists "$DESKTOP\${NEURON}\*.*" +1 two
    RMDir /r "$DESKTOP\${NEURON}"
    IfErrors +1 +2
      MessageBox MB_OK "$DESKTOP\${NEURON} could not be removed. Perhaps it is is use."
two:
    ClearErrors
    RMDir /r "$INSTDIR"
    IfErrors +1 +2
      MessageBox MB_OK "$SMPROGRAMS\${NEURON} could not be removed. Perhaps it is is use."
SectionEnd


; needed to determine if there are spaces in InstallDir
; StrStr
 ; input, top of stack = string to search for
 ;        top of stack-1 = string to search in
 ; output, top of stack (replaces with the portion of the string remaining)
 ; modifies no other variables.
 ;
 ; Usage:
 ;   Push "this is a long ass string"
 ;   Push "ass"
 ;   Call StrStr
 ;   Pop $R0
 ;  ($R0 at this point is "ass string")

 Function StrStr
 Exch $R1 ; st=haystack,old$R1, $R1=needle
   Exch    ; st=old$R1,haystack
   Exch $R2 ; st=old$R1,old$R2, $R2=haystack
   Push $R3
   Push $R4
   Push $R5
   StrLen $R3 $R1
   StrCpy $R4 0
   ; $R1=needle
   ; $R2=haystack
   ; $R3=len(needle)
   ; $R4=cnt
   ; $R5=tmp
   loop:
     StrCpy $R5 $R2 $R3 $R4
     StrCmp $R5 $R1 done
     StrCmp $R5 "" done
     IntOp $R4 $R4 + 1
     Goto loop
 done:
   StrCpy $R1 $R2 "" $R4
   Pop $R5
   Pop $R4
   Pop $R3
   Pop $R2
   Exch $R1
 FunctionEnd

; cygwin 7 needs a posix version of InstallDir
; d2u
 ; input, top of stack = dos path
 ; output, top of stack = posix path
 ; modifies no other variables
 ;
 ; Usage:
 ;  Push "c:\foo\bar"
 ;  Call d2u
 ;  Pop $R0
 ;  ($R0 at this point is "/cygdrive/c/foo/bar")

Function d2u
  Exch $R1 ; st=old$R1, $R1 = dospath
  Push $R2
  Push $R3
  Push $R4
  StrLen $R3 $R1
  StrCpy $R4 "/cygdrive/"
  loop:
    StrCpy $R2 "$R1" 1 ; first char remaining in R1
    StrCmp $R2 "" done
    StrCpy $R1 "$R1" $R3 1 ; replace with everythin except first char
    StrCmp $R2 ":" loop ; discard
    StrCmp $R2 "\" +1 +2 ; only reverse backslash, all others pass through
    StrCpy $R2 "/"
    StrCpy $R4 "$R4$R2" ; append char
    Goto loop
 done:
  StrCpy $R1 $R4
  Pop $R4
  Pop $R3
  Pop $R2
  Exch $R1
FunctionEnd
; eof

