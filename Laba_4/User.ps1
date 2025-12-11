
$KILLER_SCRIPT = ".\Killer.ps1"

if (-not (Test-Path $KILLER_SCRIPT)) {
    Write-Host "Error: Killer script $KILLER_SCRIPT not found. Ensure it is in the same folder." -ForegroundColor Red
    exit 1
}

Write-Host "               STARTING USER TEST SCENARIO          "

Write-Host "--- 1. Creating test processes (Using Notepad, Timeout, and Calc) ---" -ForegroundColor Cyan

$pName = "notepad"
$pName_proc = Start-Process $pName -WindowStyle Hidden -PassThru
$USER_NAME_PID = $pName_proc.Id
Write-Host "Created P1 (Target: Name Test - 'notepad'): PID $USER_NAME_PID"

$pIdName = "timeout"
$pId_proc = Start-Process cmd.exe -ArgumentList "/c timeout /t 3600" -WindowStyle Hidden -PassThru
$USER_ID_PID = $pId_proc.Id
Write-Host "Created P2 (Target: ID Test - 'timeout'): PID $USER_ID_PID"

$pKill1 = "notepad"
$pKill1_proc = Start-Process $pKill1 -WindowStyle Hidden -PassThru
$USER_KILL_1_PID = $pKill1_proc.Id
Write-Host "Created P3 (Target: Env Test 1 - 'notepad'): PID $USER_KILL_1_PID"

$pKill2 = "calc"
$pKill2_proc = Start-Process $pKill2 -WindowStyle Hidden -PassThru
$USER_KILL_2_PID = $pKill2_proc.Id
Write-Host "Created P4 (Target: Env Test 2 - 'calc'): PID $USER_KILL_2_PID" # Имя процесса - calc.exe

$env:PROC_TO_KILL = "$pKill1,$pKill2"
Write-Host "`n--- 2. Environment variable PROC_TO_KILL set to '$env:PROC_TO_KILL' (Targets: P3, P4) ---" -ForegroundColor Yellow


#Running Killer for all 3 required parameter variations

Write-Host "`n--- 3.1. Running Killer to terminate processes via PROC_TO_KILL (Targets P3, P4) ---"
# Killer.ps1 убивает P3 (notepad) и P4 (calc). P1 и P2 остаются.
& $KILLER_SCRIPT

# MANDATORY REQUIREMENT: Delete the variable after use.
Remove-Item Env:\PROC_TO_KILL
Write-Host "--- 3.1.1. Environment variable PROC_TO_KILL removed. ---" -ForegroundColor Yellow


Write-Host "`n--- 3.2. Running Killer to terminate by ID (Target P2: PID $USER_ID_PID) ---"
# Killer.ps1 убивает P2 (timeout). P1 остается.
& $KILLER_SCRIPT -id $USER_ID_PID

Write-Host "`n--- 3.3. Running Killer to terminate by Name (Target P1: Name $pName) ---"
# Killer.ps1 убивает P1 (notepad).
& $KILLER_SCRIPT -name $pName


Write-Host "`n--- 4. Checking final results (Verification) ---" -ForegroundColor Green

function Check-ProcessStatus {
    param(
        [int]$ProcessId,
        [string]$Name
    )
    # Проверяем, существует ли процесс
    if (Get-Process -Id $ProcessId -ErrorAction SilentlyContinue) {
        Write-Host "Error: Process '$Name' (PID $ProcessId) STILL RUNNING." -ForegroundColor Red
        # Cleanup: убиваем на случай сбоя
        Stop-Process -Id $ProcessId -Force -ErrorAction SilentlyContinue
    } else {
        Write-Host "Success: Process '$Name' (PID $ProcessId) successfully terminated." -ForegroundColor Green
    }
}

Check-ProcessStatus $USER_NAME_PID "P1 (Name Test)"
Check-ProcessStatus $USER_ID_PID "P2 (ID Test)"
Check-ProcessStatus $USER_KILL_1_PID "P3 (Kill Env Test 1)"
Check-ProcessStatus $USER_KILL_2_PID "P4 (Kill Env Test 2)"

Write-Host "                 TEST COMPLETED                     "
