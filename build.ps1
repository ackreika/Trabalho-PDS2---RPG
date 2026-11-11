# 1. Força o encerramento de qualquer Godot rodando em segundo plano para liberar o arquivo
Write-Host "Limpando processos da Godot..." -ForegroundColor Yellow
Stop-Process -Name "godot" -Force -ErrorAction SilentlyContinue
Start-Sleep -Seconds 1

# 2. Executa a limpeza e a compilação via MinGW
Write-Host "Compilando a DLL..." -ForegroundColor Cyan
mingw32-make clean
mingw32-make dll

# 3. Verifica se deu certo
if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilação bem-sucedida! DLL gerada sem bloqueios." -ForegroundColor Green
    
    # (Opcional) Descomente a linha abaixo se quiser que a Godot abra sozinha após compilar:
    # Start-Process "godot" -ArgumentList "--path `"$PSScriptRoot`""
} else {
    Write-Host "Erro durante a compilação. Verifique os erros acima." -ForegroundColor Red
}