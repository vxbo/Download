@ECHO OFF

FOR /R %%D IN (.) DO (
    IF EXIST "%%D\obj" (
        ECHO [MAID] DEVOURING: "%%D\obj"
        RMDIR /S /Q "%%D\obj"
    )
    IF EXIST "%%D\bin" (
        ECHO [MAID] DEVOURING: "%%D\bin"
        RMDIR /S /Q "%%D\bin"
    )
)

ECHO [MAID] SUCCESS

:main
CD Cownloads.Tests
IF errorlevel 1 (
    ECHO Failed to change directory to Cownloads.Tests. Exiting.
    GOTO :EOF
)

dotnet clean
IF errorlevel 1 (
    ECHO dotnet clean failed. Exiting.
    GOTO :EOF
)

dotnet restore
IF errorlevel 1 (
    ECHO dotnet restore failed. Exiting.
    GOTO :EOF
)

dotnet build
IF errorlevel 1 (
    ECHO dotnet build failed. Exiting.
    GOTO :EOF
)

ECHO Build completed successfully.

PAUSE >NUL
GOTO :EOF

:EOF