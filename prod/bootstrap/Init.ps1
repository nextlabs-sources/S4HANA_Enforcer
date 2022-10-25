$scriptPath=Split-Path $MyInvocation.MyCommand.Path -Parent;
$addPath=$scriptPath;
Write-Host "Begin add [$addPath] to system environment...";
$target='Machine'; # Machine, User, Process
#append path environment
$orgPath = [Environment]::GetEnvironmentVariable('Path', $target);
if ($orgPath.Contains($addPath)) 
{
    Write-Host "The path [$addPath] already add into system environment";
} 
else 
{
    $newPath = $orgPath + ';' + $addPath;
    $result = "Success";
    [Environment]::SetEnvironmentVariable("Path", $newPath, $target);
    if (!$?) { $result = "Failed" }
    Write-Host "$result path [$addPath] to system environment, target:[$target]";
}

#add DAEInstallPath environment
$installPath=Split-Path $scriptPath -Parent;
$installPathName="DAEInstallPath";
[Environment]::SetEnvironmentVariable($installPathName, $installPath, $target);
$result = "Success";
if (!$?) { $result = "Failed" }
Write-Host "add [$installPathName=$installPath] to system environment $result.";
