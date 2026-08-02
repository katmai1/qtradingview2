#!/bin/bash
HERE="/opt/qtradingview2"
export LD_LIBRARY_PATH="$HERE/lib:$LD_LIBRARY_PATH"
export QTWEBENGINEPROCESS_PATH="$HERE/libexec/QtWebEngineProcess"
export QTWEBENGINE_RESOURCES_PATH="$HERE/resources"
export QTWEBENGINE_LOCALES_PATH="$HERE/translations/qtwebengine_locales"
exec "$HERE/bin/qtradingview2" "$@"
