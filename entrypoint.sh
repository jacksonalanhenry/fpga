#!/bin/bash
set -e

# Check if user with this UID exists
if ! id -u "$USER_NAME" >/dev/null 2>&1; then
  echo "Creating user $USER_NAME with UID $USER_ID..."
  groupadd -g "$GROUP_ID" "$USER_NAME"
  useradd -m -u "$USER_ID" -g "$GROUP_ID" -s /bin/bash "$USER_NAME"
fi

# Switch to the new user and run bash
exec gosu "$USER_NAME" "$@"

