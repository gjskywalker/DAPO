#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/Analysis/build"
TOOLS_ROOT="$ROOT_DIR/Analysis_tools"
FEAT_DIR="$TOOLS_ROOT/feature_exctractor"
QOR_DIR="$TOOLS_ROOT/qor_estimator"

mkdir -p "$BUILD_DIR"
cmake -S "$ROOT_DIR/Analysis" -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"

mkdir -p "$TOOLS_ROOT"

if [ -f "$FEAT_DIR" ]; then
  rm -f "$FEAT_DIR"
fi
if [ -f "$QOR_DIR" ]; then
  rm -f "$QOR_DIR"
fi
mkdir -p "$FEAT_DIR" "$QOR_DIR"

cp "$BUILD_DIR/tools/feat/feature_exctractor" "$FEAT_DIR/" 2>/dev/null || cp "$BUILD_DIR/tools/feat/feature_exctractor.exe" "$FEAT_DIR/" 2>/dev/null || true
cp "$BUILD_DIR/tools/qor/qor_estimator" "$QOR_DIR/" 2>/dev/null || cp "$BUILD_DIR/tools/qor/qor_estimator.exe" "$QOR_DIR/" 2>/dev/null || true

echo "Executables copied to $TOOLS_ROOT"
