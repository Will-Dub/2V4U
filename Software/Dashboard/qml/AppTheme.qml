pragma Singleton

import QtQuick

QtObject {
    // --- ACTION & ACCENT PALETTE ---
    readonly property color colorActn: "#D5FA46" // Drive / Success / Go (Electric Lime)
    readonly property color colorPrm: "#00E5FF" // Primary highlights / Active states (Electric Cyan)
    readonly property color colorSec: "#FFB03D" // Secondary / Standby / Warnings
    readonly property color colorAlert: "#FF3B30" // Faults / Errors / E-Stop (High-vis Red)
    readonly property color colorWarn: "#FF9F0A" // Warning Amber

    // --- INTERACTIVE COMPONENT PALETTE ---
    readonly property color colorBtnBg: "#1D6FA5" // Default button / card base
    readonly property color colorBtnHover: "#3498db" // Hover state fill
    readonly property color colorBtnBorder: "#00E5FF" // Active / focused border
    readonly property color colorCardBg: "#1B221E"

    // --- NEUTRAL PALETTE ---
    readonly property color colorNeut100: "#F4F5E9" // Bone White (Primary Text)
    readonly property color colorNeut90: "#DCDFDA" // Off-white
    readonly property color colorNeut80: "#B0C4DE" // Secondary / Detail Text (Steel Blue/Sage)
    readonly property color colorNeut70: "#A2ACA0" // Light Sage (Inactive icons)
    readonly property color colorNeut60: "#838E81" // Subtle text
    readonly property color colorNeut50: "#647062" // Muted / Disabled UI
    readonly property color colorNeut40: "#4B5649" // Dark elements
    readonly property color colorNeut30: "#323C30" // Dark Olive (Panel backgrounds)
    readonly property color colorNeut20: "#262E27" // Default borders
    readonly property color colorNeut10: "#1B201C" // Deep backgrounds
    readonly property color colorNeut0: "#101311" // Deepest Charcoal (App Background)

    // --- TYPOGRAPHY ---
    readonly property string fontMono: "Roboto Mono"

    readonly property int sizeMassive: 104
    readonly property int sizeHeader: 32
    readonly property int sizeNormal: 24
    readonly property int sizeSmall: 16
    readonly property int sizeExtraSmall: 12
}
