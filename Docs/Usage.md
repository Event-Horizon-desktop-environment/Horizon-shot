# Usage

## Capture modes

```
horizon-shot                     Interactive GUI (default)
horizon-shot -s, --select        Interactive region selection
horizon-shot -f, --focused       Capture focused window
horizon-shot -a, --all           Capture all screens
horizon-shot -o, --output NAME   Capture a specific output by name
```

## Frame options

```
--cursor             Include cursor in capture
--border             Show window border
--no-shadow          Disable drop shadow
--no-frame           Hide decoration frame
--inset PX           Padding around captured image (default: 32)
--corner-radius PX   Corner radius (default: 24)
```

## Output

```
-c, --copy                Copy to clipboard (instead of saving)
-O, --output-file PATH    Save to a specific path
```

Without `--copy` or `--output-file`, captures are saved to `/tmp/horizon-shot-XXXXXX.png`.

## Utility

```
--list-outputs   List available outputs (name, resolution, position)
-h, --help       Show help and exit
```

## Keybinds

The program is intended to be launched from a compositor keybind. Example for Hyprland:

```conf
bind = Print, exec, horizon-shot --select --copy
bind = Shift+Print, exec, horizon-shot --focused --copy
```

## CLI mode examples

```sh
# Take a screenshot of the focused window with shadow and cursor
horizon-shot --focused --cursor

# Capture a specific output without decoration
horizon-shot --output DP-1 --no-frame

# Interactive selection, copy to clipboard
horizon-shot --select --copy

# Full screen capture with custom padding and save to a file
horizon-shot --inset 16 -O ~/screenshot.png
```

## Notes

- **Window mode** (`--window`) is only available from the interactive GUI — it opens a window picker UI.
- Frame options (`--border`, `--no-shadow`, etc.) only apply in non-interactive CLI modes (`--select`, `--focused`, `--all`, `--output`). In GUI mode, the "capture" button computes them at export time.
- If no output path is given and `--copy` is not set, the path is printed to stdout.
