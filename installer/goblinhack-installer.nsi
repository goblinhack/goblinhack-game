; goblinhack-installer.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install goblinhack-installer.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "GoblinHack"

; The file to write
OutFile "goblinhack-installer.exe"
ShowInstDetails "nevershow"
ShowUninstDetails "nevershow"

; The default installation directory
InstallDir $PROGRAMFILES\GoblinHack

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\NSIS_GoblinHack" "Install_Dir"

;--------------------------------

; Pages


Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "GoblinHack (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ;
  ; Usually I mess up saved files from one to the next version, 
  ; best just to remove
  ;
  ; Turn this off one day when the game is more stable
  ;
  RMDir /r "$INSTDIR\src\"

  ;
  ; For old releases that used this subdir
  ;
  RMDir /r "$INSTDIR\goblinhack\"
  
  ; Put file there
  File /r "goblinhack\*"
  File "ghack.ico"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\NSIS_GoblinHack "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GoblinHack" "DisplayName" "NSIS GoblinHack"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GoblinHack" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GoblinHack" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GoblinHack" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\GoblinHack"
  CreateDirectory "$SMPROGRAMS\GoblinHack\src"
  CreateDirectory "$SMPROGRAMS\GoblinHack\src\resources"
  CreateDirectory "$SMPROGRAMS\GoblinHack\src\resources\saved"

  CreateShortCut "$SMPROGRAMS\GoblinHack\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\ghack.ico"
  CreateShortCut "$SMPROGRAMS\GoblinHack\GoblinHack.lnk" "$INSTDIR\src\ghack.exe" "" "$INSTDIR\ghack.ico"

  CreateShortCut "$DESKTOP\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\ghack.ico"
  CreateShortCut "$DESKTOP\GoblinHack.lnk" "$INSTDIR\src\ghack.exe" "" "$INSTDIR\ghack.ico"
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GoblinHack"
  DeleteRegKey HKLM SOFTWARE\NSIS_GoblinHack

  ; Remove files and uninstaller
  Delete $INSTDIR\goblinhack-installer.nsi
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\GoblinHack\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\GoblinHack"
  RMDir "$INSTDIR"
SectionEnd

Function .onInstSuccess
  MessageBox MB_YESNO "Play GoblinHack right now? (flashing graphics warning)" IDNO NoGhack
    Exec $INSTDIR\ghack.exe
  NoGhack:
FunctionEnd
