echo off
schtasks /delete /tn * /f
at 09:00 /every:M,T,W,Th,F,S,Su "C:\Program Files\StockerSEM\EquipmentDataBackup.exe"
schtasks /change /ru Administrator /rp qhdks2008 /tn "At1"