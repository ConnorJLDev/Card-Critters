REM wpr -pmcsources
REM wpr -HeapTracingConfig CardCritters.exe enable
wpr -start Profiles.wprp!Nightjar_%1_Profile -filemode
pushd ..\Nightjar\ 
..\Build\x64\Release\CardCritters.exe
popd
wpr -stop Trace_%1.etl
REM wpr -HeapTracingConfig CardCritters.exe disable