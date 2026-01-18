@echo off
:: Устанавливаем кодировку UTF-8 для корректного отображения русского языка
chcp 65001 >nul
setlocal enabledelayedexpansion

:: 1. Работа с версией
if not exist "version.txt" echo 1.0.0.0 > version.txt
set /p OLD_VER=<version.txt

for /f "tokens=1-4 delims=." %%a in ("%OLD_VER%") do (
    set V1=%%a& set V2=%%b& set V3=%%c& set V4=%%d
)
set /a NEW_V4=V4+1
set NEW_VER=%V1%.%V2%.%V3%.%NEW_V4%

:: 2. Настройки
set "GPP=g++"
set "SDL_INC=C:/SDL/include/"
set "SDL_LIB=C:/SDL/lib/"
set "OPTS=-g -O0 -DVERSION_STRING="\"%NEW_VER%\"" -I"%SDL_INC%""
set "LIBS=-L"%SDL_LIB%" -lsdl2 -lsdl2_image -lsdl2_ttf -lsdl2_mixer -static-libgcc -static-libstdc++"

:: Создаем временную папку для объектных файлов
set "TEMP_OBJ=%TEMP%\build_tmp_%RANDOM%"
mkdir "%TEMP_OBJ%"
if not exist "debug" mkdir debug

echo [1/2] Проверка зависимостей (v%NEW_VER%)...
set "OBJECTS="
set "NEED_LINK=0"

for /r "src" %%f in (*.cpp) do (
    set "OBJ=%TEMP_OBJ%\%%~nf.o"
    set "OBJECTS=!OBJECTS! "!OBJ!""
    
    :: Упрощенная логика: компилируем всегда при использовании временной папки
    echo Компиляция: %%~nxf...
    !GPP! !OPTS! -c "%%f" -o "!OBJ!"
    if !errorlevel! neq 0 (echo [ОШИБКА] Сбой компиляции! & pause & exit /b 1)
    set "NEED_LINK=1"
)

:: 3. Ресурсы
echo [2/2] Ресурсы...
windres metadata.rc -O coff -o "%TEMP_OBJ%\res.o" ^
    --codepage=1251 ^
    -DV1=%V1% -DV2=%V2% -DV3=%V3% -DV4=%NEW_V4% -DIS_DEBUG

if !errorlevel! neq 0 (echo [ОШИБКА] Ошибка ресурсов! & pause & exit /b 1)

:: 4. Линковка
echo Сборка исполняемого файла debug_game.exe...
!GPP! !OBJECTS! "%TEMP_OBJ%\res.o" -o "debug/debug_game.exe" !LIBS!

if !errorlevel! neq 0 (
    echo [ОШИБКА] Ошибка линковки!
    rd /s /q "%TEMP_OBJ%"
    pause
    exit /b 1
)

:: Чистим временные файлы
rd /s /q "%TEMP_OBJ%"

:: Сохраняем версию только при успехе
echo %NEW_VER%>version.txt
echo [УСПЕХ] Сборка v%NEW_VER% завершена.

if exist "output" xcopy /Y /I "output\*" "debug\" >nul

echo.
echo ========================================
echo Сборка готова. Нажмите любую клавишу для запуска.
pause >nul

if exist "debug\debug_game.exe" (

    
    :: Переходим в папку с игрой, чтобы экзешник видел свои ресурсы
    cd /d "debug"
    
    :: Запускаем игру. Теперь точка отсчета — папка debug
    start "" "debug_game.exe"
)
