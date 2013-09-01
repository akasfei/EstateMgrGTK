@set GTKGLEXT_PREFIX=C:\GtkGLExt\1.0

@echo Setting environment variables for GtkGLExt.
@echo.

@echo set PATH=%GTKGLEXT_PREFIX%\bin;%%PATH%%
@set PATH=%GTKGLEXT_PREFIX%\bin;%PATH%

@echo set PKG_CONFIG_PATH=%GTKGLEXT_PREFIX%\lib\pkgconfig;%%PKG_CONFIG_PATH%%
@set PKG_CONFIG_PATH=%GTKGLEXT_PREFIX%\lib\pkgconfig;%PKG_CONFIG_PATH%

@echo.
