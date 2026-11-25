# Запускає всі програми ЛР11 по черзі та зберігає вивід у output_all.txt (UTF-8).
# Запуск (з папки проєкту, після збирання):  .\run_all.ps1
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

$programs = @(
    "part1_variant8", "part2_variant8", "part3_variant8", "part4_variant8",
    "task1_original", "task1_fixed", "task2_lambda_capture", "task3_fixed", "task4_fixed",
    "part6_variant8"
)

$lines = foreach ($p in $programs) {
    "######## $p"
    & ".\build\$p.exe" 2>&1
    ""
}
$lines | Out-File -FilePath output_all.txt -Encoding utf8
$lines
Write-Host "Збережено у output_all.txt"
