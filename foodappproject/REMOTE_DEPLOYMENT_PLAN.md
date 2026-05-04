# Remote Hosting Plan (Easiest Course-Project Path)

## Goal
Run your project so these roles can use it from different devices at the same time:
- Delivery driver
- Organization or restaurant
- People in need

## Best Easiest Approach For This Project
Use one machine as a host server and let all other devices connect to it.

Why this is easiest for your codebase:
- Your app is already desktop Qt (not web/mobile).
- Your data is in SQLite (currently local file).
- Your map service already runs as an HTTP server.
- You can make small focused changes instead of rewriting everything to a full backend.

Architecture:
1. One host PC runs map_server continuously.
2. One shared database file is used by all clients.
3. All devices connect through a private VPN network (Tailscale), so no complex cloud setup is needed.

Note:
This is perfect for course demo and small usage. For production, move to PostgreSQL + real backend API.

---

## Current Project Facts (Important)
- Database is local SQLite in database.cpp using food_sharing.db.
- Delivery map APIs call localhost:3000 in delivery.cpp.
- map_server runs from map/server.cpp and serves map pages + tracking APIs.
- HTML pages also assume local server behavior.

This means today every device has separate data and cannot collaborate.

---

## Step-By-Step Plan

## Step 1: Pick The Host Machine
Choose one laptop or desktop that stays ON during testing/demo.
- Recommended: Windows machine on stable internet.
- This machine will run map_server and hold the shared SQLite file.

## Step 2: Install Tailscale On All Devices
Install Tailscale on:
- Host machine
- Delivery device
- Organization device
- People-in-need device

Then:
1. Log all devices into same Tailscale account/team.
2. Verify each device can ping the host Tailscale IP.

Outcome:
You get private networking between devices without router port-forwarding.

## Step 3: Prepare Shared Database Location
On host machine:
1. Create folder like D:/FoodAppData.
2. Share it on network with read and write access for your test users.
3. Store database file there: food_sharing.db.

Example shared path from other devices:
- \\HOSTNAME\FoodAppData\food_sharing.db

## Step 4: Make Small Code Changes (One-Time)
You only need configuration support, not a full rewrite.

### 4.1 Database path must be configurable
Update database.cpp:
- Read database path from environment variable FOODAPP_DB_PATH.
- If missing, fallback to current local file food_sharing.db.

Result:
All devices can point to one shared DB file.

### 4.2 Server base URL must be configurable
Update delivery.cpp:
- Replace hardcoded http://localhost:3000 with FOODAPP_SERVER_BASE_URL.
- Fallback to http://localhost:3000 if env var missing.

Use this base URL for:
- /geocode
- /order
- map page URL

### 4.3 Start map_server only on host
Update delivery.cpp startTrackingServer logic:
- Add env flag FOODAPP_RUN_SERVER.
- If FOODAPP_RUN_SERVER=1, start local map_server process.
- If not, do not start server process.

Result:
Only host runs server. Client devices only consume it.

### 4.4 Remove API key hardcoding
Current code and HTML contain a Google Maps key directly.
- Read key from env GOOGLE_MAPS_API_KEY on host.
- Pass key dynamically to map URL.

Result:
Safer and easier to rotate key.

## Step 5: Build App On Host And Clients
Build your Qt app on each device (or copy built executable and required Qt runtime files).

All builds should include same version of code from Step 4.

## Step 6: Configure Host Environment Variables
On host machine set:
- FOODAPP_RUN_SERVER=1
- FOODAPP_DB_PATH=D:/FoodAppData/food_sharing.db
- FOODAPP_SERVER_BASE_URL=http://<HOST_TAILSCALE_IP>:3000
- GOOGLE_MAPS_API_KEY=<your_real_key>

Then run app once and verify:
- map_server starts
- DB is created in shared folder

## Step 7: Configure Client Devices
On each client device set:
- FOODAPP_RUN_SERVER=0
- FOODAPP_DB_PATH=\\HOSTNAME\FoodAppData\food_sharing.db
- FOODAPP_SERVER_BASE_URL=http://<HOST_TAILSCALE_IP>:3000

Then run app.

## Step 8: Firewall And Access Checks
On host machine allow:
- TCP 3000 (map_server)
- File sharing (SMB) for shared DB access

Quick checks from client devices:
1. Open in browser: http://<HOST_TAILSCALE_IP>:3000/tracking_test.html
2. Confirm shared DB path is reachable.

## Step 9: End-To-End Multi-Device Test Script
Run this exact test before demo:
1. Device A (organization): register and submit donation.
2. Device B (people): sign up and request meals.
3. Device C (delivery): open notifications, accept order, open pickup map.
4. Confirm status updates appear on other devices.

Pass condition:
All three devices see the same live workflow based on one shared DB and one map server.

## Step 10: Demo Day Runbook
Before class:
1. Start host machine and connect Tailscale.
2. Launch app on host and keep it running.
3. Confirm tracking_test page opens from another device.
4. Open app on the three role devices.
5. Execute test script from Step 9.

---

## Minimal File Change Checklist
Planned edits are in:
- database.cpp
- delivery.cpp
- map/map_view.html
- map/driver_live.html

Optional helper:
- Add a small config section in main.cpp to print active DB path and server URL for debugging.

---

## Known Limitations (Acceptable For Course)
- SQLite over network share is not ideal for high concurrency.
- If host machine is off, whole system stops.
- No authentication hardening for public internet.

For course/demo this is usually acceptable and much faster to deliver.

---

## If You Have Extra Time (Optional Upgrade)
After your demo, easiest upgrade path:
1. Keep Qt clients.
2. Replace shared SQLite file with backend API + PostgreSQL.
3. Move matching logic from app into backend service.

This removes file-share risks and scales better.
