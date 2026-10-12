@echo off
setlocal
cd /d "%~dp0"
rem The archived executable is a test build that turns AI on at launch.
rem Send one Q keystroke after its window is created to restore manual play.
powershell -NoProfile -WindowStyle Hidden -Command "$p = Start-Process -FilePath '%~dp0UNO.exe' -WorkingDirectory '%~dp0' -PassThru; Start-Sleep -Milliseconds 1200; $shell = New-Object -ComObject WScript.Shell; if ($shell.AppActivate($p.Id)) { $shell.SendKeys('q') }"
