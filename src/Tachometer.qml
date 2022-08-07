import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects
import "logic.js" as Logic
import backend

// Used for displaying the engineSpeed

Item {
    id: tachometer
    width: 600
    height: 600
    visible: true
    antialiasing: true

    property int sizeDigits: 30

    Image {
        id: ellipse_gray_brackground
        source: "/Ellipse_Gray_Background.png"
        anchors.centerIn: parent
    }

    Text {
        id: zero_digit
        x: 90
        y: 475
        text: qsTr("0")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: one_digit
        x: 10
        y: 327
        text: qsTr("1")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: two_digit
        x: 23
        y: 165
        text: qsTr("2")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: three_digit
        x: 123
        y: 34
        text: qsTr("3")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: four_digit
        x: 285
        y: 0
        text: qsTr("4")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: five_digit
        x: 431
        y: 34
        text: qsTr("5")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: six_digit
        x: 537
        y: 165
        text: qsTr("6")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: seven_digit
        x: 550
        y: 327
        text: qsTr("7")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Text {
        id: eight_digit
        x: 475
        y: 475
        text: qsTr("8")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: sizeDigits
    }

    Image {
        id: white_needles
        source: "\Tachometer_Needles.png"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#000000"
            transparentBorder: false
            horizontalOffset: 1
            verticalOffset: 1
            radius: 2
            source: white_needles
        }
    }

    Image {
        id: ellipse_silver_ring
        source: "/Ellipse_Silver_Ring.png"
        anchors.centerIn: parent
    }

    Canvas {
        id: cpb1
        anchors.centerIn: parent
        width: 500
        height: 500
        property int engineSpeed: CarEmulator.engineSpeed
        onPaint: {
            var ctx = getContext("2d");

            var grd = ctx.createRadialGradient(250, 250, 225, 250, 250, 275);
            grd.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd.addColorStop(0.5, "rgba(25, 255, 255, 0.5)");
            grd.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();
            ctx.lineWidth = 40;
            ctx.strokeStyle = grd;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-20, Logic.converDegToRad(135), Logic.converDegToRad(Logic.engineSpeedNeedleTail(CarEmulator.engineSpeed)));
            ctx.stroke();

            var grd2 = ctx.createRadialGradient(250, 250, 175, 250, 250, 225);
            grd2.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd2.addColorStop(0.5, "rgba(255, 255, 255, 0.5)");
            grd2.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();

            ctx.font = "40px Righteous";
            ctx.fillStyle = "#ffffff";
            Logic.drawTextAlongArc(ctx, "RPMx1000", cpb1.width/2, cpb1.height/2, cpb1.width/2-10, Logic.converDegToRad(35));

            ctx.lineWidth = 60;
            ctx.strokeStyle = grd2;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-80, Logic.converDegToRad(135), Logic.converDegToRad(Logic.engineSpeedNeedleTail(CarEmulator.engineSpeed)));
            ctx.stroke();
        }

        function clear()
        {
            var ctx = getContext("2d");
            ctx.reset();
            this.requestPaint();
        }

        onEngineSpeedChanged: clear()

    }

    Image {
        id: red_needle
        source: "\Red_Needle.png"
        anchors.centerIn: parent
        rotation: Logic.engineSpeedNeedleRotation(CarEmulator.engineSpeed) // [0 deg = 0km/h] | [270 deg = 280 km/h]
        //CarEmulator.onEngineSpeedChanged: console.log("test")
    }

    Image {
        id: ellipse_red_needle
        source: "\Ellipse_Red_Needle.png"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#000000"
            transparentBorder: false
            horizontalOffset: 4
            verticalOffset: 6
            radius: 16
            source: ellipse_red_needle
        }
    }

    Glow {
        id: glow_ellipse_red_needle
        anchors.fill: ellipse_red_needle
        radius: 8
        color: "red"
        source: ellipse_red_needle
    }

    Text {
        id: driving_mode
        x: 150
        y: 150
        width: 300
        height: 300
        text: String.fromCharCode(CarEmulator.driveMode)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: 120
    }

    Text {
        id: gear_mode
        x: 150+70
        y: 150-40
        width: 300
        height: 300
        text: String.fromCharCode(CarEmulator.gear)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: 30
    }

}
