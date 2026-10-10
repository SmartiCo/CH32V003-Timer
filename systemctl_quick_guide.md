# Ubuntu `systemctl` & `systemd` Quick Guide

A concise reference guide for managing background processes, understanding system/user spaces, and essential commands.

---

## 💡 What is systemd & systemctl?
* **`systemd`**: The core system engine (Process ID 1) that starts, stops, and recovers background processes (daemons).
* **`systemctl`**: The command-line tool used to interact with the `systemd` engine.

---

## ⚔️ System Space vs. User Space

| Feature | System Space (Root) | User Space (User Account) |
| :--- | :--- | :--- |
| **Privileges** | Requires `sudo` (Full Admin) | No `sudo` needed (User Account permissions) |
| **Security** | Global impact if compromised | Contained entirely within your user directory |
| **File Location** | `/etc/systemd/system/` | `~/.config/systemd/user/` |
| **Command** | `sudo systemctl <action> <service>` | `systemctl --user <action> <service>` |
| **Lifespan** | Starts at system boot up | Starts at login (unless **linger** is enabled) |

> 📌 **Tip:** To keep user services running forever without an active login session, execute:  
> `sudo loginctl enable-linger $USER`

---

## 🛠️ Essential Commands Cheat Sheet
*For User Space services, always append `--user` to the commands below.*

### 1. Controlling Current State
* **Start:** `systemctl start <service>` (Launches the app immediately)
* **Stop:** `systemctl stop <service>` (Gracefully shuts down the app)
* **Restart:** `systemctl restart <service>` (Stops and re-opens the app; perfect for updates)
* **Reload Engine:** `systemctl daemon-reload` (Forces systemd to read configuration updates)

### 2. Boot Behavior (Persistence)
* **Enable Boot-Start:** `systemctl enable <service>` (Ties service to system startup)
* **Disable Boot-Start:** `systemctl disable <service>` (Removes service from system startup)

### 3. Monitoring & Troubleshooting
* **Status Profile:** `systemctl status <service>` (Shows PID, memory usage, and health)
* **View Full Logs:** `journalctl -u <service>` (Prints stdout/stderr history)
* **Live Stream Logs:** `journalctl -u <service> -f` (Follows logs in real-time as they happen)
