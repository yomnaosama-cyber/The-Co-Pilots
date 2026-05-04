# How Tailscale Setup Works - Developer Guide

This guide explains the complete architecture and runtime flow of the Food Sharing App with Tailscale integration. It's designed for developers who need to understand the setup and build process.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Component Breakdown](#component-breakdown)
3. [Setup Process](#setup-process)
4. [Runtime Execution Flow](#runtime-execution-flow)
5. [Data Flow Between Devices](#data-flow-between-devices)
6. [Environment Variable Flow](#environment-variable-flow)
7. [Key Technical Details](#key-technical-details)

---

## Architecture Overview

### The Problem We're Solving

Before this setup, the app was **single-device only**:
- Database was local to each machine (`food_sharing.db`)
- Map server ran on `localhost:3000` (only accessible locally)
- No way to share data between devices
- Three roles (delivery, organization, people) couldn't collaborate

### The Solution: Tailscale + Shared Database

We've converted the app to **multi-device collaborative** mode:

```
┌─────────────────────────────────────────────────────┐
│         TAILSCALE VPN NETWORK (Encrypted)           │
│     All devices can talk securely without          │
│     complex network setup or port forwarding       │
└──────────────────┬────────────────────────────────┘
                   │
     ┌─────────────┼─────────────┐
     │             │             │
  ┌──▼──┐    ┌────▼────┐    ┌──▼──┐
  │HOST │    │ CLIENT 1 │    │ CLIENT2│
  │  PC │    │ (Driver) │    │ (Org) │
  └──┬──┘    └────┬────┘    └──┬──┘
     │            │             │
     │ STARTS:    │ STARTS:     │ STARTS:
     │ - Server   │ - Nothing   │ - Nothing
     │ - Database │ - Connect   │ - Connect
     │   process  │   to host   │   to host
     │            │             │
     └────────────┼─────────────┘
                  │
         ┌────────▼────────┐
         │ Shared Database │
         │ \\HOST_IP\Data\ │
         │ food_sharing.db │
         └─────────────────┘
```

---

## Component Breakdown

### 1. **Tailscale VPN Layer**

**What it is:** A secure mesh VPN that creates a private network between all connected devices.

**How it works:**
- Each device gets a unique IP in the `100.x.x.x` range (e.g., `100.117.189.110`)
- All traffic between devices is encrypted automatically
- No port forwarding or firewall configuration needed
- Free tier supports unlimited devices for personal use

**Role in our setup:**
- Makes `100.117.189.110` (host IP) accessible from any connected device
- Allows network file sharing (SMB) to work securely
- Enables HTTP requests to port 3000 on the host

---

### 2. **Host Machine**

**Responsibilities:**
1. Runs the **map server** on port 3000
2. Hosts the **shared database file** at `D:\FoodAppData\food_sharing.db`
3. Makes its Tailscale IP (`100.117.189.110`) publicly available to clients

**Code Logic (in `delivery.cpp`):**
```cpp
// On host machine:
QString runServerEnv = qgetenv("FOODAPP_RUN_SERVER");
if (runServerEnv == "1") {
    // Start the map server process
    d->serverProcess = new QProcess();
    d->serverProcess->start(serverBinary, ...);
    // Creates HTTP server listening on port 3000
}
```

**Requirements:**
- Must stay ON during testing/demo
- Must have folder `D:\FoodAppData` shared on the network
- Port 3000 must not be blocked by firewall
- Must have stable Tailscale connection

---

### 3. **Client Devices**

**Responsibilities:**
1. Do NOT start a local map server
2. Connect to the **host's map server** at `http://100.117.189.110:3000`
3. Read/write to the **shared database** at `\\100.117.189.110\FoodAppData\food_sharing.db`
4. Display real-time updates from other devices

**Code Logic (in `delivery.cpp`):**
```cpp
// On client device:
QString runServerEnv = qgetenv("FOODAPP_RUN_SERVER");
if (runServerEnv != "1") {
    // Skip starting server - we'll use host's server
    qDebug() << "Client mode: connecting to host server";
    return;  // Don't start server process
}
```

**Examples of client devices:**
- Delivery person's laptop
- Organization staff's tablet
- People in need's phone

---

### 4. **Shared Database**

**Location:** `\\100.117.189.110\FoodAppData\food_sharing.db`

**How it works:**
1. Host machine creates the folder `D:\FoodAppData`
2. Host shares this folder via SMB (Windows file sharing)
3. All clients connect to the same shared path
4. All devices read/write to the same SQLite file

**Flow:**
```
Host:    D:\FoodAppData\food_sharing.db
         ↓ (SMB Share)
Client1: \\100.117.189.110\FoodAppData\food_sharing.db
Client2: \\100.117.189.110\FoodAppData\food_sharing.db
```

**Code Implementation (in `database.cpp`):**
```cpp
QString dbPath = qgetenv("FOODAPP_DB_PATH");
if (dbPath.isEmpty()) {
    dbPath = "food_sharing.db";  // Fallback to local
} else {
    // dbPath will be the shared network path
}
db.setDatabaseName(dbPath);  // Open shared database
```

---

### 5. **Map Server**

**What it is:** A lightweight HTTP server that:
- Serves HTML map pages (`map_view.html`, `driver_live.html`)
- Provides geocoding API (`/geocode`)
- Provides order tracking API (`/order`)
- Provides driver location API (`/driver-location`)

**Runs only on:** HOST machine (not on clients)

**Accessed from:** Clients via HTTP request to `http://100.117.189.110:3000`

**Code Flow:**
```
Client app:
  1. User clicks "Pickup" button
  2. App makes HTTP request to: http://100.117.189.110:3000/geocode
  3. Host's map server receives request
  4. Returns geocoded coordinates
  5. Client receives response and displays map
```

---

## Setup Process

### Phase 1: Network Setup (Pre-Deployment)

**Steps:**
1. **Install Tailscale on all devices**
   - Download from tailscale.com
   - Each device gets its own Tailscale IP

2. **Connect all devices to same Tailscale account**
   - Login on each device with same credentials
   - Creates a mesh network

3. **Get host IP**
   - Visit https://tailscale.com/admin/machines
   - Find host machine's Tailscale IP: `100.117.189.110`

**Result:** All devices can ping each other securely

---

### Phase 2: Host Machine Setup

**Steps:**

1. **Create shared folder:**
   ```powershell
   mkdir D:\FoodAppData
   ```

2. **Share the folder via SMB:**
   - Right-click → Properties → Sharing → Advanced Sharing
   - Check "Share this folder"
   - Grant Full Control permissions

3. **Build the application:**
   ```powershell
   cd The-Co-Pilots\foodappproject
   cmake -B build
   cmake --build build
   ```

4. **Create batch file `run_host.bat`:**
   ```batch
   @echo off
   set FOODAPP_RUN_SERVER=1
   set FOODAPP_DB_PATH=D:\FoodAppData\food_sharing.db
   set FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
   
   .\build\foodappproject.exe
   pause
   ```

5. **First launch:**
   - Double-click `run_host.bat`
   - App starts and creates database at `D:\FoodAppData\food_sharing.db`
   - Map server starts listening on port 3000
   - Check output for: `Run Map Server: YES`

**Verification:**
```powershell
# From any other device:
curl http://100.117.189.110:3000/
# Should return HTML content
```

---

### Phase 3: Client Device Setup

**Steps:**

1. **Build the application** (same code as host):
   ```powershell
   cmake -B build
   cmake --build build
   ```

2. **Create batch file `run_client.bat`:**
   ```batch
   @echo off
   set FOODAPP_RUN_SERVER=0
   set FOODAPP_DB_PATH=\\100.117.189.110\FoodAppData\food_sharing.db
   set FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
   
   .\build\foodappproject.exe
   pause
   ```

3. **First launch:**
   - Double-click `run_client.bat`
   - App starts and connects to shared database
   - Map server NOT started locally
   - Check output for: `Run Map Server: NO (client mode)`

**Verification:**
```powershell
# Verify network path is accessible:
dir \\100.117.189.110\FoodAppData

# Should list: food_sharing.db
```

---

## Runtime Execution Flow

### When Host Machine Starts

```
1. User double-clicks run_host.bat
   ↓
2. Environment variables set:
   - FOODAPP_RUN_SERVER=1
   - FOODAPP_DB_PATH=D:\FoodAppData\food_sharing.db
   - FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
   ↓
3. App starts → main.cpp runs
   ↓
4. Debug output printed to console:
   === Food Sharing App Configuration ===
   Database Path: D:\FoodAppData\food_sharing.db
   Server URL: http://100.117.189.110:3000
   Run Map Server: YES
   ====================================
   ↓
5. DatabaseManager::initDatabase() called
   ↓
6. database.cpp reads FOODAPP_DB_PATH:
   QString dbPath = qgetenv("FOODAPP_DB_PATH");
   → Uses D:\FoodAppData\food_sharing.db
   ↓
7. SQLite database opened/created
   ↓
8. delivery.cpp startTrackingServer() called
   ↓
9. startTrackingServer checks FOODAPP_RUN_SERVER:
   QString runServerEnv = qgetenv("FOODAPP_RUN_SERVER");
   if (runServerEnv == "1") {
       → Starts map server process
       → Listens on port 3000
   }
   ↓
10. UI shows main window
    ↓
11. HOST IS READY ✓
```

---

### When Client Device Starts

```
1. User double-clicks run_client.bat
   ↓
2. Environment variables set:
   - FOODAPP_RUN_SERVER=0
   - FOODAPP_DB_PATH=\\100.117.189.110\FoodAppData\food_sharing.db
   - FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
   ↓
3. App starts → main.cpp runs
   ↓
4. Debug output printed to console:
   === Food Sharing App Configuration ===
   Database Path: \\100.117.189.110\FoodAppData\food_sharing.db
   Server URL: http://100.117.189.110:3000
   Run Map Server: NO (client mode)
   ====================================
   ↓
5. DatabaseManager::initDatabase() called
   ↓
6. database.cpp reads FOODAPP_DB_PATH:
   QString dbPath = qgetenv("FOODAPP_DB_PATH");
   → Uses \\100.117.189.110\FoodAppData\food_sharing.db
   → Connects to SHARED database (not local!)
   ↓
7. SQLite opens shared database over network
   ↓
8. delivery.cpp startTrackingServer() called
   ↓
9. startTrackingServer checks FOODAPP_RUN_SERVER:
   if (runServerEnv != "1") {
       → SKIPS server startup
       → No local server created
   }
   ↓
10. UI shows main window
    ↓
11. CLIENT IS READY ✓
    → All data automatically synced from host's database
    → Map requests will go to http://100.117.189.110:3000
```

---

## Data Flow Between Devices

### Scenario 1: Organization Submits Food Donation

```
Organization Device (Client):
│
├─ User fills donation form
├─ Clicks "Submit"
└─ INSERT query executed:
   INSERT INTO food_donations (name, quantity, ...)
   
   Network:
   ├─ Query reaches SQLite driver
   ├─ SQLite connects to: \\100.117.189.110\FoodAppData\food_sharing.db
   └─ Data written to SHARED database
   
   Host Machine:
   ├─ File updated at: D:\FoodAppData\food_sharing.db
   └─ Data persisted to disk
   
   Delivery Device (Client):
   ├─ Simultaneously reads from: \\100.117.189.110\FoodAppData\food_sharing.db
   ├─ Queries database for new donations
   ├─ Notification received: "New donation available!"
   └─ Delivery person sees it in app
```

### Scenario 2: Delivery Person Accepts Order and Opens Map

```
Delivery Device (Client):
│
├─ User clicks "Pickup"
├─ App executes order acceptance logic
├─ Makes HTTP request to: http://100.117.189.110:3000/geocode
│
├─ Network Request:
│  ├─ URL: http://100.117.189.110:3000/geocode
│  ├─ Host: 100.117.189.110 (on Tailscale VPN)
│  ├─ Port: 3000 (map server listening)
│  └─ Body: { "address": "123 Main St" }
│
Host Machine (Map Server):
│  ├─ Receives HTTP request on port 3000
│  ├─ Calls Google Maps API to geocode address
│  ├─ Returns: { "lat": 40.7128, "lng": -74.0060 }
│
Delivery Device (Client):
│  ├─ Receives geocoding response
│  ├─ Constructs map URL with response:
│  │  URL = http://100.117.189.110:3000/map_view.html
│  │      + ?deliveryId=123
│  │      + &pickupLat=40.7128&pickupLng=-74.0060
│  │      + &dropLat=40.7500&dropLng=-73.9900
│  │      + &apiKey=AIzaSy...
│  ├─ Opens map in WebView
│  └─ Map displays with pickup and delivery locations
```

### Scenario 3: Multiple Devices Reading Same Data

```
At exact same time:

Organization Device:
└─ SELECT all meal requests from database
   Location: \\100.117.189.110\FoodAppData\food_sharing.db

Delivery Device:
└─ SELECT all donation offers from database
   Location: \\100.117.189.110\FoodAppData\food_sharing.db

People Device:
└─ SELECT available donations from database
   Location: \\100.117.189.110\FoodAppData\food_sharing.db

All three are reading from THE SAME FILE simultaneously.
SQLite handles locking to prevent corruption.
Every device sees the same data.
```

---

## Environment Variable Flow

### What Happens When Environment Variables Are Set

#### On Host Machine

```
SET FOODAPP_RUN_SERVER=1
  ↓
delivery.cpp → startTrackingServer()
  ↓
qgetenv("FOODAPP_RUN_SERVER") == "1"
  ↓
bool shouldRunServer = true
  ↓
d->serverProcess = new QProcess()
d->serverProcess->start(serverBinary, ...)
  ↓
Map server starts on port 3000
```

```
SET FOODAPP_DB_PATH=D:\FoodAppData\food_sharing.db
  ↓
database.cpp → initDatabase()
  ↓
qgetenv("FOODAPP_DB_PATH") returns D:\FoodAppData\food_sharing.db
  ↓
db.setDatabaseName("D:\\FoodAppData\\food_sharing.db")
  ↓
SQLite opens local file
```

```
SET FOODAPP_SERVER_BASE_URL=http://100.117.189.110:3000
  ↓
delivery.cpp → handlePickup()
  ↓
qgetenv("FOODAPP_SERVER_BASE_URL") used in API calls
  ↓
QNetworkRequest(QUrl("http://100.117.189.110:3000/geocode"))
  ↓
Request sent to host's server over Tailscale VPN
```

#### On Client Device

```
SET FOODAPP_RUN_SERVER=0
  ↓
delivery.cpp → startTrackingServer()
  ↓
qgetenv("FOODAPP_RUN_SERVER") != "1"
  ↓
bool shouldRunServer = false
  ↓
qDebug() << "Map server disabled"
return;  // Skip server startup
  ↓
No local server created
```

```
SET FOODAPP_DB_PATH=\\100.117.189.110\FoodAppData\food_sharing.db
  ↓
database.cpp → initDatabase()
  ↓
qgetenv("FOODAPP_DB_PATH") returns \\100.117.189.110\FoodAppData\food_sharing.db
  ↓
db.setDatabaseName("\\\\100.117.189.110\\FoodAppData\\food_sharing.db")
  ↓
SQLite opens NETWORK FILE (shared database)
  ↓
Connection over SMB through Tailscale VPN
```

---

## Key Technical Details

### 1. Why Tailscale Instead of Direct IP?

**Before Tailscale:**
- Would need to know public IP of host
- Would need to configure port forwarding
- Would need firewall rules
- Would be exposed to internet
- Complicated to set up

**With Tailscale:**
- Private IP in `100.x.x.x` range
- All traffic encrypted automatically
- No port forwarding needed
- No firewall configuration needed
- One-click login per device

### 2. Why Network Database Instead of Syncing?

**Bad approach: Syncing**
```
Host DB ←→ Copy data ←→ Client DB
           ←→ Copy back ←→
Problems:
- Conflicts when both edit same row
- Slow synchronization delay
- Complex conflict resolution needed
```

**Good approach: Shared Database (what we use)**
```
Host:     D:\FoodAppData\food_sharing.db
           ↓ (SMB Share)
Clients:  \\100.117.189.110\FoodAppData\food_sharing.db

All devices read/write same file.
SQLite handles locking automatically.
Real-time consistency guaranteed.
```

### 3. Server on Host Only - Why?

**Why host runs map server, clients don't:**

- **Single source of truth**: One server on one IP
- **Reduced complexity**: No need to sync server state
- **API consistency**: All devices hit the same API
- **Resource efficient**: One server, not three
- **Easier debugging**: All logs in one place

**API Flow:**
```
Host:    Runs map_server → Serves /geocode, /order, etc.
Client1: HTTP → http://100.117.189.110:3000/geocode
Client2: HTTP → http://100.117.189.110:3000/geocode
Client3: HTTP → http://100.117.189.110:3000/geocode

All requests go to THE SAME server.
No conflicts, no inconsistencies.
```

### 4. Database Locking (SQLite Multi-Access)

**How SQLite handles multiple clients accessing same file:**

```
Client 1 tries to write:
  ├─ Locks database file
  ├─ Writes data
  └─ Releases lock

Client 2 tries to read (while Client 1 writing):
  ├─ Waits for lock to release
  ├─ Then reads
  └─ Gets latest data

This prevents:
  ✓ Data corruption
  ✓ Lost updates
  ✓ Dirty reads
```

**Performance consideration:**
- Network file access slower than local disk
- Acceptable for course demo with 3 devices
- Would need PostgreSQL for high concurrency in production

### 5. Fallback Behavior (Backward Compatibility)

**If environment variables NOT set:**

```cpp
// database.cpp
QString dbPath = qgetenv("FOODAPP_DB_PATH");
if (dbPath.isEmpty()) {
    dbPath = "food_sharing.db";  // Falls back to local
}

// delivery.cpp
QString serverUrl = qgetenv("FOODAPP_SERVER_BASE_URL");
if (serverUrl.isEmpty()) {
    serverUrl = "http://localhost:3000";  // Falls back to local
}

QString runServerEnv = qgetenv("FOODAPP_RUN_SERVER");
if (runServerEnv != "1") {
    // Server not started - but local fallback means
    // it would fail if user tries map features
}
```

**Result:** App works exactly as before (single device mode)

---

## Troubleshooting Mental Model

### "I set the variables but it doesn't work"

**Check this flow:**
```
Is FOODAPP_RUN_SERVER=1 on host?
  ├─ YES: Should see "Run Map Server: YES"
  └─ NO: See "Run Map Server: NO" (client mode by mistake)

Is FOODAPP_SERVER_BASE_URL correct?
  ├─ Does IP match Tailscale IP? (100.117.189.110)
  └─ Can you curl http://100.117.189.110:3000/? 
     (If not, host server not running)

Is FOODAPP_DB_PATH correct?
  ├─ On host: D:\FoodAppData\food_sharing.db exists?
  ├─ On client: Can you dir \\100.117.189.110\FoodAppData?
  └─ If not, shared folder not set up properly
```

### "Database says access denied"

**Check this:**
```
Host shared folder permissions:
  ├─ Right-click D:\FoodAppData → Properties
  ├─ Sharing tab → Advanced Sharing
  └─ Click Permissions → Everyone has Full Control?

Network connectivity:
  ├─ Can you ping 100.117.189.110?
  ├─ Can you dir \\100.117.189.110\FoodAppData?
  └─ If dir fails, network path not accessible
```

### "Map server says connection refused"

**Check this:**
```
Is host running?
  ├─ Is app open on host machine?
  └─ Does console say "Run Map Server: YES"?

Is port 3000 listening?
  ├─ On host: netstat -an | findstr :3000
  ├─ On client: curl http://100.117.189.110:3000/
  └─ Both should show connection successful

Is firewall blocking?
  ├─ Windows Firewall → Inbound Rules
  └─ Rule for Port 3000 should exist
```

---

## Summary: The Complete Picture

```
┌─────────────────────────────────────────────────────┐
│ SETUP PHASE                                         │
├─────────────────────────────────────────────────────┤
│                                                     │
│ 1. Install Tailscale on all devices                │
│    ↓ Each device gets 100.x.x.x IP                 │
│                                                     │
│ 2. Host machine:                                   │
│    ├─ Create shared folder D:\FoodAppData          │
│    ├─ Set FOODAPP_RUN_SERVER=1                     │
│    └─ Run app                                      │
│       ↓ Creates database, starts server            │
│                                                     │
│ 3. Client devices:                                 │
│    ├─ Set FOODAPP_RUN_SERVER=0                     │
│    ├─ Set FOODAPP_DB_PATH=\\HOST\FoodAppData\...   │
│    └─ Run app                                      │
│       ↓ Connects to shared database                │
│                                                     │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│ RUNTIME PHASE                                       │
├─────────────────────────────────────────────────────┤
│                                                     │
│ Organization Device:                               │
│   ├─ Reads/writes to: \\HOST\DB                    │
│   └─ Submits donation → DB updated                 │
│                                                     │
│ People Device:                                     │
│   ├─ Reads/writes to: \\HOST\DB                    │
│   ├─ Sees donation immediately (shared DB)         │
│   └─ Requests meal → DB updated                    │
│                                                     │
│ Delivery Device:                                   │
│   ├─ Reads/writes to: \\HOST\DB                    │
│   ├─ Sees new meal request (shared DB)             │
│   ├─ Accepts order → Updates DB                    │
│   ├─ HTTP request → http://100.117.189.110:3000   │
│   └─ Opens tracking map                            │
│                                                     │
│ Host Machine:                                      │
│   ├─ Stores shared database                        │
│   ├─ Runs map server on port 3000                  │
│   └─ Serves HTML + APIs to clients                 │
│                                                     │
└─────────────────────────────────────────────────────┘

RESULT: Real-time multi-device collaboration
        All devices see same data immediately
        No syncing delays or conflicts
```

---

## For Your Team's Demo

**What to tell reviewers:**

1. **"This is what we changed"**
   - Made database path configurable (FOODAPP_DB_PATH)
   - Made server URL configurable (FOODAPP_SERVER_BASE_URL)
   - Made server startup conditional (FOODAPP_RUN_SERVER)
   - All changes maintain 100% backward compatibility

2. **"How it works"**
   - One device runs the server and database (host)
   - Other devices connect to that host via Tailscale
   - All traffic encrypted, no public internet exposure
   - All three roles see same real-time data

3. **"Why Tailscale"**
   - Free, secure, no port forwarding needed
   - Just login with same account on all devices
   - Creates private network between devices
   - Industry standard for secure remote access

4. **"Why shared database"**
   - Real-time consistency vs. sync delays
   - SQLite handles locking automatically
   - No need to code conflict resolution
   - Simplest solution for course project

---

## For Developers Building This

**When building, remember:**

1. **Code changes are minimal** - only configuration, no rewrite
2. **Each file has a specific role:**
   - `database.cpp` - reads FOODAPP_DB_PATH
   - `delivery.cpp` - checks FOODAPP_RUN_SERVER, reads FOODAPP_SERVER_BASE_URL
   - `main.cpp` - logs configuration for debugging
   - HTML files - get API key passed as URL parameter

3. **Test flow:**
   - Local testing (no env vars) - works as before ✓
   - Host mode (RUN_SERVER=1) - server starts ✓
   - Client mode (RUN_SERVER=0) - connects to host ✓

4. **Deployment is just configuration** - no rebuilding needed
