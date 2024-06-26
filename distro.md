# Installing Arch Linux

The [Arch Linux wiki](https://wiki.archlinux.org/) contains a comprehensive, up-to-date guide on how to install Arch Linux. And the [Arch Linux Installation Guide](https://wiki.archlinux.org/index.php/Installation_guide) is a good starting point. The [Arch Linux website](https://www.archlinux.org/) provides the latest news on the distro which is important to keep up with.

## Pre-installation

### Install Ventoy

[Ventoy](https://www.ventoy.net/en/index.html) is a tool to create bootable USB drives. It is a great tool to have as it allows you to boot multiple ISOs from a single USB drive and not have to format the drive each time you want to try a different distro.

### Download the ISO

[Download the Arch Linux ISO from the bottom of the page](https://www.archlinux.org/download/) and place it inside the root of the Ventoy USB drive.

### Boot into the USB

Insert the Ventoy USB drive into the computer and boot into it by selecting it from the boot menu. You may need to change the boot order in the BIOS settings, or your BIOS may have a boot menu key that you can press at startup to select the USB drive.

Once Ventoy boots, you will see a list of ISOs that you can boot from. Select the Arch Linux ISO and press enter.

> **Note**: Disable Secure Boot in your BIOS settings if you have trouble booting into the USB drive.

When the Arch Linux ISO boots, you will be presented with a GRUB2 menu. Select the first option to boot into the live environment (if you are using UEFI mode). Arch Linux will boot into the live environment and you can start the installation process when it finishes copying the image to RAM.

> **Note**: If Arch Linux does not boot into the live environment because it cannot find a device or path, then you may need to enter GRUB2 mode in Ventoy by pressing `ctrl + r` before booting into the Arch Linux ISO.

Ensure the system is working by setting your font:

```bash
setfont ter-132b
```

Set the system timezone:

```bash
timedatectl set-timezone <Region>/<City>
```

## Initial network configuration

If you are using a wired connection, then you can skip this step. If you are using Wi-Fi, then you will need to connect to your network before proceeding with the installation.

Using `iwctl` (iNet Wireless Control), you can scan for networks and connect to them. First, list the available devices:

```bash
iwctl device list
```

Scan for available networks:

```bash
iwctl station <device> scan
```

List the available networks:

```bash
iwctl station <device> get-networks
```

Connect to your network:

```bash
iwctl --passphrase <password> station <device> connect <network>
```

Test the connection:

```bash
ping archlinux.org
```

If the connection is successful, you should see output from the `ping` appearing every second. Press `Ctrl + C` to stop the `ping`.

## Disk partitioning

List the available disks:

```bash
lsblk
```

Identify the disk you want to install Arch Linux on. If you are unsure, use the size of the disk to determine which is the primary disk.

Enter disk partitioning mode for the disk:

```bash
fdisk /dev/<device>
```

> **Note**: Make sure _not_ to select a partition (e.g., `/dev/sda1`) but the disk itself (e.g., `/dev/sda`).

Create an empty partition table:

```bash
g
```

> **Note**: At any point you can type `p` to print the current partition table.

### Create the boot partition

Create a new partition:

```bash
n
```

Leave the partition number as the default: press `Enter`.

Leave the first sector (beginning of the partition) as the default: press `Enter`.

Make this new partition 1 gigabyte in size:

```bash
+1G
```

If prompted to remove the signature, type `y` and press `Enter`.

### Create the EFI partition

Create a new partition:

```bash
n
```

Leave the partition number as the default: press `Enter`.

Leave the first sector (beginning of the partition) as the default: press `Enter`.

Make this new partition 1 gigabyte in size:

```bash
+1G
```

### Create the LVM partition

Create a new partition:

```bash
n
```

Leave the partition number as the default: press `Enter`.

Leave the first sector (beginning of the partition) as the default: press `Enter`.

Press `Enter` to use the remaining space on the disk.

Enter type selection mode:

```bash
t
```

Select the partition you just created: press `Enter`.

Select the Linux large volume manager (LVM) type:

```bash
44
```

### Write the changes to disk

> **Note**: Running the following command will erase all data on the disk. Make sure you have backed up any important data before proceeding.

Write the changes to disk:

```bash
w
```

## Disk formatting

Format the boot (first) partition as FAT32:

```bash
mkfs.fat -F32 /dev/<device>p1
```

Format the EFI (second) partition as FAT32:

```bash
mkfs.ext4 /dev/<device>p2
```

Encrypt the LVM (third) partition, as it will contain the root and swap volumes (i.e., the actual stuff we store and use on our computer):

```bash
cryptsetup luksFormat /dev/<device>p3
```

Type `YES` to confirm the encryption.

Enter and verify the passphrase for the encryption (i.e., the password you will use every time you log in to your computer).

Open the encrypted partition:

```bash
cryptsetup open --type luks /dev/<device>p3 lvm
```

> **Note**: The `lvm` name is arbitrary and can be anything you want - it is how we will refer to the partition in the next steps.

Create the physical volume:

```bash
pvcreate /dev/mapper/lvm
```

Create the volume group:

```bash
vgcreate vg0 /dev/mapper/lvm
```

Create the logical volume for the root partition:

```bash
lvcreate -L 30GB vg0 -n lv_root
```

[Optional] Create the logical volume for the swap partition:

```bash
lvcreate -L <RAM-size>GB vg0 -n lv_swap
```

[Optional] Configure the swap partition:

```bash
mkswap /dev/vg0/lv_swap
```

[Optional] Enable the swap partition:

```bash
swapon /dev/vg0/lv_swap
```

Create the logical volume for the home partition:

```bash
lvcreate -l 100%FREE vg0 -n lv_home
```

> **Note**: We can run `vgdisplay` to see the volume group information, and `lvdisplay` to see the logical volume information.

Load the necessary kernel modules:

```bash
modprobe dm_mod
```

Scan for the LVM volumes:

```bash
vgscan
```

Activate the volume group:

```bash
vgchange -ay
```

Format the root partition as ext4:

```bash
mkfs.ext4 /dev/vg0/lv_root
```

Format the home partition as ext4:

```bash
mkfs.ext4 /dev/vg0/lv_home
```

### Partition mounting

Mount the root partition:

```bash
mount /dev/vg0/lv_root /mnt
```

Create the boot directory:

```bash
mkdir /mnt/boot
```

Mount the EFI (second) partition:

```bash
mount /dev/<device>p2 /mnt/boot
```

> **Note**: We are not mounting the boot (first) partition...

Create the home directory:

```bash
mkdir /mnt/home
```

Mount the home partition:

```bash
mount /dev/vg0/lv_home /mnt/home
```

### Install essential packages

```bash
pacstrap -i /mnt base
```

> **Note**: If any packages ask which version to install, select the default version: press `Enter`.

### Generate the `fstab` file

Generate the `fstab` file:

```bash
genfstab -U -p /mnt >> /mnt/etc/fstab
```

> This will append the UUIDs of the partitions to the `fstab` file: root, boot, home, and swap.

### Chroot into the new system

```bash
arch-chroot /mnt
```

Set root password:

```bash
passwd
```

Enter and confirm the root password.

Create user:

```bash
useradd -m -G wheel <username>
```

Set user password:

```bash
passwd <username>
```

Enter and confirm the user password.

Grant the user sudo privileges:

```bash
sudo EDITOR=nvim visudo
```

Uncomment the line:

```bash
%wheel ALL=(ALL) ALL
```

Install packages:

```bash
pacman -S base-devel grub efibootmgr networkmanager lvm2 neovim sudo xorg xorg-xinit alsa-tools alsa-utils pipewire pipewire-alsa pipewire-audio pipewire-pulse wireplumber
```

> **Note**: If any packages ask which version to install, select the default version: press `Enter`.

Install kernel:

```bash
pacman -S linux linux-headers
```

> **Note**: If any packages ask which version to install, select the default version: press `Enter`.

Install firmware:

```bash
pacman -S linux-firmware
```

Setup GPU drivers:

-  For Intel:

   ```bash
   pacman -S mesa intel-media-driver
   ```

-  For NVIDIA:

   ```bash
   pacman -S nvidia nvidia-utils
   ```

### System configuration

Make sure the kernel knows how to deal with encrypted partitions:

```bash
nvim /etc/mkinitcpio.conf
```

Add `encrypt` to the `HOOKS` array:

```bash
HOOKS=(... block encrypt lvm2 filesystems ...)
```

Generate the ramdisk:

```bash
mkinitcpio -p linux
```

Set locale:

```bash
nvim /etc/locale.gen
```

Uncomment the locale you want to use:

```bash
en_GB.UTF-8 UTF-8
```

Generate the locale:

```bash
locale-gen
```

Add the encrypt device to the GRUB configuration:

```bash
nvim /etc/default/grub
```

Add `cryptdevice=/dev/<device>p3:vg0` to the `GRUB_CMDLINE_LINUX_DEFAULT` line:

```bash
GRUB_CMDLINE_LINUX_DEFAULT="loglevel=3 quiet cryptdevice=/dev/<device>p3:vg0"
```

Setup EFI partition:

```bash
mkdir /boot/EFI
```

Mount the EFI partition:

```bash
mount /dev/<device>p1 /boot/EFI
```

Install bootloader:

```bash
grub-install --target=x86_64-efi --bootloader-id=grub_uefi --recheck
```

Generate the GRUB configuration:

```bash
cp /usr/share/locale/en\@quot/LC_MESSAGES/grub.mo /boot/grub/locale/en.mo
```

Generate config file:

```bash
grub-mkconfig -o /boot/grub/grub.cfg
```

Enable network manager:

```bash
systemctl enable NetworkManager
```

### Reboot

Exit the chroot environment:

```bash
exit
```

Unmount the partitions:

```bash
umount -a
```

> **Note**: You can unplug the USB drive before the system reboots.

When the system restarts, you will be prompted to enter the encryption passphrase. Enter the passphrase to boot into the system. Then log in with the user you created.

### Post-installation

Connect to the network:

```bash
nmcli device wifi connect <SSID> password <password>
```

Name the device:

```bash
hostnamectl hostname <host>
```
