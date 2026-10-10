# Systemd Service Monitoring & Alerting Guide

When running critical tools like **OpenCode Web** over the public internet, you need to know immediately if the service crashes or goes offline. This guide covers the best ways to monitor your services—ranging from native `systemd` watchdogs to cloud-native monitoring scripts.

---

## 1. Native Systemd Auto-Recovery (Built-in)
Before setting up external alerts, ensure your service is configured to self-heal. Inside your `ocweb.service` file under the `[Service]` block, you should have:

```ini
[Service]
Restart=on-failure
RestartSec=5
StartLimitIntervalSec=60
StartLimitBurst=3
```
* **`Restart=on-failure`**: Automatically restarts the app if it exits with an error code or crashes.
* **`RestartSec=5`**: Waits 5 seconds before attempting a restart to prevent loop spamming.
* **`StartLimitBurst=3`**: If it crashes more than 3 times within 60 seconds, systemd stops trying and marks it as permanently failed, preventing CPU exhaustion.

---

## 2. Automated Terminal Alerts (Health Checks)
You can write a simple script that polls the service status using `systemctl is-active`.

### The Monitoring Script (`monitor_service.sh`)
```bash
#!/bin/bash
SERVICE="ocweb.service"

if ! systemctl --user is-active --quiet "$SERVICE"; then
    echo "WARNING: $SERVICE is down!"
    # Insert alerting action below (e.g., Email, Discord, Telegram webhook)
fi
```

### Automation via User Cron
To run this health check automatically every 5 minutes:
1. Open your user crontab editor: `crontab -e`
2. Add the following line at the bottom:
   ```text
   */5 * * * * /home/pvd/scripts/monitor_service.sh
   ```

---

## 3. Webhook Alerting (Discord or Telegram)
To send real-time alerts directly to your phone or desktop when a failure occurs, you can integrate a webhook directly into the monitoring script or use a custom systemd failure handler.

### Discord Webhook Integration
Replace the `# Insert alerting action` line in your script with this single `curl` command:

```bash
curl -H "Content-Type: application/json" \
     -X POST \
     -d '{"content": "🚨 **ALERT:** OpenCode Web Service (`ocweb.service`) is down on pvd-X556UR!"}' \
     YOUR_DISCORD_WEBHOOK_URL
```

### Telegram Bot Integration
```bash
TOKEN="YOUR_BOT_TOKEN"
CHAT_ID="YOUR_CHAT_ID"
MESSAGE="🚨 ALERT: ocweb.service is down!"

curl -s -X POST "https://api.telegram.org/bot$TOKEN/sendMessage" \
     -d "chat_id=$CHAT_ID" \
     -d "text=$MESSAGE"
```

---

## 4. Professional Uptime Monitoring (Recommended)
Since your OpenCode instance is exposed over the internet via a Cloudflare Tunnel, the most reliable monitoring method is an **external HTTP ping**. If your home internet drops or the machine freezes entirely, an internal script cannot save you, but an external monitor will notice instantly.

| Tool | Type | Pricing | Best Feature |
| :--- | :--- | :--- | :--- |
| **Uptime Kuma** | Self-Hosted | Free / Open Source | Can run on a cheap cloud VPS to ping your home domain. Beautiful UI. |
| **UptimeRobot** | Cloud Hosted | Free Tier Available | Configured in 2 minutes. Pings your URL every 5 minutes and sends emails. |
| **Better Stack** | Cloud Hosted | Free Tier Available | Advanced tracking with instant screenshot captures of the failure page. |

### How to set up UptimeRobot for your Tunnel:
1. Create a free account at [UptimeRobot](https://uptimerobot.com).
2. Click **Add New Monitor**.
3. Set Monitor Type to **HTTP(s)**.
4. Input your Cloudflare public domain (e.g., `https://opencode.yourdomain.com`).
5. Set the monitoring interval (e.g., every 5 minutes).
6. Select your preferred notification method (Email, SMS, or Slack).