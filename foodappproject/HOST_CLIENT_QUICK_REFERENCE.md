# HOST vs CLIENT - Quick Reference

## ENVIRONMENT VARIABLES COMPARISON

| Variable | HOST | CLIENT |
|----------|------|--------|
| `FOODAPP_RUN_SERVER` | `1` | `0` |
| `FOODAPP_DB_PATH` | `C:\FoodAppData\food_sharing.db` | `\\HOSTNAME\FoodAppData\food_sharing.db` |
| `FOODAPP_SERVER_BASE_URL` | `http://100.x.x.x:3000` | `http://100.x.x.x:3000` (same host IP) |
| `GOOGLE_MAPS_API_KEY` | Empty (uses fallback) | Empty (uses fallback) |

## WHAT EACH ENVIRONMENT VARIABLE DOES

**`FOODAPP_RUN_SERVER`**
- `1` = This machine STARTS the map server (only host should do this)
- `0` = This machine CONNECTS to a remote map server (client does this)

**`FOODAPP_DB_PATH`**
- HOST: Local path to the database file (it owns the file)
- CLIENT: Network path to the SAME file on the host's shared folder

**`FOODAPP_SERVER_BASE_URL`**
- Both use the HOST's Tailscale IP and port 3000
- This is how clients reach the map server running on the host

## WINDOWS LAUNCHER FILES

### Host: run_host.bat
```batch
@echo off
set FOODAPP_RUN_SERVER=1
set FOODAPP_DB_PATH=C:\FoodAppData\food_sharing.db
set FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
set GOOGLE_MAPS_API_KEY=
foodappproject.exe
pause
```

### Client: run_client.bat
```batch
@echo off
set FOODAPP_RUN_SERVER=0
set FOODAPP_DB_PATH=\\HOSTNAME\FoodAppData\food_sharing.db
set FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
set GOOGLE_MAPS_API_KEY=
foodappproject.exe
pause
```

**KEY DIFFERENCE:** Line 2 (`FOODAPP_RUN_SERVER`) and Line 3 (`FOODAPP_DB_PATH`)

## WSL LAUNCHER FILES

### Host: run_host_wsl.sh
```bash
#!/bin/bash
export FOODAPP_RUN_SERVER=1
export FOODAPP_DB_PATH=/mnt/c/FoodAppData/food_sharing.db
export FOODAPP_SERVER_BASE_URL=http://100.x.x.x:3000
export GOOGLE_MAPS_API_KEY=
./build-wsl/foodappproject
```

### Client: run_client_wsl.sh
```bash
#!/bin/bash
export FOODAPP_RUN_SERVER=0
export FOODAPP_DB_PATH=/mnt/c/FoodAppData/food_sharing.db
export FOODAPP_SERVER_BASE_URL=http://100.x.x.x:3000
export GOOGLE_MAPS_API_KEY=
./build-wsl-client/foodappproject
```

**KEY DIFFERENCE:** Only line 3 (`FOODAPP_RUN_SERVER`)

## ONE-MINUTE SETUP CHECKLIST

### BEFORE RUNNING ANYTHING:
- [ ] Install Tailscale on ALL machines
- [ ] Sign into same Tailscale account on ALL machines
- [ ] Get host's Tailscale IP: `tailscale ip -4`
- [ ] Create `C:\FoodAppData` on HOST and SHARE IT
- [ ] Update all launcher files with correct host Tailscale IP

### HOST SETUP (Windows):
1. Replace `100.x.x.x` in `run_host.bat` with actual Tailscale IP
2. Build project
3. Run `run_host.bat`
4. ✓ App opens, database file created

### CLIENT SETUP (Windows):
1. Replace `HOSTNAME` in `run_client.bat` with host machine name
2. Replace `100.x.x.x` in `run_client.bat` with actual Tailscale IP
3. Build project
4. Run `run_client.bat`
5. ✓ App opens, connects to shared database

### HOST SETUP (WSL):
1. Replace `100.x.x.x` in `run_host_wsl.sh` with actual Tailscale IP
2. `chmod +x run_host_wsl.sh`
3. Build project
4. Run `./run_host_wsl.sh`
5. ✓ App starts, map server running on port 3000

### CLIENT SETUP (WSL):
1. Replace `100.x.x.x` in `run_client_wsl.sh` with actual Tailscale IP
2. `chmod +x run_client_wsl.sh`
3. Build project
4. Run `./run_client_wsl.sh`
5. ✓ App connects to host's map server and database

---

## TESTING DELIVERY TRACKING (A to Z)

1. **On HOST machine:** Add food donations (Organization module or similar)
2. **On HOST machine:** Add people needing meals (People module)
3. **On CLIENT machine:** Open app, sign up as delivery driver
4. **On CLIENT machine:** Click "Delivery notifications" → should show matched deliveries
5. **On CLIENT machine:** Select a delivery and click "Accept Delivery"
6. **On CLIENT machine:** Click "Pickup" → map page should load
7. **Map page shows:** Pickup location and drop location with live tracking
8. ✓ **Success!** All machines sharing the same database and tracking works

---

## DATA FLOW DIAGRAM

```
HOST MACHINE                          CLIENT MACHINE
┌─────────────────────┐              ┌─────────────────────┐
│  foodappproject.exe │              │  foodappproject.exe │
│  FOODAPP_RUN_SERVER │              │  FOODAPP_RUN_SERVER │
│        = 1          │              │        = 0          │
│                     │              │                     │
│  ┌───────────────┐  │              │  ┌───────────────┐  │
│  │   Qt App      │  │              │  │   Qt App      │  │
│  └───────────────┘  │              │  └───────────────┘  │
│         │           │              │         │           │
│         ▼           │              │         ▼           │
│  ┌───────────────┐  │              │  ┌───────────────┐  │
│  │ Map Server    │  │              │  │ Network Call  │  │
│  │ (port 3000)   │◄──────Tailscale──────►Port 3000    │  │
│  └───────────────┘  │              │  └───────────────┘  │
│         │           │              │                     │
│         ▼           │              │                     │
│  ┌───────────────┐  │              │  ┌───────────────┐  │
│  │  SQLite DB    │  │              │  │  Network Path │  │
│  │food_sharing.db│◄──────UNC Share─────►\\HOST\Data   │  │
│  └───────────────┘  │              │  └───────────────┘  │
└─────────────────────┘              └─────────────────────┘
```

---

## KEY TAKEAWAYS

1. **Same Code, Different Config:** Same app executable, different environment variables
2. **One Database File:** All machines read/write the same `food_sharing.db`
3. **One Map Server:** Only host runs it on port 3000
4. **Tailscale = Networking Magic:** Handles IP routing so clients can reach host
5. **Environment Variables = The Switch:** FOODAPP_RUN_SERVER toggles host vs client mode
