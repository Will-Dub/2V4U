pragma Singleton

import QtQuick

QtObject {
    // ACTION / ACCENT PALETTE
    readonly property color colorActn: "#D5FA46" // Drive / Success / Go[cite: 1]
    readonly property color colorPrm: "#4AC8FA" // Primary highlights / Accents
    readonly property color colorSec: "#FFB03D" // Secondary / Standby / Warnings
    readonly property color colorAlert: "#FF543D" // Faults / Errors / E-Stop[cite: 1]
    readonly property color colorWarn: "#FF9F0A"

    // NEUTRAL PALETTE
    readonly property color colorNeut100: "#F4F5E9" // Bone White (Primary Text)[cite: 1]
    readonly property color colorNeut90: "#DCDFDA" // Interpolated off-white
    readonly property color colorNeut80: "#BFC7BE" // Interpolated light borders
    readonly property color colorNeut70: "#A2ACA0" // Light Sage (Inactive icons)[cite: 1]
    readonly property color colorNeut60: "#838E81" // Interpolated subtle text
    readonly property color colorNeut50: "#647062" // Medium Sage (Muted / Disabled UI)[cite: 1]
    readonly property color colorNeut40: "#4B5649" // Interpolated dark elements
    readonly property color colorNeut30: "#323C30" // Dark Olive (Panel backgrounds)[cite: 1]
    readonly property color colorNeut20: "#262E27" // Interpolated default borders
    readonly property color colorNeut10: "#1B201C" // Interpolated deep backgrounds
    readonly property color colorNeut0: "#101311" // Deepest Charcoal (App Background)[cite: 1]

    readonly property string fontMono: "Roboto Mono"

    readonly property int sizeMassive: 160
    readonly property int sizeHeader: 32
    readonly property int sizeNormal: 32
    readonly property int sizeSmall: 16
    readonly property int sizeExtraSmall: 9
}
