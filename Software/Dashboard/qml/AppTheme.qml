pragma Singleton
import QtQuick

QtObject {
    readonly property color colorActn:   "#D3F85A" // Drive / Success / Go
    readonly property color colorPrm:    "#007AFF" // Primary highlights / Accents
    readonly property color colorSec:    "#FF9500" // Secondary / Standby / Warnings
    readonly property color colorAlert:  "#FF3B30" // Faults / Errors / E-Stop

    // NEUTRAL PALETTE
    readonly property color colorNeut100: "#FFFFFF" // Pure white (Primary Text)
    readonly property color colorNeut90:  "#E5E5E5" // Off-white (Secondary Text)
    readonly property color colorNeut80:  "#CCCCCC" // Light borders / Dividers
    readonly property color colorNeut70:  "#B3B3B3" // Inactive icons
    readonly property color colorNeut60:  "#999999" // Subtle secondary text
    readonly property color colorNeut50:  "#808080" // Muted / Disabled UI
    readonly property color colorNeut40:  "#666666" // Dark disabled elements
    readonly property color colorNeut30:  "#4D4D4D" // Lighter panel backgrounds / Hover states
    readonly property color colorNeut20:  "#333333" // Default borders / Panel Backgrounds
    readonly property color colorNeut10:  "#1A1A1A" // Deep backgrounds / Modals
    readonly property color colorNeut0:   "#000000" // Pure black (App Background)

    readonly property string fontMono: "Courier"

    readonly property int sizeMassive: 160
    readonly property int sizeHeader: 48
    readonly property int sizeNormal: 32
    readonly property int sizeSmall: 16
}
