#!/bin/bash
# Client Machine Launcher for WSL/Ubuntu
# Replace 100.x.x.x with your host's Tailscale IP

export FOODAPP_RUN_SERVER=0
export FOODAPP_DB_PATH=/mnt/c/FoodAppData/food_sharing.db
export FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
export GOOGLE_MAPS_API_KEY=
export QT_QPA_PLATFORM=offscreen
export DISPLAY=:0

./build-wsl-client/foodappproject
