#!/bin/bash
set -e

# Check if user with this UID exists
if ! id -u "$USER_NAME" >/dev/null 2>&1; then
  echo "Creating user $USER_NAME with UID $USER_ID..."
  groupadd -g "$GROUP_ID" "$USER_NAME"
  useradd -m -u "$USER_ID" -g "$GROUP_ID" -s /bin/bash "$USER_NAME"
fi

echo "[entrypoint] User: $USER_NAME ($USER_ID:$GROUP_ID)"
echo "[entrypoint] Command: $@"

if [ $# -eq 0 ]; then
  echo "[entrypoint] No command passed — running interactive bash"
  exec gosu "$USER_NAME" bash
else
  echo "[entrypoint] Running: $@"
  exec gosu "$USER_NAME" "$@"
fi


