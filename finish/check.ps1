$orig = (Get-Content original.txt -Raw) -replace '[^0-9A-Fa-f]',''
$dec  = (Get-Content output2.txt -Raw) -replace 'Total length.*','' -replace '[^0-9A-Fa-f]',''

Write-Host "Orig: $($orig.Length) hex chars"
Write-Host "Dec:  $($dec.Length) hex chars"

if ($orig.ToUpper() -eq $dec.ToUpper()) {
    Write-Host "MATCH: Shellcode is identical"
} else {
    Write-Host "MISMATCH"
}