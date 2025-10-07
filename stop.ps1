$pidFile = "$PSScriptRoot/app.pid"
if (Test-Path $pidFile) {
  try { Stop-Process -Id (Get-Content $pidFile) -Force -ErrorAction Stop } catch {}
  Remove-Item $pidFile -ErrorAction SilentlyContinue
}
