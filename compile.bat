@echo off
setlocal enabledelayedexpansion

:: 1. Читаем текущую версию (НЕ увеличиваем её)
if not exist "version.txt" (echo [ERR] version.txt missing! & pause & exit /b 1)
set /p VER=<version.txt
for /f "tokens=1-4 delims=." %%a in ("%VER%") do (set V1=%%a& set V2=%%b& set V3=%%c& set V4=%%d)
set "REL_PATH=releases\v%VER%"

:: 2. Чистим и создаем папки
if exist "obj_rel" rmdir /s /q "obj_rel"
mkdir "obj_rel"
if exist "temp_build" rmdir /s /q "temp_build"
mkdir "temp_build"

:: 3. Параллельная компиляция объектов
echo [1/3] Compiling Release v%VER% (Parallel)...
set "OBJECTS="
set "CPU_CORES=%NUMBER_OF_PROCESSORS%"
set "RUNNING=0"

for /r "src" %%f in (*.cpp) do (
    set "OBJ=obj_rel\%%~nf.o"
    set "OBJECTS=!OBJECTS! "!OBJ!""
    
    echo Compiling: %%~nxf
    :: Запуск в фоне
    start /b g++ -O3 -DNDEBUG -DVERSION_STRING="\"%VER%\"" -I"C:/SDL/include/" -c "%%f" -o "!OBJ!"
    
    set /a RUNNING+=1
    if !RUNNING! geq !CPU_CORES! (
        timeout /t 1 /nobreak >nul
        set "RUNNING=0"
    )
)

:WAIT_GPP
tasklist /fi "IMAGENAME eq g++.exe" | find "g++.exe" >nul
if %errorlevel% equ 0 (
    timeout /t 1 /nobreak >nul
    goto WAIT_GPP
)

:: 4. Ресурсы (со всеми версиями)
echo [2/3] Resources...
windres metadata.rc -O coff -o obj_rel/res.o ^
    --codepage=1251 ^
    -DV1=%V1% -DV2=%V2% -DV3=%V3% -DV4=%V4%

if !errorlevel! neq 0 (echo [ERR] Resource Error! & pause & exit /b 1)

:: 5. Линковка
echo [3/3] Linking Release EXE...
g++ -mwindows -O3 -s !OBJECTS! obj_rel/res.o -o "temp_build/game.exe" ^
    -L"C:/SDL/lib/" -lsdl2 -lsdl2_image -lsdl2_ttf -lsdl2_mixer -static-libgcc -static-libstdc++

if !errorlevel! neq 0 (echo [ERR] Build failed! & pause & exit /b 1)

:: 6. Перенос файлов
if exist "%REL_PATH%" rmdir /s /q "%REL_PATH%"
mkdir "%REL_PATH%\bin" "%REL_PATH%\src" 2>nul
move /Y "temp_build\game.exe" "%REL_PATH%\bin\" >nul
if exist "output" xcopy /E /Y /I "output\*" "%REL_PATH%\bin\" >nul
xcopy /E /Y /I "src\*" "%REL_PATH%\src\src\" >nul
for %%f in (metadata.rc *.bat *.txt) do (copy /Y "%%f" "%REL_PATH%\src\" >nul)
rmdir /s /q temp_build

echo.
echo ========================================
echo [SUCCESS] Release v%VER% ready in %REL_PATH%
pause
