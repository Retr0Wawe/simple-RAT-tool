format PE console

include 'win32a.inc'

entry DllEntrypoint

section '.data' data readable writable

        result db 'Dll has been started!', 0


section '.text' code readable executable

        DllEntrypoint:
                push result
                call [printf]
                invoke client      ; <- Calling the entry point from the import table, where we specified the name and entry point of the dll.
		

section '.idata' import data readable

        library msvcrt, 'msvcrt.dll', \ 
                RAT, 'RAT.dll'     ; <- Import table from where we import dll.

        import RAT,\
               client, 'client'   ; <- Entry point name.

        import msvcrt,\
               printf, 'printf'
