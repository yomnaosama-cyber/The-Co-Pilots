# Tailscale Configuration Guide

This document provides all required configuration steps for running the Food Sharing App across multiple devices using Tailscale VPN.

---

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Tailscale Network Setup](#tailscale-network-setup)
3. [Host Machine Configuration](#host-machine-configuration)
4. [Client Device Configuration](#client-device-configuration)
5. [Environment Variables Reference](#environment-variables-reference)
6. [Verification Steps](#verification-steps)
7. [Troubleshooting](#troubleshooting)
8. [Network Architecture](#network-architecture)

---

## Prerequisites

### Requirements
- **Tailscale Account** (free tier is sufficient)
  - Sign up at: https://tailscale.com/download
  - Free for personal use, no credit card required

- **One host machine** (should stay ON during testing/demo)
  - Recommended: Windows 10/11 with stable internet
  - At least 2 GB RAM, 500 MB disk space

- **Client devices** (laptops, tablets, or other machines)
  - Windows, macOS, Linux, or mobile devices
  - Can run simultaneously

- **Network Access**
  - All devices need internet connectivity for Tailscale connection
  - Once connected, devices communicate via private Tailscale VPN

- **Shared Network Storage** (for database)
  - Windows: SMB share (recommended)
  - Cross-platform: NFS or SMB
  - Must have read/write access from all devices

---

## Tailscale Network Setup

### Step 1: Install Tailscale on All Devices

#### Windows / macOS / Linux

1. Download Tailscale from: https://tailscale.com/download
2. Install the application
3. Launch Tailscale

#### Mobile Devices
1. Download from App Store (iOS) or Play Store (Android)
2. Install and launch

### Step 2: Connect All Devices to Same Tailscale Account

**On Each Device:**

1. Open Tailscale application
2. Click "Login with Tailscale"
3. Browser opens → Login with your Tailscale account
4. Confirm device name (e.g., "dev-laptop", "driver-phone", "org-tablet")
5. Device connects to your Tailscale network

### Step 3: Verify Network Connectivity

**From any device, open terminal/command prompt:**

```bash
# Ping the host machine
ping 100.117.189.110
```

**Expected output:**
```
PING 100.117.189.110 (100.117.189.110) 56(84) bytes of data.
64 bytes from 100.117.189.110: icmp_seq=1 ttl=64 time=25.3 ms
```

If pings succeed, your Tailscale network is ready.

---

## Host Machine Configuration

The host machine runs the map server and hosts the shared database file.

### Step 1: Create Shared Database Folder

#### Windows Example

```powershell
# Create folder
New-Item -Path "D:\FoodAppData" -ItemType Directory -Force

# Share the folder (via GUI)
# 1. Right-click D:\FoodAppData → Properties
# 2. Sharing tab → Advanced Sharing
# 3. Check "Share this folder"
# 4. Click "Permissions" → Give Full Control to all users
# 5. Click OK
```

#### macOS/Linux Example

```bash
# Create folder with proper permissions
mkdir -p /home/shared/foodappdata
chmod 755 /home/shared/foodappdata
```

### Step 2: Get Your Host Machine's Tailscale IP

1. Go to https://tailscale.com/admin/machines
2. Find your host machine in the list
3. Copy its Tailscale IP address (format: `100.x.x.x`)

**Your Host IP:** `100.117.189.110`

### Step 3: Set Host Machine Environment Variables

#### Windows (via System Properties)

1. Press `Win + X` → "System"
2. Click "Advanced system settings"
3. Click "Environment Variables" button
4. Under "System variables" click "New..."
5. Add the following variables:

| Variable | Value |
|----------|-------|
| `FOODAPP_RUN_SERVER` | `1` |
| `FOODAPP_DB_PATH` | `D:\FoodAppData\food_sharing.db` |
| `FOODAPP_SERVER_BASE_URL` | `http://100.117.189.110:3000` |
| `GOOGLE_MAPS_API_KEY` | Your actual API key (optional) |

**Your Tailscale IP:** `100.117.189.110`

#### Windows (via PowerShell - Temporary)

```powershell
# Temporary (for current session only)
$env:FOODAPP_RUN_SERVER = "1"
$env:FOODAPP_DB_PATH = "D:\FoodAppData\food_sharing.db"
$env:FOODAPP_SERVER_BASE_URL = "http://100.117.189.110:3000"
$env:GOOGLE_MAPS_API_KEY = "your_api_key_here"

# Then run your app
.\foodappproject.exe
```

#### Windows (via .bat file - Recommended)

Create a file `run_host.bat`:

```batch
@echo off
set FOODAPP_RUN_SERVER=1
set FOODAPP_DB_PATH=D:\FoodAppData\food_sharing.db
set FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
set GOOGLE_MAPS_API_KEY=your_api_key_here

REM Run the application
.\foodappproject.exe

pause
```

Run by double-clicking `run_host.bat`

#### Linux/macOS (via .sh file)

Create a file `run_host.sh`:

```bash
#!/bin/bash

export FOODAPP_RUN_SERVER=1
export FOODAPP_DB_PATH=/home/shared/foodappdata/food_sharing.db
export FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
export GOOGLE_MAPS_API_KEY=your_api_key_here

# Run the application
./foodappproject
```

Run via:
```bash
chmod +x run_host.sh
./run_host.sh
```

### Step 4: Build and Launch on Host

1. Build the application (if not already built)
2. Launch using the batch/shell script from Step 3
3. Verify in console output:
   ```
   === Food Sharing App Configuration ===
   Database Path: D:\FoodAppData\food_sharing.db
   Server URL: http://100.117.189.110:3000
   Run Map Server: YES
   ...
   ```

### Step 5: Verify Host is Ready

**Expected signs host is ready:**
- App launches without errors
- Debug output shows `Run Map Server: YES`
- Database file appears at `D:\FoodAppData\food_sharing.db`
- Map server listens on port 3000

**Quick test from another device:**
```bash
curl http://100.117.189.110:3000/
# Should return HTML content
```

---

## Client Device Configuration

Client devices connect to the host's map server and shared database.

### Step 1: Get Host Machine's Tailscale IP

Your Host IP: `100.117.189.110` (same as host configuration)

### Step 2: Set Client Environment Variables

#### Windows (via PowerShell)

```powershell
$env:FOODAPP_RUN_SERVER = "0"
$env:FOODAPP_DB_PATH = "\\100.117.189.110\FoodAppData\food_sharing.db"
$env:FOODAPP_SERVER_BASE_URL = "http://100.117.189.110:3000"
# GOOGLE_MAPS_API_KEY is optional if using fallback

# Run the application
.\foodappproject.exe
```

**Or create `run_client.bat`:**

```batch
@echo off
set FOODAPP_RUN_SERVER=0
set FOODAPP_DB_PATH=\\100.117.189.110\FoodAppData\food_sharing.db
set FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000

REM Run the application
.\foodappproject.exe

pause
```

#### Linux/macOS (via .sh file)

```bash
#!/bin/bash

export FOODAPP_RUN_SERVER=0
export FOODAPP_DB_PATH=/mnt/host/FoodAppData/food_sharing.db
export FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000

./foodappproject
```

### Step 3: Verify Client Configuration

**Check debug output:**
```
=== Food Sharing App Configuration ===
Database Path: \\100.117.189.110\FoodAppData\food_sharing.db
Server URL: http://100.117.189.110:3000
Run Map Server: NO (client mode)
...
```

---

## Environment Variables Reference

### FOODAPP_DB_PATH

**Purpose:** Location of the SQLite database file

**Host Value:**
```
D:\FoodAppData\food_sharing.db
```

**Client Value (Windows):**
```
\\100.117.189.110\FoodAppData\food_sharing.db
```

**Client Value (Linux/macOS):**
```
/mnt/host/FoodAppData/food_sharing.db
```

**Fallback:** `food_sharing.db` (local file)

**Notes:**
- Must be the same for all devices
- Shared folder must have read/write access
- Path must be accessible from the current device

---

### FOODAPP_SERVER_BASE_URL

**Purpose:** Base URL of the map server (host machine)

**Value Format:**
```
http://[HOST_TAILSCALE_IP]:3000
```

**Your Host URL:**
```
http://100.117.189.110:3000
```

**Fallback:** `http://localhost:3000`

**Notes:**
- Same value on both host and clients
- Replace IP with your actual Tailscale IP
- Port 3000 is hardcoded in map_server

---

### FOODAPP_RUN_SERVER

**Purpose:** Controls whether to start the local map server

**Host Value:**
```
1
```

**Client Value:**
```
0
```

**Fallback:** Not set (server is NOT started)

**Notes:**
- Only host should have this set to `1`
- Prevents multiple servers on the same network
- Clients set this to `0` or leave it unset

---

### GOOGLE_MAPS_API_KEY

**Purpose:** Google Maps API key for map features

**Value:** Your actual Google Maps API key

**Fallback:** Built-in key (limited by restrictions)

**Notes:**
- Optional if you accept API limitations
- Get key from: https://console.cloud.google.com
- Can be set on host or left as fallback

---

## Verification Steps

### Pre-Launch Checks

**On Host Machine:**
- [ ] Tailscale is running and connected
- [ ] Shared folder created at `D:\FoodAppData` or equivalent
- [ ] Folder is shared with read/write access
- [ ] Host Tailscale IP identified (e.g., `100.117.189.110`)
- [ ] Environment variables set correctly
- [ ] Port 3000 is not blocked by firewall

**On Client Devices:**
- [ ] Tailscale is running and connected
- [ ] Can ping host IP: `ping 100.117.189.110`
- [ ] Environment variables set correctly
- [ ] Network path is reachable: `ping 100.117.189.110`

### Launch Verification

**Step 1: Start Host**

```bash
# Windows
run_host.bat

# Linux/macOS
./run_host.sh
```

**Expected output (first 10 lines):**
```
=== Food Sharing App Configuration ===
Database Path: D:\FoodAppData\food_sharing.db
Server URL: http://100.117.189.110:3000
Run Map Server: YES
API Key: Using fallback key
====================================

[Map server starting...]
[Database opened at...]
```

**Step 2: Verify Server is Running**

From any client device:
```bash
curl http://100.117.189.110:3000/
```

**Expected:** Returns HTML content (map_view.html page)

**Step 3: Start Client**

```bash
# Windows
run_client.bat

# Linux/macOS
./run_client.sh
```

**Expected output (first 10 lines):**
```
=== Food Sharing App Configuration ===
Database Path: \\100.117.189.110\FoodAppData\food_sharing.db
Server URL: http://100.117.189.110:3000
Run Map Server: NO (client mode)
API Key: Using fallback key
====================================

[Database opened at...]
```

**Step 4: Test Multi-Device Synchronization**

1. **Device A (Organization):** Register and submit donation
2. **Device B (People):** Sign up and request meals
3. **Device C (Delivery):** Accept order, open tracking
4. **Verify:** All devices see the same data in real-time

---

## Troubleshooting

### Problem: "Database connection failed"

**Causes:**
- Shared folder not accessible
- Network path incorrect
- File doesn't exist yet

**Solutions:**
1. Verify network share exists: `dir \\100.117.189.110\FoodAppData`
2. Check database path in `FOODAPP_DB_PATH`
3. Ensure host has created the database (run host first)

---

### Problem: "Map server connection refused"

**Causes:**
- Host machine not running
- Port 3000 blocked by firewall
- Wrong Tailscale IP

**Solutions:**
1. Verify host is running: `ping 100.117.189.110`
2. Check map server started: `curl http://100.117.189.110:3000/`
3. Verify correct Tailscale IP in `FOODAPP_SERVER_BASE_URL`
4. Check firewall allows port 3000

---

### Problem: "Ping fails to 100.117.189.110"

**Causes:**
- Tailscale not running on one device
- Devices not logged into same account
- Network connectivity issue

**Solutions:**
1. Ensure Tailscale is running on both devices
2. Check both logged into same Tailscale account
3. Restart Tailscale on problematic device
4. Check internet connectivity

---

### Problem: "Database is locked" or "Access denied"

**Causes:**
- Multiple instances accessing database simultaneously
- File permissions issue
- Antivirus blocking access

**Solutions:**
1. Close app on other devices
2. Verify network share has proper permissions
3. Whitelist database folder in antivirus
4. Run as Administrator (Windows)

---

### Problem: "API Key rejected / Maps not loading"

**Causes:**
- API key has restrictions
- Billing not enabled
- Wrong key provided

**Solutions:**
1. Go to https://console.cloud.google.com
2. Verify API key is valid
3. Check billing is enabled
4. Remove referrer restrictions if present
5. Use fallback key for testing

---

## Network Architecture

```
┌─────────────────────────────────────────────────────┐
│                 TAILSCALE VPN NETWORK               │
│        (Private network between all devices)        │
└──────────────────┬──────────────────────────────────┘
                   │
         ┌─────────┼─────────┐
         │         │         │
    ┌────▼────┐ ┌──▼──┐ ┌───▼────┐
    │  HOST   │ │CLIENT│ │CLIENT2 │
    │   PC    │ │  PC  │ │ Tablet │
    │         │ │      │ │        │
    │ Serves: │ │Connects to:      │
    │ - Map   │ │ - Shared DB      │
    │   Server│ │ - Map Server     │
    │ - DB    │ │ - Port 3000      │
    │ - Port  │ │                  │
    │   3000  │ │                  │
    └────┬────┘ └──┬───┘ └───┬────┘
         │         │        │
         └────┬────┴────┬───┘
              │         │
         ┌────▼─────────▼────┐
         │ Shared Folder     │
         │ D:\FoodAppData\   │
         │ food_sharing.db   │
         └───────────────────┘
```

### Communication Flow

1. **Database Access:**
   - Host & Clients → Shared folder via SMB/NFS → Shared database file

2. **Map Server Access:**
   - Clients → HTTP request → Port 3000 on host → Map server process

3. **Tailscale VPN:**
   - Encrypts all traffic between devices
   - No port forwarding or firewall rules needed
   - All communication is private and secure

---

## Firewall Configuration (if needed)

### Windows Firewall

If port 3000 is blocked:

```powershell
# Allow port 3000 inbound
New-NetFirewallRule -DisplayName "Map Server 3000" `
  -Direction Inbound -Action Allow -Protocol TCP -LocalPort 3000

# Allow SMB (already usually allowed)
# Port 445 for SMB sharing
```

### Linux (ufw)

```bash
sudo ufw allow 3000
sudo ufw allow 445
sudo ufw enable
```

### macOS

macOS usually doesn't block local network ports. If issues:
- System Preferences → Security & Privacy → Firewall → Firewall Options
- Add application to allowed apps

---

## Demo Day Runbook

**Before the class/demo:**

1. **30 minutes before:**
   - [ ] Start host machine
   - [ ] Launch app with `run_host.bat` (host configuration)
   - [ ] Verify database created: `D:\FoodAppData\food_sharing.db` exists
   - [ ] Verify map server running: `curl http://100.117.189.110:3000/`

2. **15 minutes before:**
   - [ ] Launch app on delivery device with `run_client.bat`
   - [ ] Launch app on organization device
   - [ ] Launch app on people device
   - [ ] Verify all three apps are open and connected

3. **During demo:**
   - [ ] Device A (Org): Register and submit donation
   - [ ] Device B (People): Sign up and request meals
   - [ ] Device C (Driver): Accept order and open tracking
   - [ ] Verify real-time synchronization across all devices

---

## Quick Start Checklist

- [ ] Install Tailscale on all devices
- [ ] Connect all devices to same Tailscale account
- [ ] Create shared database folder on host
- [ ] Note host's Tailscale IP address
- [ ] Set environment variables on host
- [ ] Set environment variables on clients
- [ ] Build application (if needed)
- [ ] Launch host with map server
- [ ] Verify map server is running (curl test)
- [ ] Launch clients
- [ ] Test multi-device synchronization
- [ ] Ready for demo!

---

## Additional Resources

- **Tailscale Documentation:** https://tailscale.com/kb/
- **Tailscale Admin Console:** https://tailscale.com/admin/machines
- **Google Maps API:** https://console.cloud.google.com
- **Food Sharing App:** See [TAILSCALE_IMPLEMENTATION_CHANGES.md](TAILSCALE_IMPLEMENTATION_CHANGES.md)
