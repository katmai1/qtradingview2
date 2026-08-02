#!/bin/bash
HERE="/opt/qtradingview2"
export LD_LIBRARY_PATH="$HERE/lib:$LD_LIBRARY_PATH"
export QT_PLUGIN_PATH="$HERE/plugins"
export QT_QPA_PLATFORM_PLUGIN_PATH="$HERE/plugins/platforms"
export QTWEBENGINEPROCESS_PATH="$HERE/libexec/QtWebEngineProcess"
export QTWEBENGINE_RESOURCES_PATH="$HERE/resources"
export QTWEBENGINE_LOCALES_PATH="$HERE/translations/qtwebengine_locales"
export QTWEBENGINE_DISABLE_SANDBOX=1
exec "$HERE/bin/qtradingview2" "$@"
