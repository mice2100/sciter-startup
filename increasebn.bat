ECHO OFF
set /P BN=<BN.TXT
if not defined BN set BN=0
set /a BN = %BN%+1
echo %BN%>BN.TXT