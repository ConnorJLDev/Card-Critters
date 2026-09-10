tar.exe -cvaf .\TemplateSystem.zip __TemplateIcon.ico TemplateSystem.vstemplate TemplateSystem.h TemplateSystem.cpp
tar.exe -cvaf .\TemplateObject.zip __TemplateIcon.ico TemplateObject.vstemplate TemplateObject.h TemplateObject.inl TemplateObject.cpp
tar.exe -cvaf .\TemplateComponent.zip __TemplateIcon.ico TemplateComponent.vstemplate TemplateComponent.h TemplateComponent.cpp TemplateEditor.cpp
tar.exe -cvaf .\TemplateBehavior.zip __TemplateIcon.ico TemplateBehavior.vstemplate TemplateBehavior.h TemplateBehavior.cpp TemplateEditor.cpp
REM pause
xcopy .\*.zip "%userprofile%\Documents\Visual Studio 2022\Templates\ItemTemplates\" /Y
xcopy .\*.zip "%userprofile%\Documents\Visual Studio 18\Templates\ItemTemplates\" /Y
pause