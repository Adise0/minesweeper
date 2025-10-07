# kill previous if any
$pidFile = "$PSScriptRoot/app.pid"
if (Test-Path $pidFile) {
  try { Stop-Process -Id (Get-Content $pidFile) -Force -ErrorAction Stop } catch {}
  Remove-Item $pidFile -ErrorAction SilentlyContinue
}

# launch in a *separate* window and save PID
$p = Start-Process -FilePath "$PSScriptRoot/build/app.exe" `
                   -WorkingDirectory "$PSScriptRoot/" `
                   -Wait `
                   -WindowStyle Minimized `
                   -PassThru  # new window by default for console apps
Set-Content -Path $pidFile -Value $p.Id
