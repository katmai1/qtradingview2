#!/bin/sh
set -e
update-desktop-database -q /usr/share/applications >/dev/null 2>&1 || true
gtk-update-icon-cache -q /usr/share/icons/hicolor >/dev/null 2>&1 || true
exit 0
