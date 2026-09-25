# Raspberry Pi deployment

The supported Pi path is native installation on Raspberry Pi OS Lite 64-bit.

From a checked-out, tagged release:

```sh
sudo HAM_BBS_SOURCE_DIR="$PWD" ./scripts/install-pi.sh
sudo HAM_BBS_SOURCE_DIR="$PWD" ./scripts/update-pi.sh v0.2.0
```

The installer uses `/opt/ham-bbs` for application files, `/etc/ham-bbs` for administrator-owned configuration, and `/var/lib/ham-bbs` for the mutable database and built frontend. Existing configuration files are not overwritten. Configure stable `/dev/serial/by-id` and ALSA identifiers in `/etc/ham-bbs/direwolf.conf`; set `RADIO_MODEL` and `RADIO_DEVICE` in `/etc/ham-bbs/ham-bbs.env` for rigctld.

`ham-bbs.service` is the application. `rigctld.service` and `direwolf.service` are separate hardware services. The update script preserves configuration and state; rollback is not implemented yet. Logs are available with `journalctl -u ham-bbs.service` and the corresponding radio unit names.
