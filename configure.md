# Post-Install Configuration

Run [`configure.sh`](./configure.sh) first, then follow the steps below to finish setting up the system.

---

## 1. GitHub SSH Key

Add the generated public key to GitHub so you can push without a password:

1. Copy your public key:
   ```bash
   cat ~/.ssh/id_ed25519.pub
   ```
2. Go to **[GitHub Settings → SSH and GPG keys](https://github.com/settings/keys)**
3. Click **New SSH key**, paste the content, and save.

## 2. KDE Plasma (System Settings)

Open **System Settings** and configure:

- Shortcuts
- Display resolution / orientation
- Wallpaper
- Theme
- Panel layout & settings
- *Optional:* Natural scrolling in mouse/touchpad settings

## 3. Brave Browser

Open `brave://settings` and configure:

- Privacy and security preferences
- Disable persistent cookies
- Disable telemetry
- Install content filters / blockers
- Apply **Catppuccin Mocha** theme

## 4. Zed Editor

Launch Zed and configure:

- **Catppuccin Mocha** theme
- **Catppuccin Mocha** icons
- Enable spell checker
- Set up inline and agent Copilot

## 5. Steam (PC only)

*Optional — skip if on laptop.*

Sign into Steam and install your games.

## 6. Work Tools

- Configure email in **KMail**
- Sign into **Mattermost**
- Sign into **Zoom**
