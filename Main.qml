import QtQuick
import QtQuick.Controls
import QtQuick.Window
import gpio_stmp_demo

ApplicationWindow {
    width: 400
    height: 300
    visible: true
    title: "GPIO HAL Hello World (Mock on desktop)"

    FontLoader {
        id: goRegular
        source: Qt.resolvedUrl("go_fonts/Go-Regular.ttf")
    }

    GpioPin {
        id: gpioPin
        pin: 14
        enabled: true
        status: true
    }

    Pane {
        anchors.fill: parent
        background: Rectangle {
            color: "blue"
        }

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "GPIO Pin:" + gpioPin.pin
                font.pixelSize: 24
            }
            Rectangle {
                width: 150
                height: 150
                radius: 75
                color: gpioPin.status ? "#4ade80" : "#ef4444"
                border.color: "black"
                border.width: 4
                Text {
                    anchors.centerIn: parent
                    text: gpioPin.status ? "ON" : "OFF"
                    font.pixelSize: 32
                    color: "white"
                }
            }
            Button {
                text: "TOGGLE GPIO"
                font.pixelSize: 18
                onClicked: gpioPin.status = !gpioPin.status
            }
        }
    }
}
