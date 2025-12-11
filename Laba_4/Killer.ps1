
param(
    [Parameter(Mandatory=$false)]
    [int]$id,

    [Parameter(Mandatory=$false)]
    [string]$name
)

function Stop-ProcessById {
    param(
        [int]$ProcessId
    )
    Write-Host "--- Stopping process by ID: $ProcessId ---" -ForegroundColor Yellow
    try {
        $process = Get-Process -Id $ProcessId -ErrorAction Stop
        Stop-Process -Id $ProcessId -Force
        Write-Host "V Process ID $ProcessId ('$($process.ProcessName)') successfully terminated." -ForegroundColor Green
    } catch {
        Write-Host "X Error/Warning: Process ID $ProcessId not found or failed to terminate." -ForegroundColor Red
    }
}

function Stop-ProcessByName {
    param(
        [string]$ProcessName
    )
    Write-Host "--- Stopping processes by Name: $ProcessName ---" -ForegroundColor Yellow
    try {
        Get-Process -Name $ProcessName -ErrorAction Stop | Stop-Process -Force
        Write-Host "V Processes named '$ProcessName' successfully terminated (if running)." -ForegroundColor Green
    } catch {
        Write-Host "X Warning: Processes named '$ProcessName' not found or failed to terminate." -ForegroundColor Red
    }
}

if ($id -ne 0) {
    Stop-ProcessById $id
}

if (-not [string]::IsNullOrEmpty($name)) {
    Stop-ProcessByName $name
}

#Handle termination by Environment Variable PROC_TO_KILL
$procToKill = $env:PROC_TO_KILL
if (-not [string]::IsNullOrEmpty($procToKill)) {
    Write-Host "`n--- Stopping processes from PROC_TO_KILL: $procToKill ---" -ForegroundColor Cyan
    
    $namesToKill = $procToKill -split ',' | ForEach-Object { $_.Trim() }

    foreach ($pName in $namesToKill) {
        if (-not [string]::IsNullOrEmpty($pName)) {
            Write-Host "Attempting to terminate process: $pName"
            
            try {
                Get-Process -Name "$pName" -ErrorAction Stop | Stop-Process -Force
                Write-Host "Processes named '$pName' successfully terminated." -ForegroundColor Green
            } catch {
                Write-Host "Warning: Processes named '$pName' not found." -ForegroundColor Yellow
            }
        }
    }
}