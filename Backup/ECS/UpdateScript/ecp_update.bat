@echo  off

mkdir c:\stocker_temp 
copy /Y observables.ini c:\stocker_temp

sqlcmd -S localhost\SQLExpress -i ecp_update.sql 


rmdir /Q /S c:\stocker_temp


