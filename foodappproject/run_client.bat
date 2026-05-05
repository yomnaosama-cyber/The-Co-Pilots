@echo off
REM Client Machine Launcher for Windows
REM Replace HOSTNAME with your actual host machine Windows hostname
REM Replace 100.x.x.x with your host's Tailscale IP

set FOODAPP_RUN_SERVER=0
set FOODAPP_DB_PATH=\\HOSTNAME\FoodAppData\food_sharing.db
set FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
set GOOGLE_MAPS_API_KEY=
foodappproject.exe
pause
