import QtQuick
import QtQuick.Controls
import QtQuick.Window

Window {
    width: 400
    height: 300
    visible: true
    title: "GPIO HAL Hello World (Mock on desktop)"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            text: "GPIO Pin " + gpioPin.offset
            font.pixelSize: 24
        }

        Rectangle {
            width: 150
            height: 150
            radius: 75
            color: gpioPin.value ? "#4ade80" : "#ef4444"
            border.color: "black"
            border.width: 4

            Text {
                anchors.centerIn: parent
                text: gpioPin.value ? "ON" : "OFF"
                font.pixelSize: 32
                color: "white"
            }
        }

        Button {
            text: "TOGGLE GPIO"
            font.pixelSize: 18
            onClicked: gpioPin.toggle()
        }

        Text {
            text: "Build type: " + (gpioPin.isReal ? "REAL (libgpiod)" : "MOCK")
            font.pixelSize: 14
            color: gpioPin.isReal ? "green" : "orange"
        }
    }
}
