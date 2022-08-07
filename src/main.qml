import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects
/***
 *  Importing Logic.js for additional Backend funcionality
 *  GOAL HERE: Splitting Backend- and Frontend Code as much as possible
 */
import "logic.js" as Logic
/**
 *  Importing Backend in order to use the C++ CarEmulator-Class
 */
import backend

/**
 *  WINDOW CONTAINER
 */
Window {
    id: mainWindow_1
    // No default width and height were define, due to a fixed window size
    minimumWidth: 1600
    minimumHeight: 600
    maximumWidth: 1600
    maximumHeight: 600
    visible: true
    color: "#3C3C3C"
    title: qsTr("Quick Cockpit")

    /**
     * When the window is created the mainWindow_1 object is handed over to the
     * listenTo-function of the CarEmulater Class to read the keyEvents of the object.
     */
    Component.onCompleted: CarEmulator.listenTo(mainWindow_1)

    Speedometer {
        id: speedometer
    }

    Rectangle {
        id: placeholder_rect
        width: 400
        height: 600
        anchors.left: speedometer.right
        color: "#3C3C3C"

        FuelTank {
            id: fuelTank
        }

        Item {
            id: myRow
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: placeholder_rect.width
            height: 50

            Text {
                id: drivenKilometers
                width: 200
                height: 50
                rightPadding: 100
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                font.family: "Righteous"
                font.pointSize: 20
                color: "#ffffff"
                text: (CarEmulator.drivenKilometer / 1000).toFixed(1) + " km"
            }

            Text {
                id: fuelConsumption
                width: 200
                height: 50
                anchors.left: drivenKilometers.right
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                font.family: "Righteous"
                font.pointSize: 20
                color: "#ffffff"
                text: (CarEmulator.fuelConsumption / 10).toFixed(1) + " l/100 km"
            }
        }

    }

    Tachometer {
        id: tachometer
        anchors.left: placeholder_rect.right
    }

    Rectangle {
        id: statusBarBackground
        color: "#1f1f1f"
        width: 300
        height: 200

        anchors.centerIn: parent
        radius: 25
        layer.enabled: true
        layer.effect: Glow {
            radius: 4
            color: "#ffffff"
            source: statusBarBackground
            transparentBorder: true
        }

        Text {
            id: statusBarText
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: CarEmulator.displayInfo
            font.family: "Ubuntu Mono"
            font.pointSize: 16
            color: "#00ff00"
        }
    }

    Image {
        id: reverseCam
        anchors.fill: parent
        visible: CarEmulator.driveBackwards
        source: "/reverseCam.PNG"
    }

}
