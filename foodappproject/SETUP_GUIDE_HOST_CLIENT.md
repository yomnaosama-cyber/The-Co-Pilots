# Food Sharing App - Host & Client Setup Guide (A to Z)

Complete setup for shared database and live tracking across multiple machines using Tailscale.

---

## PART 1: HOST MACHINE SETUP

### Option 1A: Host on Windows (Qt Creator)

#### Step 1: Install Tailscale on Host
1. Download Tailscale from https://tailscale.com/download/windows
2. Install it
3. Click "Log In" and sign in with your Tailscale account
4. Note your Tailscale IP (visible in Tailscale menu) - looks like `100.x.x.x`

#### Step 2: Create Shared Database Folder
1. On your host machine, create folder: `C:\FoodAppData`
2. Right-click `FoodAppData` → Properties → Sharing tab → "Share"
3. Add your username and set permissions to "Read/Write"
4. Note the shared path (usually `\\HOSTNAME\FoodAppData`)
5. **The database file `food_sharing.db` will be created automatically here when the app first runs**

#### Step 3: Update run_host.bat
Edit `run_host.bat` in your project folder:

```batch
@echo off
set FOODAPP_RUN_SERVER=1
set FOODAPP_DB_PATH=C:\FoodAppData\food_sharing.db
set FOODAPP_SERVER_BASE_URL=http://100.x.x.x:3000
set GOOGLE_MAPS_API_KEY=<your_actual_key_here_or_leave_empty>
foodappproject.exe
pause
```

Replace `100.x.x.x` with your actual Tailscale IP from Step 1.

#### Step 4: Build in Qt Creator
1. Open Qt Creator
2. Open `CMakeLists.txt` from your project
3. Configure for Desktop (MSVC 2022 64-bit)
4. Click Build → Build All
5. Build output will be in `build/windows-no-tests/Debug/` or similar

#### Step 5: Run Host App
1. Open File Explorer
2. Navigate to your build output folder (e.g., `build/windows-no-tests/Debug/`)
3. Double-click `run_host.bat` OR copy it to the build folder first and run it
4. **You should see:**
   - "Map server disabled" message? That's OK, it means it needs to start the C++ server
   - Database path printed to console
   - The main app window opens

#### Step 6: Verify Host is Working
- The app should create `C:\FoodAppData\food_sharing.db` automatically
- The map server should be running on port 3000
- Check: Open browser, go to `http://localhost:3000/map_view.html` - should load (might show errors but page loads)

---

### Option 1B: Host on Ubuntu (WSL)

#### Step 1: Install WSL2 and Ubuntu
```bash
# In Windows PowerShell (as Admin):
wsl --install -d Ubuntu-22.04
```

#### Step 2: Install Tailscale in WSL
```bash
# Inside WSL terminal:
curl -fsSL https://tailscale.com/install.sh | sh
sudo tailscale up
```
Follow the login prompt, then get your Tailscale IP:
```bash
sudo tailscale ip -4
# Note the IP (looks like 100.x.x.x)
```

#### Step 3: Install Qt and Build Tools in WSL
```bash
sudo apt update
sudo apt install -y qt6-base-dev qt6-tools-dev cmake build-essential libgl1-mesa-dev
```

#### Step 4: Create Shared Database Folder
```bash
# Create folder on Windows side (accessible from WSL)
# In WSL, it's at: /mnt/c/FoodAppData
mkdir -p /mnt/c/FoodAppData
chmod 777 /mnt/c/FoodAppData
```

#### Step 5: Build the App in WSL
```bash
cd /mnt/c/Users/AUC/Desktop/Malak_final/host-machine/The-Co-Pilots/foodappproject
mkdir -p build-wsl
cd build-wsl
cmake ..
make -j$(nproc)
```

#### Step 6: Create run_host_wsl.sh
Create this file in your project root:

```bash
#!/bin/bash
export FOODAPP_RUN_SERVER=1
export FOODAPP_DB_PATH=/mnt/c/FoodAppData/food_sharing.db
export FOODAPP_SERVER_BASE_URL=http://100.x.x.x:3000
export GOOGLE_MAPS_API_KEY=
export QT_QPA_PLATFORM=offscreen
export DISPLAY=:0

./build-wsl/foodappproject
```

Replace `100.x.x.x` with your WSL Tailscale IP.

```bash
chmod +x run_host_wsl.sh
```

#### Step 7: Run Host App on WSL
```bash
# If using GUI (WSLg):
./run_host_wsl.sh

# If headless (just serving map):
# Just ensure the process stays running
```

---

## PART 2: CLIENT MACHINE SETUP

### Option 2A: Client on Windows (Qt Creator)

#### Step 1: Install Tailscale on Client
1. Download and install Tailscale
2. Sign in with the **same Tailscale account** as the host
3. Verify you can ping the host:
   ```
   ping 100.x.x.x
   ```
   (Use the host's Tailscale IP from Part 1)

#### Step 2: Build the App (Same as Host)
1. Open Qt Creator
2. Build the same project (same CMakeLists.txt)
3. Build output will be in `build/windows-no-tests/Debug/`

#### Step 3: Create run_client.bat
Create this file in the build folder:

```batch
@echo off
set FOODAPP_RUN_SERVER=0
set FOODAPP_DB_PATH=\\HOSTNAME\FoodAppData\food_sharing.db
set FOODAPP_SERVER_BASE_URL=http://100.x.x.x:3000
set GOOGLE_MAPS_API_KEY=
foodappproject.exe
pause
```

**Replace:**
- `HOSTNAME` with your host machine's Windows hostname (visible in System Properties)
- `100.x.x.x` with the host's Tailscale IP

#### Step 4: Test Database Connection
1. Double-click `run_client.bat`
2. Check console for: `Database Path: \\HOSTNAME\FoodAppData\food_sharing.db`
3. App should open without errors
4. Try signing up as a delivery driver - data should be saved to the shared database

#### Step 5: Test Delivery Tracking
1. Click "Delivery notifications" → should pull data from shared database
2. Accept a delivery (if available from host's data)
3. Click "Pickup" → map should load and show coordinates
4. Verify map loads with proper tracking

---

### Option 2B: Client on Ubuntu (WSL)

#### Step 1: Install Tailscale in WSL (Same as Host Step 2)
```bash
curl -fsSL https://tailscale.com/install.sh | sh
sudo tailscale up
# Use same Tailscale account as host
```

#### Step 2: Build the App (Same as Host Step 5)
```bash
cd /mnt/c/Users/AUC/Desktop/Malak_final/host-machine/The-Co-Pilots/foodappproject
mkdir -p build-wsl-client
cd build-wsl-client
cmake ..
make -j$(nproc)
```

#### Step 3: Create run_client_wsl.sh
```bash
#!/bin/bash
export FOODAPP_RUN_SERVER=0
export FOODAPP_DB_PATH=/mnt/c/FoodAppData/food_sharing.db
export FOODAPP_SERVER_BASE_URL=http://100.x.x.x:3000
export GOOGLE_MAPS_API_KEY=
export QT_QPA_PLATFORM=offscreen
export DISPLAY=:0

./build-wsl-client/foodappproject
```

Replace `100.x.x.x` with the host's Tailscale IP.

```bash
chmod +x run_client_wsl.sh
./run_client_wsl.sh
```

---

## QUICK TROUBLESHOOTING

### "Cannot access database" or "Permission denied"
- **Windows:** Check that `C:\FoodAppData` folder is shared with Read/Write permissions
- **WSL:** Check that `/mnt/c/FoodAppData` has write permissions: `chmod 777 /mnt/c/FoodAppData`

### "Map server not responding" or "Cannot reach http://100.x.x.x:3000"
- Verify Tailscale IP is correct: `tailscale ip -4` (or check Tailscale menu on Windows)
- Verify host has `FOODAPP_RUN_SERVER=1` set
- Verify port 3000 is not blocked by firewall: `netstat -an | findstr 3000` (Windows)

### "Delivery tracking page is blank"
- Check browser console (F12) for errors
- Verify `GOOGLE_MAPS_API_KEY` is set (or using fallback)
- Verify `FOODAPP_SERVER_BASE_URL` includes the correct IP

### App crashes when opening map
- Check that Qt WebEngine is installed
- On WSL, set `export QT_QPA_PLATFORM=offscreen` if running headless
- Check console output for detailed error messages

---

## VERIFICATION CHECKLIST

### Host Machine
- [ ] Tailscale installed and running
- [ ] `C:\FoodAppData` folder created and shared
- [ ] `food_sharing.db` exists in the folder (created on first run)
- [ ] `run_host.bat` has correct Tailscale IP
- [ ] App starts and shows "Database Path: C:\FoodAppData\food_sharing.db"
- [ ] Map server starts (no "disabled" message)

### Client Machine
- [ ] Tailscale installed and signed in with same account
- [ ] Can ping host: `ping 100.x.x.x` ✓
- [ ] `run_client.bat` has correct IP and UNC path
- [ ] App starts without permission errors
- [ ] Can sign up as delivery driver → data appears on host

### Delivery Tracking Test
1. On **host**: Open app, add some food donations via the main app
2. On **client**: Open app, request meals
3. On **client**: Click "Delivery notifications" → should see matched deliveries
4. On **client**: Accept a delivery
5. On **client**: Click "Pickup" → map should load with live tracking
6. Both machines should show the same data

---

## FINAL NOTES

- **Database is SINGLE SHARED FILE:** All machines read/write the same `food_sharing.db`
- **Tailscale makes networking work:** No manual port forwarding needed
- **Host runs the map server:** Only the host has `FOODAPP_RUN_SERVER=1`
- **Clients only consume the API:** Clients set `FOODAPP_RUN_SERVER=0`
- **Environment variables are key:** Each machine's `.bat` or `.sh` file configures it as host or client

Once all steps are complete, you should be able to:
- Add donors/restaurants on any machine → appears on all machines
- Add meal requests on any machine → appears on all machines
- Accept deliveries → live tracking works across machines
- All data is persistent in the shared SQLite file
