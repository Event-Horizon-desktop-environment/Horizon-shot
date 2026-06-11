# Horizon Shot


## Dependencies

You need a C++23 toolchain, **Meson**, **Ninja**, and development packages for at least:

Wayland (client), Cairo, xkbcommon, libdrm — plus optional **libpng**, **libjxl** for extended export formats.

If configure fails, install the missing `-dev` / `-devel` package Meson names in the error.

### Fedora

```bash
sudo dnf install meson gcc-c++ just \
  wayland-devel \
  cairo-devel \
  libxkbcommon-devel \
  libdrm-devel
```

Optional export formats:

```bash
sudo dnf install libpng-devel libjxl-devel
```

### Arch Linux

```bash
sudo pacman -S meson gcc just \
  wayland \
  cairo \
  libxkbcommon \
  libdrm
```

### Debian / Ubuntu

```bash
sudo apt install meson g++ just \
  libwayland-dev \
  libcairo2-dev \
  libxkbcommon-dev \
  libdrm-dev
```

## Compile

```bash
meson setup build
meson compile -C build
```

Release build:

```bash
meson setup build-release --buildtype=release
meson compile -C build-release
```

### With `just`

The repo [`justfile`](justfile) runs plain `meson setup build-debug` / `build-release`.

- **`just build-release`** — creates `build-release/` if needed, then compiles a **release** build.
- **`sudo just install`** — configures a release build with `--prefix=/usr`, compiles, and installs in one step.
- **`just install-release`** — runs **`just build-release`**, then **`sudo meson install -C build-release`**. Run this as your normal user when you want compile + install in one step; only the install step uses `sudo`.
- **`sudo just install-release`** — runs **`meson install` only** (skips build). Use this after **`just build-release`** if you already have the binary built.

## Install to `/usr` (binary in `/usr/bin`)

Installing under **`/usr`** needs root and puts the program in **`/usr/bin/horizon-shot`**.

```bash
meson setup build --prefix=/usr
meson compile -C build
sudo meson install -C build
```

## Usage

```sh
horizon-shot --help
```

Keybinds must be configured in your compositor, e.g.:

```
bindsym Print exec horizon-shot --select --copy
bindsym Shift+Print exec horizon-shot --focused --copy
```
